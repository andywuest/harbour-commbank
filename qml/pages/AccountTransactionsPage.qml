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

import "../js/functions.js" as Functions

Page {
    id: transactionsPage
    allowedOrientations: Orientation.All

    property string accountId;
    property bool isPortfolio: false

    function connectSlots() {
        console.log("[AccountTransactionsPage] connect - slots");
        // account service
        commbankAccountService.transactionsResultAvailable.connect(transactionsResultHandler);
        commbankAccountService.requestError.connect(errorResultHandler);
    }

    function disconnectSlots() {
        console.log("[AccountTransactionsPage] disconnect - slots");
        // account service
        commbankAccountService.transactionsResultAvailable.disconnect(transactionsResultHandler);
        commbankAccountService.requestError.disconnect(errorResultHandler);
    }

    function transactionsResultHandler(result) {
        console.log("[AccountTransactionsPage] result received - " + result);
        var transactions = JSON.parse(result);
        loadingColumn.visible = false;
        for (var i = 0; i < transactions.values.length; i++) {
            transactionsModel.append(transactions.values[i])
        }
        transactionsHeader.title = qsTr("Transactions");
        transactionsColumn.visible = true;
    }

    function errorResultHandler(result) {
        console.log("[AccountTransactionsPage] error received - " + result);
        errorInfoLabel.visible = true;
        errorDetailInfoLabel.text = result;
        loading = false;
    }

    Component.onCompleted: {
        connectSlots();
        errorColumn.visible = false;
        loadingColumn.visible = true;
        transactionsResultHandler("{\n    \"aggregated\": {\n        \"account\": null,\n        \"accountId\": \"6F91E9A225C74355834F5A0D9A304697\",\n        \"bookingDateLatestTransaction\": \"2022-01-04\",\n        \"latestTransactionIncluded\": false,\n        \"pagingTimestamp\": \"2022-04-16T20:50:47+02\",\n        \"referenceLatestTransaction\": \"6287065267\"\n    },\n    \"paging\": {\n        \"index\": 0,\n        \"matches\": 32\n    },\n    \"values\": [\n        {\n            \"amount\": {\n                \"unit\": \"EUR\",\n                \"value\": \"17.1\"\n            },\n            \"bookingDate\": \"2022-04-11\",\n            \"bookingStatus\": \"BOOKED\",\n            \"creditor\": null,\n            \"deptor\": null,\n            \"directDebitCreditorId\": null,\n            \"directDebitMandateId\": null,\n            \"endToEndReference\": null,\n            \"newTransaction\": true,\n            \"reference\": \"0T5C1E6107PSHZRS/2\",\n            \"remittanceInfo\": \"01ERTRAEGNISGUTSCHRIFT VOM 11.04.22  02DEPOTBESTAND:             25,144   03DK DAXPLUS MAX.DIVIDEND            04ETFL23             EUR 0,68        \",\n            \"remitter\": null,\n            \"transactionType\": {\n                \"key\": \"INTEREST_DIVIDENDS\",\n                \"text\": \"Interest / Dividends\"\n            },\n            \"valutaDate\": \"2022-04-11\"\n        },\n        {\n            \"amount\": {\n                \"unit\": \"EUR\",\n                \"value\": \"-24.98\"\n            },\n            \"bookingDate\": \"2022-04-07\",\n            \"bookingStatus\": \"BOOKED\",\n            \"creditor\": null,\n            \"deptor\": null,\n            \"directDebitCreditorId\": null,\n            \"directDebitMandateId\": null,\n            \"endToEndReference\": null,\n            \"newTransaction\": true,\n            \"reference\": \"2R5C1E6007ZCC2I2/2\",\n            \"remittanceInfo\": \"01XTR.ST.GL.SE.DI.100SW.1D           02WPKNR: DBX1DG  ISIN: LU0292096186  \",\n            \"remitter\": null,\n            \"transactionType\": {\n                \"key\": \"SECURITIES\",\n                \"text\": \"Securities\"\n            },\n            \"valutaDate\": \"2022-04-11\"\n        },\n        {\n            \"amount\": {\n                \"unit\": \"EUR\",\n                \"value\": \"-49.92\"\n            },\n            \"bookingDate\": \"2022-04-07\",\n            \"bookingStatus\": \"BOOKED\",\n            \"creditor\": null,\n            \"deptor\": null,\n            \"directDebitCreditorId\": null,\n            \"directDebitMandateId\": null,\n            \"endToEndReference\": null,\n            \"newTransaction\": true,\n            \"reference\": \"0T5C1E6009G057N3/2\",\n            \"remittanceInfo\": \"01VANG.FTSE A.-WO.U.ETF DLD          02WPKNR: A1JX52  ISIN: IE00B3RBWM25  \",\n            \"remitter\": null,\n            \"transactionType\": {\n                \"key\": \"SECURITIES\",\n                \"text\": \"Securities\"\n            },\n            \"valutaDate\": \"2022-04-11\"\n        },\n        {\n            \"amount\": {\n                \"unit\": \"EUR\",\n                \"value\": \"38.56\"\n            },\n            \"bookingDate\": \"2022-04-07\",\n            \"bookingStatus\": \"BOOKED\",\n            \"creditor\": null,\n            \"deptor\": null,\n            \"directDebitCreditorId\": null,\n            \"directDebitMandateId\": null,\n            \"endToEndReference\": null,\n            \"newTransaction\": true,\n            \"reference\": \"1T5C1E5Y4D1K881V/2\",\n            \"remittanceInfo\": \"01ERTRAEGNISGUTSCHRIFT VOM 06.04.22  02DEPOTBESTAND:             58,000   03NOVO-NORDISK B ADR/1DK 10          04866931             USD 1,0224236   \",\n            \"remitter\": null,\n            \"transactionType\": {\n                \"key\": \"INTEREST_DIVIDENDS\",\n                \"text\": \"Interest / Dividends\"\n            },\n            \"valutaDate\": \"2022-04-07\"\n        },\n        {\n            \"amount\": {\n                \"unit\": \"EUR\",\n                \"value\": \"75\"\n            },\n            \"bookingDate\": \"2022-04-06\",\n            \"bookingStatus\": \"BOOKED\",\n            \"creditor\": null,\n            \"deptor\": {\n                \"bic\": \"BYLADEM1001\",\n                \"holderName\": \"Andreas Wuest\",\n                \"iban\": \"DE05120300000012096525\"\n            },\n            \"directDebitCreditorId\": \"DE38ZZZ00000020140\",\n            \"directDebitMandateId\": \"210613230012DE76200411440869927400\",\n            \"endToEndReference\": \"nicht angegeben\",\n            \"newTransaction\": true,\n            \"reference\": \"JQ22209591747763/2\",\n            \"remittanceInfo\": \"01DKB Konto Lastschrift Sparplan 1   02End-to-End-Ref.:                   03nicht angegeben                    04CORE / Mandatsref.:                05210613230012DE76200411440869927400 06Gläubiger-ID:                      07DE38ZZZ00000020140                 \",\n            \"remitter\": null,\n            \"transactionType\": {\n                \"key\": \"MISCELLANEOUS\",\n                \"text\": \"Miscellaneous\"\n            },\n            \"valutaDate\": \"2022-04-06\"\n        },\n        {\n            \"amount\": {\n                \"unit\": \"EUR\",\n                \"value\": \"18.61\"\n            },\n            \"bookingDate\": \"2022-04-05\",\n            \"bookingStatus\": \"BOOKED\",\n            \"creditor\": null,\n            \"deptor\": null,\n            \"directDebitCreditorId\": null,\n            \"directDebitMandateId\": null,\n            \"endToEndReference\": null,\n            \"newTransaction\": true,\n            \"reference\": \"1T5C1E5X0H2LH4FE/2\",\n            \"remittanceInfo\": \"01Ausübung Umtausch auf: A2P5N8      02vom 05.04.2022                     03Referenznummer: 0BIIAKHYTG60059N   04Gesnrgen: 2022084491227300         \",\n            \"remitter\": null,\n            \"transactionType\": {\n                \"key\": \"SECURITIES\",\n                \"text\": \"Securities\"\n            },\n            \"valutaDate\": \"2022-04-01\"\n        },\n        {\n            \"amount\": {\n                \"unit\": \"EUR\",\n                \"value\": \"5.86\"\n            },\n            \"bookingDate\": \"2022-03-31\",\n            \"bookingStatus\": \"BOOKED\",\n            \"creditor\": null,\n            \"deptor\": null,\n            \"directDebitCreditorId\": null,\n            \"directDebitMandateId\": null,\n            \"endToEndReference\": null,\n            \"newTransaction\": false,\n            \"reference\": \"555C1E3U4DLF4VW3/2\",\n            \"remittanceInfo\": \"01ERTRAEGNISGUTSCHRIFT VOM 30.03.22  02DEPOTBESTAND:             19,964   03VANG.FTSE A.-WO.U.ETF DLD          04A1JX52             USD 0,32737     \",\n            \"remitter\": null,\n            \"transactionType\": {\n                \"key\": \"INTEREST_DIVIDENDS\",\n                \"text\": \"Interest / Dividends\"\n            },\n            \"valutaDate\": \"2022-04-01\"\n        },\n        {\n            \"amount\": {\n                \"unit\": \"EUR\",\n                \"value\": \"18.97\"\n            },\n            \"bookingDate\": \"2022-03-09\",\n            \"bookingStatus\": \"BOOKED\",\n            \"creditor\": null,\n            \"deptor\": null,\n            \"directDebitCreditorId\": null,\n            \"directDebitMandateId\": null,\n            \"endToEndReference\": null,\n            \"newTransaction\": false,\n            \"reference\": \"6V5C1E384CYEF74J/2\",\n            \"remittanceInfo\": \"01ERTRAEGNISGUTSCHRIFT VOM 08.03.22  02DEPOTBESTAND:             23,000   03JOHNSON + JOHNSON    DL 1          04853260             USD 1,06        \",\n            \"remitter\": null,\n            \"transactionType\": {\n                \"key\": \"INTEREST_DIVIDENDS\",\n                \"text\": \"Interest / Dividends\"\n            },\n            \"valutaDate\": \"2022-03-10\"\n        },\n        {\n            \"amount\": {\n                \"unit\": \"EUR\",\n                \"value\": \"18.44\"\n            },\n            \"bookingDate\": \"2022-03-08\",\n            \"bookingStatus\": \"BOOKED\",\n            \"creditor\": null,\n            \"deptor\": null,\n            \"directDebitCreditorId\": null,\n            \"directDebitMandateId\": null,\n            \"endToEndReference\": null,\n            \"newTransaction\": false,\n            \"reference\": \"0N5C1E374CYT64TL/2\",\n            \"remittanceInfo\": \"01ERTRAEGNISGUTSCHRIFT VOM 07.03.22  02DEPOTBESTAND:            172,000   03MOWI ASA           NK 7,5          04924848             NOK 1,40        \",\n            \"remitter\": null,\n            \"transactionType\": {\n                \"key\": \"INTEREST_DIVIDENDS\",\n                \"text\": \"Interest / Dividends\"\n            },\n            \"valutaDate\": \"2022-03-09\"\n        },\n        {\n            \"amount\": {\n                \"unit\": \"EUR\",\n                \"value\": \"-49.97\"\n            },\n            \"bookingDate\": \"2022-03-07\",\n            \"bookingStatus\": \"BOOKED\",\n            \"creditor\": null,\n            \"deptor\": null,\n            \"directDebitCreditorId\": null,\n            \"directDebitMandateId\": null,\n            \"endToEndReference\": null,\n            \"newTransaction\": false,\n            \"reference\": \"3R5C1E380A16JPLW/2\",\n            \"remittanceInfo\": \"01VANG.FTSE A.-WO.U.ETF DLD          02WPKNR: A1JX52  ISIN: IE00B3RBWM25  \",\n            \"remitter\": null,\n            \"transactionType\": {\n                \"key\": \"SECURITIES\",\n                \"text\": \"Securities\"\n            },\n            \"valutaDate\": \"2022-03-09\"\n        },\n        {\n            \"amount\": {\n                \"unit\": \"EUR\",\n                \"value\": \"-24.99\"\n            },\n            \"bookingDate\": \"2022-03-07\",\n            \"bookingStatus\": \"BOOKED\",\n            \"creditor\": null,\n            \"deptor\": null,\n            \"directDebitCreditorId\": null,\n            \"directDebitMandateId\": null,\n            \"endToEndReference\": null,\n            \"newTransaction\": false,\n            \"reference\": \"3R5C1E380A25FSAP/2\",\n            \"remittanceInfo\": \"01XTR.ST.GL.SE.DI.100SW.1D           02WPKNR: DBX1DG  ISIN: LU0292096186  \",\n            \"remitter\": null,\n            \"transactionType\": {\n                \"key\": \"SECURITIES\",\n                \"text\": \"Securities\"\n            },\n            \"valutaDate\": \"2022-03-09\"\n        },\n        {\n            \"amount\": {\n                \"unit\": \"EUR\",\n                \"value\": \"75\"\n            },\n            \"bookingDate\": \"2022-03-04\",\n            \"bookingStatus\": \"BOOKED\",\n            \"creditor\": null,\n            \"deptor\": {\n                \"bic\": \"BYLADEM1001\",\n                \"holderName\": \"Andreas Wuest\",\n                \"iban\": \"DE05120300000012096525\"\n            },\n            \"directDebitCreditorId\": \"DE38ZZZ00000020140\",\n            \"directDebitMandateId\": \"210613230012DE76200411440869927400\",\n            \"endToEndReference\": \"nicht angegeben\",\n            \"newTransaction\": false,\n            \"reference\": \"GQ22206293039650/2\",\n            \"remittanceInfo\": \"01DKB Konto Lastschrift Sparplan 1   02End-to-End-Ref.:                   03nicht angegeben                    04CORE / Mandatsref.:                05210613230012DE76200411440869927400 06Gläubiger-ID:                      07DE38ZZZ00000020140                 \",\n            \"remitter\": null,\n            \"transactionType\": {\n                \"key\": \"MISCELLANEOUS\",\n                \"text\": \"Miscellaneous\"\n            },\n            \"valutaDate\": \"2022-03-04\"\n        },\n        {\n            \"amount\": {\n                \"unit\": \"EUR\",\n                \"value\": \"-50\"\n            },\n            \"bookingDate\": \"2022-02-07\",\n            \"bookingStatus\": \"BOOKED\",\n            \"creditor\": null,\n            \"deptor\": null,\n            \"directDebitCreditorId\": null,\n            \"directDebitMandateId\": null,\n            \"endToEndReference\": null,\n            \"newTransaction\": false,\n            \"reference\": \"0J5C1E0G080Z5MIH/2\",\n            \"remittanceInfo\": \"01VANG.FTSE A.-WO.U.ETF DLD          02WPKNR: A1JX52  ISIN: IE00B3RBWM25  \",\n            \"remitter\": null,\n            \"transactionType\": {\n                \"key\": \"SECURITIES\",\n                \"text\": \"Securities\"\n            },\n            \"valutaDate\": \"2022-02-09\"\n        },\n        {\n            \"amount\": {\n                \"unit\": \"EUR\",\n                \"value\": \"-24.98\"\n            },\n            \"bookingDate\": \"2022-02-07\",\n            \"bookingStatus\": \"BOOKED\",\n            \"creditor\": null,\n            \"deptor\": null,\n            \"directDebitCreditorId\": null,\n            \"directDebitMandateId\": null,\n            \"endToEndReference\": null,\n            \"newTransaction\": false,\n            \"reference\": \"3R5C1E0G08AG1L9A/2\",\n            \"remittanceInfo\": \"01XTR.ST.GL.SE.DI.100SW.1D           02WPKNR: DBX1DG  ISIN: LU0292096186  \",\n            \"remitter\": null,\n            \"transactionType\": {\n                \"key\": \"SECURITIES\",\n                \"text\": \"Securities\"\n            },\n            \"valutaDate\": \"2022-02-09\"\n        },\n        {\n            \"amount\": {\n                \"unit\": \"EUR\",\n                \"value\": \"75\"\n            },\n            \"bookingDate\": \"2022-02-04\",\n            \"bookingStatus\": \"BOOKED\",\n            \"creditor\": null,\n            \"deptor\": {\n                \"bic\": \"BYLADEM1001\",\n                \"holderName\": \"Andreas Wuest\",\n                \"iban\": \"DE05120300000012096525\"\n            },\n            \"directDebitCreditorId\": \"DE38ZZZ00000020140\",\n            \"directDebitMandateId\": \"210613230012DE76200411440869927400\",\n            \"endToEndReference\": \"nicht angegeben\",\n            \"newTransaction\": false,\n            \"reference\": \"HW22203492827537/2\",\n            \"remittanceInfo\": \"01DKB Konto Lastschrift Sparplan 1   02End-to-End-Ref.:                   03nicht angegeben                    04CORE / Mandatsref.:                05210613230012DE76200411440869927400 06Gläubiger-ID:                      07DE38ZZZ00000020140                 \",\n            \"remitter\": null,\n            \"transactionType\": {\n                \"key\": \"MISCELLANEOUS\",\n                \"text\": \"Miscellaneous\"\n            },\n            \"valutaDate\": \"2022-02-04\"\n        },\n        {\n            \"amount\": {\n                \"unit\": \"EUR\",\n                \"value\": \"-1787.4\"\n            },\n            \"bookingDate\": \"2022-01-13\",\n            \"bookingStatus\": \"BOOKED\",\n            \"creditor\": null,\n            \"deptor\": null,\n            \"directDebitCreditorId\": null,\n            \"directDebitMandateId\": null,\n            \"endToEndReference\": null,\n            \"newTransaction\": false,\n            \"reference\": \"115C1DXT50ND3W81/2\",\n            \"remittanceInfo\": \"01BIONTECH SE SPON. ADRS 1           02WPKNR: A2PSR2  ISIN: US09075V1026  \",\n            \"remitter\": null,\n            \"transactionType\": {\n                \"key\": \"SECURITIES\",\n                \"text\": \"Securities\"\n            },\n            \"valutaDate\": \"2022-01-17\"\n        },\n        {\n            \"amount\": {\n                \"unit\": \"EUR\",\n                \"value\": \"-49.92\"\n            },\n            \"bookingDate\": \"2022-01-07\",\n            \"bookingStatus\": \"BOOKED\",\n            \"creditor\": null,\n            \"deptor\": null,\n            \"directDebitCreditorId\": null,\n            \"directDebitMandateId\": null,\n            \"endToEndReference\": null,\n            \"newTransaction\": false,\n            \"reference\": \"2X5C1DXO02EJC8PN/2\",\n            \"remittanceInfo\": \"01VANG.FTSE A.-WO.U.ETF DLD          02WPKNR: A1JX52  ISIN: IE00B3RBWM25  \",\n            \"remitter\": null,\n            \"transactionType\": {\n                \"key\": \"SECURITIES\",\n                \"text\": \"Securities\"\n            },\n            \"valutaDate\": \"2022-01-11\"\n        },\n        {\n            \"amount\": {\n                \"unit\": \"EUR\",\n                \"value\": \"-25\"\n            },\n            \"bookingDate\": \"2022-01-07\",\n            \"bookingStatus\": \"BOOKED\",\n            \"creditor\": null,\n            \"deptor\": null,\n            \"directDebitCreditorId\": null,\n            \"directDebitMandateId\": null,\n            \"endToEndReference\": null,\n            \"newTransaction\": false,\n            \"reference\": \"2X5C1DXO02YQCMQZ/2\",\n            \"remittanceInfo\": \"01XTR.ST.GL.SE.DI.100SW.1D           02WPKNR: DBX1DG  ISIN: LU0292096186  \",\n            \"remitter\": null,\n            \"transactionType\": {\n                \"key\": \"SECURITIES\",\n                \"text\": \"Securities\"\n            },\n            \"valutaDate\": \"2022-01-11\"\n        },\n        {\n            \"amount\": {\n                \"unit\": \"EUR\",\n                \"value\": \"75\"\n            },\n            \"bookingDate\": \"2022-01-06\",\n            \"bookingStatus\": \"BOOKED\",\n            \"creditor\": null,\n            \"deptor\": {\n                \"bic\": \"BYLADEM1001\",\n                \"holderName\": \"Andreas Wuest\",\n                \"iban\": \"DE05120300000012096525\"\n            },\n            \"directDebitCreditorId\": \"DE38ZZZ00000020140\",\n            \"directDebitMandateId\": \"210613230012DE76200411440869927400\",\n            \"endToEndReference\": \"nicht angegeben\",\n            \"newTransaction\": false,\n            \"reference\": \"H322200595513910/2\",\n            \"remittanceInfo\": \"01DKB Konto Lastschrift Sparplan 1   02End-to-End-Ref.:                   03nicht angegeben                    04CORE / Mandatsref.:                05210613230012DE76200411440869927400 06Gläubiger-ID:                      07DE38ZZZ00000020140                 \",\n            \"remitter\": null,\n            \"transactionType\": {\n                \"key\": \"MISCELLANEOUS\",\n                \"text\": \"Miscellaneous\"\n            },\n            \"valutaDate\": \"2022-01-06\"\n        },\n        {\n            \"amount\": {\n                \"unit\": \"EUR\",\n                \"value\": \"-35.96\"\n            },\n            \"bookingDate\": \"2022-01-04\",\n            \"bookingStatus\": \"BOOKED\",\n            \"creditor\": {\n                \"bic\": \"INGDDEFFXXX\",\n                \"holderName\": \"Andreas Wuest\",\n                \"iban\": \"DE68500105175403348178\"\n            },\n            \"deptor\": null,\n            \"directDebitCreditorId\": null,\n            \"directDebitMandateId\": null,\n            \"endToEndReference\": \"nicht angegeben\",\n            \"newTransaction\": false,\n            \"reference\": \"GX222004J0117430/2\",\n            \"remittanceInfo\": \"01Dividenden Papa                    02End-to-End-Ref.:                   03nicht angegeben                    \",\n            \"remitter\": null,\n            \"transactionType\": {\n                \"key\": \"TRANSFER\",\n                \"text\": \"Transfer\"\n            },\n            \"valutaDate\": \"2022-01-04\"\n        }\n    ]\n}");
        // commbankAccountService.getTransactions(accountId);
    }

    SilicaFlickable {

        anchors.fill: parent
        topMargin: errorColumn.visible ? Theme.horizontalPageMargin : 0
        contentWidth: parent.width
        contentHeight: transactionsColumn.visible ? transactionsColumn.height : ( errorColumn.visible ? errorColumn.height : parent.height )

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
                source: "../../icons/commbank.png"
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
                source: "../../icons/commbank.png"
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

//            Button {
//                id: errorOkButton
//                text: qsTr("OK")
//                anchors {
//                    horizontalCenter: parent.horizontalCenter
//                }
//                onClicked: {
//                    inError = false;
//                    if (errorColumn.retryPossible) {
//                        errorColumn.visible = false;
//                        loadingColumn.visible = true;
//                    } else {
//                        errorColumn.visible = false;
//                        pageStack.pop();
//                    }

//                }
//            }
        }

        Column {
            id: transactionsColumn
            width: parent.width
            spacing: Theme.paddingMedium

            Behavior on opacity { NumberAnimation {} }
            opacity: visible ? 1 : 0
            visible: false

            PageHeader {
                id: transactionsHeader
            }

            SilicaListView {

                id: transactionsListView

                height: transactionsPage.height - transactionsHeader.height - Theme.paddingMedium
                width: parent.width
                anchors.left: parent.left
                anchors.right: parent.right

                clip: true

                model: ListModel {
                    id: transactionsModel
                }

                delegate: ListItem {
                    contentHeight: resultItem.height + ( 2 * Theme.paddingMedium )
                    contentWidth: parent.width

                    onClicked: {
                        var selectedTransaction = transactionsModel.get(index);
                        pageStack.push(Qt.resolvedUrl("SingleTransactionPage.qml"),
                                       {"singleTransaction": selectedTransaction});
                    }

                    Item {
                        id: resultItem
                        width: parent.width
                        height: resultRow.height + transactionSeparator.height
                        y: Theme.paddingMedium

                        Row {
                            id: resultRow
                            width: parent.width - ( 2 * Theme.horizontalPageMargin )
                            spacing: Theme.paddingMedium
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            Column {
                                width: parent.width / 3 * 2 - Theme.paddingSmall
                                Text {
                                    id: transactionDateText
                                    width: parent.width
                                    font.pixelSize: Theme.fontSizeTiny
                                    color: Theme.secondaryColor
                                    visible: !transactionsPage.isPortfolio
                                    text: Functions.prettyTime(bookingDate, Locale.LongFormat);
                                }
                                Text {
                                    id: otherPartyNameText
                                    width: parent.width
                                    font.pixelSize: Theme.fontSizeSmall
                                    font.bold: true
                                    color: Theme.primaryColor
                                    elide: Text.ElideRight
                                    maximumLineCount: 1
                                    wrapMode: Text.Wrap
                                    visible: text ? true : false
                                    text: Functions.determineThirdParty(deptor, creditor, remittanceInfo);
                                        // "TODO Other Party name"
                                        // transactionsPage.isPortfolio ? modelData.itemId : modelData.details.otherPartyName
                                    textFormat: Text.StyledText
                                }
                                Text {
                                    id: transactionTextText
                                    width: parent.width
                                    font.pixelSize: Theme.fontSizeTiny
                                    color: Theme.secondaryHighlightColor
                                    text: transactionsPage.isPortfolio ? "" : transactionType.text
                                    elide: Text.ElideRight
                                    maximumLineCount: 1
                                    visible: !transactionsPage.isPortfolio
                                }
                                Text {
                                    id: transactionPurposeText
                                    width: parent.width
                                    font.pixelSize: Theme.fontSizeExtraSmall
                                    color: Theme.primaryColor
                                    text: Functions.determineRemittanceInfo(remittanceInfo)
                                        // transactionsPage.isPortfolio ? ( qsTr("<b>Amount: </b> %1").arg((modelData.amountNegative ? "-" : "") + Number(modelData.amount).toLocaleString(Qt.locale(), "f", 2)) + "<br>" + qsTr("<b>Price: </b> %1 %2").arg(Number(modelData.price).toLocaleString(Qt.locale(), "f", 2)).arg(modelData.priceCurrency) ) : modelData.details.transactionPurpose
                                    textFormat: Text.StyledText
                                    wrapMode: Text.Wrap
                                    elide: Text.ElideRight
                                    maximumLineCount: 4
                                }
                            }
                            Text {
                                id: accountValueText
                                width: parent.width / 3 * 1 - Theme.paddingSmall
                                height: parent.height
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignVCenter
                                font.pixelSize: Theme.fontSizeMedium
                                color: Theme.highlightColor
                                text: Functions.formatAmount(amount);
                            }
                        }

                        Separator {
                            id: transactionSeparator
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

        VerticalScrollDecorator {}
    }

}

