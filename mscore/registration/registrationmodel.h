#ifndef REGISTRATIONMODEL_H
#define REGISTRATIONMODEL_H

#include <QObject>

//Define is sometimes really useful, btw :)

#ifdef TESTING_IT
    #define VGServerURL "http://localhost:3000"
#else
    #define VGServerURL "http://virtualguitarp.website"
#endif

namespace vg
{

    class RegistrationModel : public QObject
    {
        Q_OBJECT
    public:
        const int numberOfTrialDays = 14;

        explicit RegistrationModel(QObject *parent = 0);

        bool isApplicationRegistered();

        QString getRegistrationName() const;
        QString getRegistrationEmail() const;

        void registerWithRegistartionNumber(const QString& regNumber);

        void load();
        int getNumberTrialDaysLeft();
    private:
        class Impl;
        friend class Impl;
        Impl* impl;
        void onRegVerificationFinished(const QString& errorMessage);

    signals:
        void regVerificationStarting();
        void regVerificationEnded(const QString& message);

    public slots:
    };

}

#endif // REGISTRATIONMODEL_H
