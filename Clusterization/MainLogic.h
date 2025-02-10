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

private:
    MainView *mainView;

    void setUpConnections();

    QVector<Point> m_points;
};

#endif // MAINLOGIC_H
