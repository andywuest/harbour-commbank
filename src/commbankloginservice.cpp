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

  QNetworkRequest request = prepareNetworkRequest(url, false);

  QUrlQuery postData;
  postData.addQueryItem("client_id", clientId);
  postData.addQueryItem("client_secret", clientSecret);
  postData.addQueryItem("grant_type", "password");
  postData.addQueryItem("username", username);
  postData.addQueryItem("password", password);

  logRequest(url, request, postData.toString().toUtf8());

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

  processExecuteResourceOwnerPasswordCredentialsFlowResult(reply);
}

void CommbankLoginService::
    processExecuteResourceOwnerPasswordCredentialsFlowResult(
        QNetworkReply *reply) {
  qDebug() << "CommbankLoginService::"
              "parseExecuteResourceOwnerPasswordCredentialsFlowResult";

  QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());
  if (!jsonDocument.isObject()) {
    qDebug() << "not a json object!";
  }

  logResponse("body", reply, jsonDocument);

  QJsonObject responseObject = jsonDocument.object();
  sessionContext->setAccessTokenLogin(
      responseObject["access_token"].toString());
  sessionContext->setRefreshTokenLogin(
      responseObject["refresh_token"].toString());

  // TODO pruefen, ob wie die brauchen
  //  QString tokenType = responseObject["token_type"].toString();
  //  QString scope = responseObject["scope"].toString();
  //  QString kdnr = responseObject["kdnr"].toString();
  //  QString bpid = responseObject["bpid"].toString();
  //  QString kontaktId = responseObject["kontaktId"].toString();
  //  int expiresIn = responseObject["expires_in"].toInt();

  //  qDebug() << " expires in : " << expiresIn;

  executeGetSessionStatus(QUrl(URL_SESSIONS));
}

void CommbankLoginService::executeGetSessionStatus(const QUrl &url) {
  qDebug() << "CommbankLoginService::executeGetSessionStatus " << url;

  QNetworkRequest request = prepareNetworkRequest(url, true);
  //  request.setHeader(QNetworkRequest::ContentTypeHeader, MIME_TYPE_JSON);
  //  request.setRawHeader("Accept", MIME_TYPE_JSON);
  request.setRawHeader(
      "Authorization",
      QString("Bearer %1").arg(sessionContext->getAccessTokenLogin()).toUtf8());
  request.setRawHeader("x-http-request-info",
                       sessionContext->createRequestInfoString().toUtf8());

  logRequest(url, request, nullptr);

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

  processGetSessionStatusResult(reply);
}

void CommbankLoginService::processGetSessionStatusResult(QNetworkReply *reply) {
  qDebug() << "CommbankLoginService::processGetSessionStatusResult";

  QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());
  if (!jsonDocument.isArray()) {
    qDebug() << "not a json array!";
  }

  logResponse("body", reply, jsonDocument);

  QJsonArray responseArray = jsonDocument.array();
  if (responseArray.size() > 0) {
    QJsonObject firstObject = responseArray.at(0).toObject();
    this->identifier = firstObject["identifier"].toString();

    // TODO pruefen, ob wie die brauchen
    //    bool sessionTanActive = firstObject["sessionTanActive"].isBool();
    //    bool activated2FA = firstObject["activated2FA"].isBool();

    //    qDebug() << " identifier : " << this->identifier;
    //    qDebug() << " sessionTanActive : " << sessionTanActive;
    //    qDebug() << " activated2FA : " << activated2FA;

  } else {
    qWarning() << "unexpected number of elements in json array !";
  }

  executeCreateSessionTan(
      QUrl(QString(URL_CREATE_SESSION_TAN).arg(this->identifier)));
}

void CommbankLoginService::executeCreateSessionTan(const QUrl &url) {
  qDebug() << "CommbankLoginService::executeCreateSessionTan " << url;

  QNetworkRequest request = prepareNetworkRequest(url, true);
  request.setRawHeader(
      "Authorization",
      QString("Bearer %1").arg(sessionContext->getAccessTokenLogin()).toUtf8());
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

  logRequest(url, request, bodyString.toUtf8());

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

  processCreateSessionTanResult(reply);
}

void CommbankLoginService::processCreateSessionTanResult(QNetworkReply *reply) {
  QJsonDocument jsonDocumentBody = QJsonDocument::fromJson(reply->readAll());
  if (!jsonDocumentBody.isObject()) {
    qDebug() << "response body not a json object!";
  }

  logResponse("body", reply, jsonDocumentBody);

  QByteArray jsonResponseHeader =
      reply->rawHeader(QByteArray("x-once-authentication-info"));
  QJsonDocument jsonDocumentHeader =
      QJsonDocument::fromJson(jsonResponseHeader);
  if (!jsonDocumentHeader.isObject()) {
    qDebug() << "response header not a json object!";
  }

  logResponse("x-once-authentication-info", jsonDocumentHeader);

  QJsonObject headerObject = jsonDocumentHeader.object();
  this->challengeId = headerObject["id"].toString();
  this->challengeType = headerObject["typ"].toString();
  QString challenge = headerObject["challenge"].toString();
  QJsonArray availableTypesArray = headerObject["availableTypes"].toArray();

  // TODO pruefen, ob benoetigt
  //  qDebug() << " challengeId : " << this->challengeId;
  //  qDebug() << " challengeType : " << this->challengeType;
  //  qDebug() << " challenge : " << challenge;
  //  qDebug() << " availableTypes : " << availableTypesArray;

  //  if (QString("P_TAN").compare(this->challengeType) == 0 ||
  //      QString("M_TAN").compare(this->challengeType) == 0) {
  //    emit challengeRequired(challenge, this->challengeType);
  //  } else {
  //  }

  // TODO process push here -> accept happens in app
  emit loginResultAvailable(challenge, this->challengeType);

  //  QString result = QString(responseData);
  qDebug() << "CommbankLoginService::processCreateSessionTanResult";
  //           << responseData;
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

  QNetworkRequest request = prepareNetworkRequest(url, true);
  request.setRawHeader(
      "Authorization",
      QString("Bearer %1").arg(sessionContext->getAccessTokenLogin()).toUtf8());
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

  logRequest(url, request, bodyString.toUtf8());

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

  processActivateSessionTanResult(reply);
}

void CommbankLoginService::processActivateSessionTanResult(
    QNetworkReply *reply) {
  qDebug() << "CommbankLoginService::processActivateSessionTanResult";
  QJsonDocument jsonDocumentBody = QJsonDocument::fromJson(reply->readAll());
  if (!jsonDocumentBody.isObject()) {
    qDebug() << "response body not a json object!";
  }

  logResponse("body", reply, jsonDocumentBody);

  executeCDSecondaryFlow(QUrl(URL_OAUTH_TOKEN));
}

void CommbankLoginService::executeCDSecondaryFlow(const QUrl &url) {
  qDebug() << "CommbankLoginService::executeCDSecondaryFlow " << url;

  QNetworkRequest request = prepareNetworkRequest(url, false);

  QUrlQuery postData;
  postData.addQueryItem("client_id", CLIENT_ID);
  postData.addQueryItem("client_secret", CLIENT_SECRET);
  postData.addQueryItem("grant_type", "cd_secondary");
  postData.addQueryItem("token", sessionContext->getAccessTokenLogin());

  logRequest(url, request, postData.toString().toUtf8());

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

  processCDSecondaryFlowResult(reply);
}

void CommbankLoginService::processCDSecondaryFlowResult(QNetworkReply *reply) {
  QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());
  if (!jsonDocument.isObject()) {
    qDebug() << "response body not a json object!";
  }

  logResponse("body", reply, jsonDocument);

  qDebug() << "CommbankLoginService::processCDSecondaryFlowResult";

  QJsonObject responseObject = jsonDocument.object();
  sessionContext->setAccessToken(responseObject["access_token"].toString());
  sessionContext->setRefreshToken(responseObject["refresh_token"].toString());

  // TODO check if required
  //  QString bearer = responseObject["bearer"].toString();
  //  QString scope = responseObject["scope"].toString();
  //  int expiresIn = responseObject["expires_in"].toInt();

  //  qDebug() << "accessToken : " << sessionContext->getAccessToken();
  //  qDebug() << "refreshToken : " << sessionContext->getRefreshToken();
  //  qDebug() << "expires in : " << expiresIn;
  //  qDebug() << "scope : " << scope;

  // TODO get access token for next requests -> get scopes, etc->

  emit challengeResponseAvailable();
}
