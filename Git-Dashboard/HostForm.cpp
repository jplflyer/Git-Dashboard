#include "HostForm.h"
#include "ui_HostForm.h"

HostForm::HostForm(Repository::Pointer repo, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HostForm),
    repository(repo)
{
    ui->setupUi(this);
    ui->repoNameL->setText(QString::fromStdString(repo->getDirectory()));
}

HostForm::~HostForm()
{
    delete ui;
}
