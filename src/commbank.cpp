/**
 * harbour-commbank - Sailfish OS Version
 * Copyright © 2022 Andreas Wüst (andreas.wuest.freelancer@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
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
