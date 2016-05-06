#include "trialdialog.h"
#include "ui_trialdialog.h"
#include "enterregistrationcode.h"
#include <QDesktopServices>
#include <QUrl>
#include "registrationmodel.h"
#include <QDebug>

namespace vg
{
    TrialDialog::TrialDialog(RegistrationModel& rm, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::TrialDialog),
        model(rm)
    {
        ui->setupUi(this);

        setWindowTitle("Evaluation version");
        setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowCloseButtonHint);

        if (!model.isApplicationRegistered())
        {
            int numDaysLeft = model.getNumberTrialDaysLeft();
            if (numDaysLeft > 0)
            {
                QString message = QString("You can use the application %1 more days.").arg(numDaysLeft);
                ui->daysLeftLabel->setText(message);
            }
            else
            {
                ui->daysLeftLabel->setText("Evaluation period is over. Please purchase a license to continue using the application.");
                ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Quit");
            }
        }
        else
        {
            qDebug() << "Application already registered, no need to show this dialog!";
        }
    }

    TrialDialog::~TrialDialog()
    {
        delete ui;
    }

    void TrialDialog::on_enterRegCodeButton_clicked()
    {
        EnterRegistrationCode codeDialog(model);
        codeDialog.setModal(true);
        auto code = codeDialog.exec();

        if (code == QDialog::Accepted)
        {
            accept();
            return;
        }
    }

    void TrialDialog::on_purchaseLicenseButton_clicked()
    {
        QString platform = "";
#ifdef Q_OS_WIN
        platform = "win";
#elif defined(Q_OS_DARWIN)
        platform = "osx";
#endif

        QUrl url = QUrl(VGServerURL"/purchase", QUrl::TolerantMode);
        url.setQuery("platform="+platform);
        QDesktopServices::openUrl(url);
    }
}


void vg::TrialDialog::on_buttonBox_accepted()
{
    if (!model.isApplicationRegistered() && model.getNumberTrialDaysLeft() == 0)
    {
        reject();
        return;
    }
    accept();
}
