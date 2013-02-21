#include "connectionserver.h"

ConnectionServer::ConnectionServer(QTcpSocket* sock)
{
    socket = sock;
    connect(socket, SIGNAL(readyRead()), this, SLOT(recibirDataServer()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(desconectar()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotProcesarError(QAbstractSocket::SocketError)));
}

void ConnectionServer::parseMessage(QString msg){

}

void ConnectionServer::recibirDataServer(){
    message.append(socket->readAll());

    int pos;
    while( (pos = message.indexOf("\n\r")) > -1){
        parseMessage(message.left(por + 2));
        message = message.mid(pos+2);
    }

}

void ConnectionServer::desconectar(){
    emit signalDisconnected(this);
}

void ConnectionServer::sendListAndImage(QList<QString>, QImage){

}

void ConnectionServer::sendList(QList<QString>){

}

void ConnectionServer::sendMessage(QString){

}

void ConnectionServer::slotProcesarError(QAbstractSocket::SocketError){

}

