#pragma once

#include <QMainWindow>
#include <QVector>
#include <QWidget>

#include "HostForm.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void configurationChanged();

private slots:
    void on_actionOpenConfiguration_triggered();

private:
    Ui::MainWindow *ui;

    QVector<QWidget *> rows;
    QVector<HostForm *> hostForms;

    int displayedRows = 0;
    int displayedColumns = 0;
    int timerId = 0;

    void timerEvent(QTimerEvent *event);
};
