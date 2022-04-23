import QtQuick 2.6
import Sailfish.Silica 1.0

Column {
    id: transactionDetailItem
    width: parent.width

    property alias detailVisible: label.visible
    property alias title: title.text
    property alias label: label.text
    property alias fontBold: label.font.bold
    property alias lineCount: label.maximumLineCount

    Text {
        id: title
        width: parent.width
        visible: label.visible

        font.pixelSize: Theme.fontSizeSmall
        color: Theme.highlightColor
        text: ""
    }

    Text {
        id: label
        width: parent.width
        visible: true
        font.pixelSize: Theme.fontSizeSmall
        font.bold: false
        color: Theme.primaryColor
        text: ""
        textFormat: Text.StyledText
        wrapMode: Text.Wrap
        maximumLineCount: 1
        elide: Text.ElideRight
    }

}
