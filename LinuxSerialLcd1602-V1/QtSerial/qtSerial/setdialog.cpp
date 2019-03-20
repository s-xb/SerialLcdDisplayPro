#include "setdialog.h"
#include "ui_setdialog.h"


int usbTty=0;
int baud=0;
int dataBit=0;

setDialog::setDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setDialog)
{
    this->setMaximumSize(350,200);
    this->setMinimumSize(350,200);
    ui->setupUi(this);
    ui->comboBoxBit->setCurrentIndex(dataBit);
    ui->comboBoxTty->setCurrentIndex(usbTty);
    ui->comboBoxBaud->setCurrentIndex(baud);
    QObject::connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(cancelButtonSlot()));
    QObject::connect(ui->okButton,SIGNAL(clicked()),this,SLOT(okButtonSlot()));
}

setDialog::~setDialog()
{
    delete ui;
}


void setDialog::cancelButtonSlot()
{
    close();
}

void setDialog::okButtonSlot()
{
    usbTty=ui->comboBoxTty->currentIndex();
    baud=ui->comboBoxBaud->currentIndex();
    dataBit=ui->comboBoxBit->currentIndex();

    close();
}
