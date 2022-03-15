#ifndef COMMBANK_H
#define COMMBANK_H

#include <QNetworkAccessManager>
#include <QNetworkConfigurationManager>
#include <QObject>
#include <QSettings>

#include "commbankaccountservice.h"
#include "commbankloginservice.h"
#include "accountstorageservice.h"

class Commbank : public QObject
{
    Q_OBJECT
public:
    explicit Commbank(QObject *parent = nullptr);
    ~Commbank() = default;

    CommbankAccountService *getCommbankAccountService();
    CommbankLoginService *getCommbankLoginService();
    AccountStorageService *getAccountStorageService();

private:
    QNetworkAccessManager *const networkAccessManager;
    QNetworkConfigurationManager *const networkConfigurationManager;

    CommbankLoginService *commbankLoginService;
    CommbankAccountService *commbankAccountService;

    AccountStorageService *accountStorageService;

    QSettings settings;
    SessionContext sessionContext;
};

#endif // COMMBANK_H
