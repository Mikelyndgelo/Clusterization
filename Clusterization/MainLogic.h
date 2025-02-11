#ifndef MAINLOGIC_H
#define MAINLOGIC_H

#include <QObject>
#include "MainView.h"


class MainLogic: public QObject
{
    Q_OBJECT
public:
    MainLogic();

private slots:
    void openFile(const QString &path);
    void onClusterizationClicked(const int countClusters,
                                 const int maxIterations);
    void saveFile(const QString& filename);

private:
    MainView *mainView;

    void setUpConnections();
    void clustering(QVector<Point> &points,
                    int clusterCount, int maxIterations);

    QVector<Point> m_points;
};

#endif // MAINLOGIC_H
