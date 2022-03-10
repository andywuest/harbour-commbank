import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components/thirdparty"

Page {
    id: overviewPage

    property bool loading : false

    function connectSlots() {
        console.log("[AccountOverviewPage] connect - slots");
        commbankAccountService.allBalancesResultAvailable.connect(allBalancesResultHandler);
        commbankAccountService.requestError.connect(errorResultHandler);
    }

    function disconnectSlots() {
        console.log("[AccountOverviewPage] disconnect - slots");
        commbankAccountService.allBalancesResultAvailable.disconnect(allBalancesResultHandler);
        commbankAccountService.requestError.disconnect(errorResultHandler);
    }

    function allBalancesResultHandler(result) {
        // TODO move to 2FA page
        console.log("[AccountOverviewPage] balances received : " + result);
        loading = false;

        var accounts = JSON.parse(result);

        for (var i = 0; i < accounts.length; i++) {
            accountsModel.append(accounts[i])
        }

        overviewFlickable.visible = true;
    }

    function errorResultHandler(result) {
        console.log("[AccountOverviewPage] error received - " + result);
        errorInfoLabel.visible = true;
        errorDetailInfoLabel.text = result;
        loading = false;
    }

    SilicaFlickable {
        id: overviewFlickable
        anchors.fill: parent
        contentHeight: overviewColumn.height

        Behavior on opacity { NumberAnimation {} }
        opacity: visible ? 1 : 0
        visible: false

//        PullDownMenu {
//            MenuItem {
//                text: qsTr("About Zaster Banker")
//                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
//            }
//            MenuItem {
//                text: qsTr("Settings")
//                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
//            }
//        }

        Column {
            id: overviewColumn

            width: overviewPage.width
            spacing: Theme.paddingMedium
            PageHeader {
                id: accountsHeader
                title: qsTr("Account Balance")
            }

//            Image {
//                id: overviewImage
//                source: "../../images/zaster.png"
//                anchors {
//                    horizontalCenter: parent.horizontalCenter
//                }
//                visible: overviewPage.isPortrait

//                fillMode: Image.PreserveAspectFit
//                width: 1/2 * parent.width
//            }

            Text {
                id: bankNameText
                x: Theme.horizontalPageMargin
                width: parent.width - ( 2 * Theme.horizontalPageMargin )
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Theme.fontSizeLarge
                color: Theme.primaryColor
                wrapMode: Text.Wrap
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
            }

            Text {
                id: bankCodeText
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Theme.fontSizeMedium
                color: Theme.primaryColor
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
            }

            SilicaListView {

                id: accountsListView

                height: overviewPage.height - accountsHeader.height - bankNameText.height - bankCodeText.height - ( 3 * Theme.paddingMedium ) - ( overviewPage.isPortrait ? /*overviewImage.height*/ 0 + Theme.paddingMedium : 0 )
                width: parent.width
                anchors.left: parent.left
                anchors.right: parent.right

                clip: true

                model: ListModel {
                    id: accountsModel
                }

                delegate: ListItem {
                    contentHeight: resultItem.height + Theme.paddingMedium
                    contentWidth: parent.width

                    // enabled: finTsDialog.canRetrieveTransactions(modelData.accountId) || finTsDialog.canRetrievePortfolioInfo(modelData.accountId)

                    onClicked: {
                        var selectedAccount = accountsModel.get(index);
                        console.log("Selected: " + selectedAccount + ", index : " + index);
                        // pageStack.push(Qt.resolvedUrl("AccountTransactionsPage.qml"), {"accountId": modelData.accountId, "iban": modelData.iban})
                    }

                    Item {
                        id: resultItem
                        width: parent.width
                        height: resultRow.height + accountSeparator.height + Theme.paddingMedium

                        Row {
                            id: resultRow
                            width: parent.width - ( 2 * Theme.horizontalPageMargin )
                            spacing: Theme.paddingMedium
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            Column {
                                width: parent.width / 2 - Theme.paddingSmall
                                Text {
                                    id: accountIdText
                                    font.pixelSize: Theme.fontSizeSmall
                                    color: Theme.primaryColor
                                    text: accountDisplayId
                                }
                                Text {
                                    id: accountDescriptionText
                                    font.pixelSize: Theme.fontSizeSmall
                                    color: Theme.primaryColor
                                    text: accountTypeText
                                }
                            }
                            Text {
                                id: accountValueText
                                width: parent.width / 2 - Theme.paddingSmall
                                height: parent.height
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignVCenter
                                font.pixelSize: Theme.fontSizeMedium
                                color: Theme.highlightColor
                                text: Number(accountBalanceValue).toLocaleString(Qt.locale(), "f", 2) + " " + accountBalanceUnit
                            }
                        }

                        Separator {
                            id: accountSeparator
                            anchors.top : resultRow.bottom
                            anchors.topMargin: Theme.paddingMedium

                            width: parent.width
                            color: Theme.primaryColor
                            horizontalAlignment: Qt.AlignHCenter
                        }
                    }

                }

                VerticalScrollDecorator {}

            }

        }

    }

    Component.onCompleted: {
        connectSlots();
        loading = true;
        commbankAccountService.getAllBalances();
        // TODO trigger loader of accounts
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
