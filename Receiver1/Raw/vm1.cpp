#include "vm1.h"

// Constructor: Initializes sockets
VM1::VM1(QObject *parent) : QObject(parent), udpSocket(new QUdpSocket(this)), tcpClient(new QTcpSocket(this)) {
    // Bind UDP socket to a listening port (12346)
    if (!udpSocket->bind(12346)) {
        qCritical() << "VM1 failed to bind UDP socket on port 12346.";
    } else {
        connect(udpSocket, &QUdpSocket::readyRead, this, &VM1::onUdpDataReceived);
        qDebug() << "VM1 UDP socket is ready and listening on port 12346.";
    }

    // Connect TCP client signals
    connect(tcpClient, &QTcpSocket::connected, this, &VM1::onTcpClientConnected);
    connect(tcpClient, &QTcpSocket::disconnected, this, []() {
        qDebug() << "VM1 TCP client disconnected.";
    });
}

// Starts the VM logic
void VM1::start() {
    qDebug() << "VM1 is running and waiting for UDP data...";
}

// Handles incoming UDP data
void VM1::onUdpDataReceived() {
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray buffer;
        buffer.resize(udpSocket->pendingDatagramSize());

        QHostAddress senderAddress;
        quint16 senderPort;

        udpSocket->readDatagram(buffer.data(), buffer.size(), &senderAddress, &senderPort);
        int initialNum = buffer.toInt();

        qDebug() << "VM1 received UDP packet from:" << senderAddress.toString() << " " << initialNum;
        int processedNumber = buffer.toInt() + 2;
        QString result = QString::number(processedNumber);

        tcpClient->connectToHost(senderAddress, 12345);

        if (tcpClient->waitForConnected(2000)) {
            qDebug() << "VM1 connected to GUI.";
            tcpClient->write(result.toUtf8());
            tcpClient->flush();
            tcpClient->disconnectFromHost();
        } else {
            qCritical() << "VM1 failed to connect to GUI.";
        }
    }
}

void VM1::onTcpClientConnected() {
    qDebug() << "VM1 TCP client connected to GUI.";
}
