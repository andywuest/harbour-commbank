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

  void connectErrorSlot(QNetworkReply *reply);
  void logResponseHeaders(QNetworkReply *reply);
};

#endif // ABSTRACT_SERVICE_H
