#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    
private slots:
    void on_connectbt_clicked();
    void clientConnected();
    void clientRecvData();
    void clientError(QAbstractSocket::SocketError socketError);

    void on_openSerialButton_clicked();
    void on_closeSerialButton_clicked();
    void on_openTelnetButton_clicked();
    void on_closeTelnetButton_clicked();

private:
    void op_button_enable(bool state);
    Ui::Dialog *ui;
    QTcpSocket *tcpSocket;
};

#endif // DIALOG_H
