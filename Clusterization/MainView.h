#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QFrame>

class QAbstractButton;
class QGraphicsScene;
class QGraphicsView;

struct Point {
    double x;
    double y;
    QVector<double> attributes;
};


class MainView: public QFrame
{
    Q_OBJECT
public:
    MainView(QWidget *parent = nullptr);

    void showHeatMap(const QVector<Point> &points);
    void drawContours(const QVector<Point> &points);

signals:
    void openFileClicked(const QString &path);

private slots:
    void onOpenFileClicked();

private:
    void setUpUi();
    void setUpConnections();

    QColor getColorFromValue(double value);
    void switchMap();

    QAbstractButton *buttonOpenFile;
    QAbstractButton *buttonSwithMaps;

    QGraphicsScene *sceneHeatMap;
    QGraphicsView *viewHeatMap;

    QGraphicsScene *sceneCountur;
    QGraphicsView *viewCountur;
};

#endif // MAINVIEW_H
