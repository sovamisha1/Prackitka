#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Initialize TCP server for port 12345
    if (!tcpServer.listen(QHostAddress::Any, 12345)) {
        qCritical() << "Failed to start TCP server on port 12345:" << tcpServer.errorString();
    } else {
        connect(&tcpServer, &QTcpServer::newConnection, this, &MainWindow::onTcpConnection);
        qDebug() << "Listening for TCP connections on port 12345.";
    }

    // Initialize TCP server for port 12346
    if (!tcpServer12346.listen(QHostAddress::Any, 12346)) {
        qCritical() << "Failed to start TCP server on port 12346:" << tcpServer12346.errorString();
    } else {
        connect(&tcpServer12346, &QTcpServer::newConnection, this, &MainWindow::onTcpConnection12346);
        qDebug() << "Listening for TCP connections on port 12346.";
    }

    vm1Result = "VM1: N/A";
    vm2Result = "VM2: N/A";

    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::sendDataToVMs);
}

MainWindow::~MainWindow() {
    for (QTcpSocket *socket : vmSockets) {
        if (socket) socket->close();
    }

    for (QTcpSocket *socket : vmSockets12346) {
        if (socket) socket->close();
    }
    delete ui;
}

void MainWindow::onTcpConnection() {
    QTcpSocket *socket = tcpServer.nextPendingConnection();
    vmSockets.append(socket);

    connect(socket, &QTcpSocket::readyRead, [this, socket]() {
        QByteArray receivedData = socket->readAll();
        QHostAddress senderIP = socket->peerAddress();

        qDebug() << "Received data from" << senderIP.toString() << ":" << receivedData;
        vm1Result = receivedData;

        updateResultLabel();
    });

    // Handle socket disconnection
    connect(socket, &QTcpSocket::disconnected, [this, socket]() {
        vmSockets.removeAll(socket);
        socket->deleteLater();
        qDebug() << "VM disconnected from TCP server on port 12345.";
    });

    qDebug() << "New VM connected to TCP server on port 12345.";
}

void MainWindow::onTcpConnection12346() {
    QTcpSocket *socket = tcpServer12346.nextPendingConnection();
    vmSockets12346.append(socket);

    connect(socket, &QTcpSocket::readyRead, [this, socket]() {
        QByteArray receivedData = socket->readAll();
        QHostAddress senderIP = socket->peerAddress();

        qDebug() << "Received data from" << senderIP.toString() << ":" << receivedData;
        vm2Result = receivedData;

        updateResultLabel();
    });

    // Handle socket disconnection
    connect(socket, &QTcpSocket::disconnected, [this, socket]() {
        vmSockets12346.removeAll(socket);
        socket->deleteLater();
        qDebug() << "VM disconnected from TCP server on port 12346.";
    });

    qDebug() << "New VM connected to TCP server on port 12346.";
}

void MainWindow::updateResultLabel() {
    ui->result1->setText(vm1Result);
    ui->result2->setText(vm2Result);
}

void MainWindow::sendDataToVMs() {
    QString input = "2";
    QByteArray data = input.toUtf8();
    // Send UDP packets to both VMs
    udpSocket.writeDatagram(data, QHostAddress("192.168.153.129"), 12346); // VM1
    udpSocket.writeDatagram(data, QHostAddress("192.168.153.130"), 12346); // VM2
    qDebug() << "Sent UDP data to VMs:" << data;
}
