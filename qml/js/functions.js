.pragma library

Qt.include('constants.js');

function deptorOrCreditorAvailable(transaction) {
   // TODO DEBTOR_CREDITOR_TRANSACTION - not all types are mapped yet
   if (transaction.transactionType && transaction.transactionType.key) {
       if (DEBTOR_CREDITOR_TRANSACTION.indexOf(transaction.transactionType.key) !== -1) {
           return true;
       }
   }
   if (transaction.deptor && transaction.deptor.iban) {
       return true;
   }
   if (transaction.creditor && transaction.creditor.iban) {
       return true;
   }
   return false;
}

function determineThirdPartyDetails(transaction, key) {
    if (deptorOrCreditorAvailable(transaction)) {
        if (transaction.deptor && transaction.deptor[key]) {
            return transaction.deptor[key];
        }
        if (transaction.creditor && transaction.creditor[key]) {
            return transaction.creditor[key];
        }
    }
    return "";
}

function determineThirdParty(debtor, creditor, remittanceInfo) {
    if (debtor && debtor.holderName) {
        return debtor.holderName;
    }
    if (creditor && creditor.holderName) {
        return creditor.holderName;
    }
    if (remittanceInfo) {
        if (remittanceInfo.length > 37) {
            return remittanceInfo.substring(2, 37);
        }
        return remittanceInfo;
    }
    return "???";
}

function valueAvailable(value) {
    if (value && value.length > 0) {
        return true;
    }
    return false;
}

function determineValue(value) {
    if (value) {
        return value;
    }
    return "";
}

function determineRemittanceInfo(remittanceInfo, linebreak) {
    var result = "";
    if (remittanceInfo) {
        result = getRemittanceSegmentText(result, remittanceInfo, 1, linebreak);
        result = getRemittanceSegmentText(result, remittanceInfo, 2, linebreak);
        result = getRemittanceSegmentText(result, remittanceInfo, 3, linebreak);
        result = getRemittanceSegmentText(result, remittanceInfo, 4, linebreak);
//        result = getRemittanceSegmentText(result, remittanceInfo, 5, linebreak);
//        result = getRemittanceSegmentText(result, remittanceInfo, 6, linebreak);
//        result = getRemittanceSegmentText(result, remittanceInfo, 7, linebreak);
    }
    return result;
}

function getRemittanceSegmentText(providedResult, remittanceInfo, segmentId, linebreak) {
    var result = providedResult;
    if (remittanceInfo.length >= (37 * segmentId)) {
       result += remittanceInfo.substring(2 + ((segmentId - 1) * 37), 37 * segmentId);
       result += (linebreak ? '<br>' : '');
    }
    return result;
}

function getMappedCurrency(currency) {
    if (currency === "EUR") {
        return "€";
    }
    return currency;
}

function formatAmount(amount) {
    if (amount) {
       return Number(amount.value).toLocaleString(Qt.locale(), "f", 2) + " " + getMappedCurrency(amount.unit);
    }
    return "-";
}

function determineTransactionTypeLabel(transaction) {
    var result = TRANSACTION_TYPES[transaction.transactionType.key];
    if (result) {
        return result;
    }
    console.log("============> unmapped key : " + transaction.transactionType.key);
    return transaction.transactionType.key;
}

function prettyTime(dateString, format) {
    // expects date in format "yyyy-mm-dd"
    var date = Date.fromLocaleString(Qt.locale(), dateString, "yyyy-MM-dd");
    // var date = Date.parse(dateString);
    console.log("date -> " + date)
    return date.toLocaleDateString(Qt.locale(), format)

}

// found here: https://stackoverflow.com/questions/17260238/how-to-insert-space-every-4-characters-for-iban-registering
function formatIBAN(str, after) {
    if (!str) {
        return false;
    }
    after = after || 4;
    var v = str.replace(/[^\dA-Z]/g, ''),
        reg = new RegExp(".{" + after + "}", "g");
    return v.replace(reg, function (a) {
        return a + ' ';
    });
}

