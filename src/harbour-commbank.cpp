#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>
#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>
#include <QScopedPointer>
#include <QtQml>

#include "commbank.h"

int main(int argc, char *argv[]) {
  QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
  QScopedPointer<QQuickView> view(SailfishApp::createView());

  QQmlContext *context = view.data()->rootContext();

  Commbank commbank;
  context->setContextProperty("commbank", &commbank);

  CommbankLoginService *commbankLoginService = commbank.getCommbankLoginService();
  context->setContextProperty("commbankLoginService", commbankLoginService);

  CommbankAccountService *commbankAccountService = commbank.getCommbankAccountService();
  context->setContextProperty("commbankAccountService", commbankAccountService);

  AccountStorageService *accountStorageService = commbank.getAccountStorageService();
  context->setContextProperty("accountStorageService", accountStorageService);

  context->setContextProperty("applicationVersion", QString(VERSION_NUMBER));

  view->setSource(SailfishApp::pathTo("qml/harbour-commbank.qml"));
  view->show();
  return app->exec();
}
