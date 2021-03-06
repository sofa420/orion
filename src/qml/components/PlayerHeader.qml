import QtQuick 2.5
import "../styles.js" as Styles

Item {
    property string text
    property int thickness: dp(60)
    property string color: Styles.shadeColor
    id: root

    height: thickness

    Rectangle {
        id: shade
        color: root.color
        opacity: .8
        anchors.fill: parent
    }

    Item {
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
            margins: dp(20)
        }
        width: _text.width

        Text {
            id: _text
            anchors.centerIn: parent
            color: Styles.textColor
            font.pixelSize: Styles.titleFont.bigger
            text: root.text
            z: root.z + 1
            //renderType: Text.NativeRendering
        }
    }

    Behavior on height {
        NumberAnimation {
            duration: 200
            easing.type: Easing.OutCubic
        }
    }

    function show(){
        height = thickness
    }

    function hide(){
        height = -thickness
    }

    Component.onCompleted: hide()
}
