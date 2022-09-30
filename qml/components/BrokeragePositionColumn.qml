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
    property alias columnLabel: labelText.text
    property alias columnValue: valueText.text

    Text {
        id: labelText
        width: parent.width
        font.pixelSize: Theme.fontSizeTiny
        color: Theme.secondaryColor
    }
    Text {
        id: valueText
        width: parent.width
        font.pixelSize: Theme.fontSizeTiny
        color: Theme.secondaryHighlightColor
    }
}
