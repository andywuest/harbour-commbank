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
#ifndef COMMBANK_ACCOUNT_SERVICE_H
#define COMMBANK_ACCOUNT_SERVICE_H

#include "abstractservice.h"

#include <QObject>

class CommbankAccountService : public AbstractService {
  Q_OBJECT

public:
  explicit CommbankAccountService(QNetworkAccessManager *manager,
                                  QObject *parent = nullptr,
                                  SessionContext *sessionContext = nullptr);
  ~CommbankAccountService() = default;

  Q_INVOKABLE void getAllBalances();
  Q_INVOKABLE void getTransactions(const QString &accountId, int startIndex);

  Q_SIGNAL void allBalancesResultAvailable(const QString &challenge);
  Q_SIGNAL void transactionsResultAvailable(const QString &result);

private:
  void executeGetAccountBalances(const QUrl &url);
  void executeGetTransactions(const QUrl &url);

  void processGetAccountBalancesResult(QNetworkReply *reply);
  void processGetTransactionsResult(QNetworkReply *reply);

private slots:
  void handleGetAccountBalancesFinished();
  void handleGetTransactionsFinished();
};

#endif // COMMBANK_ACCOUNT_SERVICE_H
