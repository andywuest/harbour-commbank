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
