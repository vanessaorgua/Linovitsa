
#include "mnemo.h"
#include "ui_mnemo.h"

#include <QtGui>
#include <IoNetClient.h>
#include "trendchart.h"
#include "panelReg.h"


#include <QVBoxLayout>
#include "kfone.h"
#include "kfupld.h"
#include "kfpanel.h"
#include "dlgvodactrl.h"

Mnemo::Mnemo(IoNetClient &src, QWidget *p) : QLabel(p), m_ui(new Ui::mnemo),s(src)
{
    m_ui->setupUi(this);

    connect(m_ui->bnY_08,SIGNAL(clicked()),this,SLOT(slotCallVodaCtrl()));
    connect(m_ui->bnY_09,SIGNAL(clicked()),this,SLOT(slotCallVodaCtrl()));

    connect(&s,SIGNAL(updateDataRaw()),this,SLOT(updateDataRaw())); // при отриманні нових даних, засвітити їх на картинці
    connect(&s,SIGNAL(updateDataScaled()),this,SLOT(updateDataScaled())); // при отриманні нових даних, засвітити їх на картинці

    connect(m_ui->cX_01,SIGNAL(clicked()),this,SLOT(slotCallReg()));
    connect(m_ui->cX_02,SIGNAL(clicked()),this,SLOT(slotCallReg()));

    sw << m_ui->kf_1 << m_ui->kf_2 << m_ui->kf_3;

    trC=new TrendChart(this);
    QVBoxLayout *vbl=new QVBoxLayout(m_ui->frameTr);
    vbl->addWidget(trC);
    m_ui->frameTr->setLayout(vbl);

    for(int i=0;i<3;++i)
    {
            KfOne *o=new KfOne(this);
            o->setObjectName(QString("kf_%1").arg(i));

            KfUpld *u=new KfUpld(this);
            u->setObjectName(QString("kf_%1").arg(i));

            sw[i]->addWidget(o);
            sw[i]->addWidget(u);
            sw[i]->setCurrentIndex(0);
            ko << o;
            ku << u;
            connect(o,SIGNAL(signalCallOne()),this,SLOT(slotCallKfOnePanel()));
    }

    le
            << m_ui->le_Gsusp
            << m_ui->le_Gvoda
            << m_ui->le_Lcs
            << m_ui->le_Lp
            << m_ui->le_Lsusp
            << m_ui->le_Lvoda
            << m_ui->le_Nps
            << m_ui->le_Npv
            << m_ui->le_Xv_01
            << m_ui->le_Xv_02
            << m_ui->le_Pair
            << m_ui->le_Pvoda
            << m_ui->le_Tvoda;
    pb
            << m_ui->pb_Lcs
            << m_ui->pb_Lp
            << m_ui->pb_Lsusp
            << m_ui->pb_Lvoda;
    cb
            //<< m_ui->cb_X_07
            //<< m_ui->cb_X_08
            << m_ui->cb_Y_08
            << m_ui->cb_Y_09
            << m_ui->cb_AM_01
            << m_ui->cb_AM_02
            << m_ui->cb_AM_Tv;

    State
            << m_ui->le_State_1
            << m_ui->le_State_2
            << m_ui->le_State_3 ;
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
}

Mnemo::~Mnemo()
{
    delete m_ui;
}


void Mnemo::updateDataRaw()
{
    //qDebug() << s[0]->getTags();

    for(int i=0;i<3;++i)
    {
        ko[i]->updateData(*s[i]);
        ku[i]->updateData(*s[i]);

        if(s[i]->getValue16("State")==11)
            sw[i]->setCurrentIndex(1);
        else
            sw[i]->setCurrentIndex(0);

    }
    foreach(QCheckBox *cbx,cb)
    {
        cbx->setChecked(s[3]->getValue16(cbx->objectName().right(cbx->objectName().size()-3)));
    }

    foreach(QProgressBar *pbx,pb)
    {
        pbx->setValue(s[3]->getValueFloat(pbx->objectName().right(pbx->objectName().size()-3)));
    }

    foreach(QLineEdit *l,State)
    {

        int i =s[l->objectName().right(1).toInt()-1]->getValue16("State")+1;
        if(i>-1 && i<tState.size())
            l->setText(tState[i]);
    }
    m_ui->bnY_08->setIcon(QIcon(s[3]->getValue16("X_07")?QString(":/valves/valve_green_l_20x32.png"):QString(":/valves/valve_off_l_20x32.png")));
    m_ui->bnY_09->setIcon(QIcon(s[3]->getValue16("X_08")?QString(":/valves/valve_green_20x32.png"):QString(":/valves/valve_off_20x32.png")));


}

void Mnemo::updateDataScaled() // слот обновляє дані на мнемосхемі
{
    foreach(QLineEdit *l,le)
    {
        l->setText(QString("%1").arg(s[3]->getValueScaled(l->objectName().right(l->objectName().size()-3)),3,'f',0));
    }
}

void Mnemo::slotCallReg()
{
    RpanelReg p(*s[3],sender()->objectName().right(2).toInt()-1,this);
    p.exec();
}

void Mnemo::slotCallKfOnePanel()
{
    KfPanel p(s,this);
    p.setWindowTitle(QString(tr("Фільтр № %1")).arg(sender()->objectName().right(1).toInt()+1));
    p.exec();
}

void Mnemo::slotCallVodaCtrl()
{
    dlgVodaCtrl p(*s[3],this);
    p.exec();

}

