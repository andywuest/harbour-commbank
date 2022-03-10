#ifndef COMMBANK_LOGIN_SERVICE_H
#define COMMBANK_LOGIN_SERVICE_H

#include <QNetworkAccessManager>
#include <QNetworkConfigurationManager>
#include <QNetworkReply>
#include <QObject>
#include <QSettings>
#include <QUrlQuery>

#include "abstractservice.h"
#include "sessioncontext.h"

class CommbankLoginService :public AbstractService {
  Q_OBJECT

public:
    explicit CommbankLoginService(QNetworkAccessManager *manager, QObject *parent = nullptr, SessionContext *sessionContext = nullptr);
    ~CommbankLoginService() = default;

  Q_INVOKABLE void performLogin(const QString &clientId, const QString &clientSecret,
                                const QString &username, const QString &password);
  Q_INVOKABLE void sendChallengeResponse(const QString &challengeResponse);

  Q_SIGNAL void loginResultAvailable(const QString &challenge,
                                     const QString &challengeType);
  Q_SIGNAL void challengeResponseAvailable();

signals:

public slots:

private:

  QSettings settings;

  QString identifier;
  QString challengeId;
  QString challengeType;

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

private slots:
  void handleExecuteResourceOwnerPasswordCredentialsFlowFinished();
  void handleGetSessionStatusFinished();
  void handleCreateSessionTanFinished();
  void handleActivateSessionTanFinished();
  void handleCDSecondaryFlowFinished();

};

#endif // COMMBANK_LOGIN_SERVICE_H
