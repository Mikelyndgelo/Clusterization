#ifndef HEATMAPFRAME_H
#define HEATMAPFRAME_H

#include <QFrame>

#include "Point.h"


class HeatMapFrame : public QFrame {
    Q_OBJECT

public:
    HeatMapFrame(QWidget *parent = nullptr);

    void setPoints(const QVector<Point> &points_);

protected:
    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    QColor getColorFromValue(double value);

private:
    QVector<QPointF> points;
    QVector<int> clusters;

    QRectF calculateBounds() const;
};

#endif // HEATMAPFRAME_H
