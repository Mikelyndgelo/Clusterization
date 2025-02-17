#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QFrame>

#include "Point.h"

class QAbstractButton;
class QGraphicsScene;
class QGraphicsView;
class QSpinBox;
class ContourMapFrame;
class HeatMapFrame;


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

    HeatMapFrame *heatMap;
    ContourMapFrame *counturMap;
};

#endif // MAINVIEW_H
