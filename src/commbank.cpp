#include "commbank.h"

Commbank::Commbank(QObject *parent)
    : QObject(parent)
    , networkAccessManager(new QNetworkAccessManager(this))
    , networkConfigurationManager(new QNetworkConfigurationManager(this))
    , settings("harbour-commbank", "settings") {
    commbankAccountService = new CommbankAccountService(this->networkAccessManager, this, &sessionContext);
    commbankLoginService = new CommbankLoginService(this->networkAccessManager, this, &sessionContext);
}

CommbankAccountService *Commbank::getCommbankAccountService() {
    return this->commbankAccountService;
}

CommbankLoginService *Commbank::getCommbankLoginService() {
    return this->commbankLoginService;
}
