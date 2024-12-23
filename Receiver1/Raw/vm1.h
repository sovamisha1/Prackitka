#ifndef VM1_H
#define VM1_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QDebug>

class VM1 : public QObject {
    Q_OBJECT
public:
    explicit VM1(QObject *parent = nullptr);
    void start();

private slots:
    void onUdpDataReceived();
    void onTcpClientConnected();

private:
    QUdpSocket *udpSocket;
    QTcpSocket *tcpClient;
};

#endif // VM1_H
