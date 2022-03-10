import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components/thirdparty"

import "../js/credentials.js" as Credentials

Page {
    id: credentialsPage
    allowedOrientations: Orientation.All

    property bool loading : false

    function connectSlots() {
        console.log("[LoginPage] connect - slots");
        // var dataBackend = getSecurityDataBackend(watchlistSettings.dataBackend);
        commbankLoginService.loginResultAvailable.connect(loginResultHandler);
        commbankLoginService.requestError.connect(errorResultHandler);
    }

    function disconnectSlots() {
        console.log("[LoginPage] disconnect - slots");
        // var dataBackend = getSecurityDataBackend(watchlistSettings.dataBackend);
        commbankLoginService.loginResultAvailable.disconnect(loginResultHandler);
        commbankLoginService.requestError.disconnect(errorResultHandler);
    }

    function loginResultHandler(challenge, challengeType) {
        console.log("[LoginPage] success received - " + challenge + ", " + challengeType);
        loading = false;
        disconnectSlots();
        pageStack.clear();
        pageStack.push(Qt.resolvedUrl("SecondFactorLoginPage.qml"), { challenge: challenge, challengeType: challengeType });
    }

    function errorResultHandler(result) {
        console.log("[LoginPage] error received - " + result);
        errorInfoLabel.visible = true;
        errorDetailInfoLabel.text = result;
        loading = false;
    }

    SilicaFlickable {

        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: credentialsColumn.height

        Column {
            id: credentialsColumn
            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                id: searchHeader
                title: qsTr("Enter your Credentials")
            }

            SectionHeader {
                text: qsTr("Your Credentials")
            }

            TextField {
                id: clientIdTextField
                width: parent.width
                placeholderText: qsTr("Client ID")
                labelVisible: false
            }

            TextField {
                id: clientSecretTextField
                width: parent.width
                placeholderText: qsTr("Client Secret")
                labelVisible: false
            }

            TextField {
                id: usernameTextField
                width: parent.width
                placeholderText: qsTr("Username")
                labelVisible: false
            }

            PasswordField {
                id: passwordField
                width: parent.width
                placeholderText: qsTr("Password")
                labelVisible: false
            }

            Button {
                id: loginButton
                text: qsTr("Login")
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
                enabled: ( clientIdTextField.text !== "" && clientSecretTextField.text !== ""
                          && usernameTextField.text !== "" && passwordField.text !== "")
                onClicked: {
                    console.log("[CredentialsPage] login button clicked");
                    loading = true;
                    errorInfoLabel.visible = false;
                    errorDetailInfoLabel.text = "";
                    commbankLoginService.performLogin(clientIdTextField.text, clientSecretTextField.text
                                                 , usernameTextField.text, passwordField.text);
                }
            }

//            Button {
//                id: abortButton
//                text: qsTr("Abort")
//                anchors {
//                    horizontalCenter: parent.horizontalCenter
//                }
//                onClicked: {
//                    finTsAccounts.removeCurrentAccount();
//                    pageStack.clear();
//                    pageStack.push(finTsDialog.isInitialized() ? Qt.resolvedUrl("OverviewPage.qml") : Qt.resolvedUrl("InstitutesSearchPage.qml"));
//                }
//            }

            Label {
                id: separatorLabel
                width: parent.width
                font.pixelSize: Theme.fontSizeExtraSmall
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
            }

            InfoLabel {
                id: errorInfoLabel
                visible: false
                font.pixelSize: Theme.fontSizeLarge
                text: qsTr("Unable to login to comdirect account. Please check the following error messages.")
            }

            InfoLabel {
                id: errorDetailInfoLabel
                visible: errorInfoLabel.visible
                font.pixelSize: Theme.fontSizeMedium
                text: ""
            }

        }

    }

    Component.onCompleted: {
        clientIdTextField.text = Credentials.clientId;
        clientSecretTextField.text = Credentials.clientSecret;
        usernameTextField.text = Credentials.username;
        passwordField.text = Credentials.password;

        connectSlots();
    }

    LoadingIndicator {
        id: loginLoadingIndicator
        visible: loading
        Behavior on opacity {
            NumberAnimation {
            }
        }
        opacity: loading ? 1 : 0
        height: parent.height
        width: parent.width
    }

}
