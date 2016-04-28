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
