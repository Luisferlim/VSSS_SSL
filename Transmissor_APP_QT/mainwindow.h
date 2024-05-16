#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonSend_clicked();

    void on_pushButtonActive_pressed();

    void on_pushButtonActive_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort* COMPORT;
};
#endif // MAINWINDOW_H
