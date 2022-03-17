import QtQuick 2.2
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
