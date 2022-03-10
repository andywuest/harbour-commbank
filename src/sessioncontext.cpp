#include "sessioncontext.h"

#include <QUuid>
#include <QDateTime>
#include <QtCore/qdebug.h>

QString SessionContext::createRequestId() {
  return QString::number(QDateTime::currentMSecsSinceEpoch()).right(9);
}

QString SessionContext::createSessionId() {
  // TODO replace with better UUID generator in later APIs
  return QUuid::createUuid()
      .toString()
      .replace("{", "")
      .replace("}", "")
      .replace("-", "");
}

QString SessionContext::createRequestInfoString() {
    if (requestId.isEmpty()) {
        this->requestId = createRequestId();
    }
    if (sessionId.isEmpty()) {
        this->sessionId = createSessionId();
    }

    qDebug() << "SessionContext::sessionId : " << this->sessionId;
    qDebug() << "SessionContext::requestId : " << this->requestId;

    return QString("{\"clientRequestId\":{\"sessionId\":\"{{%1}}\",\"requestId\":\"{{%2}}\"}}")
                                    .arg(this->sessionId, this->requestId);
}

QString SessionContext::getAccessTokenLogin() {
    return accessTokenLogin;
}

void SessionContext::setAccessTokenLogin(const QString &accessTokenLogin) {
    this->accessTokenLogin = accessTokenLogin;
}

QString SessionContext::getAccessToken() {
    return accessToken;
}

void SessionContext::setAccessToken(const QString &accessToken) {
    this->accessToken = accessToken;
}

QString SessionContext::getRefreshTokenLogin() {
    return refreshTokenLogin;
}

void SessionContext::setRefreshTokenLogin(const QString &refreshTokenLogin) {
    this->refreshTokenLogin = refreshTokenLogin;
}

QString SessionContext::getRefreshToken() {
    return refreshToken;
}

void SessionContext::setRefreshToken(const QString &refreshToken) {
    this->refreshToken = refreshToken;
}


