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
#include "commbankbrokerageservice.h"

#include "constants.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

CommbankBrokerageService::CommbankBrokerageService(
    QNetworkAccessManager *manager, QObject *parent,
    SessionContext *sessionContext)
    : AbstractService(manager, parent, sessionContext) {
  qDebug() << "Initializing CommbankBrokerageService...";
}

void CommbankBrokerageService::getAllDepots() {
  executeGetDepots(QUrl(URL_BROKERAGE_DEPOTS));
}

void CommbankBrokerageService::executeGetDepots(const QUrl &url) {
  qDebug() << "CommbankBrokerageService::executeGetDepotPositions " << url;

  QNetworkRequest request = prepareNetworkRequest(url, true);
  request.setRawHeader(
      "Authorization",
      QString("Bearer ").append(sessionContext->getAccessToken()).toUtf8());
  request.setRawHeader("x-http-request-info",
                       sessionContext->createRequestInfoString().toUtf8());

  logRequest(url, request, nullptr);

  QNetworkReply *reply = networkAccessManager->get(request);

  connectErrorSlot(reply);
  connect(reply, SIGNAL(finished()), this, SLOT(handleGetDepotsFinished()));
}

void CommbankBrokerageService::handleGetDepotsFinished() {
  qDebug() << "CommbankBrokerageService::handleGetDepotsFinished";
  QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
  reply->deleteLater();
  if (reply->error() != QNetworkReply::NoError) {
    return;
  }

  processGetDepotsResult(reply);
}

void CommbankBrokerageService::processGetDepotsResult(QNetworkReply *reply) {
  qDebug() << "CommbankBrokerageService::processGetDepotsResult";
  QByteArray responseData = reply->readAll();
  QJsonDocument jsonDocument = QJsonDocument::fromJson(responseData);
  if (!jsonDocument.isObject()) {
    qDebug() << "response body not a json object!";
  }

  logResponse("body", reply, jsonDocument);

  QJsonDocument resultDocument;
  QJsonArray resultArray;

  QJsonArray accountsArray = jsonDocument.object()["values"].toArray();

  foreach (const QJsonValue &value, accountsArray) {
    QJsonObject depot = value.toObject();
    qDebug() << "depotId: " << depot["depotId"].toString();
    qDebug() << "display id: " << depot["depotDisplayId"].toString();
    qDebug() << "type key: " << depot["depotType"].toString();
    qDebug() << "holder: " << depot["holderName"].toString();

    QJsonObject resultAccount;
    resultAccount.insert("accountId", depot["depotId"].toString());
    resultAccount.insert("accountDisplayId",
                         depot["depotDisplayId"].toString());
    resultAccount.insert("accountBalanceValue", "");
    resultAccount.insert("accountTypeKey", depot["depotType"].toString());
    resultAccount.insert("accountTypeText",
                         "Depot"); // TODO is this the only one STANDARD_DEPOT

    resultArray.push_back(resultAccount);
  }

  resultDocument.setArray(resultArray);
  QString dataToString(resultDocument.toJson());

  emit allDepotsResultAvailable(dataToString);
}

void CommbankBrokerageService::getPositions(const QString &depotId,
                                            int startIndex) {
  executeGetPositions(
      QUrl(QString(URL_BROKERAGE_POSITIONS).arg(depotId).arg(startIndex)));
}

void CommbankBrokerageService::executeGetPositions(const QUrl &url) {
  qDebug() << "CommbankBrokerageService::executeGetPositions " << url;

  QNetworkRequest request = prepareNetworkRequest(url, true);
  request.setRawHeader(
      "Authorization",
      QString("Bearer ").append(sessionContext->getAccessToken()).toUtf8());
  request.setRawHeader("x-http-request-info",
                       sessionContext->createRequestInfoString().toUtf8());

  logRequest(url, request, nullptr);

  QNetworkReply *reply = networkAccessManager->get(request);

  connectErrorSlot(reply);
  connect(reply, SIGNAL(finished()), this, SLOT(handleGetPositionsFinished()));

  // TODO SLOT in lambda funktion ändern
}

void CommbankBrokerageService::handleGetPositionsFinished() {
  qDebug() << "CommbankBrokerageService::handleGetPositionsFinished";
  QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
  reply->deleteLater();
  if (reply->error() != QNetworkReply::NoError) {
    return;
  }

  processGetPositionsResult(reply);
}

void CommbankBrokerageService::processGetPositionsResult(QNetworkReply *reply) {
  qDebug() << "CommbankBrokerageService::processGetpositionsResult";
  QByteArray responseData = reply->readAll();
  QJsonDocument jsonDocument = QJsonDocument::fromJson(responseData);
  if (!jsonDocument.isObject()) {
    qDebug() << "response body not a json object!";
  }

  logResponse("body", reply, jsonDocument);

  QString result = QString(responseData);
  emit positionsResultAvailable(result);
}
