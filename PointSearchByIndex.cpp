#include "PointSearchByIndex.h"
#include <QFileDialog>
#include <unordered_map>
#include <QMessageBox>

using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::endl;

// 定义常量
const string DATA_FILE = "data.csv"; // 数据文件名
const string INDEX_FILE = "index.txt"; // 索引文件名
const char DELIMITER = ','; // 分隔符
const int MAX_NUM = 1000000000;
//const int EXPANSION = 10000;//扩大倍数，减少浮点运算


PointSearchByIndex::PointSearchByIndex(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    startMainWindow();

    //信号槽部分
    connect(ui.loadBtn, &QPushButton::clicked, this, [&]() {
        if (!readCsvFile()) {
            warningMessage("Open File Error.");
            return;
        }
        if (!buildIndexFile()) {
            warningMessage("Build Index File Error.");
            return;
        }
        allPointsBackup = allPoints;
        warningMessage("Data File has built index file.");
        loadData = true;
    });
    //输入查询点坐标
    connect(ui.inputX, &QLineEdit::textChanged, this, [&]() {
        targetPoint.x = ui.inputX->text().toDouble();
        cor[0] = true;
    });
    connect(ui.inputY, &QLineEdit::textChanged, this, [&]() {
        targetPoint.y = ui.inputY->text().toDouble();
        cor[1] = true;
    });
    //查询并展示结果
    connect(ui.searchBtn, &QPushButton::clicked, this, [&]() {
        if (!loadData) {
            warningMessage("\nPlease load data first!\n");
            return;
        }
        if (!cor[0] || !cor[1]) {
            warningMessage("\nPlease input target coordinate!\n");
            return;
        }
        targetPoint = closestPoint(allPoints, 0, allPoints.size() - 1, targetPoint);
        showPointsList(searchNearestPoint());
        allPoints = allPointsBackup;
    });


}

PointSearchByIndex::~PointSearchByIndex()
{}

void PointSearchByIndex::startMainWindow()
{
    ui.pointsView->setColumnCount(3);//设置列数
    ui.pointsView->setRowCount(40);//设置行数
    //ui.pointsView->setWindowTitle("QTableWidget");
    ui.pointsView->verticalHeader()->setVisible(true);//纵向表头可视化
    ui.pointsView->horizontalHeader()->setVisible(true);//横向表头可视化
    ui.pointsView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置编辑方式：禁止编辑表格
    ui.pointsView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置表格选择方式：设置表格为整行选中
    ui.pointsView->setSelectionMode(QAbstractItemView::SingleSelection);//选择目标方式
    ui.pointsView->setStyleSheet("selection-background-color:pink");//设置选中颜色：粉色
    ui.pointsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);//设置垂直滚动条
    //设置行和列的大小
    ui.pointsView->setColumnWidth(0, 80);
    ui.pointsView->setColumnWidth(1, 110);
    ui.pointsView->setColumnWidth(2, 110);
    //设置行列标签
    QStringList HStrList;
    HStrList.push_back(QString("ID"));
    HStrList.push_back(QString("X (经度)"));
    HStrList.push_back(QString("Y (纬度)"));
    ui.pointsView->setHorizontalHeaderLabels(HStrList);
    
}

void PointSearchByIndex::warningMessage(QString msg)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.exec();
}


double PointSearchByIndex::distanceSquare(const Point& a, const Point& b)
 {
    double dx = a.x  - b.x ;
    double dy = a.y  - b.y ;
    return dx * dx + dy * dy;
}

bool PointSearchByIndex::compareX(const Point& a, const Point& b)
{
    return a.x < b.x;
}

bool PointSearchByIndex::compareY(const Point& a, const Point& b)
{
    return a.y < b.y;
}

bool PointSearchByIndex::buildIndexFile()
{
    ifstream datafile(DATA_FILE); // 打开数据文件
    ofstream indexfile(INDEX_FILE); // 创建索引文件

    if (!datafile.is_open() || !indexfile.is_open()) 
    {
        return false;
    }

    string line;
    indexfile << "ID" << DELIMITER << "BLOCK" << endl; // Index：写入表头

    sort(allPoints.begin(), allPoints.end(), compareX);
    int group = 1;
    for (int i = 0; i < allPoints.size();)
    {
        int start = i;
        double mid_x = allPoints[i].x;
        while (i < allPoints.size() && allPoints[i].x == mid_x)
        {
            indexfile << allPoints[i].id << DELIMITER << group << endl;
            ++i;
        }
        blocks.emplace_back(start, i - 1, mid_x);
        group++;
    }

    indexfile.close(); // 关闭索引文件
    datafile.close();  // 关闭数据文件
    return true;
}

bool PointSearchByIndex::readCsvFile()
{
    // 打开csv文件
    ifstream infile(DATA_FILE);
    if (!infile.is_open()) {
        return false;
    }

    // 读取文件头
    ui.targetView->append(QString::fromStdString("CSV Header Information:"));
    string header1, header2;
    getline(infile, header1);
    ui.targetView->append(QString::fromStdString(header1));
    getline(infile, header1);
    ui.targetView->append(QString::fromStdString(header1));
    getline(infile, header1);
    ui.targetView->append(QString::fromStdString(header1));
    getline(infile, header1);
    ui.targetView->append(QString::fromStdString(header1));
    getline(infile, header2);
    ui.targetView->append(QString::fromStdString(header2));

    // 分割表头行
    vector < string > headers;
    string field;
    for (int i = 0; i < header2.length(); i++) {
        if (header2[i] == ',') {
            headers.push_back(field);
            field.clear();
        }
        else {
            field.push_back(header2[i]);
        }
    }
    headers.push_back(field);

    // 读取数据
    string line;
    while (getline(infile, line)) {
        // 分割数据行
        string id;
        string x;
        string y;
        int cnt = 0;
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == ',') {
                cnt++;
                continue;
            }
            if (cnt == 0) {
                id.push_back(line[i]);
            }
            else if (cnt == 1) {
                x.push_back(line[i]);
            }
            else {
                y.push_back(line[i]);
            }
        }
        // 保存数据
        Point p;
        p.id = stoi(id);
        p.x = stod(x);
        p.y = stod(y);
        allPoints.push_back(p);
    }
    // 关闭文件
    infile.close();
    return true;
}

int PointSearchByIndex::FindNearestPointInBlock(const Block& block)
{
    double min_distance = MAX_NUM;
    int result_id = -1;

    for (int i = block.Start(); i <= block.End(); ++i)
    {
        /*if (block.End() >= allPoints.size())continue;*/
        double distance = distanceSquare(targetPoint, allPoints[i]);
        if (distance < min_distance)
        {
            min_distance = distance;
            result_id = allPoints[i].id;
        }
    }
    
    return result_id;
}

Point PointSearchByIndex::closestPoint(vector<Point>& points, int left, int right, Point target)
{
    // 如果只有一个点，直接返回
    if (left == right) {
        return points[left];
    }

    // 如果有两个点，比较它们和目标点的距离，返回较近的那个
    if (left + 1 == right) {
        if (distanceSquare(points[left], target) < distanceSquare(points[right], target)) {
            return points[left];
        }
        else {
            return points[right];
        }
    }

    // 如果有多于两个点，先按照x坐标排序，然后找到中间位置
    sort(points.begin() + left, points.begin() + right + 1, compareX);
    int mid = (left + right) / 2;

    if (target.x < points[mid].x) {
        return closestPoint(points, left, mid, target);
    }
    else if (target.x > points[mid].x) {
        return closestPoint(points, mid + 1, right, target);
    }
    else {
        // 递归查找左半部分和右半部分的最近点
        Point leftClosest = closestPoint(points, left, mid - 1, target);
        Point rightClosest = closestPoint(points, mid + 1, right, target);
        // 比较两个最近点和中间位置的点和目标点的距离，返回最小的那个
        double d1 = distanceSquare(leftClosest, target);
        double d2 = distanceSquare(rightClosest, target);
        double d3 = distanceSquare(points[mid], target);
        if (d1 <= d2 && d1 <= d3) {
            return leftClosest;
        }
        else if (d2 <= d1 && d2 <= d3) {
            return rightClosest;
        }
        else {
            return points[mid];
        }
    }

}

int PointSearchByIndex::searchNearestPoint()
{
    // 在目标点所在的块以及左侧和右侧的两个块中查找距离最近的点
    double min_distance = INFINITY;
    int result_id = -1;
    //ofstream fl ("debug.txt");
    int count = 0;
    //std::cout << "total:" << allPoints.size() << "\t" << blocks.size() << endl;
    for (/*int i=0;i<blocks.size();i++*/const auto& block : blocks)
    {
        //Block block = blocks[i];
        //std::cout << ++count<< ":\t" << block.Start() << "," << block.End() << endl;

        if (block.Start() > 0 && block.Start() - 1 < allPoints.size())
        {
            double distance_to_left_block = abs(targetPoint.x - block/*s[block.Start() - 1]*/.MidX());
            if (distance_to_left_block < min_distance)
            {
                int nearest_point_in_left_block =FindNearestPointInBlock(block/*s[block.Start() - 1]*/);
                if (nearest_point_in_left_block < allPoints.size()) {
                    double distance_to_nearest_point_in_left_block =
                        distanceSquare(targetPoint,allPoints[nearest_point_in_left_block]);
                    if (distance_to_nearest_point_in_left_block < min_distance)
                    {
                        min_distance = distance_to_nearest_point_in_left_block;
                        result_id = nearest_point_in_left_block;
                    }
                }
                else {
                    // 打印一些错误信息或者抛出异常
                }
            }
        }

        int nearest_point_in_block =FindNearestPointInBlock(block);
        if (nearest_point_in_block >= 0 && nearest_point_in_block < allPoints.size())
        {
            double distance_to_nearest_point_in_block =
                distanceSquare(targetPoint, allPoints[nearest_point_in_block]);
            if (distance_to_nearest_point_in_block < min_distance)
            {
                min_distance = distance_to_nearest_point_in_block;
                result_id = nearest_point_in_block;
            }
        }
        else {
            // 打印一些错误信息或者抛出异常
            //std::cerr << "Out of Range!" << endl;
            //exit(3);
        }

        if (block.End() < allPoints.size() - 1 /*&& block.End() + 1 < allPoints.size()*/)
        {
            double distance_to_right_block =abs(block/*s[block.End() + 1]*/.MidX() - targetPoint.x);
            if (distance_to_right_block < min_distance)
            {
                int nearest_point_in_right_block =
                    FindNearestPointInBlock(block/*s[block.End() + 1]*/);
                if (nearest_point_in_right_block < allPoints.size()) {
                    double distance_to_nearest_point_in_right_block =
                        distanceSquare(targetPoint,allPoints[nearest_point_in_right_block]);
                    if (distance_to_nearest_point_in_right_block < min_distance)
                    {
                        min_distance = distance_to_nearest_point_in_right_block;
                        result_id = nearest_point_in_right_block;
                    }
                }
                else {
                    // 打印一些错误信息或者抛出异常
                }
            }
        }
    }
    //std::cout << result_id;
    return result_id;
}

void PointSearchByIndex::showPointsList(int pt_id)
{
    ui.pointsView->clearContents();
    vector<Point> nearPoints;
    int startID = (targetPoint.id - 20 < 1) ? 1 : targetPoint.id - 20;//开始的点
    int endID = (allPoints.size() < targetPoint.id + 20) ? allPoints.size() : targetPoint.id + 20;//结束的点
    for (int i = 0; i <= (endID - startID); i++) {
        if (allPoints[startID + i - 1].id == targetPoint.id)continue;
        nearPoints.push_back(allPoints[startID + i - 1]);
    }
    for (unsigned int i = 0; i < nearPoints.size(); i++) {
        ui.pointsView->setItem(i, 0, new QTableWidgetItem(QString::number(nearPoints[i].id)));
        ui.pointsView->setItem(i, 1, new QTableWidgetItem(QString::number(nearPoints[i].x)));
        ui.pointsView->setItem(i, 2, new QTableWidgetItem(QString::number(nearPoints[i].y)));
    }
    QString msg = QString("\nThe nearest(or target) point information:")
        .append("\ntargetID:").append(QString::number(targetPoint.id))
        .append("\ntargetX:").append(QString::number(targetPoint.id))
        .append("\ntargetY:").append(QString::number(targetPoint.y))
        .append("\ndistance:").append(QString::number(sqrt(distanceSquare(targetPoint, targetPoint))));
    ui.targetView->append(msg);
}



