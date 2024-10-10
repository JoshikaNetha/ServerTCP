import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import TCPLinkServer 1.0
Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Server")

    TCPLinkServer{
        id: linkManager
    }

    Rectangle{
        id:    root
        width: parent.width * 0.9
        height: parent.height * 0.9
        anchors.centerIn:  parent
        border.color: "black"
        Row{
            Rectangle{
                height:root.height * 0.9
                width: root.width / 2
                border.color: "black"
                Column{
                    Repeater{
                        model: linkManager.serverMessages
                        delegate:Text{
                            text: modelData
                        }
                    }
                }
            }
            Rectangle{
                height:root.height * 0.9
                width: root.width / 2
                border.color: "black"
                Column{
                    Repeater{
                        model: linkManager.clientMessages
                        delegate:Text{
                            text: modelData
                        }
                    }
                }
            }
        }

        Row{
            anchors.bottom:  parent.bottom
            TextField{
                id:    messageTF
                placeholderText: "Enter Message"
            }
            Button{
                text: "Send"
                onClicked:{
                    linkManager.sendMessage(messageTF.text)
                }
            }
        }
    }
}
