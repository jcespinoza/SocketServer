#ifndef CONNECTIONSERVER_H
#define CONNECTIONSERVER_H
#include <QTcpServer>
#include <QByteArray>
#include <QDataStream>
#include <QTcpSocket>
#include <QImage>

class ConnectionServer: public QObject
{
    Q_OBJECT
public:
    explicit ConnectionServer(QTcpSocket*);
    QTcpSocket* socket;
    QByteArray message;
    QString getIP()const{return socket->peerAddress().toString();}
    ~ConnectionServer(){ this->socket->deleteLater();}
private:
    void parseMessage(QString);
signals:
    void newMessage(ConnectionServer*, QString);
    //void connected(ConnectionServer*, QString);
    void signalDisconnected(ConnectionServer*);
    void connectionError(ConnectionServer*, QString,QString);
private slots:
    void recibirDataServer();
    void slotProcesarError(QAbstractSocket::SocketError);
public slots:
    void sendMessage(QString);
    void sendList(QList<QString>);
    void sendListAndImage(QList<QString>, QImage);
    void desconectar();
};

#endif // CONNECTIONSERVER_H
