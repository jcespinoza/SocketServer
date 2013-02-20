#include "connectionserver.h"

ConnectionServer::ConnectionServer(QTcpSocket* sock)
{
    socket = sock;
    connect(socket, SIGNAL(readyRead()), this, SLOT(recibirDataServer()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(desconectar()));
    //connect del error
}

void ConnectionServer::parseMessage(QString msg){

}
