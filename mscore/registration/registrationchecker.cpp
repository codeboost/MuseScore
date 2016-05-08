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

#include "registrationchecker.h"
#include "registrationmodel.h"
#include "trialdialog.h"
#include <QDebug>
#include <QApplication>

namespace vg
{

    class RegistrationChecker::Impl : public QObject
    {
    public:
        RegistrationModel registrationModel;
        TrialDialog* trialDialog = nullptr;

        Impl(RegistrationChecker* parent): QObject(parent)
        {

        }
        void showTrialDialog(QWidget* parent)
        {
            trialDialog = new TrialDialog(registrationModel, parent);
            auto code = trialDialog->exec();

            if (code == QDialog::Rejected)
            {
                exit(-1);
            }

        }
    };

    RegistrationChecker::RegistrationChecker(QObject *parent) : QObject(parent)
    {
    }

    void RegistrationChecker::checkRegistration(QWidget *mainWindow)
    {
        if (impl == nullptr)
            impl = new Impl(this);

        impl->registrationModel.load();

        if (impl->registrationModel.isApplicationRegistered())
        {
            qDebug() << "App is registered to: " << impl->registrationModel.getRegistrationName() << " - " << impl->registrationModel.getRegistrationEmail();
            return;
        }

        impl->showTrialDialog(mainWindow);
    }
}

