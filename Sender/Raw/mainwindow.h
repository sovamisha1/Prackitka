#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void sendDataToVMs();
    void onTcpConnection();           // Handle connections on port 12345
    void onTcpConnection12346();      // Handle connections on port 12346
    void updateResultLabel();

private:
    Ui::MainWindow *ui;

    QTcpServer tcpServer;             // TCP Server for port 12345
    QTcpServer tcpServer12346;        // TCP Server for port 12346
    QUdpSocket udpSocket;
    QList<QTcpSocket*> vmSockets;     // List of sockets for port 12345
    QList<QTcpSocket*> vmSockets12346;  // List of sockets for port 12346

    QString vm1Result;
    QString vm2Result;
};

#endif // MAINWINDOW_H
