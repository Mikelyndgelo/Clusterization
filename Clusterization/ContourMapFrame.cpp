#include "ContourMapFrame.h"

#include <QPainter>
#include <QVector>
#include <QPointF>
#include <QImage>
#include <QRectF>
#include <QLineF>

#include "ContourMapFrame.h"

ContourMapFrame::ContourMapFrame(QWidget *parent) : QFrame(parent)
{
    setMinimumSize(400, 300);
}

void ContourMapFrame::setPoints(const QVector<Point> &points_)
{
    points = points_;
    updateContourMap();
}

void ContourMapFrame::updateContourMap()
{
    if (points.isEmpty())
        return;

    int gridSize = 100;
    grid = interpolateData(points, gridSize);
    update();
}

void ContourMapFrame::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if (grid.empty())
        return;

    QImage image(size(), QImage::Format_ARGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);

    qreal scaleX = width() / static_cast<qreal>(grid.size());
    qreal scaleY = height() / static_cast<qreal>(grid[0].size());

    QVector<QColor> clusterColors = {
        Qt::red, Qt::blue, Qt::green, Qt::yellow, Qt::magenta,
        Qt::cyan, Qt::darkRed, Qt::darkBlue, Qt::darkGreen, Qt::darkYellow
    };

    for (const Point &point: points)
    {
        int x = (point.x - bounds.left()) * scaleX;
        int y = (point.y - bounds.top()) * scaleY;

        if (x >= 0 && x < image.width() && y >= 0 && y < image.height())
        {
            QColor pointColor;
            if (point.cluster != -1)
                pointColor = clusterColors[point.cluster % clusterColors.size()];
            else
                pointColor = Qt::red;
            image.setPixel(x, y, pointColor.rgba());
        }
    }

    for (double level = 0.0; level <= 1.0; level += 0.1)
    {
        QVector<QLineF> lines = generateContourLines(grid, level);

        painter.setPen(QPen(Qt::black, 2));
        for (const QLineF &line : lines)
            painter.drawLine(
                QPointF(line.x1() * scaleX, line.y1() * scaleY),
                QPointF(line.x2() * scaleX, line.y2() * scaleY)
            );
    }

    QPainter widgetPainter(this);
    widgetPainter.drawImage(0, 0, image);
}

QRectF ContourMapFrame::calculateBounds(const QVector<Point> &points) const
{
    if (points.isEmpty())
        return QRectF();

    double minX = points[0].x;
    double minY = points[0].y;
    double maxX = minX;
    double maxY = minY;

    for (const Point &point : points)
    {
        if (point.x < minX)
            minX = point.x;
        if (point.y < minY)
            minY = point.y;
        if (point.x > maxX)
            maxX = point.x;
        if (point.y > maxY)
            maxY = point.y;
    }

    return QRectF(minX, minY, maxX - minX, maxY - minY);
}

QVector<QVector<double>> ContourMapFrame::interpolateData(const QVector<Point> &points, int gridSize)
{
    QVector<QVector<double>> grid(gridSize, QVector<double>(gridSize, 0.0));

    bounds = calculateBounds(points);
    qreal scaleX = gridSize / bounds.width();
    qreal scaleY = gridSize / bounds.height();

    for (const Point &point : points)
    {
        int x = static_cast<int>((point.x - bounds.left()) * scaleX);
        int y = static_cast<int>((point.y - bounds.top()) * scaleY);

        if (x >= 0 && x < gridSize && y >= 0 && y < gridSize)
            grid[x][y] = point.attributes[0];
    }

    return grid;
}

QVector<QLineF> ContourMapFrame::generateContourLines(const QVector<QVector<double>> &grid, double level)
{
    QVector<QLineF> lines;

    int rows = grid.size();
    int cols = grid[0].size();

    for (int i = 0; i < rows - 1; ++i)
    {
        for (int j = 0; j < cols - 1; ++j)
        {
            double v1 = grid[i][j];
            double v2 = grid[i + 1][j];
            double v3 = grid[i + 1][j + 1];
            double v4 = grid[i][j + 1];

            int caseId = 0;
            if (v1 > level)
                caseId |= 1;
            if (v2 > level)
                caseId |= 2;
            if (v3 > level)
                caseId |= 4;
            if (v4 > level)
                caseId |= 8;

            switch (caseId)
            {
                case 1:
                case 14:
                    lines.append(QLineF(i, j, i + 0.5, j));
                    break;
                case 2:
                case 13:
                    lines.append(QLineF(i + 1, j, i + 1, j + 0.5));
                    break;
                case 3:
                case 12:
                    lines.append(QLineF(i, j + 0.5, i + 1, j + 0.5));
                    break;
                case 4:
                case 11:
                    lines.append(QLineF(i + 1, j + 1, i + 0.5, j + 1));
                    break;
                case 5:
                    lines.append(QLineF(i, j, i + 0.5, j));
                    lines.append(QLineF(i + 1, j + 1, i + 0.5, j + 1));
                    break;
                case 6:
                case 9:
                    lines.append(QLineF(i + 0.5, j, i + 0.5, j + 1));
                    break;
                case 7:
                case 8:
                    lines.append(QLineF(i, j + 0.5, i + 0.5, j + 1));
                    break;
                case 10:
                    lines.append(QLineF(i + 1, j, i + 1, j + 0.5));
                    lines.append(QLineF(i, j + 1, i + 0.5, j + 1));
                    break;
            }
        }
    }

    return lines;
}
