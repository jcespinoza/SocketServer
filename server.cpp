#include "server.h"

Server::Server(QObject* parent):QTcpServer(parent)
{
    server_started = false;
    connect(this, SIGNAL(newConnection()), this, SLOT(newConnection()));
    listaCon.clear();
}

void Server::sendMessage(int cual, QString message){
}


void Server::sendMessage(int cual, QList<QString> lista){

}

void Server::sendMessage(int cual, QList<QString> lista, QImage img){

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
            temp->sendMessage("SERVERSTOP");
            delete temp;
        }
        this->close();
        emit newLogMessage("Server Stopped");
    }
}

void Server::disconnected(ConnectionServer *){

}

void Server::newConnection(){
    ConnectionServer* con = new ConnectionServer(nextPendingConnection());
    listaCon.push_back(con);
    //connect(con, SIGNAL(newMessage(ConnectionServer*, QString)), this, SLOT(procesarMensaje(ConnectionServer*, QString)));
    //connect signal disconnected eith slot disonnected
    //emit newConexion(
}
