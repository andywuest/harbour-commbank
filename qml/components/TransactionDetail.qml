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
