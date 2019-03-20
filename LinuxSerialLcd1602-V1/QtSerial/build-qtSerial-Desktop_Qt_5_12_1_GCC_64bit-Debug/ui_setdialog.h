/********************************************************************************
** Form generated from reading UI file 'setdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETDIALOG_H
#define UI_SETDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_setDialog
{
public:
    QComboBox *comboBoxTty;
    QComboBox *comboBoxBaud;
    QComboBox *comboBoxBit;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *setDialog)
    {
        if (setDialog->objectName().isEmpty())
            setDialog->setObjectName(QString::fromUtf8("setDialog"));
        setDialog->resize(339, 197);
        comboBoxTty = new QComboBox(setDialog);
        comboBoxTty->addItem(QString());
        comboBoxTty->addItem(QString());
        comboBoxTty->addItem(QString());
        comboBoxTty->addItem(QString());
        comboBoxTty->addItem(QString());
        comboBoxTty->addItem(QString());
        comboBoxTty->addItem(QString());
        comboBoxTty->addItem(QString());
        comboBoxTty->setObjectName(QString::fromUtf8("comboBoxTty"));
        comboBoxTty->setGeometry(QRect(160, 20, 86, 25));
        comboBoxBaud = new QComboBox(setDialog);
        comboBoxBaud->addItem(QString());
        comboBoxBaud->addItem(QString());
        comboBoxBaud->addItem(QString());
        comboBoxBaud->addItem(QString());
        comboBoxBaud->addItem(QString());
        comboBoxBaud->setObjectName(QString::fromUtf8("comboBoxBaud"));
        comboBoxBaud->setGeometry(QRect(160, 50, 86, 25));
        comboBoxBit = new QComboBox(setDialog);
        comboBoxBit->addItem(QString());
        comboBoxBit->setObjectName(QString::fromUtf8("comboBoxBit"));
        comboBoxBit->setGeometry(QRect(160, 80, 86, 25));
        label = new QLabel(setDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(80, 50, 67, 17));
        label_2 = new QLabel(setDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(80, 80, 67, 17));
        label_3 = new QLabel(setDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(80, 20, 67, 17));
        okButton = new QPushButton(setDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setGeometry(QRect(120, 140, 89, 25));
        cancelButton = new QPushButton(setDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(230, 140, 89, 25));

        retranslateUi(setDialog);

        QMetaObject::connectSlotsByName(setDialog);
    } // setupUi

    void retranslateUi(QDialog *setDialog)
    {
        setDialog->setWindowTitle(QApplication::translate("setDialog", "Dialog", nullptr));
        comboBoxTty->setItemText(0, QApplication::translate("setDialog", "USBtty0", nullptr));
        comboBoxTty->setItemText(1, QApplication::translate("setDialog", "USBtty1", nullptr));
        comboBoxTty->setItemText(2, QApplication::translate("setDialog", "USBtty2", nullptr));
        comboBoxTty->setItemText(3, QApplication::translate("setDialog", "USBtty3", nullptr));
        comboBoxTty->setItemText(4, QApplication::translate("setDialog", "USBtty4", nullptr));
        comboBoxTty->setItemText(5, QApplication::translate("setDialog", "USBtty5", nullptr));
        comboBoxTty->setItemText(6, QApplication::translate("setDialog", "USBtty5", nullptr));
        comboBoxTty->setItemText(7, QApplication::translate("setDialog", "USBtty6", nullptr));

        comboBoxBaud->setItemText(0, QApplication::translate("setDialog", "1200", nullptr));
        comboBoxBaud->setItemText(1, QApplication::translate("setDialog", "2400", nullptr));
        comboBoxBaud->setItemText(2, QApplication::translate("setDialog", "4800", nullptr));
        comboBoxBaud->setItemText(3, QApplication::translate("setDialog", "9600", nullptr));
        comboBoxBaud->setItemText(4, QApplication::translate("setDialog", "115200", nullptr));

        comboBoxBit->setItemText(0, QApplication::translate("setDialog", "8", nullptr));

        label->setText(QApplication::translate("setDialog", "Buad:", nullptr));
        label_2->setText(QApplication::translate("setDialog", "DataBit:", nullptr));
        label_3->setText(QApplication::translate("setDialog", "USBtty:", nullptr));
        okButton->setText(QApplication::translate("setDialog", "OK", nullptr));
        cancelButton->setText(QApplication::translate("setDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class setDialog: public Ui_setDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETDIALOG_H
