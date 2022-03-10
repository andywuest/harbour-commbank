#include "commbankservice.h"
#include "constants.h"
#include "credentials.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QBuffer>
#include <QUuid>

CommbankService::CommbankService(QObject *parent)
    : QObject(parent), networkAccessManager(new QNetworkAccessManager(this)),
      networkConfigurationManager(new QNetworkConfigurationManager(this)),
      settings("harbour-commbank", "settings") {
  qDebug() << "Initializing Data Backend...";
}

void CommbankService::setSessionContext(SessionContext *sessionContext) {
    this->sessionContext = sessionContext;
}

void CommbankService::performLogin(const QString &clientId, const QString &clientSecret,
                                   const QString &username, const QString &password) {
  qDebug() << "CommbankService::performLogin : ";

  executeResourceOwnerPasswordCredentialsFlow(QUrl(URL_OAUTH_TOKEN), clientId, clientSecret, username, password);
}

void CommbankService::connectErrorSlot(QNetworkReply *reply) {
  // connect the error and also emit the error signal via a lambda expression
  connect(reply,
          static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(
              &QNetworkReply::error),
          [=](QNetworkReply::NetworkError error) {
            QByteArray result = reply->readAll();
            // TODO test reply->deleteLater();
            qWarning() << "CommbankService::handleRequestError:"
                       << static_cast<int>(error) << reply->errorString()
                       << result;

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
                  QString errorDescription = messageArray.at(0).toObject()["message"].toString();
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

void CommbankService::logResponseHeaders(QNetworkReply *reply) {
  QList<QByteArray> headerList = reply->rawHeaderList();
  foreach (QByteArray head, headerList) {
    qDebug() << head << ":" << reply->rawHeader(head);
  }
}

void CommbankService::executeResourceOwnerPasswordCredentialsFlow(
    const QUrl &url, const QString &clientId, const QString &clientSecret,
        const QString &username, const QString &password) {
  qDebug() << "CommbankService::executeGetRequest " << url;

  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    MIME_TYPE_WWW_FORM_URL_ENCODED);
  request.setRawHeader("Accept", MIME_TYPE_JSON);
  request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
  request.setHeader(QNetworkRequest::UserAgentHeader, USER_AGENT);

  QUrlQuery postData;
  postData.addQueryItem("client_id", clientId);
  postData.addQueryItem("client_secret", clientSecret);
  postData.addQueryItem("grant_type", "password");
  postData.addQueryItem("username", username);
  postData.addQueryItem("password", password);

  QNetworkReply *reply =
      networkAccessManager->post(request, postData.toString().toUtf8());

  connectErrorSlot(reply);
  connect(reply, SIGNAL(finished()), this,
          SLOT(handleExecuteResourceOwnerPasswordCredentialsFlowFinished()));
}

void CommbankService::
    handleExecuteResourceOwnerPasswordCredentialsFlowFinished() {
  qDebug() << "CommbankService::"
              "handleExecuteResourceOwnerPasswordCredentialsFlowFinished";
  QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
  reply->deleteLater();
  if (reply->error() != QNetworkReply::NoError) {
    return;
  }

  logResponseHeaders(reply);

  processExecuteResourceOwnerPasswordCredentialsFlowResult(reply->readAll());
}

void CommbankService::processExecuteResourceOwnerPasswordCredentialsFlowResult(
    QByteArray responseData) {
  QJsonDocument jsonDocument = QJsonDocument::fromJson(responseData);
  if (!jsonDocument.isObject()) {
    qDebug() << "not a json object!";
  }

  QJsonObject responseObject = jsonDocument.object();
  sessionContext->setAccessTokenLogin(responseObject["access_token"].toString());
  sessionContext->setRefreshTokenLogin(responseObject["refresh_token"].toString());

  QString tokenType = responseObject["token_type"].toString();
  QString scope = responseObject["scope"].toString();
  QString kdnr = responseObject["kdnr"].toString();
  QString bpid = responseObject["bpid"].toString();
  QString kontaktId = responseObject["kontaktId"].toString();
  int expiresIn = responseObject["expires_in"].toInt();

  QString result = QString(responseData);
  qDebug() << "CommbankService::"
              "parseExecuteResourceOwnerPasswordCredentialsFlowResult => "
           << responseData;

  qDebug() << " access token (Login) : " << sessionContext->getAccessTokenLogin();
  qDebug() << " refresh token (Login) : " << sessionContext->getRefreshTokenLogin();
  qDebug() << " expires in : " << expiresIn;

  executeGetSessionStatus(QUrl(URL_SESSIONS));
}

void CommbankService::executeGetSessionStatus(const QUrl &url) {
  qDebug() << "CommbankService::executeGetSessionStatus " << url;

  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader, MIME_TYPE_JSON);
  request.setRawHeader("Accept", MIME_TYPE_JSON);
  request.setRawHeader("Authorization",
                       QString("Bearer ").append(sessionContext->getAccessTokenLogin()).toUtf8());
  request.setRawHeader("x-http-request-info", sessionContext->createRequestInfoString().toUtf8());

  QNetworkReply *reply = networkAccessManager->get(request);

  connectErrorSlot(reply);
  connect(reply, SIGNAL(finished()), this,
          SLOT(handleGetSessionStatusFinished()));
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

    qWarning() << "unexpected number of elements in json array !";
  }

  QString result = QString(responseData);
  qDebug() << "CommbankService::processGetSessionStatusResult => "
           << responseData;

  executeCreateSessionTan(
      QUrl(QString(URL_CREATE_SESSION_TAN).arg(this->identifier)));
}

void CommbankService::executeCreateSessionTan(const QUrl &url) {
  qDebug() << "CommbankService::executeCreateSessionTan " << url;

  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader, MIME_TYPE_JSON);
  request.setRawHeader("Accept", MIME_TYPE_JSON);
  request.setRawHeader("Authorization",
                       QString("Bearer ").append(sessionContext->getAccessTokenLogin()).toUtf8());
  request.setRawHeader("x-http-request-info", sessionContext->createRequestInfoString().toUtf8());

  QJsonObject body;
  body.insert("identifier", this->identifier);
  body.insert("sessionTanActive", true);
  body.insert("activated2FA", true);

  QJsonDocument bodyDocument;
  bodyDocument.setObject(body);
  QString bodyString(bodyDocument.toJson());

  qDebug() << "request body is : " << bodyString;

  QNetworkReply *reply =
      networkAccessManager->post(request, bodyString.toUtf8());

  connectErrorSlot(reply);
  connect(reply, SIGNAL(finished()), this,
          SLOT(handleCreateSessionTanFinished()));
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

void CommbankService::processCreateSessionTanResult(QByteArray responseData,
                                                    QNetworkReply *reply) {
  QJsonDocument jsonDocumentBody = QJsonDocument::fromJson(responseData);
  if (!jsonDocumentBody.isObject()) {
    qDebug() << "response body not a json object!";
  }

  QByteArray jsonResponseHeader =
      reply->rawHeader(QByteArray("x-once-authentication-info"));
  QJsonDocument jsonDocumentHeader =
      QJsonDocument::fromJson(jsonResponseHeader);
  if (!jsonDocumentHeader.isObject()) {
    qDebug() << "response header not a json object!";
  }

  QJsonObject headerObject = jsonDocumentHeader.object();
  this->challengeId = headerObject["id"].toString();
  this->challengeType = headerObject["typ"].toString();
  QString challenge = headerObject["challenge"].toString();
  QJsonArray availableTypesArray = headerObject["availableTypes"].toArray();

  qDebug() << " challengeId : " << this->challengeId;
  qDebug() << " challengeType : " << this->challengeType;
  qDebug() << " challenge : " << challenge;
  qDebug() << " availableTypes : " << availableTypesArray;

//  if (QString("P_TAN").compare(this->challengeType) == 0 ||
//      QString("M_TAN").compare(this->challengeType) == 0) {
//    emit challengeRequired(challenge, this->challengeType);
//  } else {
//  }

  // TODO process push here -> accept happens in app
  emit loginResultAvailable(challenge, this->challengeType);


  QString result = QString(responseData);
  qDebug() << "CommbankService::processCreateSessionTanResult => "
           << responseData;
}

void CommbankService::sendChallengeResponse(const QString &challengeResponse) {
    qDebug() << "CommbankService::sendChallengeResponse " << challengeResponse;

    executeActivateSessionTan(QUrl(QString(URL_ACTIVATE_SESSION_TAN).arg(this->identifier)), challengeResponse);
}

void CommbankService::executeActivateSessionTan(const QUrl &url, const QString &challengeResponse) {
    qDebug() << "CommbankService::executeActivateSessionTan " << challengeResponse;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, MIME_TYPE_JSON);
    request.setRawHeader("Accept", MIME_TYPE_JSON);
    request.setRawHeader("Authorization",
                         QString("Bearer ").append(sessionContext->getAccessTokenLogin()).toUtf8());
    request.setRawHeader("x-http-request-info", sessionContext->createRequestInfoString().toUtf8());
    request.setRawHeader("x-once-authentication-info",
                         QString("{\"id\":\"%1\"}").arg(this->challengeId).toUtf8());

    // only add challenge (TAN) if the challgen type requires it.
    if (QString("P_TAN").compare(this->challengeType) == 0 ||
        QString("M_TAN").compare(this->challengeType) == 0) {
        request.setRawHeader("x-once-authentication", challengeResponse.toUtf8());
    }

    // TODO copied from above
    QJsonObject body;
    body.insert("identifier", this->identifier);
    body.insert("sessionTanActive", true);
    body.insert("activated2FA", true);

    QJsonDocument bodyDocument;
    bodyDocument.setObject(body);
    QString bodyString(bodyDocument.toJson());

    qDebug() << "request Body for PATCH : " << bodyString;

    qDebug() << "request body is : " << bodyString;
    // UNTIL here -> refactor it

    QBuffer *buffer=new QBuffer();
    buffer->open((QBuffer::ReadWrite));
    buffer->write(bodyString.toUtf8());
    buffer->seek(0);

    QNetworkReply *reply = networkAccessManager->sendCustomRequest(request, "PATCH", buffer);

    connectErrorSlot(reply);
    connect(reply, SIGNAL(finished()), this,
            SLOT(handleActivateSessionTanFinished()));
}

void CommbankService::handleActivateSessionTanFinished() {
    qDebug() << "CommbankService::handleActivateSessionTanFinished";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
      return;
    }

    logResponseHeaders(reply);

    processActivateSessionTanResult(reply->readAll(), reply);
}

void CommbankService::processActivateSessionTanResult(QByteArray responseData, QNetworkReply *reply) {
    QJsonDocument jsonDocumentBody = QJsonDocument::fromJson(responseData);
    if (!jsonDocumentBody.isObject()) {
      qDebug() << "response body not a json object!";
    }

    QString result = QString(responseData);
    qDebug() << "CommbankService::processActivateSessionTanResult => "
             << responseData;

    executeCDSecondaryFlow(QUrl(URL_OAUTH_TOKEN));
}

void CommbankService::executeCDSecondaryFlow(const QUrl &url) {
    qDebug() << "CommbankService::executeCDSecondaryFlow " << url;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      MIME_TYPE_WWW_FORM_URL_ENCODED);
    request.setRawHeader("Accept", MIME_TYPE_JSON);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::UserAgentHeader, USER_AGENT);

    QUrlQuery postData;
    postData.addQueryItem("client_id", CLIENT_ID);
    postData.addQueryItem("client_secret", CLIENT_SECRET);
    postData.addQueryItem("grant_type", "cd_secondary");
    postData.addQueryItem("token", sessionContext->getAccessTokenLogin());

    QNetworkReply *reply =
        networkAccessManager->post(request, postData.toString().toUtf8());

    connectErrorSlot(reply);
    connect(reply, SIGNAL(finished()), this,
            SLOT(handleCDSecondaryFlowFinished()));
}

void CommbankService::handleCDSecondaryFlowFinished() {
    qDebug() << "CommbankService::handleCDSecondaryFlowFinished";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
      return;
    }

    logResponseHeaders(reply);

    processCDSecondaryFlowResult(reply->readAll(), reply);
}

void CommbankService::processCDSecondaryFlowResult(QByteArray responseData, QNetworkReply *reply) {
    QJsonDocument jsonDocument = QJsonDocument::fromJson(responseData);
    if (!jsonDocument.isObject()) {
      qDebug() << "response body not a json object!";
    }

    QString result = QString(responseData);
    qDebug() << "CommbankService::processCDSecondaryFlowResult => "
             << responseData;

    QJsonObject responseObject = jsonDocument.object();
    sessionContext->setAccessToken(responseObject["access_token"].toString());
    sessionContext->setRefreshToken(responseObject["refresh_token"].toString());

    QString bearer = responseObject["bearer"].toString();
    QString scope = responseObject["scope"].toString();
    int expiresIn = responseObject["expires_in"].toInt();

    qDebug () << "accessToken : " << sessionContext->getAccessToken();
    qDebug () << "refreshToken : " << sessionContext->getRefreshToken();
    qDebug () << "expires in : " << expiresIn;
    qDebug () << "scope : " << scope;

    // TODO get access token for next requests -> get scopes, etc->

//"{\"access_token\":\"the_access_token\",\"token_type\":\"bearer\","
//    "\"refresh_token\":\"asfasdfasdfasdf\",\"expires_in\":599,"
//    "\"scope\":\"BANKING_RW BROKERAGE_RW MESSAGES_RO REPORTS_RO SESSION_RW\",\"kdnr\":\"12341231234\","
//    "\"bpid\":124312348,\"kontaktId\":13412}"

    emit challengeResponseAvailable();

    // TODO nicht von hier aus aufrufen !!
    executeGetAccountBalances(QUrl(URL_ACCOUNT_BALANCES));
}


void CommbankService::executeGetAccountBalances(const QUrl &url) {
    qDebug() << "CommbankService::executeGetAccountBalances " << url;

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

void CommbankService::handleGetAccountBalancesFinished() {
    qDebug() << "CommbankService::handleGetAccountBalancesFinished";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
      return;
    }

    logResponseHeaders(reply);

    processGetAccountBalancesResult(reply->readAll());
}

void CommbankService::processGetAccountBalancesResult(QByteArray responseData) {
    QJsonDocument jsonDocumentBody = QJsonDocument::fromJson(responseData);
    if (!jsonDocumentBody.isObject()) {
      qDebug() << "response body not a json object!";
    }

    QString result = QString(responseData);
    qDebug() << "CommbankService::processGetAccountBalancesResult => "
             << responseData;



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

    emit challengeResponseAvailable();
}


