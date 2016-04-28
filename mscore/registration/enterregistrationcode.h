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
