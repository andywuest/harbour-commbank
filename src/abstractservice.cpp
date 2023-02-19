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
#include "abstractservice.h"
#include "constants.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

AbstractService::AbstractService(QNetworkAccessManager *networkAccessManager,
                                 QObject *parent,
                                 SessionContext *sessionContext)
    : QObject(parent) {
  qDebug() << "Initializing AbstractService...";
  this->networkAccessManager = networkAccessManager;
  this->sessionContext = sessionContext;
}

AbstractService::~AbstractService() {
  qDebug() << "Shutting down AbstractService...";
}

void AbstractService::logRequest(const QUrl url, QNetworkRequest request,
                                 QByteArray payload) {
  QString logMessage;
  logMessage = logMessage.append(">> ======= REQUEST - (%1) START ==== \n")
                   .arg("info"); // TODO
  logMessage = logMessage.append("URL: %1 \n").arg(url.toString());
  if (payload != nullptr) {
    QJsonDocument jsonDocument = QJsonDocument::fromJson(payload);
    if (jsonDocument.isArray() || jsonDocument.isObject()) {
      logMessage = logMessage.append("Payload : \n%1\n")
                       .arg(jsonDocument.toJson(QJsonDocument::Indented)
                                .toStdString()
                                .c_str());
    } else {
      logMessage =
          logMessage.append("Payload : \n'%1'\n").arg(QString(payload));
    }
  }

  // TODO filter
  QList<QString> relevantHeaders = {"Content-Type",
                                    "x-once-authentication-info",
                                    "x-http-request-info", "Authorization"};

  foreach (const QByteArray &head, request.rawHeaderList()) {
    if (relevantHeaders.contains(QString(head))) {
      logMessage = logMessage.append(" * header [%1] = %2\n")
                       .arg(QString(head), QString(request.rawHeader(head)));
    }
  }

  logMessage = logMessage.append(" * sessionContext [accessTokenLogin]  = %1\n")
                   .arg(sessionContext->getAccessTokenLogin());
  logMessage = logMessage.append(" * sessionContext [refreshTokenLogin] = %1\n")
                   .arg(sessionContext->getRefreshTokenLogin());
  logMessage = logMessage.append(" * sessionContext [accessToken]       = %1\n")
                   .arg(sessionContext->getAccessToken());
  logMessage = logMessage.append(" * sessionContext [refreshToken]      = %1\n")
                   .arg(sessionContext->getRefreshToken());
  logMessage = logMessage.append(" * sessionContext [sessionId]         = %1\n")
                   .arg(sessionContext->getSessionId());
  logMessage = logMessage.append(" * sessionContext [requestId]         = %1\n")
                   .arg(sessionContext->getRequestId());

  logMessage = logMessage.append(">> ======= REQUEST - END   ====");
  qDebug().noquote() << logMessage;
}

void AbstractService::logResponse(QString info, QNetworkReply *reply,
                                  QJsonDocument jsonDocument) {
  QString logMessage;
  logMessage =
      logMessage.append("<< ======= RESPONSE - (%1) START ==== \n").arg(info);
  logMessage = logMessage.append("Payload : \n%1\n")
                   .arg(jsonDocument.toJson(QJsonDocument::Indented)
                            .toStdString()
                            .c_str());

  // TODO filter
  QList<QString> relevantHeaders = {"Content-Type",
                                    "x-once-authentication-info",
                                    "x-http-request-info", "Authorization"};

  foreach (const QByteArray &head, reply->rawHeaderList()) {
    if (relevantHeaders.contains(QString(head))) {
      logMessage = logMessage.append(" * header [%1]  = %2\n")
                       .arg(QString(head), QString(reply->rawHeader(head)));
    }
  }

  logMessage = logMessage.append("<< ======= RESPONSE - END   ====");
  qDebug().noquote() << logMessage;
}

QNetworkRequest AbstractService::prepareNetworkRequest(const QUrl url,
                                                       bool contentTypeJson) {
  QNetworkRequest request(url);
  if (contentTypeJson) {
    request.setHeader(QNetworkRequest::ContentTypeHeader, MIME_TYPE_JSON);
  } else {
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      MIME_TYPE_WWW_FORM_URL_ENCODED);
  }
  request.setRawHeader("Accept", MIME_TYPE_JSON);
  request.setHeader(QNetworkRequest::UserAgentHeader, USER_AGENT);
  return request;
}

void AbstractService::connectErrorSlot(QNetworkReply *reply) {
  // connect the error and also emit the error signal via a lambda expression
  connect(reply,
          static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(
              &QNetworkReply::error),
          [=](QNetworkReply::NetworkError error) {
            const QByteArray &result = reply->readAll();
            reply->deleteLater();

            qWarning() << "AbstractService::handleRequestError:"
                       << static_cast<int>(error) << reply->errorString()
                       << result;

            // TODO -> move to separate method
            QJsonDocument jsonDocument = QJsonDocument::fromJson(result);
            if (jsonDocument.isObject()) {
              QJsonObject responseObject = jsonDocument.object();

              // responses do not always look the same
              if (!responseObject["error_description"].toString().isEmpty()) {
                QString errorDescription =
                    responseObject["error_description"].toString();
                qWarning() << "error description : " << errorDescription;
                emit requestError(errorDescription);
              } else if (!responseObject["messages"].toArray().isEmpty()) {
                QJsonArray messageArray = responseObject["messages"].toArray();
                QString errorDescription =
                    messageArray.at(0).toObject()["message"].toString();
                qWarning() << "error description : " << errorDescription;
                emit requestError(errorDescription);
              }
            } else {
              emit requestError(
                  "Return code: " + QString::number(static_cast<int>(error)) +
                  " - " + reply->errorString());
            }
          });
}
