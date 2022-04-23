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
                title: qsTr("Transaction Details")
            }

            Column {
                id: transactionColumn
                width: parent.width - ( 2 * Theme.horizontalPageMargin )
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: Theme.paddingMedium

                TransactionDetail {
                    title: qsTr("Amount")
                    label: Functions.formatAmount(singleTransaction.amount)
                    fontBold: true
                }

                TransactionDetail {
                    title: qsTr("Payer Name")
                    detailVisible: Functions.deptorOrCreditorAvailable(singleTransaction)
                    label: Functions.determineThirdPartyDetails(singleTransaction, "holderName")
                    lineCount: 2
                }

                TransactionDetail {
                    title: qsTr("IBAN")
                    detailVisible: Functions.deptorOrCreditorAvailable(singleTransaction)
                    label: Functions.formatIBAN(Functions.determineThirdPartyDetails(singleTransaction, "iban"))
                }

                TransactionDetail {
                    title: qsTr("BIC")
                    detailVisible: Functions.deptorOrCreditorAvailable(singleTransaction)
                    label: Functions.determineThirdPartyDetails(singleTransaction, "bic")
                }

                TransactionDetail {
                    title: qsTr("Booking Text")
                    label: Functions.determineRemittanceInfo(singleTransaction.remittanceInfo, true)
                    lineCount: 4
                }

                TransactionDetail {
                    title: qsTr("End-To-End-Reference")
                    detailVisible: Functions.valueAvailable(singleTransaction.endToEndReference)
                    label: Functions.determineValue(singleTransaction.endToEndReference)
                }

                TransactionDetail {
                    title: qsTr("Mandate Reference")
                    detailVisible: Functions.valueAvailable(singleTransaction.directDebitMandateId)
                    label: Functions.determineValue(singleTransaction.directDebitMandateId)
                }

                TransactionDetail {
                    title: qsTr("Creditor Identification Number")
                    detailVisible: Functions.valueAvailable(singleTransaction.directDebitCreditorId);
                    label: Functions.determineValue(singleTransaction.directDebitCreditorId)
                }

                TransactionDetail {
                    title: qsTr("Reference")
                    label: singleTransaction.reference
                }

                TransactionDetail {
                    title: qsTr("Booking Day")
                    label: Functions.prettyTime(singleTransaction.bookingDate, Locale.ShortFormat)
                }

                TransactionDetail {
                    title: qsTr("Valuation Day")
                    label: Functions.prettyTime(singleTransaction.valutaDate, Locale.ShortFormat)
                }

                TransactionDetail {
                    title: qsTr("Transaction Type")
                    label: Functions.determineTransactionTypeLabel(singleTransaction)
                }
            }

            VerticalScrollDecorator {}
        }

    }

}
