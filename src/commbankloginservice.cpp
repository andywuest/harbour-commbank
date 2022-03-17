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
#include "commbankloginservice.h"
#include "constants.h"
#include "credentials.h"

#include <QBuffer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUuid>

CommbankLoginService::CommbankLoginService(QNetworkAccessManager *manager,
                                           QObject *parent,
                                           SessionContext *sessionContext)
    : AbstractService(manager, parent, sessionContext) {
  qDebug() << "Initializing CommbankLoginService...";
}

void CommbankLoginService::performLogin(const QString &clientId,
                                        const QString &clientSecret,
                                        const QString &username,
                                        const QString &password) {
  qDebug() << "CommbankLoginService::performLogin : ";

  executeResourceOwnerPasswordCredentialsFlow(QUrl(URL_OAUTH_TOKEN), clientId,
                                              clientSecret, username, password);
}

void CommbankLoginService::executeResourceOwnerPasswordCredentialsFlow(
    const QUrl &url, const QString &clientId, const QString &clientSecret,
    const QString &username, const QString &password) {
  qDebug() << "CommbankLoginService::executeGetRequest " << url;

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

  //  QJsonObject accountData1;
  //  accountData1.insert("name", "the name 1");
  //  accountData1.insert("client_id", clientId);
  //  accountData1.insert("client_secret", clientSecret);
  //  accountData1.insert("username", username);

  //  QJsonObject accountData2;
  //  accountData2.insert("name", "the name 2");
  //  accountData2.insert("client_id", clientId + "2");
  //  accountData2.insert("client_secret", clientSecret + "2");
  //  accountData2.insert("username", username + "2");

  //  qDebug() << "account names : " <<
  //  accountStorageService->loadAllAccountNames();
  //  accountStorageService->resetAccounts();
  //  accountStorageService->storeAccountCredentials(accountData1);
  //  qDebug() << "loaded result : " <<
  //  accountStorageService->loadAccountCredentials(username);

  //  accountStorageService->storeAccountCredentials(accountData2);
  //  qDebug() << "loaded result : " <<
  //  accountStorageService->loadAccountCredentials(username);

  QNetworkReply *reply =
      networkAccessManager->post(request, postData.toString().toUtf8());

  connectErrorSlot(reply);
  connect(reply, SIGNAL(finished()), this,
          SLOT(handleExecuteResourceOwnerPasswordCredentialsFlowFinished()));
}

void CommbankLoginService::
    handleExecuteResourceOwnerPasswordCredentialsFlowFinished() {
  qDebug() << "CommbankLoginService::"
              "handleExecuteResourceOwnerPasswordCredentialsFlowFinished";
  QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
  reply->deleteLater();
  if (reply->error() != QNetworkReply::NoError) {
    return;
  }

  logResponseHeaders(reply);

  processExecuteResourceOwnerPasswordCredentialsFlowResult(reply->readAll());
}

void CommbankLoginService::
    processExecuteResourceOwnerPasswordCredentialsFlowResult(
        QByteArray responseData) {
  QJsonDocument jsonDocument = QJsonDocument::fromJson(responseData);
  if (!jsonDocument.isObject()) {
    qDebug() << "not a json object!";
  }

  QJsonObject responseObject = jsonDocument.object();
  sessionContext->setAccessTokenLogin(
      responseObject["access_token"].toString());
  sessionContext->setRefreshTokenLogin(
      responseObject["refresh_token"].toString());

  QString tokenType = responseObject["token_type"].toString();
  QString scope = responseObject["scope"].toString();
  QString kdnr = responseObject["kdnr"].toString();
  QString bpid = responseObject["bpid"].toString();
  QString kontaktId = responseObject["kontaktId"].toString();
  int expiresIn = responseObject["expires_in"].toInt();

  QString result = QString(responseData);
  qDebug() << "CommbankLoginService::"
              "parseExecuteResourceOwnerPasswordCredentialsFlowResult => "
           << responseData;

  qDebug() << " access token (Login) : "
           << sessionContext->getAccessTokenLogin();
  qDebug() << " refresh token (Login) : "
           << sessionContext->getRefreshTokenLogin();
  qDebug() << " expires in : " << expiresIn;

  executeGetSessionStatus(QUrl(URL_SESSIONS));
}

void CommbankLoginService::executeGetSessionStatus(const QUrl &url) {
  qDebug() << "CommbankLoginService::executeGetSessionStatus " << url;

  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader, MIME_TYPE_JSON);
  request.setRawHeader("Accept", MIME_TYPE_JSON);
  request.setRawHeader("Authorization",
                       QString("Bearer ")
                           .append(sessionContext->getAccessTokenLogin())
                           .toUtf8());
  request.setRawHeader("x-http-request-info",
                       sessionContext->createRequestInfoString().toUtf8());

  QNetworkReply *reply = networkAccessManager->get(request);

  connectErrorSlot(reply);
  connect(reply, SIGNAL(finished()), this,
          SLOT(handleGetSessionStatusFinished()));
}

void CommbankLoginService::handleGetSessionStatusFinished() {
  qDebug() << "CommbankLoginService::handleGetSessionStatusFinished";
  QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
  reply->deleteLater();
  if (reply->error() != QNetworkReply::NoError) {
    return;
  }

  logResponseHeaders(reply);

  processGetSessionStatusResult(reply->readAll());
}

void CommbankLoginService::processGetSessionStatusResult(
    QByteArray responseData) {
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
  qDebug() << "CommbankLoginService::processGetSessionStatusResult => "
           << responseData;

  executeCreateSessionTan(
      QUrl(QString(URL_CREATE_SESSION_TAN).arg(this->identifier)));
}

void CommbankLoginService::executeCreateSessionTan(const QUrl &url) {
  qDebug() << "CommbankLoginService::executeCreateSessionTan " << url;

  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader, MIME_TYPE_JSON);
  request.setRawHeader("Accept", MIME_TYPE_JSON);
  request.setRawHeader("Authorization",
                       QString("Bearer ")
                           .append(sessionContext->getAccessTokenLogin())
                           .toUtf8());
  request.setRawHeader("x-http-request-info",
                       sessionContext->createRequestInfoString().toUtf8());

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

void CommbankLoginService::handleCreateSessionTanFinished() {
  qDebug() << "CommbankLoginService::handleCreateSessionTanFinished";
  QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
  reply->deleteLater();
  if (reply->error() != QNetworkReply::NoError) {
    return;
  }

  logResponseHeaders(reply);

  processCreateSessionTanResult(reply->readAll(), reply);
}

void CommbankLoginService::processCreateSessionTanResult(
    QByteArray responseData, QNetworkReply *reply) {
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
  qDebug() << "CommbankLoginService::processCreateSessionTanResult => "
           << responseData;
}

void CommbankLoginService::sendChallengeResponse(
    const QString &challengeResponse) {
  qDebug() << "CommbankLoginService::sendChallengeResponse "
           << challengeResponse;

  executeActivateSessionTan(
      QUrl(QString(URL_ACTIVATE_SESSION_TAN).arg(this->identifier)),
      challengeResponse);
}

void CommbankLoginService::executeActivateSessionTan(
    const QUrl &url, const QString &challengeResponse) {
  qDebug() << "CommbankLoginService::executeActivateSessionTan "
           << challengeResponse;

  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader, MIME_TYPE_JSON);
  request.setRawHeader("Accept", MIME_TYPE_JSON);
  request.setRawHeader("Authorization",
                       QString("Bearer ")
                           .append(sessionContext->getAccessTokenLogin())
                           .toUtf8());
  request.setRawHeader("x-http-request-info",
                       sessionContext->createRequestInfoString().toUtf8());
  request.setRawHeader(
      "x-once-authentication-info",
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

  QBuffer *buffer = new QBuffer();
  buffer->open((QBuffer::ReadWrite));
  buffer->write(bodyString.toUtf8());
  buffer->seek(0);

  QNetworkReply *reply =
      networkAccessManager->sendCustomRequest(request, "PATCH", buffer);

  connectErrorSlot(reply);
  connect(reply, SIGNAL(finished()), this,
          SLOT(handleActivateSessionTanFinished()));
}

void CommbankLoginService::handleActivateSessionTanFinished() {
  qDebug() << "CommbankLoginService::handleActivateSessionTanFinished";
  QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
  reply->deleteLater();
  if (reply->error() != QNetworkReply::NoError) {
    return;
  }

  logResponseHeaders(reply);

  processActivateSessionTanResult(reply->readAll(), reply);
}

void CommbankLoginService::processActivateSessionTanResult(
    QByteArray responseData, QNetworkReply *reply) {
  QJsonDocument jsonDocumentBody = QJsonDocument::fromJson(responseData);
  if (!jsonDocumentBody.isObject()) {
    qDebug() << "response body not a json object!";
  }

  QString result = QString(responseData);
  qDebug() << "CommbankLoginService::processActivateSessionTanResult => "
           << responseData;

  executeCDSecondaryFlow(QUrl(URL_OAUTH_TOKEN));
}

void CommbankLoginService::executeCDSecondaryFlow(const QUrl &url) {
  qDebug() << "CommbankLoginService::executeCDSecondaryFlow " << url;

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

void CommbankLoginService::handleCDSecondaryFlowFinished() {
  qDebug() << "CommbankLoginService::handleCDSecondaryFlowFinished";
  QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
  reply->deleteLater();
  if (reply->error() != QNetworkReply::NoError) {
    return;
  }

  logResponseHeaders(reply);

  processCDSecondaryFlowResult(reply->readAll(), reply);
}

void CommbankLoginService::processCDSecondaryFlowResult(QByteArray responseData,
                                                        QNetworkReply *reply) {
  QJsonDocument jsonDocument = QJsonDocument::fromJson(responseData);
  if (!jsonDocument.isObject()) {
    qDebug() << "response body not a json object!";
  }

  QString result = QString(responseData);
  qDebug() << "CommbankLoginService::processCDSecondaryFlowResult => "
           << responseData;

  QJsonObject responseObject = jsonDocument.object();
  sessionContext->setAccessToken(responseObject["access_token"].toString());
  sessionContext->setRefreshToken(responseObject["refresh_token"].toString());

  QString bearer = responseObject["bearer"].toString();
  QString scope = responseObject["scope"].toString();
  int expiresIn = responseObject["expires_in"].toInt();

  qDebug() << "accessToken : " << sessionContext->getAccessToken();
  qDebug() << "refreshToken : " << sessionContext->getRefreshToken();
  qDebug() << "expires in : " << expiresIn;
  qDebug() << "scope : " << scope;

  // TODO get access token for next requests -> get scopes, etc->

  //"{\"access_token\":\"the_access_token\",\"token_type\":\"bearer\","
  //    "\"refresh_token\":\"asfasdfasdfasdf\",\"expires_in\":599,"
  //    "\"scope\":\"BANKING_RW BROKERAGE_RW MESSAGES_RO REPORTS_RO
  //    SESSION_RW\",\"kdnr\":\"12341231234\","
  //    "\"bpid\":124312348,\"kontaktId\":13412}"

  emit challengeResponseAvailable();

  // TODO nicht von hier aus aufrufen !!
  // executeGetAccountBalances(QUrl(URL_ACCOUNT_BALANCES));
}
