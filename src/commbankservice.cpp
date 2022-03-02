#include "commbankservice.h"
#include "constants.h"
#include "credentials.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QUuid>

CommbankService::CommbankService(QObject *parent)
    : QObject(parent)
    , networkAccessManager(new QNetworkAccessManager(this))
    , networkConfigurationManager(new QNetworkConfigurationManager(this))
    , settings("harbour-commbank", "settings") {
    qDebug() << "Initializing Data Backend...";
}

QString CommbankService::createRequestId() {
  return QString::number(QDateTime::currentMSecsSinceEpoch()).right(9);
}

QString CommbankService::createSessionId() {
  return QUuid::createUuid().toString();
}

void CommbankService::performLogin(const QString &loginData) {
    qDebug() << "login data : " << loginData;

    // qDebug() << createSessionId();
    // qDebug() << createRequestId();

    executeResourceOwnerPasswordCredentialsFlow(QUrl(URL_OAUTH_TOKEN));
}

void CommbankService::connectErrorSlot(QNetworkReply *reply) {
    // connect the error and also emit the error signal via a lambda expression
    connect(reply,
            static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
            [=](QNetworkReply::NetworkError error) {
                QByteArray result = reply->readAll();
                // TODO test reply->deleteLater();
                qWarning() << "CommbankService::handleRequestError:" << static_cast<int>(error)
                           << reply->errorString() << result;

                QJsonDocument jsonDocument = QJsonDocument::fromJson(result);
                if (jsonDocument.isObject()) {
                    QJsonObject responseObject = jsonDocument.object();
                    QString errorDescription = responseObject["error_description"].toString();
                    qWarning () << "error description : " << errorDescription;
                    emit requestError(errorDescription);
                } else {
                    emit requestError("Return code: " + QString::number(static_cast<int>(error)) + " - "
                                      + reply->errorString());
                }
            });
}

void CommbankService::executeResourceOwnerPasswordCredentialsFlow(const QUrl &url) {
    qDebug() << "CommbankService::executeGetRequest " << url;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, MIME_TYPE_WWW_FORM_URL_ENCODED);
    request.setRawHeader("Accept", MIME_TYPE_JSON);
    request.setHeader(QNetworkRequest::UserAgentHeader, USER_AGENT);

    QUrlQuery postData;
    postData.addQueryItem("client_id", CLIENT_ID);
    postData.addQueryItem("client_secret", CLIENT_SECRET);
    postData.addQueryItem("grant_type", "password");
    postData.addQueryItem("username", USERNAME);
    postData.addQueryItem("password", PASSWORD);

    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    QNetworkReply *reply = networkAccessManager->post(request, postData.toString().toUtf8());

    connectErrorSlot(reply);
    connect(reply, SIGNAL(finished()), this, SLOT(handleExecuteResourceOwnerPasswordCredentialsFlowFinished()));
}

void CommbankService::handleExecuteResourceOwnerPasswordCredentialsFlowFinished() {
    qDebug() << "CommbankService::handleExecuteResourceOwnerPasswordCredentialsFlowFinished";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    processExecuteResourceOwnerPasswordCredentialsFlowResult(reply->readAll());
}

void CommbankService::processExecuteResourceOwnerPasswordCredentialsFlowResult(QByteArray replyData) {
    QJsonDocument jsonDocument = QJsonDocument::fromJson(replyData);
    if (!jsonDocument.isObject()) {
        qDebug() << "not a json object!";
    }

    QJsonObject responseObject = jsonDocument.object();
    this->accessToken = responseObject["access_token"].toString();
    this->refreshToken = responseObject["refreshToken"].toString();
    QString tokenType = responseObject["token_type"].toString();
    QString scope = responseObject["scope"].toString();
    QString kdnr = responseObject["kdnr"].toString();
    QString bpid = responseObject["bpid"].toString();
    QString kontaktId = responseObject["kontaktId"].toString();
    int expiresIn = responseObject["expires_in"].toInt();

    QString result = QString(replyData);
    qDebug() << "CommbankService::parseExecuteResourceOwnerPasswordCredentialsFlowResult => " << replyData;

    qDebug() << " access token : " << this->accessToken;
    qDebug() << " refresh token : " << this->refreshToken;
    qDebug() << " expires in : " << expiresIn;

    executeGetSessionStatus(QUrl(URL_SESSIONS));
}

void CommbankService::executeGetSessionStatus(const QUrl &url) {
    qDebug() << "CommbankService::executeGetSessionStatus " << url;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, MIME_TYPE_JSON);
    request.setRawHeader("Accept", MIME_TYPE_JSON);
    request.setRawHeader("Authorization", QString("Bearer ").append(this->accessToken).toUtf8());
    this->sessionId = createSessionId();
    this->requestId = createRequestId();

    qDebug() << "sessionId : " << this->sessionId;
    qDebug() << "requestId : " << this->requestId;

    request.setRawHeader("x-http-request-info",
                         QString("{\"clientRequestId\":{\"sessionId\":\"{{%1}}\",\"requestId\":\"{{%2}}\"}}")
                         .arg(sessionId, requestId).toUtf8());

    QNetworkReply *reply = networkAccessManager->get(request);

    connectErrorSlot(reply);
    connect(reply, SIGNAL(finished()), this, SLOT(handleGetSessionStatusFinished()));
}

void CommbankService::handleGetSessionStatusFinished() {

}

void CommbankService::processGetSessionStatusResult(QByteArray replyData) {

}

