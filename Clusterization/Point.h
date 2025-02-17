#ifndef POINT_H
#define POINT_H

#include <QVector>

struct Point {
    double x;
    double y;
    QVector<double> attributes;
    int cluster = -1;
};

#endif // POINT_H
