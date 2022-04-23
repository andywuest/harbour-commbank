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

Dialog {
    id: storeCredentialsDialog

    property string name
    property string username

    Column {
        width: parent.width

        DialogHeader {
        }

        SectionHeader {
            text: qsTr("Store credentials")
        }

        Label {
            id: storeCredentialsHintText
            text: qsTr("Please provide the name under which you want to store the credentials for the username '%1'.").arg(username);
            visible: true
            font.pixelSize: Theme.fontSizeSmall
            wrapMode: Text.Wrap
            anchors {
                left: parent.left
                right: parent.right
                margins: Theme.paddingLarge
            }
        }

        TextField {
            id: accountNameTextField
            width: parent.width
            placeholderText: qsTr("Name for the credentials")
            label: qsTr("Account credentials name")
        }
    }

    onDone: {
        if (result == DialogResult.Accepted) {
            name = accountNameTextField.text
        }
    }
}
