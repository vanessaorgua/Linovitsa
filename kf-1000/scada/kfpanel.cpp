#include <QtGui>

#include "kfpanel.h"
#include "ui_kfpanel.h"

#include "IoNetClient.h"

#include "kfone.h"
#include "kfupld.h"
#include "dlgkfmenu.h"
#include "dlgkfparam.h"

KfPanel::KfPanel(IoNetClient &source, int KfNum, QWidget *parent) :
    QDialog(parent),
    src(source),
    Nf(KfNum),
    ui(new Ui::KfPanel)
{
    ui->setupUi(this);
    kfo = new KfOne(this);
    ui->sw->addWidget(kfo);

    kfu= new KfUpld(this);
    ui->sw->addWidget(kfu);

    ui->sw->setCurrentIndex(0);

    tState
            << tr("Тестування") // -1
            << tr("Зупинено")   // 0
            << tr("Старт")      // 1
            << tr("Зтиск") //2
            << tr("")   //3
            << tr("Фільтрування")   //  4
            << tr("Висолодження") //5
            << tr("Промивка") // 6
            << tr("Продувка") // 7
            << tr("Сушіння") // 8
            << tr("Скид тиску") // 9
            << tr("Запуск ПТС") // 10
            << tr("Вивантаження") ;// 11


    connect(&src,SIGNAL(updateDataRaw()),this,SLOT(updateDataRaw())); // при отриманні нових даних, засвітити їх на картинці
    connect(&src,SIGNAL(updateDataScaled()),this,SLOT(updateDataScaled())); // при отриманні нових даних, засвітити їх на картинці

    connect(ui->bnAlarm,SIGNAL(clicked()),this,SLOT(slotAlarm()));
    connect(ui->bnStart,SIGNAL(clicked()),this,SLOT(slotStart()));
    connect(ui->bnMenu,SIGNAL(clicked()),this,SLOT(slotMenu()));
    connect(ui->bnParm,SIGNAL(clicked()),this,SLOT(slotParam()));
    connect(ui->bnTrend,SIGNAL(clicked()),this,SLOT(slotTrend()));
    connect(ui->bnUpload,SIGNAL(clicked()),this,SLOT(slotUpload()));



}

KfPanel::~KfPanel()
{
    delete ui;
}

void KfPanel::changeEvent(QEvent *e)
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

void KfPanel::updateDataRaw()
{
    kfo->updateData(*src[Nf]);

    ui->le_Nc->setText(QString("%1").arg(src[Nf]->getValue32("Nc")));
    ui->le_Nf->setText(QString("%1").arg(src[Nf]->getValue32("Nf")));

    {
        int i =src[Nf]->getValue16("State")+1;
        if(i>-1 && i<tState.size())
            ui->le_State->setText(tState[i]);
    }

    ui->le_Tall->setText(QString("%1").arg(src[Nf]->getValue32("Tall")/1000));
    ui->le_Tf->setText(QString("%1").arg(src[Nf]->getValue32("Tf")/1000));
    ui->le_Tfilt->setText(QString("%1").arg(src[Nf]->getValue32("Tfilt")/1000));
    ui->le_Tvs->setText(QString("%1").arg(src[Nf]->getValue32("Tvs")/1000));
    ui->le_Tvw->setText(QString("%1").arg(src[Nf]->getValue32("Tvw")/1000));
    ui->le_t_Tp->setText(QString("%1").arg(src[Nf]->getValue32("t_Tp")/1000));

    if(src[Nf]->getValue16("State")==11)
        ui->sw->setCurrentIndex(1);
    else
        ui->sw->setCurrentIndex(0);

}


void KfPanel::updateDataScaled()
{
    //qDebug() << "Nf=" << Nf;
    ui->le_Qsusp->setText(QString("%1").arg(src[Nf]->getValueScaled("Qsusp")));
    ui->le_Qvs->setText(QString("%1").arg(src[Nf]->getValueScaled("Qvs")));
    ui->le_Qvw->setText(QString("%1").arg(src[Nf]->getValueScaled("Qvw")));

}


void KfPanel::slotAlarm()
{

}

void KfPanel::slotStart()
{
    src[Nf]->sendValue("State", qint16(1));
}

void KfPanel::slotMenu()
{
    dlgKfMenu p(ui->le_State->text(),this);
    if(p.exec()==QDialog::Accepted)
    {
        qDebug() << "Result" << p.getRes();
        src[Nf]->sendValue("State", p.getRes());
    }
}

void KfPanel::slotParam()
{
    dlgKfParam p(*src[Nf],this);
    p.exec();

}


void KfPanel::slotTrend()
{

}

void KfPanel::slotUpload()
{
    if(src[Nf]->getValue16("State")==0)
        src[Nf]->sendValue("State", qint16(11));
}



