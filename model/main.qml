import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15

ApplicationWindow {
    width: 400
    height: 400
    visible: true

    ListView {
        id: treeView
        width: 400
        height: 400
        delegate: treeDelegate
        model: treeModel

        Component {
            id: treeDelegate

            Item {
                id: wrapper
                height: 50
                width: treeView.width
                Rectangle {
                    height: 1
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    color: "black"
                }

                Item {
                    id: levelMarginElement
                    width: (level>5?6:level)*32 + 5
                    anchors.left: parent.left
                }

                Item {
                    id: nodeOpenElement
                    anchors.left: levelMarginElement.right
                    anchors.verticalCenter: wrapper.verticalCenter
                    height: 24
                    state: "leafNode"

                    Image {
                        id: triangleOpenImage
                        width: nodeOpenElement.width * 0.5
                        height: nodeOpenElement.height * 0.5
                        source: "qrc:/triangle.png"
                        anchors.verticalCenter: nodeOpenElement.verticalCenter
                        fillMode : Image.Stretch
                        transform: Rotation {id: route
                            origin.x: triangleOpenImage.paintedHeight
                            origin.y:  triangleOpenImage.paintedWidth

                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                isOpened ?
                                treeModel.closeItem(index) :
                                treeModel.openItem(index)
                            }
                        }
                    }
                    states: [
                        State {
                            name: "leafNode"
                            when: !hasChildren
                            PropertyChanges {
                                target: nodeOpenElement
                                visible: false
                                width: 0
                            }
                        },
                        State {
                            name: "openedNode"
                            when: (hasChildren)&&(isOpened)
                            PropertyChanges {
                                target: nodeOpenElement
                                visible: true
                                width: 32
                            }
                            PropertyChanges {
                                target: route
                                angle: 90
                            }
                        },
                        State {
                            name: "closedNode"
                            when: (hasChildren)&&(!isOpened)
                            PropertyChanges {
                                target: nodeOpenElement
                                visible: true
                                width: 32

                            }
                            PropertyChanges {
                                target: route
                                angle: 0
                            }
                        }
                    ]
                }

                Text {
                    id: nameTextElement
                    text: name
                    verticalAlignment: "AlignVCenter"
                    anchors.left: nodeOpenElement.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                }
            }
        }
    }
}
