#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PointSearchByIndex.h"

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
    double distance(Point a, Point b);//����ŷ����þ���
    static bool compareX(Point a, Point b);//�Ƚ�x����Ĵ�С
    bool compareY(Point a, Point b);//�Ƚ�y����Ĵ�С
    Point closestPoint(std::vector< Point>& points, int left, int right, Point target);//���η����ҵ�


private:
    Ui::PointSearchByIndexClass ui;
};
