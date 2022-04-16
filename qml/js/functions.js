.pragma library

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

function determineRemittanceInfo(remittanceInfo) {
    var result = "";
    if (remittanceInfo) {
        if (remittanceInfo.length >= (37 * 1)) {
           result = result + remittanceInfo.substring(2 + (0 * 37), 37 * 1);
        }
        if (remittanceInfo.length >= (37 * 2)) {
           result = result + remittanceInfo.substring(2 + (1 * 37), 37 * 2);
        }
        if (remittanceInfo.length >= (37 * 3)) {
           result = result + remittanceInfo.substring(2 + (2 * 37), 37 * 3);
        }
        if (remittanceInfo.length >= (37 * 4)) {
           result = result + remittanceInfo.substring(2 + (3 * 37), 37 * 4);
        }
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

function prettyTime(dateString, format) {
    // expects date in format "yyyy-mm-dd"
    var date = Date.fromLocaleString(Qt.locale(), dateString, "yyyy-MM-dd");
    // var date = Date.parse(dateString);
    console.log("date -> " + date)
    return date.toLocaleDateString(Qt.locale(), format)

}
