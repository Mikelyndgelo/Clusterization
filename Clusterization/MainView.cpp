#include "MainView.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QColor>
#include <QApplication>


MainView::MainView(QWidget *parent):
    QFrame(parent)
{
    setUpUi();
    setUpConnections();
}

void MainView::setUpUi()
{
    setMinimumSize(QSize(400, 400));
    buttonOpenFile = new QPushButton("Открыть файл");
    buttonSwithMaps = new QPushButton("Сменить карту");

    sceneHeatMap = new QGraphicsScene(this);
    viewHeatMap = new QGraphicsView(sceneHeatMap);

    sceneCountur = new QGraphicsScene(this);
    viewCountur = new QGraphicsView(sceneCountur);
    viewCountur->hide();

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(buttonOpenFile);
    buttonsLayout->addWidget(buttonSwithMaps);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(buttonsLayout);
    layout->addWidget(viewHeatMap);
    layout->addWidget(viewCountur);
}

void MainView::setUpConnections()
{
    connect(buttonOpenFile, &QPushButton::clicked, this, &MainView::onOpenFileClicked);
    connect(buttonSwithMaps, &QPushButton::clicked, this, &MainView::switchMap);
}

void MainView::onOpenFileClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,
            "Выберите файл",
            "",
            "DAT файлы (*.dat)");
    emit openFileClicked(filePath);
}

void MainView::showHeatMap(const QVector<Point> &points)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    sceneHeatMap->clear();
    for (const auto &point: points) {
        QColor color;
        if (!point.attributes.empty())
            color = getColorFromValue(point.attributes[0]);
        else
            color = QColor(Qt::black);
        sceneHeatMap->addEllipse(point.x, point.y, 1, 1, color, QBrush(Qt::white));
    }
    QApplication::restoreOverrideCursor();
}

void MainView::drawContours(const QVector<Point> &points)
{
     QPolygonF contour;
     for (const auto &point : points) {
         contour << QPointF(point.x, point.y);
     }
     sceneCountur->addPolygon(contour, QPen(Qt::black), QBrush(Qt::NoBrush));
}

QColor MainView::getColorFromValue(double value)
{
    return QColor(qBound(0, static_cast<int>(value * 255), 255),
                  qBound(0, 0, 255),
                  qBound(0, 255 - static_cast<int>(value * 255), 255), 255);
}

void MainView::switchMap()
{
    buttonSwithMaps->setEnabled(false);
    if (viewCountur->isHidden())
    {
        viewHeatMap->hide();
        viewCountur->show();
    }
    else
    {
        viewCountur->hide();
        viewHeatMap->show();
    }
    buttonSwithMaps->setEnabled(true);
}


