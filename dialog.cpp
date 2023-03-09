#include "dialog.h"
#include "./ui_dialog.h"
#include <QInputDialog>
#include <QMessageBox>
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    list.clear();

    connect(&clientSocket,&QTcpSocket::connected,this,&Dialog::connected);
    connect(&clientSocket,&QTcpSocket::readyRead,this,&Dialog::readyRead);
    connect(&clientSocket,&QTcpSocket::disconnected,this,&Dialog::disconnected);
    connect(&clientSocket,&QTcpSocket::errorOccurred,this,&Dialog::error);
    ui->btnConnect->setEnabled(true);
    ui->btnDisconnect->setEnabled(false);
    ui->btnSend->setEnabled(false);
    listModel.setStringList(list);
    ui->listView->setModel(&listModel);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_btnConnect_clicked()
{
    if(clientSocket.isOpen()) clientSocket.close();

    m_name=QInputDialog::getText(this,"enter name","Name :",QLineEdit::EchoMode::Normal,m_name);

    bool ok;
    QHostAddress address(ui->txtServer->text()); // or whatever IP the server is listening on
    quint16 port = static_cast<quint16> (ui->txtPort->text().toUInt(&ok)); // or whatever port the server is listening on

    if(!ok)
    {
        QMessageBox::critical(this,"Error","Please enter a valid port number!");
        return;
    }
    qDebug()<<address;
    clientSocket.connectToHost(address,port);

    ui->btnConnect->setEnabled(false);
    ui->btnDisconnect->setEnabled(true);
    ui->btnSend->setEnabled(true);

    if(clientSocket.waitForConnected(3000)){
        QMessageBox::critical(this,"Error","Could not connect to server");
        on_btnSend_clicked();
        return;
    }

}
void Dialog::on_btnDisconnect_clicked()
{
    clientSocket.close();
    ui->btnConnect->setEnabled(true);
    ui->btnDisconnect->setEnabled(false);
    ui->btnSend->setEnabled(false);

}

void Dialog::on_btnSend_clicked()
{
     if(!clientSocket.isOpen()) return;
    QByteArray data;
    data.append(m_name.toLatin1());
    data.append(QString(" ").toLatin1());
    data.append(ui->txtSend->text().toLatin1());

     clientSocket.write(data);
     ui->txtSend->setText(QString());
}

void Dialog::connected()
{
    qDebug() << "Connected!";
}

void Dialog::readyRead()
{
    QByteArray data = clientSocket.readAll();
    QString message(data);
    list.append(message);
    listModel.setStringList(list);

    ui->listView->scrollToBottom();

}

void Dialog::disconnected()
{
    qDebug() << "Disconnected!";
}
void Dialog::error(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError)
    QMessageBox::critical(this,"Error", clientSocket.errorString());
}

