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
        parseMessage(message.left(pos + 2));
        message = message.mid(pos+2);
    }

}

void ConnectionServer::desconectar(){
    emit signalDisconnected(this);
}

void ConnectionServer::sendListAndImage(QList<QString> lista, QImage image){
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    if (socket->isValid())
       // socket->write(msg.toAscii());
    {

        out.setVersion(QDataStream::Qt_5_0);
        out << quint32(0) ;  // Reservar el espacio para el size
        out << quint8('X') ;  //Tipo de mensaje M
        out << lista;
        out << image;
        // Irse al inicio para actualizar el size
        out.device()->seek(0);
        out << quint32(block.size() - sizeof(quint32));

        this->socket->write(block);

    }
}

void ConnectionServer::sendList(QList<QString>){

}

void ConnectionServer::sendMessage(QString){
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    if (socket->isValid())
    {

        out.setVersion(QDataStream::Qt_4_6);
        out << quint32(0);
        out << quint8('M');
        out << message;
        out.device()->seek(0);
        out << quint32(block.size() - sizeof(quint32));

        this->socket->write(block);

    }
}

void ConnectionServer::slotProcesarError(QAbstractSocket::SocketError){

}

