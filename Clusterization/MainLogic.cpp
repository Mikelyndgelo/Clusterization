#include "MainLogic.h"

#include "MainView.h"

#include <QFile>
#include <QTextStream>


MainLogic::MainLogic()
{
    mainView = new MainView();
    mainView->show();

    setUpConnections();
}

void MainLogic::setUpConnections()
{
    connect(mainView, &MainView::openFileClicked, this, &MainLogic::openFile);
}

void MainLogic::openFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QTextStream textStream(&file);
    m_points.clear();

    while (!textStream.atEnd()) {
        QString line = textStream.readLine().trimmed();
        if (line.isEmpty())
            continue;

        QStringList parts = line.split(' ');
        if (parts.size() < 2)
            continue;

        Point point;
        point.x = parts[0].toDouble();
        point.y = parts[1].toDouble();

        for (int i = 2; i < parts.size(); ++i)
            point.attributes.append(parts[i].toDouble());

        m_points.append(point);
    }
    file.close();

    mainView->showHeatMap(m_points);
    mainView->drawContours(m_points);
}
