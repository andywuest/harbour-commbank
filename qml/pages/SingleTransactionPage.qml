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
import QtQuick 2.2
import Sailfish.Silica 1.0

import "../js/functions.js" as Functions

Page {
    id: transactionPage
    allowedOrientations: Orientation.All

    property variant singleTransaction;

    SilicaFlickable {

        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: singleTransactionColumn.height

        Column {
            id: singleTransactionColumn
            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                id: singleTransactionHeader
                title: qsTr("Transaction")
            }
            Column {
                id: transactionColumn
                width: parent.width - ( 2 * Theme.horizontalPageMargin )
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: Theme.paddingMedium

                SectionHeader {
                    text: qsTr("Amount")
                }

                Text {
                    id: amountText
                    width: parent.width
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    font.bold: true
                    text: Functions.formatAmount(singleTransaction.amount)
                }

                SectionHeader {
                    text: qsTr("Payer Name")
                    visible: otherPartyNameText.visible
                }

                Text {
                    id: otherPartyNameText
                    width: parent.width
                    font.pixelSize: Theme.fontSizeMedium
                    font.bold: true
                    color: Theme.primaryColor
                    elide: Text.ElideRight
                    maximumLineCount: 2
                    wrapMode: Text.Wrap
                    visible: Functions.deptorOrCreditorAvailable(singleTransaction)
                    text: Functions.determineThirdPartyDetails(singleTransaction, "holderName")
                }

                SectionHeader {
                    text: qsTr("IBAN")
                    visible: ibanText.visible
                }

                Text {
                    id: ibanText
                    width: parent.width
                    visible: Functions.deptorOrCreditorAvailable(singleTransaction)
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    text: Functions.determineThirdPartyDetails(singleTransaction, "iban") // TODO iban format
                    maximumLineCount: 1
                }

                SectionHeader {
                    text: qsTr("BIC")
                    visible: bicText.visible
                }

                Text {
                    id: bicText
                    width: parent.width
                    visible: Functions.deptorOrCreditorAvailable(singleTransaction)
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    text: Functions.determineThirdPartyDetails(singleTransaction, "bic")
                    maximumLineCount: 1
                }

                SectionHeader {
                    text: qsTr("Booking Text")
                    visible: transactionTextText.visible
                }

                Text {
                    id: transactionTextText
                    width: parent.width
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    text: Functions.determineRemittanceInfo(singleTransaction.remittanceInfo)
                    elide: Text.ElideRight
                    maximumLineCount: 4
                }

                SectionHeader {
                    text: qsTr("End-To-End-Reference")
                    visible: endToEndReferenceText.visible
                }

                Text {
                    id: endToEndReferenceText
                    width: parent.width
                    visible: Functions.valueAvailable(singleTransaction.endToEndReference);
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    text: Functions.determineValue(singleTransaction.endToEndReference)
                    wrapMode: Text.Wrap
                    maximumLineCount: 1
                    elide: Text.ElideRight
                }

                SectionHeader {
                    text: qsTr("Mandate Reference")
                    visible: mandateReferenceText.visible
                }

                Text {
                    id: mandateReferenceText
                    width: parent.width
                    visible: Functions.valueAvailable(singleTransaction.directDebitMandateId);
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    text: Functions.determineValue(singleTransaction.directDebitMandateId)
                    wrapMode: Text.Wrap
                    maximumLineCount: 1
                    elide: Text.ElideRight
                }

                SectionHeader {
                    text: qsTr("Creditor Identification Number")
                    visible: creditorIdText.visible
                }

                Text {
                    id: creditorIdText
                    width: parent.width
                    visible: Functions.valueAvailable(singleTransaction.directDebitCreditorId);
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    text: Functions.determineValue(singleTransaction.directDebitCreditorId)
                    wrapMode: Text.Wrap
                    maximumLineCount: 1
                    elide: Text.ElideRight
                }

                SectionHeader {
                    text: qsTr("Reference")
                }

                Text {
                    id: transactionReferenceText
                    width: parent.width
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    text: singleTransaction.reference
                    wrapMode: Text.Wrap
                    maximumLineCount: 1
                    elide: Text.ElideRight
                }

                SectionHeader {
                    text: qsTr("Booking Day")
                }

                Text {
                    id: accountValueBookingText
                    width: parent.width
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    text: Functions.prettyTime(singleTransaction.bookingDate, Locale.ShortFormat)
                }

                SectionHeader {
                    text: qsTr("Valuation Day")
                }

                Text {
                    id: accountValueText
                    width: parent.width
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    text: Functions.prettyTime(singleTransaction.valutaDate, Locale.ShortFormat)
                }

                SectionHeader {
                    text: qsTr("Transaction Type")
                }

                Text {
                    id: transactionTypeText
                    width: parent.width
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    text: singleTransaction.transactionType.key
                    wrapMode: Text.Wrap
                    elide: Text.ElideRight
                }
            }

            VerticalScrollDecorator {}

        }

    }

}
