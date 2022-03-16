#include "commbank.h"

Commbank::Commbank(QObject *parent)
    : QObject(parent), networkAccessManager(new QNetworkAccessManager(this)),
      networkConfigurationManager(new QNetworkConfigurationManager(this)),
      settings("harbour-commbank", "settings") {
  commbankAccountService = new CommbankAccountService(
      this->networkAccessManager, this, &sessionContext);
  commbankLoginService = new CommbankLoginService(this->networkAccessManager,
                                                  this, &sessionContext);
  accountStorageService = new AccountStorageService(this);
  commbankLoginService->accountStorageService = accountStorageService;
}

CommbankAccountService *Commbank::getCommbankAccountService() {
  return this->commbankAccountService;
}

CommbankLoginService *Commbank::getCommbankLoginService() {
  return this->commbankLoginService;
}

AccountStorageService *Commbank::getAccountStorageService() {
  return this->accountStorageService;
}
