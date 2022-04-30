import QtQuick 2.6
import Sailfish.Silica 1.0

import "../js/functions.js" as Functions

Column {
    property variant singleTransaction;

    id: transactionColumn
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
