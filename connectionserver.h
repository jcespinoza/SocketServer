#ifndef CONNECTIONSERVER_H
#define CONNECTIONSERVER_H
#include <QTcpServer>
#include <QByteArray>
#include <QDataStream>

class ConnectionServer: public QObject
{
    Q_OBJECT
public:
    explict ConnectionServer(QTcpSocket*);
    QTcpSocket* socket;
    ~ConnectionServer(){ this->socket->deleteLater();}
    QByteArray message;
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
