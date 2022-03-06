#ifndef COMMBANKSERVICE_H
#define COMMBANKSERVICE_H

#include <QNetworkAccessManager>
#include <QNetworkConfigurationManager>
#include <QNetworkReply>
#include <QObject>
#include <QSettings>
#include <QUrlQuery>

class CommbankService : public QObject {
  Q_OBJECT

public:
  explicit CommbankService(QObject *parent = nullptr);
  ~CommbankService() = default;
  Q_INVOKABLE void performLogin(const QString &loginData);
  Q_INVOKABLE void sendChallengeResponse(const QString &challengeResponse);

  Q_SIGNAL void loginResultAvailable(const QString &reply);
  Q_SIGNAL void requestError(const QString &errorMessage);
  Q_SIGNAL void challengeRequired(const QString &challenge,
                                  const QString &challengeType);

signals:

public slots:

private:
  QNetworkAccessManager *networkAccessManager;
  QNetworkConfigurationManager *const networkConfigurationManager;

  QSettings settings;

  QString accessToken;
  QString refreshToken;
  QString sessionId;
  QString requestId;
  QString identifier;
  QString challengeId;
  QString challengeType;

  QString createRequestId();
  QString createSessionId();

  void connectErrorSlot(QNetworkReply *reply);

  void logResponseHeaders(QNetworkReply *reply);

  void executeResourceOwnerPasswordCredentialsFlow(const QUrl &url);
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

#endif // COMMBANKSERVICE_H
