#pragma once

#include <QMainWindow>

#include "Configuration.h"
#include "GenericItemModel.h"

namespace Ui {
class CreateConfigWindow;
}

class CreateConfigWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CreateConfigWindow(QWidget *parent = nullptr);
    ~CreateConfigWindow();

    void update();

private slots:
    void on_addRepoPB_clicked();
    void on_mainWindowPB_clicked();

    void on_rowsSpin_valueChanged(int arg1);

    void on_columnsSpin_valueChanged(int arg1);

private:
    Ui::CreateConfigWindow *ui;

    GenericItemModel<Repository> model;
};

