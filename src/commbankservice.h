#ifndef COMMBANKSERVICE_H
#define COMMBANKSERVICE_H

#include <QNetworkAccessManager>
#include <QNetworkConfigurationManager>
#include <QNetworkReply>
#include <QObject>
#include <QSettings>
#include <QUrlQuery>

#include "sessioncontext.h"

class CommbankService : public QObject {
  Q_OBJECT

public:
  explicit CommbankService(QObject *parent = nullptr);
  ~CommbankService() = default;
  Q_INVOKABLE void performLogin(const QString &clientId, const QString &clientSecret,
                                const QString &username, const QString &password);
  Q_INVOKABLE void sendChallengeResponse(const QString &challengeResponse);

  Q_SIGNAL void loginResultAvailable(const QString &challenge,
                                     const QString &challengeType);
  Q_SIGNAL void challengeResponseAvailable();
  Q_SIGNAL void requestError(const QString &errorMessage);

  void setSessionContext(SessionContext *sessionContext);

signals:

public slots:

private:
  QNetworkAccessManager *networkAccessManager;
  QNetworkConfigurationManager *const networkConfigurationManager;

  SessionContext *sessionContext;

  QSettings settings;

  QString identifier;
  QString challengeId;
  QString challengeType;

  void connectErrorSlot(QNetworkReply *reply);

  void logResponseHeaders(QNetworkReply *reply);

  void executeResourceOwnerPasswordCredentialsFlow(const QUrl &url, const QString &clientId,
                                                   const QString &clientSecret,
                                                   const QString &username, const QString &password);
  void processExecuteResourceOwnerPasswordCredentialsFlowResult(
      QByteArray responseData);

  void executeGetSessionStatus(const QUrl &url);
  void processGetSessionStatusResult(QByteArray responseData);

  void executeCreateSessionTan(const QUrl &url);
  void processCreateSessionTanResult(QByteArray responseData,
                                     QNetworkReply *reply);

  void executeActivateSessionTan(const QUrl &url, const QString &challengeResponse);
  void processActivateSessionTanResult(QByteArray responseData,
                                     QNetworkReply *reply);

  void executeCDSecondaryFlow(const QUrl &url);
  void processCDSecondaryFlowResult(QByteArray responseData,
                                    QNetworkReply *reply);

  // TODO move to other service class
  void executeGetAccountBalances(const QUrl &url);
  void processGetAccountBalancesResult(QByteArray responseData);


private slots:
  void handleExecuteResourceOwnerPasswordCredentialsFlowFinished();
  void handleGetSessionStatusFinished();
  void handleCreateSessionTanFinished();
  void handleActivateSessionTanFinished();
  void handleCDSecondaryFlowFinished();
  void handleGetAccountBalancesFinished();
};

#endif // COMMBANKSERVICE_H
