#include "dlgvodactrl.h"
#include "ui_dlgvodactrl.h"

#include <QTimer>

#include "iodev.h"

dlgVodaCtrl::dlgVodaCtrl(IoDev &source,QWidget *parent) :
    QDialog(parent),
    src(source),
    ui(new Ui::dlgVodaCtrl)
{
    ui->setupUi(this);


    ui->sb_Lv_max->setValue(src.getValueScaled("Lv_max"));
    ui->sb_Lv_max->setValue(src.getValueScaled("Lv_min"));
    ui->sb_Lv_max->setValue(src.getValueScaled("Tv_max"));
    ui->sb_Lv_max->setValue(src.getValueScaled("Tv_min"));

    ui->sl_Lv_max->setValue(src.getValueFloat("Lv_max"));
    ui->sl_Lv_max->setValue(src.getValueFloat("Lv_min"));

    ui->cb_AM_Tv->setCurrentIndex(src.getValue16("AM_Tv")?1:0);
    ui->cb_Y_08->setChecked(src.getValue16("Y_08"));
    ui->cb_Y_09->setChecked(src.getValue16("Y_09"));

    connect(ui->sb_Lv_max,SIGNAL(valueChanged(double)),this,SLOT(slotSetData(double)));
    connect(ui->sb_Lv_min,SIGNAL(valueChanged(double)),this,SLOT(slotSetData(double)));
    connect(ui->sb_Tv_max,SIGNAL(valueChanged(double)),this,SLOT(slotSetData(double)));
    connect(ui->sb_Tv_min,SIGNAL(valueChanged(double)),this,SLOT(slotSetData(double)));

    connect(ui->sl_Lv_max,SIGNAL(valueChanged(int)),this,SLOT(slotSetData(int)));
    connect(ui->sl_Lv_min,SIGNAL(valueChanged(int)),this,SLOT(slotSetData(int)));
    connect(ui->cb_AM_Tv,SIGNAL(currentIndexChanged(int)),this,SLOT(slotSetData(int)));

    connect(ui->cb_Y_08,SIGNAL(clicked(bool)),this,SLOT(slotSetData(bool)));
    connect(ui->cb_Y_09,SIGNAL(clicked(bool)),this,SLOT(slotSetData(bool)));


    QTimer *t=new QTimer(this)    ;
    t->setInterval(1000);
    t->start();
        connect(t,SIGNAL(timeout()),this,SLOT(slotUpdateData()));

    // заблокувати в автомати сигнали від клапанів
    if(ui->cb_AM_Tv->currentIndex())
    {
        ui->cb_Y_08->blockSignals(true);
        ui->cb_Y_09->blockSignals(true);
    }

}

dlgVodaCtrl::~dlgVodaCtrl()
{
    delete ui;
}

void dlgVodaCtrl::changeEvent(QEvent *e)
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


void dlgVodaCtrl::slotSetData(bool v)
{
    src.sendValue(sender()->objectName().right(sender()->objectName().size()-3),qint16(v?-1:0));
}

void dlgVodaCtrl::slotSetData(int v )
{
    if(sender()->objectName()=="cb_AM_Tv") // відправити температуру води
    {
        src.sendValue("AM_Tv",qint16(-v));
        ui->cb_Y_08->blockSignals(v);
        ui->cb_Y_09->blockSignals(v);
    }
    else
    {
        src.sendValue(sender()->objectName().right(sender()->objectName().size()-3),(double)v);
        if(sender()->objectName().mid(3,1)=="L")
        {
            if(sender()->objectName().right(1)=="n")
            {
                ui->sb_Lv_min->blockSignals(true);
                ui->sb_Lv_min->setValue((double)v/4000.0*(src.scaleFull("Lv_min")-src.scaleZero("Lv_min"))+src.scaleZero("Lv_min"));
                ui->sb_Lv_min->blockSignals(false);
            }
            else
            {
                ui->sb_Lv_max->blockSignals(true);
                ui->sb_Lv_max->setValue((double)v/4000.0*(src.scaleFull("Lv_max")-src.scaleZero("Lv_max"))+src.scaleZero("Lv_max"));
                ui->sb_Lv_max->blockSignals(false);
            }
        }

    }
}

void dlgVodaCtrl::slotSetData(double v)
{
    src.sendValueScaled(sender()->objectName().right(sender()->objectName().size()-3),v);
    if(sender()->objectName().mid(3,1)=="L")
    {
        if(sender()->objectName().right(1)=="n")
        {
            ui->sl_Lv_min->blockSignals(true);
            ui->sl_Lv_min->setValue((v-src.scaleZero("Lv_min"))/(src.scaleFull("Lv_min")-src.scaleZero("Lv_min"))*4000.0);
            ui->sl_Lv_min->blockSignals(false);
        }
        else
        {
            ui->sl_Lv_max->blockSignals(true);
            ui->sl_Lv_max->setValue((v-src.scaleZero("Lv_max"))/(src.scaleFull("Lv_max")-src.scaleZero("Lv_max"))*4000.0);
            ui->sl_Lv_max->blockSignals(false);
        }
    }
}


void dlgVodaCtrl::slotUpdateData()
{
    ui->pb_Lvoda->setValue(src.getValueFloat("Lvoda"));
    ui->le_Lvoda->setText(QString("%1").arg(src.getValueScaled("Lvoda"),3,'f',0));
    ui->le_Tvoda->setText(QString("%1").arg(src.getValueScaled("Tvoda"),3,'f',0));
    ui->cb_X_07->setChecked(src.getValue16("X_07"));
    ui->cb_X_08->setChecked(src.getValue16("X_08"));

    if(ui->cb_AM_Tv->currentIndex())
    {
        ui->cb_Y_08->setChecked(src.getValue16("Y_08"));
        ui->cb_Y_09->setChecked(src.getValue16("Y_09"));
    }

}


