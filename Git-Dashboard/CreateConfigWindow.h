#pragma once

#include <QMainWindow>
#include <QItemSelectionModel>

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
    void on_mainWindowPB_clicked();

    void on_rowsSpin_valueChanged(int arg1);
    void on_columnsSpin_valueChanged(int arg1);

    void on_upPB_clicked();
    void on_downPB_clicked();
    void on_removePB_clicked();

    void on_addRepoPB_clicked();

    void selectionChanged(const QItemSelection &, const QItemSelection &);

private:
    Ui::CreateConfigWindow *ui;
    GenericItemModel<Repository> model;
    std::string startFromDir;
    int selectedRow = -1;

    QString getStartingDirectory();
    void rememberParent(const QString & dirName);

    void fixButtons();
};

