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

import "../components/thirdparty"

Page {
    id: credentialsPage
    allowedOrientations: Orientation.All

    property bool usernameKnown
    property string challenge
    property string challengeType
    property string clientId
    property string clientSecret
    property string username

    property bool loading : false

    function connectSlots() {
        console.log("[SecondFactorLoginPage] connect - slots");
        // login service
        commbankLoginService.challengeResponseAvailable.connect(challengeResultHandler);
        commbankLoginService.requestError.connect(errorResultHandler);
        // account service
        commbankAccountService.allBalancesResultAvailable.connect(allBalancesResultHandler);
        commbankAccountService.requestError.connect(errorResultHandler);
        // account storage service
        accountStorageService.requestError.connect(errorResultHandler)
    }

    function disconnectSlots() {
        console.log("[SecondFactorLoginPage] disconnect - slots");
        // login service
        commbankLoginService.challengeResponseAvailable.disconnect(challengeResultHandler);
        commbankLoginService.requestError.disconnect(errorResultHandler);
        // account service
        commbankAccountService.allBalancesResultAvailable.disconnect(allBalancesResultHandler);
        commbankAccountService.requestError.disconnect(errorResultHandler);
        // account storage service
        accountStorageService.requestError.disconnect(errorResultHandler)
    }

    function challengeResultHandler(result) {
        console.log("[SecondFactorLoginPage] challenge success received");
        console.log("[SecondFactorLoginPage] start loading balances");

        // TODO getAllBalances sollte auch Depots liefern
        commbankAccountService.getAllBalances();
    }

    function allBalancesResultHandler(result) {
        console.log("[SecondFactorLoginPage] balances received : " + result);
        var accountBalances = JSON.parse(result);
        loading = false;

        if (usernameKnown) {
            navigateToBalancesPage(accountBalances);
        } else {
            var dialog = pageStack.push(Qt.resolvedUrl("StoreCredentialsDialog.qml"), {
                                            "username": username
                                        });
            dialog.accepted.connect(function() {
                console.log("[SecondFactorLoginPage] storing credentials data under name : " + dialog.name);
                var credentials = {
                    "name" : dialog.name,
                    "client_id" : clientId,
                    "client_secret" : clientSecret,
                    "username" : username
                };
                accountStorageService.storeAccountCredentials(credentials)
                navigateToBalancesPage(accountBalances);
            });
            dialog.rejected.connect(function () {
                console.log("[SecondFactorLoginPage] not storing credentials data ");
                navigateToBalancesPage(accountBalances);
            });
        }
    }

    function navigateToBalancesPage(accountBalances) {
        disconnectSlots();
        pageStack.clear();
        pageStack.push(Qt.resolvedUrl("AccountOverviewPage.qml"), {
                        "accountBalances" : accountBalances
                       });
    }

    function errorResultHandler(result) {
        console.log("[SecondFactorLoginPage] error received - " + result);
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
                title: qsTr("2FA Authentication")
            }

            Image {
                id: overviewImage
                source: "../icons/commbank.png"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
                visible: overviewPage.isPortrait

                fillMode: Image.PreserveAspectFit
                width: 1/2 * parent.width
            }

            SectionHeader {
                id: secondFactorSectionHeader
            }

            InfoLabel {
                id: secondFactorInfoLabel
                horizontalAlignment: Text.AlignLeft
                font {
                    pixelSize: Theme.fontSizeMedium
                    family: Theme.fontFamily
                }
            }

            TextField {
                id: challengeTextField
                width: parent.width
                placeholderText: qsTr("TAN")
                labelVisible: false
            }

            Button {
                id: continueButton
                text: qsTr("Continue")
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
                onClicked: {
                    console.log("[SecondFactorLoginPage] continue button clicked");
                    loading = true;
                    errorInfoLabel.visible = false;
                    errorDetailInfoLabel.text = "";
                    commbankLoginService.sendChallengeResponse(challengeTextField.text, clientId, clientSecret);
                }
            }

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
                text: qsTr("Second factor authentication failed. Please check the following error messages.")
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
        connectSlots();
        console.log("[SecondFactorLoginPage] usernameKnown = " + usernameKnown);
        var sectionHeaderText = "";
        if (challengeType === "P_TAN") {
            challengeTextField.visible = true;
            secondFactorInfoLabel.text = qsTr("Please scan the image with your photoTAN device.")
            sectionHeaderText = qsTr("photoTAN");
        } else if (challengeType === "M_TAN") {
            challengeTextField.visible = true;
            secondFactorInfoLabel.text = qsTr("The mobileTAN has been sent to your mobile phone with the number %1").arg(challenge);
            sectionHeaderText = qsTr("mobileTAN");
        } else if (challengeType === "P_TAN_PUSH") {
            challengeTextField.visible = false;
            secondFactorInfoLabel.text = qsTr("Please clear the login request within your mobile phone application before you continue.")
            sectionHeaderText = qsTr("photoTAN push");
        }
        secondFactorSectionHeader.text = sectionHeaderText;
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
