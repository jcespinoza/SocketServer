#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server = new Server();
    connect(server, SIGNAL(nuevaConexion(QString)), this, SLOT(showNewConnection(QString)));
    connect(server, SIGNAL(newLogMessage(QString)), this, SLOT(showNewLogMessage(QString)));
    connect(server, SIGNAL(removerConexion(int)), this, SLOT(removerConexion(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete server;
}

void MainWindow::on_pbStart_clicked()
{
    server->start(7777);
    ui->lwConexions->clear();
}

void MainWindow::on_pbStop_clicked()
{
    server->stop();
    ui->lwConexions->clear();
}

void MainWindow::on_pbSendMT_clicked()
{
    if(ui->lwConexions->selectedItems().count() == 1)
        server->sendMessage(ui->lwConexions->currentRow(), ui->teMsgT->toPlainText());
}

void MainWindow::showNewConnection(QString ip){
    ui->lwConexions->addItem(ip);
}

void MainWindow::showNewLogMessage(QString message){
    ui->teLog->append(message);
}

void MainWindow::removerConexion(int c){
    QString temp;
    temp.setNum(c);
    ui->teLog->append("Removiendo conexion " + temp);
    ui->lwConexions->item(c)->setText("Disconnected");
    //delete ui->lwConexions->takeItem(c);
}
