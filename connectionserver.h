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
    bool isLoggedIn()const{return auth;}
private:
    void parseMessage(QString);
    void setAuthorized(bool b){auth = b;}
    bool auth;
    QString user;
signals:
    void newMessage(ConnectionServer*, QString);
    //void connected(ConnectionServer*, QString);
    void signalDisconnected(ConnectionServer*);
    void connectionError(ConnectionServer*, QString,QString);
    void loggedIn(QString);
private slots:
    void recibirDataServer();
    void slotProcesarError(QAbstractSocket::SocketError);
    void loggedIn(QString);
public slots:
    void sendMessage(QString);
    void sendList(QList<QString>);
    void sendListAndImage(QList<QString>, QImage);
    void desconectar();
};

#endif // CONNECTIONSERVER_H
