#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PointSearchByIndex.h"

struct Point {
    int id;
    int x;//经度
    int y;//纬度
};


class PointSearchByIndex : public QMainWindow
{
    Q_OBJECT

public:
    PointSearchByIndex(QWidget *parent = nullptr);
    ~PointSearchByIndex();

    void startMainWindow();
    double distance(Point a, Point b);//计算欧几里得距离
    static bool compareX(Point a, Point b);//比较x坐标的大小
    bool compareY(Point a, Point b);//比较y坐标的大小
    Point closestPoint(std::vector< Point>& points, int left, int right, Point target);//分治法查找点


private:
    Ui::PointSearchByIndexClass ui;
};
