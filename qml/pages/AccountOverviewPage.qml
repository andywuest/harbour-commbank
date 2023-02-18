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
import QtQuick 2.6
import Sailfish.Silica 1.0

import "../components/thirdparty"

import "../js/constants.js" as Constants
import "../js/functions.js" as Functions

Page {
    id: overviewPage

    property bool loading : false
    property var accountBalances
    property var depots

    SilicaFlickable {
        id: overviewFlickable
        anchors.fill: parent
        width: parent.width
        height: parent.height

        Behavior on opacity { NumberAnimation {} }
        opacity: visible ? 1 : 0
        visible: false

        PullDownMenu {
            MenuItem {
                text: qsTr("About Commbank")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
        }

        Column {
            id: overviewColumn

            width: overviewPage.width
            spacing: Theme.paddingMedium

            PageHeader {
                id: accountsHeader
                title: qsTr("Accounts Overview")
            }

            Image {
                id: overviewImage
                source: "../icons/commbank.png"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
                visible: overviewPage.isPortrait

                fillMode: Image.PreserveAspectFit
                width: parent.width / 2
                height: overviewImage.width
            }

            SectionHeader {
                text: qsTr("Accounts")
            }

            SilicaListView {
                id: accountsListView

                height: overviewFlickable.height - (overviewImage.visible ? overviewImage.height : 0) - accountsHeader.height - Theme.paddingMedium;
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

                    onClicked: {
                        var selectedAccount = accountsModel.get(index);
                        console.log("[AccountOverviewPage] Selected: " + selectedAccount + ", index : " + index);
                        console.log("[AccountOverviewPage] Selected: " + selectedAccount + ", accountId : " + selectedAccount.accountId);
                        console.log("[AccountOverviewPage] Selected: " + selectedAccount + ", accountType : " + selectedAccount.accountTypeKey);
                        if (Constants.ACCOUNT_TYPE_DEPOT === selectedAccount.accountTypeKey) {
                          pageStack.push(Qt.resolvedUrl("BrokeragePositionsPage.qml"), {"depotId": selectedAccount.accountId});
                        } else {
                          pageStack.push(Qt.resolvedUrl("AccountTransactionsPage.qml"), {"accountId": selectedAccount.accountId});
                        }
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
                                text: Functions.renderAccountBalance(accountBalanceValue, accountBalanceUnit)
                                    // accountBalanceValue !== "" ? Number(accountBalanceValue).toLocaleString(Qt.locale(), "f", 2) + " " + accountBalanceUnit : ""
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
        if (accountBalances) {
            for (var i = 0; i < accountBalances.length; i++) {
                accountsModel.append(accountBalances[i])
            }
        }
        if (depots) {
            for (var j = 0; j < depots.length; j++) {
                accountsModel.append(depots[j]);
            }
        }
        overviewFlickable.visible = true;
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
