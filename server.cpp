#include "server.h"

Server::Server(QObject* parent):QTcpServer(parent)
{
    server_started = false;
    god = 0;
    connect(this, SIGNAL(newConnection()), this, SLOT(newConnection()));
    listaCon.clear();
}

void Server::sendMessage(int cual, QString message){
    if(listaCon.at(cual) == 0)
        return;
    if(listaCon.at(cual)->socket->isValid())
            listaCon.at(cual)->sendMessage(message);
}


void Server::sendMessage(int cual, QList<QString> lista){
    if(listaCon.at(cual) == 0)
        return;
    if(listaCon.at(cual)->socket->isValid()){
        listaCon.at(cual)->sendList(lista);
    }
}

void Server::sendMessage(int cual, QList<QString> lista, QImage img){
    if(listaCon.at(cual) == 0)
        return;
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
        while(!listaCon.isEmpty()){
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
        return;
    }
        ConnectionServer* con = new ConnectionServer(nextPendingConnection());
        listaCon.push_back(con);
        connect(con, SIGNAL(newMessage(ConnectionServer*,QString)), this, SLOT(procesarMensaje(ConnectionServer*,QString)));
        connect(con, SIGNAL(signalDisconnected(ConnectionServer*)), this, SLOT(disconnected(ConnectionServer*)));
        connect(con, SIGNAL(connectionError(ConnectionServer*,QString,QString)), this, SLOT(procesarError(ConnectionServer*,QString,QString)));
        connect(con, SIGNAL(autenticate(ConnectionServer*,QString)), this, SLOT(validate(ConnectionServer*,QString)));
        connect(con, SIGNAL(broadCastM(ConnectionServer*,QString)), this, SLOT(broadCastMessage(ConnectionServer*, QString)));
        connect(con, SIGNAL(sendMTo(ConnectionServer*,QString,QString)), this, SLOT(sendMTo(ConnectionServer*,QString,QString)));
        connect(con, SIGNAL(askList(ConnectionServer*)), this, SLOT(sendList(ConnectionServer*)));
        emit nuevaConexion(con->getIP());
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

void Server::validate(ConnectionServer* con, QString user){
    if(user.compare("")==0)
        return;
    for(int i = 0; i < listaCon.size(); i++){
        if(listaCon.at(i) != 0){
            if(listaCon.at(i)->getUser().compare(user, Qt::CaseInsensitive) == 0){
                con->sendMessage("Invalid Credentials. Maybe this user user is already logged in");
                return;
            }
        }
    }
    emit con->loggedIn(user);
    emit updateNames();
    con->sendMessage("Credentials Accepted. You are now Logged in.");
}

void Server::broadCastMessage(ConnectionServer *owner, QString msg){
    for(int i = 0; i < listaCon.count(); i++){
        if(listaCon.at(i) != 0){
            listaCon.at(i)->sendMessage(owner->getUser() + " says: " + msg);
        }
    }
}

void Server::sendMTo(ConnectionServer *from, QString dest, QString msg){
    for(int i = 0; i < listaCon.count(); i++){
        ConnectionServer* temp = listaCon.at(i);
        if( temp != 0 && temp->getUser().compare(dest,Qt::CaseInsensitive) == 0){
            temp->sendMessage(from->getUser() + " to you: " + msg);
            return;
        }
    }
}

void Server::sendList(ConnectionServer* sol){
    QString list = "Conectados: ";
    for(int i = 0; i < listaCon.count(); i++){
        ConnectionServer* temp = listaCon.at(i);
        if( temp != 0 && temp->isLoggedIn()){
            list.append(temp->getUser());
            list.append(",");
        }
    }
    sol->sendMessage(list);
}
