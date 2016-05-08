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

#ifndef REGISTRATIONCHECKER_H
#define REGISTRATIONCHECKER_H

#include <QObject>

namespace vg
{
    class RegistrationChecker : public QObject
    {
        Q_OBJECT
    public:
        explicit RegistrationChecker(QObject *parent = 0);

        void checkRegistration(QWidget* mainWindow);

    private:
        class Impl;
        Impl* impl = nullptr;
    };

}

#endif // REGISTRATIONCHECKER_H
