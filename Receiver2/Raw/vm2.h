#ifndef VM2_H
#define VM2_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QDebug>

class VM2 : public QObject {
    Q_OBJECT
public:
    explicit VM2(QObject *parent = nullptr);
    void start();
private slots:
    void onUdpDataReceived();
    void onTcpClientConnected();

private:
    QUdpSocket *udpSocket;
    QTcpSocket *tcpClient;
};

#endif
