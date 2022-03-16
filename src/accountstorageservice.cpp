#include <Sailfish/Secrets/createcollectionrequest.h>
#include <Sailfish/Secrets/deletecollectionrequest.h>
#include <Sailfish/Secrets/deletesecretrequest.h>
#include <Sailfish/Secrets/storedsecretrequest.h>
#include <Sailfish/Secrets/storesecretrequest.h>

#include <QJsonArray>
#include <QJsonDocument>

#include "accountstorageservice.h"

const QString WALLET_COLLECTION_NAME =
    QStringLiteral("xx"); // TODO give final name

AccountStorageService::AccountStorageService(QObject *parent)
    : QObject(parent), secretsIdentifier(Sailfish::Secrets::Secret::Identifier(
                           QStringLiteral("secrets"), WALLET_COLLECTION_NAME,
                           Sailfish::Secrets::SecretManager::
                               DefaultEncryptedStoragePluginName)) {
  qDebug() << "Initializing AccountStorageService...";
}

void AccountStorageService::loadSavedAccountData() {
  QString resultString(loadAllAccountNames().toJson());
  emit savedAccountDataAvailable(resultString);
}

void AccountStorageService::resetAccounts() { deleteCollection(); }

void AccountStorageService::createCollection() {
  qDebug() << "AccountStorageService::createCollection";
  Sailfish::Secrets::CreateCollectionRequest createCollection;
  createCollection.setManager(&secretManager);
  createCollection.setCollectionLockType(
      Sailfish::Secrets::CreateCollectionRequest::DeviceLock);
  createCollection.setDeviceLockUnlockSemantic(
      Sailfish::Secrets::SecretManager::DeviceLockKeepUnlocked);
  // createCollection.setAccessControlMode(Sailfish::Secrets::SecretManager::SystemAccessControlMode);
  createCollection.setUserInteractionMode(
      Sailfish::Secrets::SecretManager::SystemInteraction);
  createCollection.setCollectionName(WALLET_COLLECTION_NAME);
  createCollection.setStoragePluginName(
      Sailfish::Secrets::SecretManager::DefaultEncryptedStoragePluginName);
  createCollection.setEncryptionPluginName(
      Sailfish::Secrets::SecretManager::DefaultEncryptedStoragePluginName);
  createCollection.startRequest();
  createCollection.waitForFinished();

  if (createCollection.result().errorCode()) {
    error = AccountStorageError::ErrorCreatingAccountData;
  }

  qDebug() << createCollection.result().code();
  qDebug() << createCollection.result().errorMessage();
}

void AccountStorageService::deleteCollection() {
  qDebug() << "AccountStorageService::deleteCollection";
  Sailfish::Secrets::DeleteCollectionRequest deleteCollection;
  deleteCollection.setManager(&secretManager);
  deleteCollection.setCollectionName(WALLET_COLLECTION_NAME);
  deleteCollection.setStoragePluginName(
      Sailfish::Secrets::SecretManager::DefaultEncryptedStoragePluginName);
  deleteCollection.setUserInteractionMode(
      Sailfish::Secrets::SecretManager::SystemInteraction);
  deleteCollection.startRequest();
  deleteCollection.waitForFinished();

  if (deleteCollection.result().errorCode()) {
    error = AccountStorageError::ErrorDeletingAccountData;
  }

  qDebug() << deleteCollection.result().code();
  qDebug() << deleteCollection.result().errorMessage();
}

void AccountStorageService::storeAccountCredentials(
    QJsonObject accountCredentials) {
  qDebug() << "AccountStorageService::storeAccountCredentials";

  QString username = accountCredentials["username"].toString();

  // TODO check if username already exists
  QJsonObject allAccountData = loadAccountCredentials();
  allAccountData.insert(username, accountCredentials);

  // reset and create
  deleteCollection();
  createCollection();

  QJsonDocument document;
  document.setObject(allAccountData);
  QString documentString(document.toJson());

  // store data in wallet
  Sailfish::Secrets::Secret secret(secretsIdentifier);
  secret.setData(documentString.toUtf8());

  Sailfish::Secrets::StoreSecretRequest storeCode;
  storeCode.setManager(&secretManager);
  storeCode.setSecretStorageType(
      Sailfish::Secrets::StoreSecretRequest::CollectionSecret);
  storeCode.setUserInteractionMode(
      Sailfish::Secrets::SecretManager::SystemInteraction);
  storeCode.setSecret(secret);
  storeCode.startRequest();
  storeCode.waitForFinished();

  if (storeCode.result().errorCode()) {
    error = AccountStorageError::ErrorStoringAccountData;
  }

  qDebug() << storeCode.result().code();
  qDebug() << storeCode.result().errorMessage();
}

void AccountStorageService::deleteAccountCredentials(
    const QString &accountName) {
  qDebug() << "AccountStorageService::deleteAccountCredentials";
}

QJsonObject AccountStorageService::loadAccountCredentials() {
  qDebug() << "AccountStorageService::loadAccountCredentials - all";

  auto fetchCode = new Sailfish::Secrets::StoredSecretRequest;

  fetchCode->setManager(&secretManager);
  fetchCode->setUserInteractionMode(
      Sailfish::Secrets::SecretManager::SystemInteraction);
  fetchCode->setIdentifier(secretsIdentifier);

  fetchCode->startRequest();
  fetchCode->waitForFinished();

  qDebug() << QStringLiteral(
      "AccountStorageService::loadAccountCredentials Accounts loaded");
  qDebug() << fetchCode->result().code();
  qDebug() << fetchCode->result().errorCode();
  qDebug() << fetchCode->result().errorMessage();

  if (fetchCode->result().code() != Sailfish::Secrets::Result::Succeeded) {
    QJsonObject emptyObject;
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
