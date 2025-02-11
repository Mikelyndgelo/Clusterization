#include "SettingClusteringView.h"

#include <QLabel>
#include <QBoxLayout>
#include <QPushButton>


SettingClusteringView::SettingClusteringView(QWidget *parent)
    :QDialog(parent)
{
    setUpUi();
}

void SettingClusteringView::setUpUi()
{
    setMinimumSize(QSize(200, 100));
    setWindowTitle("Параметры кластеризации");

    labelClusterCounts = new QLabel("Количество кластеров");
    countClasters = new QSpinBox();
    countClasters->setRange(1, 10);
    countClasters->setValue(1);

    labelMaxIterations = new QLabel("Макс. количество итераций");

    maxIterations = new QSpinBox();
    maxIterations->setRange(1, 100);
    maxIterations->setValue(100);

    lauchButton = new QPushButton("Запустить кластеризацию");
    connect(lauchButton, &QPushButton::clicked, this, &QDialog::accept);

    QHBoxLayout *layClasters = new QHBoxLayout();
    layClasters->addWidget(labelClusterCounts);
    layClasters->addWidget(countClasters);

    QHBoxLayout *layIterations = new QHBoxLayout();
    layIterations->addWidget(labelMaxIterations);
    layIterations->addWidget(maxIterations);

    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->addLayout(layClasters);
    lay->addLayout(layIterations);
    lay->addWidget(lauchButton);
}
