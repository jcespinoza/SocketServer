#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QImage>
#include "connectionserver.h"
class Server: public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject* parent=0);
    ~Server();
    void start(int port);
    void stop();
    bool getStatus(){return server_started;}
    void sendMessage(int cual, QString);
    void sendMessage(int cual, QList<QString>);
    void sendMessage(int cual, QList<QString>,QImage);
    QList<ConnectionServer*> listaCon;

signals:
    void newLogMessage(QString);
    void nuevaConexion(QString);
    void removerConexion(int cual);
private slots:
    void procesarError(ConnectionServer*, QString, QString);
    void newConnection();
    void procesarMensaje(ConnectionServer*, QString);
    void disconnected(ConnectionServer*);
    void validate(ConnectionServer*, QString);

private:
    bool server_started;
    ConnectionServer* god;
};

#endif // SERVER_H
