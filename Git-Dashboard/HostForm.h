#pragma once

#include <QWidget>
#include "Configuration.h"

namespace Ui {
class HostForm;
}

class HostForm : public QWidget
{
    Q_OBJECT

public:
    explicit HostForm(Repository::Pointer repo, QWidget *parent = nullptr);
    ~HostForm();

private:
    Ui::HostForm *ui;

    Repository::Pointer repository;
};

