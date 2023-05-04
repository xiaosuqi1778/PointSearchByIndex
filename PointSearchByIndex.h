#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PointSearchByIndex.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
//#include <iterator>
#include <cmath>
//#include <unordered_map>

struct Point {
    int id;
    double x;//����
    double y;//γ��
};

class Block
{
public:
    Block(int _start, int _end, double _mid_x) : start(_start), end(_end), mid_x(_mid_x) {}

    int Start() const { return start; }
    int End() const { return end; }
    double MidX() const { return mid_x; }

private:
    int start;
    int end;
    double mid_x;
};


class PointSearchByIndex : public QMainWindow
{
    Q_OBJECT

public:
    PointSearchByIndex(QWidget *parent = nullptr);
    ~PointSearchByIndex();

    void startMainWindow();
    void startOriginData();

    void warningMessage(QString msg);

    int distanceSquare(const Point& a, const Point& b);//����ŷ����þ����ƽ��
    bool compareX(const Point& a, const Point& b);//�Ƚ�x����Ĵ�С
    bool compareY(const Point& a, const Point& b);//�Ƚ�y����Ĵ�С

    bool buildIndexFile();
    bool readCsvFile();

    int FindNearestPointInBlock(const Block& block);
    int searchNearestPoint();
    void showPointsList(int pt_id);
private:
    Ui::PointSearchByIndexClass ui;

    std::vector<Point> allPoints;
    std::vector<Block> blocks;

    Point targetPoint = { -1,-1,-1 };
    bool cor[2] = {false,false};
    bool loadData = false; 
};
