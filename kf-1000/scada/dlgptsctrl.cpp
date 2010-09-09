#include "dlgptsctrl.h"
#include "ui_dlgptsctrl.h"

#include "iodev.h"
#include "QTimer"

DlgPtsCtrl::DlgPtsCtrl(IoDev &source, QWidget *parent) :
    QDialog(parent),
    src(source),
    ui(new Ui::DlgPtsCtrl)
{
    ui->setupUi(this);
    ui->man->setCurrentIndex(src.getValue16("man")?1:0);
    ui->trm->setCurrentIndex(src.getValue16("trm")?1:0);
    ui->Y_run_1->setCurrentIndex(src.getValue16("Y_run_1")?1:0);
    ui->Y_run_2->setCurrentIndex(src.getValue16("Y_run_2")?1:0);
    ui->Y_run_3->setCurrentIndex(src.getValue16("Y_run_3")?1:0);

    if(src.getValue16("man"))
    {
        ui->trm->blockSignals(true);
    }
    else
    {
        ui->Y_run_1->blockSignals(true);
        ui->Y_run_2->blockSignals(true);
        ui->Y_run_3->blockSignals(true);
    }
    connect(ui->man,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetBool(int)));
    connect(ui->trm,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetBool(int)));
    connect(ui->Y_run_1,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetBool(int)));
    connect(ui->Y_run_2,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetBool(int)));
    connect(ui->Y_run_3,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetBool(int)));

    ui->Tw_1->setValue(src.getValue32("Tw_1")/1000);
    ui->Tw_2->setValue(src.getValue32("Tw_2")/1000);
    ui->Tw_3->setValue(src.getValue32("Tw_3")/1000);

    connect(ui->Tw_1,SIGNAL(valueChanged(int)),this,SLOT(slotSetInt(int)));
    connect(ui->Tw_2,SIGNAL(valueChanged(int)),this,SLOT(slotSetInt(int)));
    connect(ui->Tw_3,SIGNAL(valueChanged(int)),this,SLOT(slotSetInt(int)));

    QTimer *t = new QTimer(this);
    t->setInterval(1000);
    connect(t,SIGNAL(timeout()),this,SLOT(updateData()));
    t->start();
}

DlgPtsCtrl::~DlgPtsCtrl()
{
    delete ui;
}

void DlgPtsCtrl::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DlgPtsCtrl::slotSetBool(int v) // приймає сигнали від  комбобоксів
{
    src.sendValue(sender()->objectName(),qint16(-v));
    if(sender()->objectName()=="man")
    {
        if(v)
        {
            ui->trm->blockSignals(true);
            ui->Y_run_1->blockSignals(false);
            ui->Y_run_2->blockSignals(false);
            ui->Y_run_3->blockSignals(false);
        }
        else
        {
            ui->trm->blockSignals(false);
            ui->Y_run_1->blockSignals(true);
            ui->Y_run_2->blockSignals(true);
            ui->Y_run_3->blockSignals(true);
        }
    }
}


void DlgPtsCtrl::slotSetInt(int v)
{
    src.sendValue(sender()->objectName(),v*1000);
}
void DlgPtsCtrl::updateData()
{
    if(ui->man->currentIndex()==0)
    {
        ui->Y_run_1->setCurrentIndex(src.getValue16("Y_run_1")?1:0);
        ui->Y_run_2->setCurrentIndex(src.getValue16("Y_run_2")?1:0);
        ui->Y_run_3->setCurrentIndex(src.getValue16("Y_run_3")?1:0);
    }
}
