#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QFrame>

class QAbstractButton;
class QGraphicsScene;
class QGraphicsView;
class QSpinBox;

struct Point {
    double x;
    double y;
    QVector<double> attributes;
    int cluster = -1;
};

#include <QPainter>

class PointsWidget : public QFrame {
    Q_OBJECT

public:
    PointsWidget(QWidget *parent = nullptr) : QFrame(parent) {}

    void setPoints(const QVector<Point> &points_)
    {
        points.clear();
        for (const auto &point: points_)
        {
            points << QPointF(point.x, point.y);
        }
        update();
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event);

        QImage image(size(), QImage::Format_ARGB32);
        image.fill(Qt::white);

        QRectF bounds = calculateBounds();
        qreal scaleX = width() / bounds.width();
        qreal scaleY = height() / bounds.height();

        QColor pointColor = Qt::red;

        for (const QPointF &point : points)
        {
            int x = (point.x() - bounds.left()) * scaleX;
            int y = (point.y() - bounds.top()) * scaleY;

            if (x >= 0 && x < image.width() && y >= 0 && y < image.height())
            {
                image.setPixel(x, y, pointColor.rgba());
            }
        }

        QPainter painter(this);
        painter.drawImage(0, 0, image);
    }

    void resizeEvent(QResizeEvent *event) override
    {
        Q_UNUSED(event);
        update();
    }

    QColor getColorFromValue(double value)
    {
        return QColor(qBound(0, static_cast<int>(value * 255), 255),
                      qBound(0, 0, 255),
                      qBound(0, 255 - static_cast<int>(value * 255), 255), 255);
    }

private:
    QVector<QPointF> points;

    QRectF calculateBounds() const
    {
        if (points.isEmpty())
            return QRectF();

        qreal minX = points[0].x();
        qreal minY = points[0].y();
        qreal maxX = minX;
        qreal maxY = minY;

        for (const QPointF &point : points)
        {
            if (point.x() < minX)
                minX = point.x();
            if (point.y() < minY)
                minY = point.y();
            if (point.x() > maxX)
                maxX = point.x();
            if (point.y() > maxY)
                maxY = point.y();
        }

        return QRectF(minX, minY,
                      maxX - minX,
                      maxY - minY);
    }
};


class MainView: public QFrame
{
    Q_OBJECT
public:
    MainView(QWidget *parent = nullptr);

    void drawHeatMap(const QVector<Point> &points);
    void drawContours(const QVector<Point> &points);

signals:
    void openFileClicked(const QString &path);
    void clusterizationClicked(const int countClusters,
                               const int maxIterations);
    void saveFileClicked(const QString &filePath);

private slots:
    void onOpenFileClicked();
    void onSaveFileClicked();

private:
    void setUpUi();
    void setUpConnections();

    QColor getColorFromValue(double value);
    void switchMap();

    QAbstractButton *buttonOpenFile;
    QAbstractButton *buttonSwithMaps;
    QAbstractButton *buttonSave;
    QAbstractButton *buttonClusterization;

    QGraphicsScene *sceneCountur;
    QGraphicsView *viewCountur;

    QSpinBox *maxIterations;
    QSpinBox *countClasters;

    PointsWidget *heatMapPainter;
};

#endif // MAINVIEW_H
