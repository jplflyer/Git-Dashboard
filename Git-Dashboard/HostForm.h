#pragma once

#include <QPalette>
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

public slots:
    void update();

private:
    Ui::HostForm *ui;

    //----------------------------------------------------------------------
    // We use nested widgets and set their background colors as indicators
    // of our status.
    //
    //		-Do we have un-pushed commits? Use palEven
    //		-Does origin have unpulled commits? Use palBehind.
    //		-Otherwise we use palEven.
    //
    // We actually have two frames, one for each of the first two conditions,
    // as they can exist simultaneously.
    //
    // And then our main area changes background color to warn me when
    // I'm working on a non-master (or main) branch.
    //
    //		-Branch name is either "master" or "main" -- use palMaster.
    //		-Otherwise use palBranch.
    //----------------------------------------------------------------------
    QPalette palEvenOutside;
    QPalette palAhead;
    QPalette palBehind;
    QPalette palMaster;
    QPalette palBranch;

    Repository::Pointer repository;
};

