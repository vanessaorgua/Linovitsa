#include "kfone.h"
#include "ui_kfone.h"

#include <QWidget>
#include "iodev.h"
#include <QCheckBox>

KfOne::KfOne(QWidget *parent) :
    QLabel(parent),
    ui(new Ui::KfOne)
{
    ui->setupUi(this);

    cb
            << ui->cb_X_A
            << ui->cb_X_B
            << ui->cb_X_C
            << ui->cb_X_D
            << ui->cb_X_E
            << ui->cb_X_F
            << ui->cb_X_G
            << ui->cb_X_H
            << ui->cb_X_presh
            << ui->c2_Y_F
            << ui->cb_Y_B
            << ui->cb_Y_C
            << ui->cb_Y_D
            << ui->cb_Y_E
            << ui->cb_Y_F
            << ui->cb_Y_G
            << ui->cb_Y_H ;

    connect(ui->bnCallOne,SIGNAL(clicked()),this,SLOT(slotCallOne()));


}

KfOne::~KfOne()
{
    delete ui;
}

void KfOne::changeEvent(QEvent *e)
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

void KfOne::updateData(IoDev &src)
{
    foreach(QCheckBox *cbx,cb)
    {
        cbx->setChecked(src.getValue16(cbx->objectName().right(cbx->objectName().size()-3)));
    }
    ui->le_Psusp->setText(QString("%1").arg(src.getValueScaled("Psusp"),3,'f',0));
    ui->le_Poil->setText(QString("%1").arg(src.getValueScaled("Poil"),3,'f',0));
}


void KfOne::slotCallOne()
{
    emit signalCallOne();
}
