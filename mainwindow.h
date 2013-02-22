#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "server.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pbStart_clicked();
    void on_pbStop_clicked();
    void on_pbSendMT_clicked();
    void showNewLogMessage(QString);
    void showNewConnection(QString);
    void removerConexion(int);

private:
    Ui::MainWindow *ui;
    Server* server;
};

#endif // MAINWINDOW_H
