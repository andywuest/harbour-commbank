# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-commbank

CONFIG += link_pkgconfig sailfishapp

HEADERS += \
    src/abstractservice.h \
    src/accountstorageservice.h \
    src/commbank.h \
    src/commbankaccountservice.h \
    src/commbankbrokerageservice.h \
    src/commbankloginservice.h \
    src/constants.h \
    src/credentials.h \
    src/sessioncontext.h

SOURCES += src/harbour-commbank.cpp \
    src/abstractservice.cpp \
    src/accountstorageservice.cpp \
    src/commbank.cpp \
    src/commbankaccountservice.cpp \
    src/commbankbrokerageservice.cpp \
    src/commbankloginservice.cpp \
    src/sessioncontext.cpp

DEFINES += VERSION_NUMBER=\\\"$$(VERSION_NUMBER)\\\"

PKGCONFIG += \
    sailfishsecrets

DISTFILES += qml/harbour-commbank.qml \
    qml/components/BrokeragePositionColumn.qml \
    qml/components/TransactionDetail.qml \
    qml/components/TransactionDetailsColumn.qml \
    qml/cover/CoverPage.qml \
    qml/js/constants.js \
    qml/pages/AboutPage.qml \
    qml/pages/BrokeragePositionsPage.qml \
    qml/pages/CredentialsPage.qml \
    qml/pages/SecondFactorLoginPage.qml \
    qml/pages/StoreCredentialsDialog.qml \
    qml/pages/AccountOverviewPage.qml \
    qml/pages/AccountTransactionsPage.qml \
    qml/pages/SingleTransactionPage.qml \
    qml/pages/icons/github.svg \
    qml/pages/icons/paypal.svg \
    qml/components/thirdparty/LoadingIndicator.qml \
    qml/components/thirdparty/AboutDescription.qml \
    qml/components/thirdparty/AboutIconLabel.qml \
    qml/js/credentials.js \
    qml/js/functions.js \
    qml/icons/commbank.png \
    rpm/harbour-commbank.changes.in \
    rpm/harbour-commbank.changes.run.in \
    rpm/harbour-commbank.spec \
    rpm/harbour-commbank.yaml \
    translations/*.ts \
    harbour-commbank.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/harbour-commbank-de.ts
