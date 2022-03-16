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
