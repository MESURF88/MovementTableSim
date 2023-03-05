import QtQuick 2.12
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QmlTableModel 1.0
import Direction 1.0

ApplicationWindow {
    id: root
    visible: true
    width: 760
    height: 400
    minimumWidth: 475
    minimumHeight: 300

    readonly property int darkGreenA: 255
    readonly property int darkGreenR: 23
    readonly property int darkGreenG: 90
    readonly property int darkGreenB: 42

    color: "White"
    title: qsTr("Conway’s Game of Life")

    //! [tableview]
    TableView {
        id: tableView
        anchors.fill: parent
        focus: true
        ScrollBar.horizontal: ScrollBar {}
        ScrollBar.vertical: ScrollBar {}

        Keys.onRightPressed: {
            tableModelVis.mover(Direction.RIGHT);
        }
        Keys.onLeftPressed: {
            tableModelVis.mover(Direction.LEFT);
        }
        Keys.onUpPressed: {
            tableModelVis.mover(Direction.UP);
        }
        Keys.onDownPressed: {
            tableModelVis.mover(Direction.DOWN);
        }

        delegate: Rectangle {
            id: cell
            implicitWidth: 2
            implicitHeight: 2

            color: Qt.rgba(Math.min(1, (darkGreenR + 255 * model.value)/255),Math.min(1, (darkGreenG + 255 * model.value)/255), Math.min(1, (darkGreenB + 255 * model.value)/255), darkGreenA)
        }
        //! [tableview]

        //! [model]
        model: QmlTableModel
        {
            id: tableModelVis
        }

        //! [model]

        //! [scroll]
        contentX: (contentWidth - width) / 2;
        contentY: (contentHeight - height) / 2;
        //! [scroll]
    }

    footer: Rectangle {
        signal nextStep

        id: footer
        height: 50
        color: "#F3F3F4"

        RowLayout {
            anchors.centerIn: parent

            //! [next]
            Button {
                text: qsTr("Start")
                onClicked: { tableModelVis.startSim() }
            }
            //! [next]

            Item {
                width: 50
            }
        }

    }
}

