#include "server.h"

Server::Server(QObject* parent):QTcpServer(parent)
{
    server_started = false;
    connect(this, SIGNAL(newConnection()), this, SLOT(newConnection()));
    listaCon.clear();
}

void Server::sendMessage(int cual, QString message){
    if(listaCon.at(cual)->socket->isValid())
        listaCon.at(cual)->sendMessage(message);
}


void Server::sendMessage(int cual, QList<QString> lista){
    if(listaCon.at(cual)->socket->isValid())
        listaCon.at(cual)->sendList(lista);
}

void Server::sendMessage(int cual, QList<QString> lista, QImage img){
    if(listaCon.at(cual)->socket->isValid())
        listaCon.at(cual)->sendListAndImage(lista, img);
}

void Server::start(int port){
    if(!server_started){
        if(listen(QHostAddress::Any, port)){
            server_started = true;
            listaCon.clear();
            emit newLogMessage("Servidor iniciado en puerto: " + QString::number(port));
        }else{
            server_started = false;
            emit newLogMessage("Servidor no se pudo iniciar: " + errorString());
        }
    }
}

void Server::stop(){
    if(server_started){
        server_started = false;
        while(listaCon.isEmpty()){
            ConnectionServer* temp = listaCon.takeFirst();
            //temp->sendMessage("SERVERSTOP");
            if(temp != 0)
                delete temp;
        }
        this->close();
        emit newLogMessage("Server Stopped");
    }
}

void Server::disconnected(ConnectionServer *con){
    int i = 0;
    qDebug() << "Disconnected " << con;
    while(i < listaCon.count()){
        if(listaCon.at(i) == con){
            listaCon[i] = 0;
            break;
        }
        i++;
    }
    delete con;
    emit removerConexion(i);
}

void Server::newConnection(){
    if( !hasPendingConnections()){
        qDebug() << "It didn't";
        return;
    }
        ConnectionServer* con = new ConnectionServer(nextPendingConnection());
        qDebug() << "it gets here";
        listaCon.push_back(con);
        connect(con, SIGNAL(newMessage(ConnectionServer*,QString)), this, SLOT(procesarMensaje(ConnectionServer*,QString)));
        connect(con, SIGNAL(signalDisconnected(ConnectionServer*)), this, SLOT(disconnected(ConnectionServer*)));
        connect(con, SIGNAL(connectionError(ConnectionServer*,QString,QString)), this, SLOT(procesarError(ConnectionServer*,QString,QString)));
        qDebug() << con->socket->peerAddress().toString();
        emit nuevaConexion(con->socket->peerAddress().toString());
}

Server::~Server(){
    this->stop();
}

void Server::procesarMensaje(ConnectionServer *, QString msg){
    emit newLogMessage("Mensaje de Cliente: " + msg);
}

void Server::procesarError(ConnectionServer *, QString title, QString error){
    emit newLogMessage(title + " : " + error);
}
