#include "PointSearchByIndex.h"
#include <QFileDialog>
#include <unordered_map>

using std::ifstream;
using std::ofstream;
using std::endl;


PointSearchByIndex::PointSearchByIndex(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    startMainWindow();

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

void PointSearchByIndex::startOriginData()
{

}
