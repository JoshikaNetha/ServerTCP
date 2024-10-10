#include "TCPManager.h"

TCPManager::TCPManager(QObject *parent)
    : QObject(parent)
{
    _worker = new TCPWorker(this);
    startServer();
}

void TCPManager::startServer()
{
    qDebug()<<"[START SERVER]";
    if (_worker) {
        _worker->start();  // Safely start the worker thread
    } else {
        qDebug() << "Error: Worker thread is not initialized.";
    }

}

void TCPManager::sendMessage(QString msg)
{
    // _worker->SendMessage(msg);
    emit sendNotification(msg);
    _serverMessages.append(msg);
    qDebug()<<"_serverMessages "<<msg;
    emit serverMessagesChanged();
}

void TCPManager::receivedMessage(QString msg)
{
    _clientMessages.append(msg);
    emit clientMessagesChanged();
}

TCPWorker::TCPWorker(TCPManager *tcpManager)
    :_tcpManager(tcpManager)
{
    /*
     * In your case, the QTcpServer was created in one thread (probably the main thread), but then you tried to
     *  use it in another thread (TCPWorker). Since it was created in one thread, but you're trying to work with
     *   it in a different one, Qt doesn’t know how to handle it properly and crashes.
   */

}

void TCPWorker::run()
{

    // TCPServer  server;
    // server = new TCPServer(this);

    // connect(server, &TCPServer::messageReceived, _tcpManager, &TCPManager::receivedMessage);
    // connect(_tcpManager,&TCPManager::sendNotification,server,&TCPServer::SendMessagetoClient);

    // server->startServer();

    /* REASON:
    Incorrect Signal-Slot Connections or Object Ownership:

    You may be connecting signals and slots that cross thread boundaries without moving objects to the appropriate thread.
    This could explain the errors about children being in different threads.
*/
    TCPServer  server;
    connect( &server, &TCPServer::messageReceived, _tcpManager, &TCPManager::receivedMessage);
    connect(_tcpManager,&TCPManager::sendNotification, &server,&TCPServer::SendMessagetoClient);

    server.startServer();
    exec();


}


TCPServer::TCPServer(QObject *parent)
    :QTcpServer(parent)
{
    connect(this,&QTcpServer::newConnection,this,&TCPServer::onNewConnection);
}

void TCPServer::SendMessagetoClient(QString msg)
{
    qDebug()<<"TCPServer::SendMessagetoClient";
    if(!_socket){
        qDebug()<<"Error";
        return;
    }
    _socket->write(msg.toUtf8());
    _socket->flush();
}

void TCPServer::startServer()
{
    qDebug()<<"[SERVER LISTENING]";
    // Start listening on port 12345
    if (this->listen(QHostAddress::Any, 12345)) {
        qDebug() << "Server started on port" << this->serverPort();
    } else {
        qDebug() << "Server could not start!";
    }
}

void TCPServer::onNewConnection()
{
    qDebug()<<"[onNEWCONNECTION]";
    _socket = this->nextPendingConnection();

    if(!_socket){
        qDebug()<<"Error Client Socket ";
        return;
    }
    connect(_socket, SIGNAL(readyRead()), this, SLOT(onReadReady()));
    connect(_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    // QTcpSocket* _clientsocket = this->nextPendingConnection();
    // if(!_clientsocket){
    //     qDebug()<<"Error Client Socket ";
    //     return;
    // }
    // _client.append(_clientsocket);
    // connect(_clientsocket,&QTcpSocket::readyRead,this,&TCPServer::onReadReady);
    // connect(_clientsocket, &QTcpSocket::disconnected, this, &TCPServer::onDisconnected);

}

void TCPServer::onReadReady()
{

    qDebug()<<"[onReadReady]";
     QByteArray data = _socket->readAll();
    // QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    // QByteArray data = clientSocket->readAll();

    qDebug() << "Received data from" << _socket->peerAddress().toString() << ":" << data;
    emit messageReceived(data);

    // // Echo the received data back to the client
    // _socket->write(data);
    qDebug()<<"Echo the received data back to the client ";

}

void TCPServer::onDisconnected()
{
    qDebug()<<"[onDisconnected]";
    // _server->removeClient(this);
    _socket->deleteLater();
    _socket = nullptr;
    exit(0);

}
