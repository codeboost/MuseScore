#include "enterregistrationcode.h"
#include "ui_enterregistrationcode.h"
#include <QMessageBox>
#include <QDebug>

namespace vg
{
    EnterRegistrationCode::EnterRegistrationCode(RegistrationModel& m, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::EnterRegistrationCode),
        model(m)
    {
        ui->setupUi(this);

        setWindowTitle("Registration");
        setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowCloseButtonHint);

        connect(&model, &RegistrationModel::regVerificationEnded, this, [this](const QString& errMessage) {

            if (!model.isApplicationRegistered())
            {
                QMessageBox::warning(this, "Error", "Could not validate registration number:\n" + errMessage);
                ui->validateButton->setEnabled(true);
            }
            else
            {
                QMessageBox::information(this, "Success", "Registration successful.");
                accept();
                return;
            }
        });

        connect(&model, &RegistrationModel::regVerificationStarting, this, [this]{
            ui->validateButton->setEnabled(false);
        });
    }

    EnterRegistrationCode::~EnterRegistrationCode()
    {
        qDebug() << "Enter registration code destructor";

        delete ui;
    }

    void EnterRegistrationCode::on_cancelButton_clicked()
    {
        reject();
    }

    void EnterRegistrationCode::on_validateButton_clicked()
    {

        QString regNumber = ui->registrationCodeTextEdit->toPlainText().trimmed();
        if (regNumber.length() > 0)
        {
            model.registerWithRegistartionNumber(regNumber);
            ui->validateButton->setEnabled(false);
        }
    }

    void EnterRegistrationCode::on_regStarting()
    {
        ui->validateButton->setEnabled(false);
    }

}


