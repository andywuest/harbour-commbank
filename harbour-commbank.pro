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

CONFIG += sailfishapp

SOURCES += src/harbour-commbank.cpp \
    src/commbankservice.cpp \
    src/sessioncontext.cpp

DEFINES += VERSION_NUMBER=\\\"$$(VERSION_NUMBER)\\\"

DISTFILES += qml/harbour-commbank.qml \
    qml/components/thirdparty/LoadingIndicator.qml \
    qml/cover/CoverPage.qml \
    qml/js/credentials.js \
    qml/pages/AccountOverviewPage.qml \
    qml/pages/FirstPage.qml \
    qml/pages/LoginPage.qml \
    qml/pages/CredentialsPage.qml \
    qml/pages/SecondFactorLoginPage.qml \
    qml/pages/SecondPage.qml \
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

HEADERS += \
    src/commbankservice.h \
    src/constants.h \
    src/credentials.h \
    src/sessioncontext.h
