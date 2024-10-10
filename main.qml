import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import TCPLinkServer 1.0
Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Server")
    color:  "#E8E8E8"

    TCPLinkServer{
        id: linkManager
    }

    Rectangle{
        id:    root
        width: parent.width * 0.9
        height: parent.height * 0.9
        anchors.centerIn:  parent
        color:   "#E8E8E8"
        Rectangle{
            height:                     root.height * 0.9
            width:                      root.width / 2
            anchors.left:               parent.left
            color:                      "transparent"
            Label{
                id:                        titleServer
                anchors.horizontalCenter:  parent.horizontalCenter
                text:                      "Server"
            }

            Flickable{
                anchors.top:            titleServer.bottom
                height:                 parent.height * 0.95
                width:                  parent.width
                contentHeight:          sMsgCol.height
                clip:                   true
                Column{
                    id:                      sMsgCol
                    spacing:                 height * 0.05
                    Repeater{
                        model:              linkManager.serverMessages
                        delegate:
                            Rectangle{
                            height:             sMsg.height * 1.6
                            width:              root.width * 0.45
                            border.color:       "black"
                            radius:              height/15
                            Text{
                                id:               sMsg
                                text:              modelData
                                anchors.centerIn:  parent
                                width:             parent.width * 0.9
                                wrapMode:          Text.WordWrap
                            }
                        }
                    }
                }
            }
        }
        Rectangle{
            height:                     root.height * 0.9
            width:                      root.width / 2
            color:                      "transparent"
            anchors.right:              parent.right
            Label{
                id:                        titleClient
                anchors.horizontalCenter:  parent.horizontalCenter
                text:                      "Client"
            }
            Flickable{
                anchors.top:             titleClient.bottom
                height:                 parent.height * 0.95
                width:                  parent.width
                contentHeight:          cMsgCol.height
                clip:                   true
                Column{
                    id:                         cMsgCol
                    spacing:                    height * 0.05
                    anchors.right:              parent.right
                    Repeater{
                        id:      cMessagesRepeater
                        model:   linkManager.clientMessages
                        delegate:
                            Rectangle{
                            height:          cMsg.height * 1.6
                            width:           root.width * 0.45
                            border.color:    "black"
                            radius:              height/15
                            Text{
                                id:   cMsg
                                text: modelData
                                anchors.centerIn:  parent
                                width:             parent.width
                                wrapMode:          Text.WordWrap
                            }
                        }
                    }
                }
            }
        }

        Row{
            anchors.bottom:  parent.bottom
            spacing:          parent.width * 0.05
            TextField{
                id:    messageTF
                placeholderText: "Enter Message"
            }
            Rectangle{
                height:   root.height * 0.09
                width:    root.width  * 0.1
                radius:   height/ 8
                border.color:  "#BEBEBE"
                Text{
                    anchors.centerIn:   parent
                    text:     "Send"
                }
                MouseArea{
                    anchors.fill:  parent
                    onClicked: {
                        linkManager.sendMessage(messageTF.text)
                        messageTF.clear()
                    }
                }
            }
        }
    }
}
