#ifndef COMMBANKSERVICE_H
#define COMMBANKSERVICE_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkConfigurationManager>
#include <QUrlQuery>
#include <QObject>
#include <QSettings>

class CommbankService : public QObject {
    Q_OBJECT

public:
    explicit CommbankService(QObject *parent = nullptr);
    ~CommbankService() = default;
    Q_INVOKABLE void performLogin(const QString &loginData);

    Q_SIGNAL void loginResultAvailable(const QString &reply);
    Q_SIGNAL void requestError(const QString &errorMessage);

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

    void connectErrorSlot(QNetworkReply *reply);

    void executeResourceOwnerPasswordCredentialsFlow(const QUrl &url);
    void processExecuteResourceOwnerPasswordCredentialsFlowResult(QByteArray replyData);

    void executeGetSessionStatus(const QUrl &url);
    void processGetSessionStatusResult(QByteArray replyData);

private slots:
    void handleExecuteResourceOwnerPasswordCredentialsFlowFinished();
    void handleGetSessionStatusFinished();

};

#endif // COMMBANKSERVICE_H
