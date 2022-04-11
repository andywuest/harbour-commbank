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
#ifndef ABSTRACT_SERVICE_H
#define ABSTRACT_SERVICE_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

#include "sessioncontext.h"

class AbstractService : public QObject {
  Q_OBJECT

public:
  explicit AbstractService(QNetworkAccessManager *networkAccessManager,
                           QObject *parent = nullptr,
                           SessionContext *sessionContext = nullptr);
  ~AbstractService() = 0;

  Q_SIGNAL void requestError(const QString &errorMessage);

protected:
  QNetworkAccessManager *networkAccessManager;
  SessionContext *sessionContext;

  QNetworkRequest prepareNetworkRequest(const QUrl url, bool contentTypeJson);

  void connectErrorSlot(QNetworkReply *reply);
  void logRequest(const QUrl url, QNetworkRequest request, QByteArray payload);
  void logResponse(QString info, QNetworkReply *reply,
                   QJsonDocument jsonDocument);
};

#endif // ABSTRACT_SERVICE_H
