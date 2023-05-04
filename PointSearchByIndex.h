#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PointSearchByIndex.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>


struct Point {
    int id;
    int x;//����
    int y;//γ��
};


class PointSearchByIndex : public QMainWindow
{
    Q_OBJECT

public:
    PointSearchByIndex(QWidget *parent = nullptr);
    ~PointSearchByIndex();

    void startMainWindow();
    void startOriginData();

    int distance(Point a, Point b);//����ŷ����þ����ƽ��
    static bool compareX(Point a, Point b);//�Ƚ�x����Ĵ�С
    bool compareY(Point a, Point b);//�Ƚ�y����Ĵ�С
    Point closestPoint(std::vector< Point>& points, int left, int right, Point target);//���η����ҵ�

    void buildIndexFile();

private:
    Ui::PointSearchByIndexClass ui;
};
