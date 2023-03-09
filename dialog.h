#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QHostAddress>
#include <QStringListModel>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_btnConnect_clicked();

    void on_btnDisconnect_clicked();

    void on_btnSend_clicked();

    void connected();
    void readyRead();
    void disconnected();
    void error(QAbstractSocket::SocketError socketError);

private:
    Ui::Dialog *ui;
    QTcpSocket clientSocket;
    QStringListModel listModel;
    QStringList list;
    QString m_name;
};
#endif // DIALOG_H
