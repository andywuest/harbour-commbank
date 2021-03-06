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
#ifndef COMMBANK_H
#define COMMBANK_H

#include <QNetworkAccessManager>
#include <QNetworkConfigurationManager>
#include <QObject>
#include <QSettings>

#include "accountstorageservice.h"
#include "commbankaccountservice.h"
#include "commbankbrokerageservice.h"
#include "commbankloginservice.h"

class Commbank : public QObject {
  Q_OBJECT
public:
  explicit Commbank(QObject *parent = nullptr);
  ~Commbank() = default;

  CommbankAccountService *getCommbankAccountService();
  CommbankBrokerageService *getCommbankBrokerageService();
  CommbankLoginService *getCommbankLoginService();
  AccountStorageService *getAccountStorageService();

private:
  QNetworkAccessManager *const networkAccessManager;
  QNetworkConfigurationManager *const networkConfigurationManager;

  CommbankLoginService *commbankLoginService;
  CommbankAccountService *commbankAccountService;
  CommbankBrokerageService *commbankBrokerageService;

  AccountStorageService *accountStorageService;

  QSettings settings;
  SessionContext sessionContext;
};

#endif // COMMBANK_H
