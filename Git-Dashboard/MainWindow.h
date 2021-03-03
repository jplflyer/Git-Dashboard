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

private slots:
    void on_actionOpenConfiguration_triggered();

private:
    Ui::MainWindow *ui;

    QVector<QWidget *> rows;
    QVector<HostForm *> hostForms;
};
