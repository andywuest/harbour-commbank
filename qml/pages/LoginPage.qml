import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    property bool loaded : false

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    function connectSlots() {
        console.log("connect - slots");
        // var dataBackend = getSecurityDataBackend(watchlistSettings.dataBackend);
        commbankService.loginResultAvailable.connect(loginResultHandler);
        commbankService.requestError.connect(errorResultHandler);
    }

    function disconnectSlots() {
        console.log("disconnect - slots");
        // var dataBackend = getSecurityDataBackend(watchlistSettings.dataBackend);
        commbankService.loginResultAvailable.disconnect(loginResultHandler);
        commbankService.requestError.disconnect(errorResultHandler);
    }

    function loginResultHandler(result) {

    }

    function errorResultHandler(result) {
        console.log("error !!!!");
        loaded = true;
    }

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            MenuItem {
                text: qsTr("Show Page 2")
                onClicked: pageStack.push(Qt.resolvedUrl("SecondPage.qml"))
            }
        }

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("UI Template")
            }
            Label {
                x: Theme.horizontalPageMargin
                text: qsTr("Login Page")
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }
            Button {
                text: "Login"
                onClicked: {
                    console.log("clicked");
                    commbankService.performLogin("username");
                }
            }
        }
    }

    Component.onCompleted: {
        connectSlots();
    }

}
