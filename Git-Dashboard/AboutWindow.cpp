#include "AboutWindow.h"
#include "ui_AboutWindow.h"
#include "Version.h"

using std::string;

AboutWindow::AboutWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);

    string vStr = string{"Version "} + version;
    ui->versionL->setText(QString::fromStdString(vStr));
}

AboutWindow::~AboutWindow()
{
    delete ui;
}
