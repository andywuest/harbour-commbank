#include "commbankservice.h"
#include "constants.h"
#include "credentials.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
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
  // TODO replace with better UUID generator in later APIs
  return QUuid::createUuid().toString().replace("{", "").replace("}", "").replace("-", "");
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

void CommbankService::logResponseHeaders(QNetworkReply *reply) {
    QList<QByteArray> headerList = reply->rawHeaderList();
    foreach(QByteArray head, headerList) {
        qDebug() << head << ":" << reply->rawHeader(head);
    }
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

    logResponseHeaders(reply);

    processExecuteResourceOwnerPasswordCredentialsFlowResult(reply->readAll());
}

void CommbankService::processExecuteResourceOwnerPasswordCredentialsFlowResult(QByteArray responseData) {
    QJsonDocument jsonDocument = QJsonDocument::fromJson(responseData);
    if (!jsonDocument.isObject()) {
        qDebug() << "not a json object!";
    }

    QJsonObject responseObject = jsonDocument.object();
    this->accessToken = responseObject["access_token"].toString();
    this->refreshToken = responseObject["refresh_token"].toString();
    QString tokenType = responseObject["token_type"].toString();
    QString scope = responseObject["scope"].toString();
    QString kdnr = responseObject["kdnr"].toString();
    QString bpid = responseObject["bpid"].toString();
    QString kontaktId = responseObject["kontaktId"].toString();
    int expiresIn = responseObject["expires_in"].toInt();

    QString result = QString(responseData);
    qDebug() << "CommbankService::parseExecuteResourceOwnerPasswordCredentialsFlowResult => " << responseData;

    qDebug() << " access token : " << this->accessToken;
    qDebug() << " refresh token : " << this->refreshToken;
    qDebug() << " expires in : " << expiresIn;

    executeGetSessionStatus(QUrl(URL_SESSIONS));
}

void CommbankService::executeGetSessionStatus(const QUrl &url) {
    qDebug() << "CommbankService::executeGetSessionStatus " << url;

    this->sessionId = createSessionId();
    this->requestId = createRequestId();

    qDebug() << "sessionId : " << this->sessionId;
    qDebug() << "requestId : " << this->requestId;

    QString requestInfoString = QString("{\"clientRequestId\":{\"sessionId\":\"{{%1}}\",\"requestId\":\"{{%2}}\"}}")
            .arg(sessionId, requestId);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, MIME_TYPE_JSON);
    request.setRawHeader("Accept", MIME_TYPE_JSON);
    request.setRawHeader("Authorization", QString("Bearer ").append(this->accessToken).toUtf8());
    request.setRawHeader("x-http-request-info", requestInfoString.toUtf8());

    QNetworkReply *reply = networkAccessManager->get(request);

    connectErrorSlot(reply);
    connect(reply, SIGNAL(finished()), this, SLOT(handleGetSessionStatusFinished()));
}

void CommbankService::handleGetSessionStatusFinished() {
    qDebug() << "CommbankService::handleGetSessionStatusFinished";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    logResponseHeaders(reply);

    processGetSessionStatusResult(reply->readAll());
}

void CommbankService::processGetSessionStatusResult(QByteArray responseData) {
    QJsonDocument jsonDocument = QJsonDocument::fromJson(responseData);
    if (!jsonDocument.isArray()) {
        qDebug() << "not a json array!";
    }

    QJsonArray responseArray = jsonDocument.array();
    if (responseArray.size() > 0) {
        QJsonObject firstObject = responseArray.at(0).toObject();
        this->identifier = firstObject["identifier"].toString();
        bool sessionTanActive = firstObject["sessionTanActive"].isBool();
        bool activated2FA = firstObject["activated2FA"].isBool();

        qDebug() << " identifier : " << this->identifier;
        qDebug() << " sessionTanActive : " << sessionTanActive;
        qDebug() << " activated2FA : " << activated2FA;

    } else {

        qWarning () << "unexpected number of elements in json array !";
    }

    QString result = QString(responseData);
    qDebug() << "CommbankService::processGetSessionStatusResult => " << responseData;

    executeCreateSessionTan(QUrl(QString(URL_CREATE_SESSION_TAN).arg(this->identifier)));
}

void CommbankService::executeCreateSessionTan(const QUrl &url) {
    qDebug() << "CommbankService::executeCreateSessionTan " << url;

    qDebug() << "sessionId : " << this->sessionId;
    qDebug() << "requestId : " << this->requestId;

    QString requestInfoString = QString("{\"clientRequestId\":{\"sessionId\":\"{{%1}}\",\"requestId\":\"{{%2}}\"}}")
            .arg(sessionId, requestId);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, MIME_TYPE_JSON);
    request.setRawHeader("Accept", MIME_TYPE_JSON);
    request.setRawHeader("Authorization", QString("Bearer ").append(this->accessToken).toUtf8());
    request.setRawHeader("x-http-request-info", requestInfoString.toUtf8());

    QJsonObject body;
    body.insert("identifier", this->identifier);
    body.insert("sessionTanActive", true);
    body.insert("activated2FA", true);

    QJsonDocument bodyDocument;
    bodyDocument.setObject(body);
    QString bodyString(bodyDocument.toJson());

    qDebug() << "request body is : " << bodyString;

    QNetworkReply *reply = networkAccessManager->post(request, bodyString.toUtf8());

    connectErrorSlot(reply);
    connect(reply, SIGNAL(finished()), this, SLOT(handleCreateSessionTanFinished()));
}

void CommbankService::handleCreateSessionTanFinished() {
    qDebug() << "CommbankService::handleCreateSessionTanFinished";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    logResponseHeaders(reply);

    processCreateSessionTanResult(reply->readAll(), reply);
}

void CommbankService::processCreateSessionTanResult(QByteArray responseData, QNetworkReply *reply) {
    QJsonDocument jsonDocumentBody = QJsonDocument::fromJson(responseData);
    if (!jsonDocumentBody.isObject()) {
        qDebug() << "response body not a json object!";
    }

    QByteArray jsonResponseHeader = reply->rawHeader(QByteArray("x-once-authentication-info"));
    QJsonDocument jsonDocumentHeader = QJsonDocument::fromJson(jsonResponseHeader);
    if (!jsonDocumentHeader.isObject()) {
        qDebug() << "response header not a json object!";
    }

    QJsonObject headerObject = jsonDocumentHeader.object();
    QString challengeId = headerObject["id"].toString();
    QString challengeType = headerObject["typ"].toString();
    QString challenge = headerObject["challenge"].toString();
    QJsonArray availableTypesArray = headerObject["availableTypes"].toArray();

    qDebug() << " challengeId : " << challengeId;
    qDebug() << " challengeType : " << challengeType;
    qDebug() << " challenge : " << challenge;
    qDebug() << " availableTypes : " << availableTypesArray;

    if (QString("P_TAN").compare(challengeType) == 0
            || QString("M_TAN").compare(challengeType) == 0) {
        emit challengeRequired(challenge, challengeType);
    } else {
       // TODO process push here -> accept happens in app
    }

    QString result = QString(responseData);
    qDebug() << "CommbankService::processCreateSessionTanResult => " << responseData;
}

