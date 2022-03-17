/*
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
import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    property bool loaded : false

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    function connectSlots() {
        console.log("[LoginPage] connect - slots");
        // var dataBackend = getSecurityDataBackend(watchlistSettings.dataBackend);
//        commbankService.loginResultAvailable.connect(loginResultHandler);
//        commbankService.challengeRequired.connect(challengeRequiredHandler);
//        commbankService.requestError.connect(errorResultHandler);
    }

    function disconnectSlots() {
        console.log("[LoginPage] disconnect - slots");
        // var dataBackend = getSecurityDataBackend(watchlistSettings.dataBackend);
//        commbankService.loginResultAvailable.disconnect(loginResultHandler);
//        commbankService.challengeRequired.disconnect(challengeRequiredHandler);
//        commbankService.requestError.disconnect(errorResultHandler);
    }

    function loginResultHandler(result) {
    }

    function challengeRequiredHandler(challenge, challengeType) {
        console.log("[LoginPage] challenge/challengeType " + challenge + "/" + challengeType);
        sendChallengeButton.visible = true;
    }

    function errorResultHandler(result) {
        console.log("[LoginPage] error received - " + result);
        errorTextField.text = result;
        loaded = true;
    }

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            MenuItem {
                text: qsTr("Show Page 2")
                onClicked: pageStack.push(Qt.resolvedUrl("SecondPage.qml"))
            }
        }

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("UI Template")
            }
            Label {
                x: Theme.horizontalPageMargin
                text: qsTr("Login Page")
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }

            Button {
                text: "Login"
                onClicked: {
                    console.log("clicked");
                    errorTextField.text = "";
                    commbankService.performLogin("username");
                }
            }

            Button {
                text: "Credentials"
                onClicked: {
                    console.log("credentials clicked");
                    errorTextField.text = "";
                    pageStack.push(Qt.resolvedUrl("CredentialsPage.qml"))
                }
            }

            Label {
                text: qsTr("Error:")
                width: parent.width
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }

            TextField {
                id: errorTextField
                placeholderText: "-"
                width: parent.width
                readOnly: true
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
            }

            Button {
                id: sendChallengeButton
                text: qsTr("Send Challenge")
                visible: false
                onClicked: {
                    console.log("clicked send challenge");
                    errorTextField.text = "";
                    commbankService.sendChallengeResponse("");
                }
            }

        }
    }

    Component.onCompleted: {
        connectSlots();
    }

}
