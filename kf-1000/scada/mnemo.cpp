
#include "mnemo.h"
#include "ui_mnemo.h"

#include <QtGui>
#include <IoNetClient.h>

#include <QVBoxLayout>
#include "kfone.h"
#include "kfupld.h"

Mnemo::Mnemo(IoNetClient &src, QWidget *p) : QLabel(p), m_ui(new Ui::mnemo),s(src)
{
    m_ui->setupUi(this);

    connect(&s,SIGNAL(updateDataRaw()),this,SLOT(updateDataRaw())); // при отриманні нових даних, засвітити їх на картинці
    connect(&s,SIGNAL(updateDataScaled()),this,SLOT(updateDataScaled())); // при отриманні нових даних, засвітити їх на картинці

    QVector<QStackedWidget*> sw;
    sw << m_ui->kf_1 << m_ui->kf_2 << m_ui->kf_3;

    for(int i=0;i<3;++i)
    {
            KfOne *o=new KfOne(this);
            KfUpld *u=new KfUpld(this);
            sw[i]->addWidget(o);
            sw[i]->addWidget(u);
            sw[i]->setCurrentIndex(0);
    }

    sw[1]->setCurrentIndex(1);

}

Mnemo::~Mnemo()
{
    delete m_ui;
}


void Mnemo::updateDataRaw()
{

}

void Mnemo::updateDataScaled() // слот обновляє дані на мнемосхемі
{

}
