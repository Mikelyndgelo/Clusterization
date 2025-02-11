#ifndef SETTINGCLUSTERINGVIEW_H
#define SETTINGCLUSTERINGVIEW_H

#include <QDialog>
#include <QSpinBox>

class QLabel;
class QSpinBox;
class QAbstractButton;

class SettingClusteringView: public QDialog
{
    Q_OBJECT
public:
    SettingClusteringView(QWidget *parent = nullptr);

    int getMaxIterations() const { return maxIterations->value(); }
    int getCountClasters() const { return countClasters->value(); }

private:
    QLabel *labelClusterCounts;
    QLabel *labelMaxIterations;
    QSpinBox *countClasters;
    QSpinBox *maxIterations;
    QAbstractButton *lauchButton;

    void setUpUi();
};

#endif // SETTINGCLUSTERINGVIEW_H
