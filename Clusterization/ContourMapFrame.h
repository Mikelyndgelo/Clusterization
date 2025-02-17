#ifndef CONTOURMAPFRAME_H
#define CONTOURMAPFRAME_H

#include <QFrame>
#include "Point.h"

class ContourMapFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ContourMapFrame(QWidget *parent = nullptr);
    void setPoints(const QVector<Point> &points_);
    void updateContourMap();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<Point> points;
    QVector<QVector<double>> grid;
    QRectF bounds;

    QRectF calculateBounds(const QVector<Point> &points) const;
    QVector<QVector<double>> interpolateData(const QVector<Point> &points, int gridSize);
    QVector<QLineF> generateContourLines(const QVector<QVector<double>> &grid, double level);
};

#endif // CONTOURMAPFRAME_H
