#include "connectionserver.h"

ConnectionServer::ConnectionServer(QTcpSocket* sock)
{
    socket = sock;
    auth = false;
    connect(socket, SIGNAL(readyRead()), this, SLOT(recibirDataServer()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(desconectar()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotProcesarError(QAbstractSocket::SocketError)));
    connect(this, SIGNAL(loggedIn(QString)), this, SLOT(credentialAccepted(QString)));
}

void ConnectionServer::parseMessage(QString msg){
    qDebug() << "Recibiendo " << msg;
    if(msg.startsWith("LOGOFF",Qt::CaseInsensitive) && !isLoggedIn()){
        setAuthorized(false);
        return;
    }
    if(msg.startsWith("LOGIN:",Qt::CaseInsensitive)){
        msg = msg.mid(6);
        QString u = msg.left(msg.lastIndexOf(":"));
        emit autenticate(this, u);
        return;
    }
    if(!isLoggedIn()){
        sendMessage("Man auntenticate!");
        return;
    }
    if(msg.startsWith("REQMESSAGE:",Qt::CaseInsensitive))
    {
        msg = ":" + msg.mid(11);
        emit newMessage(this,msg);
    }
    if(msg.startsWith("BCM:",Qt::CaseInsensitive)){
        msg = msg.mid(4);
        QString bd = msg.left(msg.size() - 1).trimmed();
        emit broadCastM(this, bd);
    }
    if(msg.startsWith("@")){
        msg = msg.right(msg.size() - 1);
        QString dest = msg.left(msg.indexOf(":"));
        QString mess = msg.mid(dest.size()+1).trimmed();
        sendMessage("You to "+ dest + ": " + mess);
        emit sendMTo(this, dest, mess);
    }
    if(msg.startsWith("HELP",Qt::CaseInsensitive)){
        QString comandos = "\nHELP -> Muestra esta ayuda.";
        comandos.append("\nLOGIN:<user>:<pass> -> Se auntentica en el Servidor.");
        comandos.append("\n@<user>:<mensaje> -> Envia un mensaje a ese usuario.");
        comandos.append("\nBCM:<mensaje> -> Envia un mensaje.");
        comandos.append("\nREQLIST -> Solicita la lista de conectados.\n");
        sendMessage(comandos);
    }
    if(msg.startsWith("REQLIST", Qt::CaseInsensitive)){
        emit askList(this);
    }
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

void ConnectionServer::sendList(QList<QString> lista){
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    if (socket->isValid())
    {
        out.setVersion(QDataStream::Qt_5_0);
        out << quint32(0);
        out << quint8('M');
        out << lista;
        out.device()->seek(0);
        out << quint32(block.size() - sizeof(quint32));

        this->socket->write(block);
    }
}

void ConnectionServer::sendMessage(QString messag){
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    if (socket->isValid())
    {
        out.setVersion(QDataStream::Qt_5_0);
        out << quint32(0);
        out << quint8('M');
        out << messag;
        out.device()->seek(0);
        out << quint32(block.size() - sizeof(quint32));

        this->socket->write(block);
    }
}

void ConnectionServer::slotProcesarError(QAbstractSocket::SocketError){

}

void ConnectionServer::setUser(QString s){
    user = s;
}

void ConnectionServer::credentialAccepted(QString s){
    setUser(s);
    setAuthorized(true);
}
