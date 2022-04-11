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
#include "sessioncontext.h"

#include <QDateTime>
#include <QUuid>
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
    qDebug() << "SessionContext::requestId created : " << this->requestId;
  }
  if (sessionId.isEmpty()) {
    this->sessionId = createSessionId();
    qDebug() << "SessionContext::sessionId created : " << this->sessionId;
  }

  return QString("{\"clientRequestId\":{\"sessionId\":\"{{%1}}\",\"requestId\":"
                 "\"{{%2}}\"}}")
      .arg(this->sessionId, this->requestId);
}

QString SessionContext::getAccessTokenLogin() { return accessTokenLogin; }

void SessionContext::setAccessTokenLogin(const QString &accessTokenLogin) {
  this->accessTokenLogin = accessTokenLogin;
}

QString SessionContext::getAccessToken() { return accessToken; }

void SessionContext::setAccessToken(const QString &accessToken) {
  this->accessToken = accessToken;
}

QString SessionContext::getRefreshTokenLogin() { return refreshTokenLogin; }

void SessionContext::setRefreshTokenLogin(const QString &refreshTokenLogin) {
  this->refreshTokenLogin = refreshTokenLogin;
}

QString SessionContext::getRefreshToken() { return refreshToken; }

QString SessionContext::getRequestId() { return requestId; }

QString SessionContext::getSessionId() { return sessionId; }

void SessionContext::setRefreshToken(const QString &refreshToken) {
  this->refreshToken = refreshToken;
}
