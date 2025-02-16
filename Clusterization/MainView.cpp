#include "MainView.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QColor>
#include <QLabel>
#include <QSpinBox>


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
    buttonSave = new QPushButton("Сохранить");

    countClasters = new QSpinBox();
    countClasters->setRange(1, 10);
    countClasters->setValue(1);

    QHBoxLayout *layClasters = new QHBoxLayout();
    layClasters->addWidget(new QLabel("Количество кластеров"));
    layClasters->addWidget(countClasters);

    maxIterations = new QSpinBox();
    maxIterations->setRange(1, 100);
    maxIterations->setValue(100);

    QHBoxLayout *layIterations = new QHBoxLayout();
    layIterations->addWidget(new QLabel("Макс. количество итераций"));
    layIterations->addWidget(maxIterations);

    buttonClusterization = new QPushButton("Запустить кластеризацию");

    sceneHeatMap = new QGraphicsScene(this);
    viewHeatMap = new QGraphicsView(sceneHeatMap);

    sceneCountur = new QGraphicsScene(this);
    viewCountur = new QGraphicsView(sceneCountur);
    viewCountur->hide();

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(buttonOpenFile);
    buttonsLayout->addWidget(buttonSwithMaps);
    buttonsLayout->addWidget(buttonSave);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(buttonsLayout);
    layout->addLayout(layIterations);
    layout->addLayout(layClasters);
    layout->addWidget(buttonClusterization);
    layout->addWidget(viewHeatMap);
    layout->addWidget(viewCountur);
}

void MainView::setUpConnections()
{
    connect(buttonOpenFile, &QPushButton::clicked, this, &MainView::onOpenFileClicked);
    connect(buttonSwithMaps, &QPushButton::clicked, this, &MainView::switchMap);
    connect(buttonClusterization, &QPushButton::clicked, [&](){
        emit clusterizationClicked(countClasters->value(),
                                   maxIterations->value());
    });
    connect(buttonSave, &QPushButton::clicked, this, &MainView::onSaveFileClicked);
}

void MainView::onOpenFileClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,
            "Выберите файл",
            "",
            "DAT файлы (*.dat)");
    if (!filePath.isEmpty())
        emit openFileClicked(filePath);
}

void MainView::drawHeatMap(const QVector<Point> &points)
{
    sceneHeatMap->clear();
    for (const auto &point: points) {
        QColor color;
        if (!point.attributes.empty())
            color = getColorFromValue(point.attributes[0]);
        else
            color = QColor(Qt::black);
        sceneHeatMap->addEllipse(point.x, point.y, 1, 1, color, QBrush(Qt::white));
    }
}

void MainView::drawContours(const QVector<Point> &points)
{
    sceneCountur->clear();
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
    viewHeatMap->setVisible(!viewHeatMap->isVisible());
    viewCountur->setVisible(!viewCountur->isVisible());
    buttonSwithMaps->setEnabled(true);
}

void MainView::onSaveFileClicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "DAT Files (*.dat);)");

    if (!filename.isEmpty()) {
        emit saveFileClicked(filename);
    }
}


