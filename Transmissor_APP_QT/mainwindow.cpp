#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    COMPORT =  new QSerialPort();
    COMPORT->setPortName("/dev/ttyUSB0");
    COMPORT->setBaudRate(QSerialPort::BaudRate::Baud115200);
    COMPORT->setParity(QSerialPort::Parity::NoParity);
    COMPORT->setDataBits(QSerialPort::DataBits::Data8);
    COMPORT->setStopBits(QSerialPort::StopBits::OneStop);
    COMPORT->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    COMPORT->open(QIODevice::ReadWrite);

    if (COMPORT->isOpen())
    {
        qDebug() << "Serial Port is Connected";
    }
    else
    {
        qDebug() << "Serial Por is not Connected";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool isOpen = false;

//button to send data
void MainWindow::on_pushButtonSend_clicked()
{
    if (COMPORT->isOpen()){
        //char(10) = \n
        //char(13) = \r
        COMPORT->write(ui->lineEditSerialData->text().toLatin1());
        /*
            if (isOpen == false)
        {
            COMPORT->write("");
            isOpen = true;
        }
        else
        {
            isOpen = false;
        }
    */
    }
}


void MainWindow::on_pushButtonActive_clicked()
{
    if (COMPORT->isOpen()){
        //char(10) = \n
        //char(13) = \r
        for (int i =1;i< 20; i++)
        {
            QThread::sleep(10);
            COMPORT->write("1");
        }
        /*
            if (isOpen == false)
        {
            COMPORT->write("");
            isOpen = true;
        }
        else
        {
            isOpen = false;
        }
    */
    }
}

