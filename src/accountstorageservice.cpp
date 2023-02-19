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
#include <Sailfish/Secrets/collectionnamesrequest.h>
#include <Sailfish/Secrets/createcollectionrequest.h>
#include <Sailfish/Secrets/deletecollectionrequest.h>
#include <Sailfish/Secrets/deletesecretrequest.h>
#include <Sailfish/Secrets/storedsecretrequest.h>
#include <Sailfish/Secrets/storesecretrequest.h>

#include <QJsonArray>
#include <QJsonDocument>

#include "accountstorageservice.h"

using namespace Sailfish::Secrets;

const QString WALLET_COLLECTION_NAME =
    QStringLiteral("xx5"); // TODO give final name

AccountStorageService::AccountStorageService(QObject *parent)
    : QObject(parent), secretsIdentifier(Secret::Identifier(
                           QStringLiteral("secrets"), WALLET_COLLECTION_NAME,
                           SecretManager::DefaultEncryptedStoragePluginName)) {
  qDebug() << "Initializing AccountStorageService...";
}

bool AccountStorageService::ensureCollection() {
  CollectionNamesRequest cnr;
  cnr.setManager(&secretManager);
  cnr.setStoragePluginName(SecretManager::DefaultEncryptedStoragePluginName);
  cnr.startRequest();
  cnr.waitForFinished();
  if (checkResult(cnr) &&
      cnr.collectionNames().contains(WALLET_COLLECTION_NAME)) {
    qDebug() << "Collection initialized and available !!";
    return true;
  }

  bool initialized = createCollection();
  if (!initialized) {
    qDebug() << "Collection could not be initialized - storing credentials "
                "unavailable !!";
  } else {
    qDebug() << "Collection successfully initialized !!";
  }
  return initialized;
}

void AccountStorageService::loadSavedAccountData() {
  QString resultString(loadAllAccountNames().toJson());
  emit savedAccountDataAvailable(resultString);
}

void AccountStorageService::deleteAllAccountData() {
  qDebug() << "AccountStorageService::deleteAllAccountData";
  deleteCollection();
}

bool AccountStorageService::checkResult(const Request &req) {
  Result result = req.result();
  bool success = result.errorCode() == Result::NoError;
  if (!success) {
    qDebug() << result.errorMessage();
  }
  return success;
}

bool AccountStorageService::createCollection() {
  qDebug() << "AccountStorageService::createCollection";
  CreateCollectionRequest createCollection;
  createCollection.setManager(&secretManager);
  createCollection.setCollectionLockType(CreateCollectionRequest::DeviceLock);
  createCollection.setDeviceLockUnlockSemantic(
      SecretManager::DeviceLockKeepUnlocked);
  // createCollection.setAccessControlMode(SecretManager::SystemAccessControlMode);
  createCollection.setUserInteractionMode(SecretManager::SystemInteraction);
  createCollection.setCollectionName(WALLET_COLLECTION_NAME);
  createCollection.setStoragePluginName(
      SecretManager::DefaultEncryptedStoragePluginName);
  createCollection.setEncryptionPluginName(
      SecretManager::DefaultEncryptedStoragePluginName);
  createCollection.startRequest();
  createCollection.waitForFinished();

  return checkResult(createCollection);
}

void AccountStorageService::deleteCollection() {
  qDebug() << "AccountStorageService::deleteCollection";
  DeleteCollectionRequest deleteCollection;
  deleteCollection.setManager(&secretManager);
  deleteCollection.setCollectionName(WALLET_COLLECTION_NAME);
  deleteCollection.setStoragePluginName(
      SecretManager::DefaultEncryptedStoragePluginName);
  deleteCollection.setUserInteractionMode(SecretManager::SystemInteraction);
  deleteCollection.startRequest();
  deleteCollection.waitForFinished();

  checkResult(deleteCollection);
}

void AccountStorageService::storeAccountCredentials(
    QJsonObject accountCredentials) {
  qDebug() << "AccountStorageService::storeAccountCredentials";

  QString username = accountCredentials["username"].toString();

  // TODO check if username already exists
  QJsonObject allAccountData = loadAccountCredentials();
  allAccountData.insert(username, accountCredentials);

  qDebug() << "adding accoun to json object : " << allAccountData;

  // reset and create
  deleteCollection();
  createCollection();

  QJsonDocument document;
  document.setObject(allAccountData);
  QString documentString(document.toJson());

  qDebug() << "data to store in collection : " << documentString;

  // store data in wallet
  Secret secret(secretsIdentifier);
  secret.setData(documentString.toUtf8());

  StoreSecretRequest storeCode;
  storeCode.setManager(&secretManager);
  storeCode.setSecretStorageType(StoreSecretRequest::CollectionSecret);
  storeCode.setUserInteractionMode(SecretManager::SystemInteraction);
  storeCode.setSecret(secret);
  storeCode.startRequest();
  storeCode.waitForFinished();

  bool success = checkResult(storeCode);
  if (!success) {
    emit requestError("Error Account credential data");
  }

  loadAccountCredentials();
}

void AccountStorageService::deleteAccountCredentials(
    const QString &accountName) {
  qDebug() << "AccountStorageService::deleteAccountCredentials";
}

QJsonObject AccountStorageService::loadAccountCredentials() {
  qDebug() << "AccountStorageService::loadAccountCredentials - all";

  auto fetchCode = new StoredSecretRequest;

  fetchCode->setManager(&secretManager);
  fetchCode->setUserInteractionMode(SecretManager::SystemInteraction);
  fetchCode->setIdentifier(secretsIdentifier);

  fetchCode->startRequest();
  fetchCode->waitForFinished();

  qDebug() << QStringLiteral(
      "AccountStorageService::loadAccountCredentials Accounts loaded");
  qDebug() << fetchCode->result().code();
  qDebug() << fetchCode->result().errorCode();
  qDebug() << fetchCode->result().errorMessage();

  if (fetchCode->result().code() != Result::Succeeded) {
    QJsonObject emptyObject;
    fetchCode->deleteLater();
    return emptyObject;
  }

  QByteArray data = fetchCode->secret().data();
  QJsonDocument document = QJsonDocument::fromJson(data);

  qDebug() << "data array : " << QString::fromUtf8(data);

  fetchCode->deleteLater();

  return document.object();
}

QJsonObject
AccountStorageService::loadAccountCredentials(const QString &userName) {
  qDebug() << "AccountStorageService::loadAccountCredentials";

  QJsonObject object = loadAccountCredentials();
  QJsonValue objectForAccountName = object[userName];
  if (!objectForAccountName.isNull() && objectForAccountName.isObject()) {
    return objectForAccountName.toObject();
  }

  QJsonObject empty;
  return empty;
}

QJsonDocument AccountStorageService::loadAllAccountNames() {
  qDebug() << "AccountStorageService::loadAllAccountNames";
  QJsonDocument document;
  document.setObject(loadAccountCredentials());
  return document;
}

QString AccountStorageService::errorString() const {
  switch (error) {
  case AccountStorageService::ErrorNone:
    //% "No error"
    return "No error";

  case AccountStorageService::ErrorCreatingAccountData:
    //% "An undefined error occured"
    return "Error Creating account data";

  default:
    return QString();
  }
}
