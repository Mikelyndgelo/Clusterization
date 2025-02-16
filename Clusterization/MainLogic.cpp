#include "MainLogic.h"

#include "MainView.h"

#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QFileDialog>


MainLogic::MainLogic()
{
    mainView = new MainView();
    mainView->show();

    setUpConnections();
}

void MainLogic::setUpConnections()
{
    connect(mainView, &MainView::openFileClicked, this, &MainLogic::openFile);
    connect(mainView, &MainView::clusterizationClicked, this, &MainLogic::onClusterizationClicked);
    connect(mainView, &MainView::saveFileClicked, this, &MainLogic::saveFile);
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

    QApplication::setOverrideCursor(Qt::WaitCursor);
    mainView->drawHeatMap(m_points);
    mainView->drawContours(m_points);
    QApplication::restoreOverrideCursor();
}

void MainLogic::onClusterizationClicked(const int countClusters, const int maxIterations)
{
    mainView->setEnabled(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    auto temp = m_points;
    clustering(m_points, countClusters, maxIterations);
    QApplication::restoreOverrideCursor();
    mainView->setEnabled(true);

    mainView->drawHeatMap(m_points);
    mainView->drawContours(m_points);
}

void MainLogic::clustering(QVector<Point> &points, int clusterCount, int maxIterations)
{
    if (points.isEmpty() || clusterCount <= 0)
        return;

    QVector<Point> centroids;
    for (int i = 0; i < clusterCount; ++i)
    {
        int randomIndex = QRandomGenerator::global()->bounded(points.size());
        centroids.append(points[randomIndex]);
    }

    for (int it = 0; it < maxIterations; ++it)
    {
        bool changed = false;
        for (int i = 0; i < points.size(); ++i)
        {
            double minDistance = std::numeric_limits<double>::max();
            int bestCluster = -1;

            for (int j = 0; j < centroids.size(); ++j)
            {
                double dx = points[i].x - centroids[j].x;
                double dy = points[i].y - centroids[j].y;
                double distance = std::sqrt(dx * dx + dy * dy);

                if (distance < minDistance)
                {
                    minDistance = distance;
                    bestCluster = j;
                }
            }

            if (points[i].cluster != bestCluster)
            {
                points[i].cluster = bestCluster;
                changed = true;
            }
        }

        if (!changed)
            break;

        QVector<Point> newCentroids(clusterCount, {0.0, 0.0, {}, -1});
        QVector<int> counts(clusterCount, 0);

        for (int i = 0; i < points.size(); ++i)
        {
            int cluster = points[i].cluster;
            newCentroids[cluster].x += points[i].x;
            newCentroids[cluster].y += points[i].y;
            counts[cluster]++;
        }

        for (int j = 0; j < clusterCount; ++j)
        {
            if (counts[j] > 0)
            {
                newCentroids[j].x /= counts[j];
                newCentroids[j].y /= counts[j];
            }
        }
        centroids = newCentroids;
    }
    QMessageBox::warning(mainView, "", "Кластеризация завершена, вы можете сохранить результаты");
}

void MainLogic::saveFile(const QString& filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(mainView, "Ошибка", "Ошибка при экспорте данных.");
        return;
    }

    QTextStream out(&file);

    for (const Point& point: m_points) {
        out << point.x << " " << point.y << " " << point.cluster << " ";

        for (int i = 0; i < point.attributes.size(); ++i) {
            out << point.attributes[i];
            if (i < point.attributes.size() - 1) {
                out << " ";
            }
        }
        out << "\n";
    }
    QMessageBox::information(mainView, "Успех", "Данные успешно экспортированы в " + filename);

    file.close();
}
