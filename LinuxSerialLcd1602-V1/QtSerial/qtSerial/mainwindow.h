#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<pthread.h>

#include"setdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    friend void* timeThread(void *arg);
private:
    Ui::MainWindow *ui;
    int serial_fd;
    int connectStatus;
    int modeStatus;
    pthread_t userThread;
    int threadStatus;
private slots:
    void connectButtonSlot();
    void quitButtonSlot();
    void disConnectButtonSlot();
    void sendButtonSlot();
    void showTimeButtonSlot();
    void clearButtonSlot();
    void setButtonSlot();
};

#endif // MAINWINDOW_H
