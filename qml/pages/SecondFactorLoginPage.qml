import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components/thirdparty"

Page {
    id: credentialsPage
    allowedOrientations: Orientation.All

    property string challenge
    property string challengeType

    property bool loading : false

    function connectSlots() {
        console.log("[SecondFactorLoginPage] connect - slots");
        commbankLoginService.challengeResponseAvailable.connect(challengeResultHandler);
        commbankLoginService.requestError.connect(errorResultHandler);
    }

    function disconnectSlots() {
        console.log("[SecondFactorLoginPage] disconnect - slots");
        commbankLoginService.challengeResponseAvailable.disconnect(challengeResultHandler);
        commbankLoginService.requestError.disconnect(errorResultHandler);
    }

    function challengeResultHandler(result) {
        // TODO move to 2FA page
        console.log("[SecondFactorLoginPage] challenge success received");
        loading = false;


        // TODO accounts laden
        // TODO nachdem laden der accounts fragen, ob man die credentialsspeichern will, oder nicht
        disconnectSlots();
        pageStack.clear();
        pageStack.push(Qt.resolvedUrl("AccountOverviewPage.qml"));
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
                    commbankLoginService.sendChallengeResponse(challengeTextField.text);
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
        var sectionHeaderText = "";
        if (challengeType === "P_TAN") {
            challengeTextField.visible = true;
            secondFactorInfoLabel.text = qsTr("Please can the image with your photoTAN device.")
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
