
#include "MainWindow.h"
#include "ui_MainWindow.h"

// Yeah, I know, this is naughty.
extern void showConfigWindow();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->menubar->setVisible(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpenConfiguration_triggered()
{
    showConfigWindow();
}
