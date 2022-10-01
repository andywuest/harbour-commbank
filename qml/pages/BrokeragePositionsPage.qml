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

import "../components"

import "../js/functions.js" as Functions

// TODO remove before checkin
import "../js/credentials.js" as Credentials

Page {
    id: positionsPage
    allowedOrientations: Orientation.All

    property string depotId;
    property int pageSize: 20 // page size for the backend responses (20 positions)
    property int startIndex: 0
    property bool hasMore: true;

    function connectSlots() {
        console.log("[BrokeragePositionsPage] connect - slots");
        // account service
        commbankBrokerageService.positionsResultAvailable.connect(positionsResultHandler);
        commbankBrokerageService.requestError.connect(errorResultHandler);
    }

    function disconnectSlots() {
        console.log("[BrokeragePositionsPage] disconnect - slots");
        // account service
        commbankBrokerageService.positionsResultAvailable.disconnect(positionsResultHandler);
        commbankBrokerageService.requestError.disconnect(errorResultHandler);
    }

    function positionsResultHandler(result) {
        console.log("[BrokeragePositionsPage] result received - " + result);
        var positions = JSON.parse(result);
        loadingColumn.visible = false;
        for (var i = 0; i < positions.values.length; i++) {
            positionsModel.append(positions.values[i])
        }
        hasMore = ((positions.paging.index + pageSize) < positions.paging.matches)
        console.log(" more positions available : " + hasMore);
        positionsHeader.title = qsTr("Positions");
        positionsHeader.description = qsTr("Value: %1").arg(Functions.formatAmount(positions.aggregated.currentValue));
        positionsColumn.visible = true;
    }

    function errorResultHandler(result) {
        console.log("[BrokeragePositionsPage] error received - " + result);
        errorInfoLabel.visible = true;
        errorDetailInfoLabel.text = result;
        loading = false;
    }

    Component.onCompleted: {
        connectSlots();
        errorColumn.visible = false;
        loadingColumn.visible = true;
        // TODO just for debuggin
        // positionsResultHandler(Credentials.positions);
        commbankBrokerageService.getPositions(depotId, startIndex);
    }

    SilicaFlickable {
        anchors.fill: parent
        topMargin: errorColumn.visible ? Theme.horizontalPageMargin : 0
        contentWidth: parent.width
        contentHeight: positionsColumn.visible ? positionsColumn.height : ( errorColumn.visible ? errorColumn.height : parent.height )

        PushUpMenu {
            visible: hasMore
            MenuItem {
                text: qsTr("Load more")
                onClicked: {
                    loadingColumn.visible = true;
                    startIndex += pageSize;
                    console.log("start loading from index: " + startIndex);
                    commbankBrokerageService.getPositions(depotId, startIndex);
                }
            }
        }

        Column {
            id: loadingColumn
            width: parent.width
            height: loadingLabel.height + loadingBusyIndicator.height + commbankLoadingImage.height + ( 2 * Theme.paddingMedium )
            spacing: Theme.paddingMedium
            anchors.verticalCenter: parent.verticalCenter

            Behavior on opacity { NumberAnimation {} }
            opacity: visible ? 1 : 0
            visible: false

            Image {
                id: commbankLoadingImage
                source: "../icons/commbank.png"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }

                fillMode: Image.PreserveAspectFit
                width: 1/2 * parent.width
            }

            InfoLabel {
                id: loadingLabel
                text: qsTr("Retrieving information...")
            }

            BusyIndicator {
                id: loadingBusyIndicator
                anchors.horizontalCenter: parent.horizontalCenter
                running: loadingColumn.visible
                size: BusyIndicatorSize.Large
            }
        }

        Column {
            property bool retryPossible: false

            id: errorColumn
            width: parent.width
            spacing: Theme.paddingMedium
            anchors.verticalCenter: parent.verticalCenter

            Behavior on opacity { NumberAnimation {} }
            opacity: visible ? 1 : 0
            visible: false

            Image {
                id: commbankErrorImage
                source: "../icons/commbank.png"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }

                fillMode: Image.PreserveAspectFit
                width: 1/2 * parent.width
            }

            InfoLabel {
                id: errorInfoLabel
                font.pixelSize: Theme.fontSizeLarge
                text: ""
            }

            Repeater {
                id: errorMessageRepeater
                width: parent.width
                delegate: Text {
                    id: errorMessageText
                    width: parent.width
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.primaryColor
                    wrapMode: Text.Wrap
                    text: "TODO - error "
                    // qsTr("%1 (Error Code: %2)").arg(modelData.text).arg(modelData.code)
                }
            }
        }

        Column {
            id: positionsColumn
            width: parent.width
            spacing: Theme.paddingMedium

            Behavior on opacity { NumberAnimation {} }
            opacity: visible ? 1 : 0
            visible: false

            PageHeader {
                id: positionsHeader
            }

            SilicaListView {
                id: positionsListView

                height: positionsPage.height - positionsHeader.height - Theme.paddingMedium
                width: parent.width
                anchors.left: parent.left
                anchors.right: parent.right

                clip: true

                model: ListModel {
                    id: positionsModel
                }

                delegate: ListItem {
                    contentHeight: resultItem.height + ( 2 * Theme.paddingMedium )
                    contentWidth: parent.width

                    onClicked: {
                        var selectedPosition = positionsModel.get(index);
                        console.log("selection position index : " + index);
                        console.log("position : " + selectedPosition);
//                        pageStack.push(Qt.resolvedUrl("SingleTransactionPage.qml"),
//                                       {"singleTransaction": selectedTransaction});
                    }

                    Item {
                        id: resultItem
                        width: parent.width
                        height:  resultRowInfo.height + positionSeparator.height
                        y: Theme.paddingMedium

                        Row {
                            id: resultRowInfo
                            width: parent.width - ( 2 * Theme.horizontalPageMargin )
                            spacing: Theme.paddingMedium
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter

                            Column {
                                width: parent.width
                                height: labelsRow.height + columnsRow.height

                                Row {
                                    id: labelsRow
                                    width: parent.width
                                    Column {
                                        width: parent.width
                                        Text {
                                            id: isinMnemonicText
                                            width: parent.width
                                            font.pixelSize: Theme.fontSizeTiny
                                            color: Theme.secondaryColor
                                            elide: Text.ElideRight
                                            text: instrument.isin + " - " + instrument.mnemonic
                                            textFormat: Text.StyledText
                                        }
                                        Row {
                                            width: parent.width
                                            Text {
                                                id: positionName
                                                width: parent.width * 7 / 10
                                                font.pixelSize: Theme.fontSizeSmall
                                                font.bold: true
                                                color: Theme.primaryColor
                                                elide: Text.ElideRight
                                                text: instrument.shortName
                                                textFormat: Text.StyledText
                                            }
                                            Text {
                                                id: positionValue
                                                width: parent.width * 3 / 10
                                                font.pixelSize: Theme.fontSizeSmall
                                                font.bold: true
                                                color: Theme.primaryColor
                                                elide: Text.ElideRight
                                                text: Functions.formatAmount(currentValue);
                                                textFormat: Text.StyledText
                                                horizontalAlignment: Text.AlignRight
                                            }
                                        }
                                    }
                                }

                                Row {
                                    id: columnsRow
                                    width: parent.width
                                    BrokeragePositionColumn {
                                        width: parent.width / 4 * 1 - Theme.paddingSmall
                                        columnLabel: qsTr("STK/NOM")
                                        columnValue: quantity.value // formatter
                                    }
                                    BrokeragePositionColumn {
                                        width: parent.width / 4 * 1 - Theme.paddingSmall
                                        columnLabel: qsTr("KAUFKURS")
                                        columnValue: Functions.formatAmount(purchasePrice);
                                    }
                                    BrokeragePositionColumn {
                                        width: parent.width / 4 * 1 - Theme.paddingSmall
                                        columnLabel: qsTr("AKT. KURS")
                                        columnValue: Functions.formatAmount(currentPrice.price);
                                    }
                                    BrokeragePositionColumn {
                                        width: parent.width / 4 * 1 - Theme.paddingSmall
                                        columnLabel: qsTr("G/V")
                                        columnValue: Functions.formatAmount(profitLossPurchaseAbs);
                                    }
                                }
                            }
                        }

                        RowSeparator {
                            id: positionSeparator
                            anchors.top : resultRowInfo.bottom
                        }
                    }
                }

                VerticalScrollDecorator {}

            }
        }
    }

}
