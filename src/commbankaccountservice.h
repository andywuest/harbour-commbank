#ifndef COMMBANK_ACCOUNT_SERVICE_H
#define COMMBANK_ACCOUNT_SERVICE_H

#include "abstractservice.h"

#include <QObject>

class CommbankAccountService : public AbstractService
{
    Q_OBJECT

  public:
    explicit CommbankAccountService(QNetworkAccessManager *manager, QObject *parent = nullptr, SessionContext *sessionContext = nullptr);
    ~CommbankAccountService() = default;

    Q_INVOKABLE void getAllBalances();
    Q_SIGNAL void allBalancesResultAvailable(const QString &challenge);

private:
    void executeGetAccountBalances(const QUrl &url);
    void processGetAccountBalancesResult(QByteArray responseData);

private slots:
  void handleGetAccountBalancesFinished();

};

#endif // COMMBANK_ACCOUNT_SERVICE_H
