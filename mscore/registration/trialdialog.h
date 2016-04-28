#ifndef TRIALDIALOG_H
#define TRIALDIALOG_H

#include <QDialog>


namespace Ui {
class TrialDialog;
}

namespace vg
{
    class RegistrationModel;
    class TrialDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit TrialDialog(RegistrationModel& regModel, QWidget *parent = 0);
        ~TrialDialog();

    private slots:
        void on_enterRegCodeButton_clicked();

        void on_purchaseLicenseButton_clicked();

        void on_buttonBox_accepted();

    private:
        Ui::TrialDialog *ui;
        RegistrationModel& model;
    };
}

#endif // TRIALDIALOG_H
