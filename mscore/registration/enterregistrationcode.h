//=============================================================================
//  Virtual Guitar Player
//  Copyright (C) 2016 Larry Jones
//  Developed by Florin Braghis
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================

#ifndef ENTERREGISTRATIONCODE_H
#define ENTERREGISTRATIONCODE_H

#include <QDialog>
#include "registrationmodel.h"

namespace Ui {
class EnterRegistrationCode;
}

namespace vg
{
    class EnterRegistrationCode : public QDialog
    {
        Q_OBJECT

    public:
        explicit EnterRegistrationCode(RegistrationModel& m, QWidget *parent = 0);
        ~EnterRegistrationCode();

    private slots:
        void on_cancelButton_clicked();
        void on_validateButton_clicked();
        void on_regStarting();
    private:
        Ui::EnterRegistrationCode *ui;
        RegistrationModel& model;
    };
}

#endif // ENTERREGISTRATIONCODE_H
