import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components/thirdparty"

import "../js/credentials.js" as Credentials

Page {
    id: credentialsPage
    allowedOrientations: Orientation.All

    property bool loading: false
    property var knownUsernames: []

    function connectSlots() {
        console.log("[CredentialsPage] connect - slots")
        // var dataBackend = getSecurityDataBackend(watchlistSettings.dataBackend);
        commbankLoginService.loginResultAvailable.connect(loginResultHandler)
        commbankLoginService.requestError.connect(errorResultHandler)
        accountStorageService.savedAccountDataAvailable.connect(
                    savedAccountDataHandler)
    }

    function disconnectSlots() {
        console.log("[CredentialsPage] disconnect - slots")
        // var dataBackend = getSecurityDataBackend(watchlistSettings.dataBackend);
        commbankLoginService.loginResultAvailable.disconnect(loginResultHandler)
        commbankLoginService.requestError.disconnect(errorResultHandler)
    }

    function savedAccountDataHandler(accountNames) {
        console.log("[CredentialsPage] account names - " + accountNames)
        var accountNamesResponse = JSON.parse(accountNames)
        if (accountNamesResponse) {
            for (var key in accountNamesResponse) {
                if (accountNamesResponse.hasOwnProperty(key)) {
                    console.log(key + " -> " + accountNamesResponse[key])
                    pullDownMenuModel.append(accountNamesResponse[key])
                    knownUsernames.push(accountNamesResponse[key]["username"]);
                }
            }
        }
    }

    function isKnownUsername(loginUsername) {
        for (var i = 0; i < knownUsernames.length; i++) {
            if (knownUsernames[i] === loginUsername) {
                return true;
            }
        }
        return false;
    }

    function populateInputFieldsWithLoadedCredentials(model) {
        clientIdTextField.text = model.client_id
        clientSecretTextField.text = model.client_secret
        usernameTextField.text = model.name
        passwordField.text = ""
    }

    function clearInputFields() {
        clientIdTextField.text = ""
        clientSecretTextField.text = ""
        usernameTextField.text = ""
        passwordField.text = ""
    }

    function loginResultHandler(challenge, challengeType) {
        console.log("[CredentialsPage] success received - " + challenge + ", " + challengeType)
        loading = false
        disconnectSlots();
        pageStack.clear();
        pageStack.push(Qt.resolvedUrl("SecondFactorLoginPage.qml"), {
                           "usernameKnown": isKnownUsername(usernameTextField.text),
                           "challenge": challenge,
                           "challengeType": challengeType,
                           "clientId": clientIdTextField.text,
                           "clientSecret": clientSecretTextField.text,
                           "username": usernameTextField.text
                       });
    }

    function errorResultHandler(result) {
        console.log("[CredentialsPage] error received - " + result)
        errorInfoLabel.visible = true
        errorDetailInfoLabel.text = result
        loading = false
    }

    SilicaFlickable {

        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: credentialsColumn.height

        ListModel {
            id: pullDownMenuModel
        }

        PullDownMenu {
            Repeater {
                model: pullDownMenuModel
                MenuItem {
                    text: qsTr("Load %1").arg(model.name)
                    onClicked: populateInputFieldsWithLoadedCredentials(model)
                }
            }
        }

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
                enabled: (clientIdTextField.text !== ""
                          && clientSecretTextField.text !== ""
                          && usernameTextField.text !== ""
                          && passwordField.text !== "")
                onClicked: {
                    console.log("[CredentialsPage] login button clicked")
                    loading = true
                    errorInfoLabel.visible = false
                    errorDetailInfoLabel.text = ""
                    commbankLoginService.performLogin(
                                clientIdTextField.text,
                                clientSecretTextField.text,
                                usernameTextField.text, passwordField.text)
                }
            }

            Button {
                id: clearButton
                text: qsTr("Clear")
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
                onClicked: clearInputFields()
            }


// TODO this logic has to be called after a successful second factor login
//            Button {
//                id: todoRemove
//                text: qsTr("Store credentials")
//                anchors {
//                    horizontalCenter: parent.horizontalCenter
//                }
//                onClicked: {
//                    var dialog = pageStack.push(Qt.resolvedUrl("StoreCredentialsDialog.qml"),
//                                                                     {
//                                                    "clientId": clientIdTextField.text,
//                                                    "clientSecret": clientSecretTextField.text,
//                                                    "username": usernameTextField.text
//                                                }
//                                                )
//                    dialog.accepted.connect(function() {
//                                             console.log("Credentials name: " + dialog.name)
//                                         })
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
        connectSlots()
        accountStorageService.loadSavedAccountData()
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
