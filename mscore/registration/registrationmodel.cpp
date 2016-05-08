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

#include "registrationmodel.h"
#include <QSettings>
#include <QDate>
#include <QDebug>
#include <QtGlobal>
#include <QTimer>
#include <QtNetwork>
#include <QCryptographicHash>

namespace vg
{
    static const char* RegistrationServerURL = VGServerURL"/api/v1/verifyLicense";

    static const char* vgTrialStartKey = "vgTrialStart";
    static const char* vgRegNumberKey = "vgRegNumber";
    static const char* vgRegNameKey = "vgRegName";
    static const char* vgRegEmailKey = "vgRegEmail";
    //static const char* vgRegTokenKey = "vgRegToken";
    static const char* vgRegDateKey = "vgRegDate";

    struct RegInfo
    {
        QDate trialStartDate;
        QString registrationNumberHash;
        QString registrationName;
        QString registrationEmail;
        QDate   registrationDate;

        void load()
        {
            QSettings settings;

            qDebug() << "Loading registration info from " << settings.fileName();


            if (settings.contains(vgTrialStartKey))
            {
                trialStartDate = settings.value(vgTrialStartKey).toDate();
            }

            if (!trialStartDate.isValid())
            {
                qDebug() << "No trial start date found. Starting trial.";
                trialStartDate = QDate::currentDate();
            }

            qDebug() << "Trial started on: " << trialStartDate;

            //Load registration info
            if (settings.contains(vgRegNumberKey))
            {
                registrationNumberHash = settings.value(vgRegNumberKey).toString();
                registrationName = settings.value(vgRegNameKey).toString();
                registrationEmail = settings.value(vgRegEmailKey).toString();
                registrationDate = settings.value(vgRegDateKey).toDate();
            }

            log();
        }

        void save()
        {

            QSettings settings;
            settings.setValue(vgTrialStartKey, trialStartDate);
            if (registrationNumberHash.length() > 0)
            {
                settings.setValue(vgRegNumberKey, registrationNumberHash);
                settings.setValue(vgRegNameKey, registrationName);
                settings.setValue(vgRegEmailKey, registrationEmail);
                settings.setValue(vgRegDateKey, registrationDate);
            }
            settings.sync();
        }

        void log()
        {
            qDebug() << "trialStartDate: " << trialStartDate;
            qDebug() << "registrationNumber: " << registrationNumberHash;
            qDebug() << "registrationName: " << registrationName;
            qDebug() << "registrationEmail: " << registrationEmail;
            qDebug() << "registrationDate: " << registrationDate;
        }

        void reset()
        {
            registrationNumberHash = "";
            registrationName = "";
            registrationEmail = "";
        }
    };

    class RegistrationModel::Impl : public QObject
    {

    public:
        Impl(RegistrationModel* parent) : QObject(parent)
        {}

        RegInfo regInfo;

        QNetworkAccessManager* networkManager = nullptr;
        QNetworkReply* currentReply = nullptr;
        QString lastNetworkErrorMessage = "";
        QString pendingRegNumberHash;

        QString toSHA256(const QString& s)
        {
            auto result = QCryptographicHash::hash(s.toStdString().c_str(), QCryptographicHash::Sha256);
            return result.toHex();
        }

        void validateRegNumberWithServer(const QString& regNumber)
        {
            regInfo.reset();

            pendingRegNumberHash = toSHA256(regNumber);
            qDebug() << "RegNumber hash: " << pendingRegNumberHash;

            if (networkManager == nullptr)
            {
                networkManager = new QNetworkAccessManager(this);
                connect(networkManager, &QNetworkAccessManager::finished, this, &Impl::onHttpRequestFinished);
            }

            QUrl url(RegistrationServerURL);
            url.setQuery("registrationKeyHash=" + pendingRegNumberHash);

            QNetworkRequest request;
            request.setUrl(url);
            lastNetworkErrorMessage = "";
            currentReply = networkManager->get(request);
        }

        void onHttpRequestFinished(QNetworkReply* reply)
        {
            auto responseData = reply->readAll();

            qDebug() << "Http request finished with code: " << reply->error();
            qDebug() << "Data: " << responseData;

            QString errorMessage = "";
            switch (reply->error())
            {
                case QNetworkReply::NoError:
                {
                    if (!parseSuccessResponse(responseData))
                    {
                        qDebug() << "Invalid reply: " << responseData;
                        errorMessage = "Could not parse server response.";
                    }
                }
                break;

                case QNetworkReply::OperationNotImplementedError:
                {
                    errorMessage = extractServerErrorMessage(responseData);
                }
                break;

                default:
                    errorMessage = extractServerErrorMessage(responseData);
                break;

            }

            if (errorMessage.length() == 0)
                errorMessage = QString("Unkown server error: %1").arg(reply->error());

            lastNetworkErrorMessage = errorMessage;

            auto parentPtr = static_cast<RegistrationModel*>(parent());
            parentPtr->onRegVerificationFinished(errorMessage);
            reply->deleteLater();
        }

        bool parseSuccessResponse(const QByteArray& data)
        {
            QJsonParseError err;
            auto doc = QJsonDocument::fromJson(data, &err);

            if (doc.isNull())
            {
                qDebug() << "Error parsing document: " << err.errorString() << "line: " << err.error;
                return false;
            }

            auto json = doc.object();

            QString status = json["status"].toString();
            if (status == "success")
            {
                regInfo.registrationNumberHash = pendingRegNumberHash;
                regInfo.registrationName = json["name"].toString();
                regInfo.registrationEmail = json["email"].toString();
                regInfo.save();
                regInfo.load();
                return true;
            }
            else
            {
            }
            return false;
        }

        QString extractServerErrorMessage(const QByteArray& data)
        {
            auto doc = QJsonDocument::fromJson(data, nullptr);

            if (doc.isNull())
                return "Could not parse server reply";

            return doc.object()["message"].toString();
        }
    };

    RegistrationModel::RegistrationModel(QObject *parent) : QObject(parent)
    {
        impl = new Impl(this);
    }

    bool RegistrationModel::isApplicationRegistered()
    {
        return impl->regInfo.registrationNumberHash.length() > 0;
    }

    QString RegistrationModel::getRegistrationName() const
    {
        return impl->regInfo.registrationName;
    }

    QString RegistrationModel::getRegistrationEmail() const
    {
        return impl->regInfo.registrationEmail;
    }

    void RegistrationModel::registerWithRegistartionNumber(const QString &regNumber)
    {
        qDebug() << "registerWithRegistartionNumber:" << regNumber;
        emit regVerificationStarting();
        impl->validateRegNumberWithServer(regNumber);
    }

    void RegistrationModel::load()
    {
      impl->regInfo.load();
    }

    int RegistrationModel::getNumberTrialDaysLeft()
    {
        QDate today = QDate::currentDate();
        int daysLeft = numberOfTrialDays - impl->regInfo.trialStartDate.daysTo(today);
        return qMax(0, daysLeft);
    }

    void RegistrationModel::onRegVerificationFinished(const QString& errorMessage)
    {
        emit regVerificationEnded(errorMessage);
    }

}
