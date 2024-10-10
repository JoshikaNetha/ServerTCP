#ifndef TCPMANAGER_H
#define TCPMANAGER_H

#include <QObject>
#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
class TCPManager;
class TCPServer : public QTcpServer{
    Q_OBJECT
public:
    TCPServer(QObject *parent = nullptr);
    void startServer();

signals:
    void messageReceived(QString msg);

public slots:
    void onNewConnection();
    void onReadReady();
    void onDisconnected();
    void SendMessagetoClient(QString msg);

private:
    QList<QTcpSocket*>  _client;

    QTcpSocket               *_socket;
};

class TCPWorker: public QThread{
    Q_OBJECT
public:
    TCPWorker(TCPManager* tcpManager);

    void run();
    // void SendMessage(QString msg);


private:
    TCPManager*         _tcpManager;
    TCPServer*  server;

};

class TCPManager : public QObject
{
    Q_OBJECT
public:
    TCPManager(QObject *parent = nullptr);
    Q_PROPERTY(QStringList serverMessages READ serverMessages NOTIFY serverMessagesChanged)
    Q_PROPERTY(QStringList clientMessages READ clientMessages NOTIFY clientMessagesChanged)

    QStringList serverMessages (){ return _serverMessages; }
    QStringList clientMessages (){ return _clientMessages; }

    void startServer();

    Q_INVOKABLE void sendMessage(QString msg);
public slots:
    void receivedMessage(QString msg);

signals:
    void serverMessagesChanged();
    void clientMessagesChanged();
    void sendNotification(QString msg);
private:
    QStringList _serverMessages;
    QStringList _clientMessages ;
    TCPWorker*  _worker;
};

#endif // TCPMANAGER_H
