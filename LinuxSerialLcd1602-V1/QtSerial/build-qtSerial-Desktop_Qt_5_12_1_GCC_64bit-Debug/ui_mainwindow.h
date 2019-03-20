/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *connectButton;
    QPushButton *disConnectButton;
    QPushButton *quitButton;
    QPushButton *settingButton;
    QComboBox *comboBox;
    QComboBox *comboBox_2;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEdit;
    QPushButton *sendButton;
    QLabel *label_4;
    QPushButton *showTimeButton;
    QPushButton *clearButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(387, 252);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        connectButton = new QPushButton(centralWidget);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));
        connectButton->setGeometry(QRect(20, 70, 89, 25));
        disConnectButton = new QPushButton(centralWidget);
        disConnectButton->setObjectName(QString::fromUtf8("disConnectButton"));
        disConnectButton->setGeometry(QRect(20, 100, 89, 25));
        quitButton = new QPushButton(centralWidget);
        quitButton->setObjectName(QString::fromUtf8("quitButton"));
        quitButton->setGeometry(QRect(290, 160, 89, 25));
        settingButton = new QPushButton(centralWidget);
        settingButton->setObjectName(QString::fromUtf8("settingButton"));
        settingButton->setGeometry(QRect(20, 40, 89, 25));
        comboBox = new QComboBox(centralWidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(320, 40, 51, 25));
        comboBox_2 = new QComboBox(centralWidget);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));
        comboBox_2->setGeometry(QRect(210, 40, 51, 25));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(160, 40, 41, 17));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(270, 40, 41, 17));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(160, 60, 67, 17));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(160, 80, 211, 25));
        sendButton = new QPushButton(centralWidget);
        sendButton->setObjectName(QString::fromUtf8("sendButton"));
        sendButton->setGeometry(QRect(160, 110, 89, 25));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(90, 10, 281, 21));
        showTimeButton = new QPushButton(centralWidget);
        showTimeButton->setObjectName(QString::fromUtf8("showTimeButton"));
        showTimeButton->setGeometry(QRect(20, 130, 89, 25));
        clearButton = new QPushButton(centralWidget);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));
        clearButton->setGeometry(QRect(20, 160, 89, 25));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 387, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        connectButton->setText(QApplication::translate("MainWindow", "Connect", nullptr));
        disConnectButton->setText(QApplication::translate("MainWindow", "Disconnect", nullptr));
        quitButton->setText(QApplication::translate("MainWindow", "Quit", nullptr));
        settingButton->setText(QApplication::translate("MainWindow", "Setting", nullptr));
        comboBox->setItemText(0, QApplication::translate("MainWindow", "1", nullptr));
        comboBox->setItemText(1, QApplication::translate("MainWindow", "2", nullptr));
        comboBox->setItemText(2, QApplication::translate("MainWindow", "3", nullptr));
        comboBox->setItemText(3, QApplication::translate("MainWindow", "4", nullptr));
        comboBox->setItemText(4, QApplication::translate("MainWindow", "5", nullptr));
        comboBox->setItemText(5, QApplication::translate("MainWindow", "6", nullptr));
        comboBox->setItemText(6, QApplication::translate("MainWindow", "7", nullptr));
        comboBox->setItemText(7, QApplication::translate("MainWindow", "8", nullptr));
        comboBox->setItemText(8, QApplication::translate("MainWindow", "9", nullptr));
        comboBox->setItemText(9, QApplication::translate("MainWindow", "10", nullptr));
        comboBox->setItemText(10, QApplication::translate("MainWindow", "11", nullptr));
        comboBox->setItemText(11, QApplication::translate("MainWindow", "12", nullptr));
        comboBox->setItemText(12, QApplication::translate("MainWindow", "13", nullptr));
        comboBox->setItemText(13, QApplication::translate("MainWindow", "14", nullptr));
        comboBox->setItemText(14, QApplication::translate("MainWindow", "15", nullptr));
        comboBox->setItemText(15, QApplication::translate("MainWindow", "16", nullptr));

        comboBox_2->setItemText(0, QApplication::translate("MainWindow", "1", nullptr));
        comboBox_2->setItemText(1, QApplication::translate("MainWindow", "2", nullptr));

        label->setText(QApplication::translate("MainWindow", "ROW:", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "COL:", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "data:", nullptr));
        sendButton->setText(QApplication::translate("MainWindow", "Send", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:14pt; font-weight:600; color:#cc0000;\">USB-Serial  LCD1602 </span></p></body></html>", nullptr));
        showTimeButton->setText(QApplication::translate("MainWindow", "TimeDisplay", nullptr));
        clearButton->setText(QApplication::translate("MainWindow", "Clear", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
