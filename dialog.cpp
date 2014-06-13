#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>

static const char PKT_ADD_TELNETT[] 	= 	{0x7e, 0x95, 0xeb, 0x0d, 0xad, 0x1a, 0x84, 0x9d, 0xa9, 0x79, 0xce, 0x97, 0xe6, 0x12, 0x59, 0xbb, 0x7b, 0x7e};
static const char PKT_REMOVE_TELNET[] 	= 	{0x7e, 0xbd, 0x47, 0x6d, 0xd5, 0x39, 0x69, 0x28, 0x55, 0x9c, 0x0c, 0x1d, 0x6c, 0x5c, 0xb0, 0xf3, 0x28, 0x7e};
static const char PKT_ADD_SERIAL[] 		= 	{0x7e, 0xdf, 0x40, 0x6a, 0xa9, 0xf8, 0x14, 0xf2, 0xa0, 0x3e, 0x1f, 0x2f, 0x12, 0xed, 0xdc, 0x2c, 0xb9, 0x7e};
static const char PKT_REMOVE_SERIAL[] 	= 	{0x7e, 0x02, 0xd7, 0x25, 0xa1, 0x7a, 0x92, 0xd6, 0xc8, 0xef, 0xe9, 0xeb, 0x28, 0x58, 0x2e, 0xd2, 0x39, 0x7e};
static const char PKT_SUCCESS[] = {0x7e, 0x36, 0x37, 0x88, 0x6f, 0xb3, 0xd9, 0x8a, 0xd1, 0x47, 0x23, 0xf8, 0x0b, 0x5f, 0x0f, 0x40, 0x83, 0x7e};

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    op_button_enable(false);
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket,SIGNAL(connected()),this,SLOT(clientConnected()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(clientRecvData()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(clientError(QAbstractSocket::SocketError)));
}

Dialog::~Dialog()
{
    tcpSocket->abort();
    delete ui;
}

void Dialog::op_button_enable(bool state)
{
    ui->openSerialButton->setEnabled(state);
    ui->openTelnetButton->setEnabled(state);
    ui->closeSerialButton->setEnabled(state);
    ui->closeTelnetButton->setEnabled(state);
}

void Dialog::on_connectbt_clicked()
{
    op_button_enable(false);
    tcpSocket->abort();
    tcpSocket->connectToHost(ui->serverIP->text(), 5005);
    ui->statusLabel->setText(QString("connect to snapgate..."));
}

void Dialog::clientConnected()
{
    ui->statusLabel->setText(QString("connected"));
    op_button_enable(true);
}

void Dialog::clientRecvData()
{
    while(tcpSocket->bytesAvailable()>0)
   {
       QByteArray datagram;
       int len, i;
       const char *p = NULL;
       QString msg = "recv#";
       QString str;
       datagram.resize(tcpSocket->bytesAvailable());
       tcpSocket->read(datagram.data(),datagram.size());
       p = datagram.data();
       len = strlen(p);
       for (i = 0; i < len; i++) {
           msg.append(str.sprintf(" %2x", (unsigned char)p[i]));
       }

       ui->textOutput->append(msg);
   }
}

void Dialog::clientError(QAbstractSocket::SocketError socketError)
{
    QString name = tr("snapgate");
    QString msg = tr("nothing");

    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        msg = tr("Romote server closed illegally.");
        break;
    case QAbstractSocket::HostNotFoundError:
        msg = tr("The host was not found.");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        msg = tr("The connection was refused by the peer.");
        break;
    default:
        msg = tr("Error %1.").arg(tcpSocket->errorString());
    }

    op_button_enable(false);
    ui->statusLabel->setText(msg);
    QMessageBox::information(this, name, msg);
}

void Dialog::on_openSerialButton_clicked()
{
    ui->textOutput->append(tr("add serial"));
    tcpSocket->write(PKT_ADD_SERIAL, sizeof(PKT_ADD_SERIAL));
}

void Dialog::on_closeSerialButton_clicked()
{
    ui->textOutput->append(tr("remove serial"));
    tcpSocket->write(PKT_REMOVE_SERIAL, sizeof(PKT_REMOVE_SERIAL));
}

void Dialog::on_openTelnetButton_clicked()
{
    ui->textOutput->append(tr("open telnet"));
    tcpSocket->write(PKT_ADD_TELNETT, sizeof(PKT_ADD_TELNETT));
}

void Dialog::on_closeTelnetButton_clicked()
{
    ui->textOutput->append(tr("remove telnet"));
    tcpSocket->write(PKT_REMOVE_TELNET, sizeof(PKT_REMOVE_TELNET));
}
