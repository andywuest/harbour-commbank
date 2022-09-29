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
#ifndef COMMBANK_BROKERAGE_SERVICE_H
#define COMMBANK_BROKERAGE_SERVICE_H

#include "abstractservice.h"

#include <QObject>

class CommbankBrokerageService : public AbstractService {
  Q_OBJECT

public:
  explicit CommbankBrokerageService(QNetworkAccessManager *manager,
                                    QObject *parent = nullptr,
                                    SessionContext *sessionContext = nullptr);
  ~CommbankBrokerageService() = default;

  Q_INVOKABLE void getAllDepots();
  Q_INVOKABLE void getPositions(const QString &depotId, int startIndex);

  Q_SIGNAL void allDepotsResultAvailable(const QString &depots);
  Q_SIGNAL void positionsResultAvailable(const QString &positions);

private:
  void executeGetDepots(const QUrl &url);
  void executeGetPositions(const QUrl &url);

  void processGetDepotsResult(QNetworkReply *reply);
  void processGetPositionsResult(QNetworkReply *reply);

private slots:
  void handleGetDepotsFinished();
  void handleGetPositionsFinished();
};

#endif // COMMBANK_BROKERAGE_SERVICE_H
