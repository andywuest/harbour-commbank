#include "commbankaccountservice.h"
#include "constants.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

CommbankAccountService::CommbankAccountService(QNetworkAccessManager *manager, QObject *parent, SessionContext *sessionContext)
    : AbstractService(manager, parent, sessionContext) {
  qDebug() << "Initializing CommbankAccountService...";
}

void CommbankAccountService::getAllBalances() {
   executeGetAccountBalances(QUrl(URL_ACCOUNT_BALANCES));
}

void CommbankAccountService::executeGetAccountBalances(const QUrl &url) {
    qDebug() << "CommbankAccountService::executeGetAccountBalances " << url;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, MIME_TYPE_JSON);
    request.setRawHeader("Accept", MIME_TYPE_JSON);
    request.setRawHeader("Authorization",
                         QString("Bearer ").append(sessionContext->getAccessToken()).toUtf8());
    request.setRawHeader("x-http-request-info", sessionContext->createRequestInfoString().toUtf8());

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

    logResponseHeaders(reply);

    processGetAccountBalancesResult(reply->readAll());
}

void CommbankAccountService::processGetAccountBalancesResult(QByteArray responseData) {
    QJsonDocument jsonDocumentBody = QJsonDocument::fromJson(responseData);
    if (!jsonDocumentBody.isObject()) {
      qDebug() << "response body not a json object!";
    }

    QString result = QString(responseData);
    qDebug() << "CommbankAccountService::processGetAccountBalancesResult => "
             << responseData;

    QJsonDocument resultDocument;
    QJsonArray resultArray;


    QJsonArray accountsArray = jsonDocumentBody.object()["values"].toArray();

    foreach (const QJsonValue &value, accountsArray) {
        QJsonObject accountBalance = value.toObject();
        qDebug() << "accountId: " << accountBalance["accountId"].toString();
        qDebug() << "display id: " << accountBalance["account"].toObject()["accountDisplayId"].toString();
        qDebug() << "type key: " << accountBalance["account"].toObject()["accountType"].toObject()["key"].toString();
        qDebug() << "type text: " << accountBalance["account"].toObject()["accountType"].toObject()["text"].toString();
        qDebug() << "balance value: " << accountBalance["balanceEUR"].toObject()["value"].toString();
        qDebug() << "balance  unit: " << accountBalance["balanceEUR"].toObject()["unit"].toString();

        QJsonObject resultAccount;
        resultAccount.insert("accountDisplayId", accountBalance["account"].toObject()["accountDisplayId"].toString());
        resultAccount.insert("accountBalanceValue", accountBalance["balanceEUR"].toObject()["value"].toString());
        resultAccount.insert("accountBalanceUnit", accountBalance["balanceEUR"].toObject()["unit"].toString());
        resultAccount.insert("accountTypeKey", accountBalance["account"].toObject()["accountType"].toObject()["key"].toString());
        resultAccount.insert("accountTypeText", accountBalance["account"].toObject()["accountType"].toObject()["text"].toString());


        resultArray.push_back(resultAccount);

    }

    resultDocument.setArray(resultArray);
    QString dataToString(resultDocument.toJson());

    emit allBalancesResultAvailable(dataToString);

//    {
//      "paging": {
//        "index": 0,
//        "matches": 1
//      },
//      "values": [
//        {
//          "account": {
//            "accountId": "6F91E9A225C74355834F5A0D9A304697",
//            "accountDisplayId": "977869927400",
//            "currency": "EUR",
//            "clientId": "40E61E65146441CEBC3DD7F0C70E7051",
//            "accountType": {
//              "key": "SA",
//              "text": "Verrechnungskonto"
//            },
//            "iban": "DE76200411440869927400",
//            "creditLimit": {
//              "value": "0",
//              "unit": "EUR"
//            }
//          },
//          "accountId": "6F91E9A225C74355834F5A0D9A304697",
//          "balance": {
//            "value": "4106.31",
//            "unit": "EUR"
//          },
//          "balanceEUR": {
//            "value": "4106.31",
//            "unit": "EUR"
//          },
//          "availableCashAmount": {
//            "value": "4106.31",
//            "unit": "EUR"
//          },
//          "availableCashAmountEUR": {
//            "value": "4106.31",
//            "unit": "EUR"
//          }
//        }
//      ]
//    }

    //emit challengeResponseAvailable();
}
