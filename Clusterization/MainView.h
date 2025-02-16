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

    QGraphicsScene *sceneHeatMap;
    QGraphicsView *viewHeatMap;

    QGraphicsScene *sceneCountur;
    QGraphicsView *viewCountur;

    QSpinBox *maxIterations;
    QSpinBox *countClasters;
};

#endif // MAINVIEW_H
