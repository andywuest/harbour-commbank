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

void AbstractService::logResponseHeaders(QNetworkReply *reply) {
  QList<QByteArray> headerList = reply->rawHeaderList();
  foreach (QByteArray head, headerList) {
    qDebug() << head << ":" << reply->rawHeader(head);
  }
}

void AbstractService::connectErrorSlot(QNetworkReply *reply) {
  // connect the error and also emit the error signal via a lambda expression
  connect(reply,
          static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(
              &QNetworkReply::error),
          [=](QNetworkReply::NetworkError error) {
            QByteArray result = reply->readAll();

            // TODO test reply->deleteLater();
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
