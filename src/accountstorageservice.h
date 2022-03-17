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
#ifndef ACCOUNT_STORAGE_SERVICE_H
#define ACCOUNT_STORAGE_SERVICE_H

#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

#include <Sailfish/Secrets/createcollectionrequest.h>
#include <Sailfish/Secrets/deletecollectionrequest.h>
#include <Sailfish/Secrets/deletesecretrequest.h>
#include <Sailfish/Secrets/storedsecretrequest.h>
#include <Sailfish/Secrets/storesecretrequest.h>

class AccountStorageService : public QObject {
  Q_OBJECT
public:
  explicit AccountStorageService(QObject *parent = nullptr);
  ~AccountStorageService() = default;

  Q_INVOKABLE void loadSavedAccountData();
  Q_SIGNAL void savedAccountDataAvailable(QString);

  void resetAccounts();
  void storeAccountCredentials(QJsonObject accountCredentials);
  void deleteAccountCredentials(const QString &accountName);
  QJsonDocument loadAllAccountNames();
  QJsonObject loadAccountCredentials(const QString &userName);
  QJsonObject loadAccountCredentials();

  enum AccountStorageError {
    ErrorNone,
    ErrorLoadingAccountData,
    ErrorStoringAccountData,
    ErrorCreatingAccountData,
    ErrorDeletingAccountData
  };
  Q_ENUM(AccountStorageError)

  quint8 error{ErrorNone};

  QString errorString() const;

protected:
  bool isAccountCredentialDataAlreadyStored(QJsonObject);

private:
  void createCollection();
  void deleteCollection();
  void storeCollection();

  // Credentials credentials;
  Sailfish::Secrets::SecretManager secretManager;
  Sailfish::Secrets::Secret::Identifier secretsIdentifier;
};

#endif // ACCOUNT_STORAGE_SERVICE_H
