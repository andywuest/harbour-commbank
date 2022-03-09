import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components/thirdparty"

import "../js/credentials.js" as Credentials

Page {
    id: credentialsPage
    allowedOrientations: Orientation.All

    property string challenge
    property string challengeType

    property bool loading : false

    function connectSlots() {
        console.log("[SecondFactorLoginPage] connect - slots");
        commbankService.challengeResponseAvailable.connect(challengeResultHandler);
        commbankService.requestError.connect(errorResultHandler);
    }

    function disconnectSlots() {
        console.log("[SecondFactorLoginPage] disconnect - slots");
        commbankService.challengeResponseAvailable.disconnect(challengeResultHandler);
        commbankService.requestError.disconnect(errorResultHandler);
    }

    function challengeResultHandler(result) {
        // TODO move to 2FA page
        loading = false;
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
                    commbankService.sendChallengeResponse(challengeTextField.text);
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
