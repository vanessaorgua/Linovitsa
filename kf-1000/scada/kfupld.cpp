#include "kfupld.h"
#include "ui_kfupld.h"
#include <QWidget>
#include "iodev.h"
#include <QCheckBox>


KfUpld::KfUpld(QWidget *parent) :
    QLabel(parent),
    ui(new Ui::KfUpld)
{
    ui->setupUi(this);

    cb
            << ui->c1_X_I
            << ui->c1_X_J
            << ui->c1_Y_IJ
            << ui->c2_X_I
            << ui->c2_X_J
            << ui->c2_Y_IJ
            << ui->cb_X_dl
            << ui->cb_X_kh
            << ui->cb_X_kt
            << ui->cb_X_pa
            << ui->cb_X_pause
            << ui->cb_X_presh
            << ui->cb_X_ps
            << ui->cb_X_ton
            << ui->cb_X_tauto
            << ui->cb_Y_bw
            << ui->cb_Y_fw;

}

KfUpld::~KfUpld()
{
    delete ui;
}

void KfUpld::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void KfUpld::updateData(IoDev &src)
{
    foreach(QCheckBox *cbx,cb)
    {
        cbx->setChecked(src.getValue16(cbx->objectName().right(cbx->objectName().size()-3)));
    }
    ui->cb_Y_1->setChecked(src.getValue16("Y_pump")!=0 && src.getValue16("Y_zol")==0);
    ui->cb_Y_2->setChecked(src.getValue16("Y_pump")!=0 && src.getValue16("Y_zo;")!=0);

}
