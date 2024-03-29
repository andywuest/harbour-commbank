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
#include "commbankaccountservice.h"
#include "constants.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

CommbankAccountService::CommbankAccountService(QNetworkAccessManager *manager,
                                               QObject *parent,
                                               SessionContext *sessionContext)
    : AbstractService(manager, parent, sessionContext) {
  qDebug() << "Initializing CommbankAccountService...";
}

void CommbankAccountService::getAllBalances() {
  executeGetAccountBalances(QUrl(URL_ACCOUNT_BALANCES));
}

void CommbankAccountService::getTransactions(const QString &accountId,
                                             int startIndex) {
  executeGetTransactions(
      QUrl(QString(URL_ACCOUNT_TRANSACTIONS).arg(accountId).arg(startIndex)));
}

void CommbankAccountService::executeGetTransactions(const QUrl &url) {
  qDebug() << "CommbankAccountService::executeGetTransactions " << url;

  QNetworkRequest request = prepareNetworkRequest(url, true);
  request.setRawHeader(
      "Authorization",
      QString("Bearer ").append(sessionContext->getAccessToken()).toUtf8());
  request.setRawHeader("x-http-request-info",
                       sessionContext->createRequestInfoString().toUtf8());

  logRequest(url, request, nullptr);

  QNetworkReply *reply = networkAccessManager->get(request);

  connectErrorSlot(reply);
  connect(reply, SIGNAL(finished()), this,
          SLOT(handleGetTransactionsFinished()));
}

void CommbankAccountService::handleGetTransactionsFinished() {
  qDebug() << "CommbankAccountService::handleGetTransactionsFinished";
  QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
  reply->deleteLater();
  if (reply->error() != QNetworkReply::NoError) {
    return;
  }

  processGetTransactionsResult(reply);
}

void CommbankAccountService::processGetTransactionsResult(
    QNetworkReply *reply) {
  qDebug() << "CommbankAccountService::processGetTransactionsResult";
  QByteArray responseData = reply->readAll();
  QJsonDocument jsonDocument = QJsonDocument::fromJson(responseData);
  if (!jsonDocument.isObject()) {
    qDebug() << "response body not a json object!";
  }

  logResponse("body", reply, jsonDocument);

  QString result = QString(responseData);
  emit transactionsResultAvailable(result);
}

void CommbankAccountService::executeGetAccountBalances(const QUrl &url) {
  qDebug() << "CommbankAccountService::executeGetAccountBalances " << url;

  QNetworkRequest request = prepareNetworkRequest(url, true);
  request.setRawHeader(
      "Authorization",
      QString("Bearer ").append(sessionContext->getAccessToken()).toUtf8());
  request.setRawHeader("x-http-request-info",
                       sessionContext->createRequestInfoString().toUtf8());

  logRequest(url, request, nullptr);

  QNetworkReply *reply = networkAccessManager->get(request);

  connectErrorSlot(reply);
  connect(reply, SIGNAL(finished()), this,
          SLOT(handleGetAccountBalancesFinished()));
}

void CommbankAccountService::handleGetAccountBalancesFinished() {
  qDebug() << "CommbankAccountService::handleGetAccountBalancesFinished";
  QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
  reply->deleteLater();
  if (reply->error() != QNetworkReply::NoError) {
    return;
  }

  processGetAccountBalancesResult(reply);
}

void CommbankAccountService::processGetAccountBalancesResult(
    QNetworkReply *reply) {
  qDebug() << "CommbankAccountService::processGetAccountBalancesResult";
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
    QJsonObject accountBalance = value.toObject();
    qDebug() << "accountId: " << accountBalance["accountId"].toString();
    qDebug()
        << "display id: "
        << accountBalance["account"].toObject()["accountDisplayId"].toString();
    qDebug() << "type key: "
             << accountBalance["account"]
                    .toObject()["accountType"]
                    .toObject()["key"]
                    .toString();
    qDebug() << "type text: "
             << accountBalance["account"]
                    .toObject()["accountType"]
                    .toObject()["text"]
                    .toString();
    qDebug() << "balance value: "
             << accountBalance["balanceEUR"].toObject()["value"].toString();
    qDebug() << "balance  unit: "
             << accountBalance["balanceEUR"].toObject()["unit"].toString();

    QJsonObject resultAccount;
    resultAccount.insert("accountId", accountBalance["accountId"].toString());
    resultAccount.insert(
        "accountDisplayId",
        accountBalance["account"].toObject()["accountDisplayId"].toString());
    resultAccount.insert(
        "accountBalanceValue",
        accountBalance["balanceEUR"].toObject()["value"].toString());
    resultAccount.insert(
        "accountBalanceUnit",
        accountBalance["balanceEUR"].toObject()["unit"].toString());
    resultAccount.insert("accountTypeKey", accountBalance["account"]
                                               .toObject()["accountType"]
                                               .toObject()["key"]
                                               .toString());
    resultAccount.insert("accountTypeText", accountBalance["account"]
                                                .toObject()["accountType"]
                                                .toObject()["text"]
                                                .toString());

    resultArray.push_back(resultAccount);
  }

  resultDocument.setArray(resultArray);
  QString dataToString(resultDocument.toJson());

  emit allBalancesResultAvailable(dataToString);
}
