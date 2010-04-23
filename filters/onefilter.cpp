#include "onefilter.h"
#include "ui_onefilter.h"

#include <QTimer>

#include <iodev.h>


OneFilter::OneFilter(IoDev &src,QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::OneFilter),
    n(1),
    s(src)
{
    m_ui->setupUi(this);

    QTimer *t=new QTimer(this);
    t->setInterval(1000);
    t->start();
    connect(t,SIGNAL(timeout()),this,SLOT(updateData()));

    connect(m_ui->bx_Am,SIGNAL(currentIndexChanged(int)),this,SLOT(setAm(int)));

}

OneFilter::~OneFilter()
{
    delete m_ui;
}

void OneFilter::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

 void OneFilter::setFn(int i)
 {
     n=i;
     m_ui->ld_Nf->display(i);

     // ініціалізувати перемикач режиму роботи.
     int v=s.getValue16(QString("Am_%1_").arg(n));
     m_ui->bx_Am->setCurrentIndex(v?1:0);

     if(v)
    {
        disconnect(m_ui->cb_Vl_1_1,SIGNAL(stateChanged(int)),this,SLOT(setValve(int)));
        disconnect(m_ui->cb_Vl_1_2,SIGNAL(stateChanged(int)),this,SLOT(setValve(int)));
        disconnect(m_ui->cb_Vl_1_3,SIGNAL(stateChanged(int)),this,SLOT(setValve(int)));
    }
    else
    {
        connect(m_ui->cb_Vl_1_1,SIGNAL(stateChanged(int)),this,SLOT(setValve(int)));
        connect(m_ui->cb_Vl_1_2,SIGNAL(stateChanged(int)),this,SLOT(setValve(int)));
        connect(m_ui->cb_Vl_1_3,SIGNAL(stateChanged(int)),this,SLOT(setValve(int)));
    }

}


 void OneFilter::updateData()
 {
        qint16 mode;
        mode=s.getValue16(QString("Amr_%1_").arg(n));

        m_ui->le_Amr->setText(mode?tr("Remote"):tr("Local"));

        if(mode==0 && m_ui->bx_Am->currentIndex()>0 ) // в локальному режимі
        {
            m_ui->bx_Am->setCurrentIndex(0); // вимкнути автомат
        }

        m_ui->cb_Cl_1_1->setChecked(s.getValue16(QString("Cl_%1_1").arg(n)));
        m_ui->cb_Cl_1_2->setChecked(s.getValue16(QString("Cl_%1_2").arg(n)));
        m_ui->cb_Cl_1_3->setChecked(s.getValue16(QString("Cl_%1_3").arg(n)));

        if(m_ui->bx_Am->currentIndex()) // якщо працюємо в автоматичному режимі -  показати команди
        {
            m_ui->cb_Vl_1_1->setChecked(s.getValue16(QString("Vl_%1_1").arg(n)));
            m_ui->cb_Vl_1_2->setChecked(s.getValue16(QString("Vl_%1_2").arg(n)));
            m_ui->cb_Vl_1_3->setChecked(s.getValue16(QString("Vl_%1_3").arg(n)));

        }

 }

void OneFilter::setAm(int v)
{
    s.sendValue(QString("Am_%1_").arg(n),qint16(v?-1:0));
    if(v)
    {
        disconnect(m_ui->cb_Vl_1_1,SIGNAL(stateChanged(int)),this,SLOT(setValve(int)));
        disconnect(m_ui->cb_Vl_1_2,SIGNAL(stateChanged(int)),this,SLOT(setValve(int)));
        disconnect(m_ui->cb_Vl_1_3,SIGNAL(stateChanged(int)),this,SLOT(setValve(int)));
    }
    else
    {
        connect(m_ui->cb_Vl_1_1,SIGNAL(stateChanged(int)),this,SLOT(setValve(int)));
        connect(m_ui->cb_Vl_1_2,SIGNAL(stateChanged(int)),this,SLOT(setValve(int)));
        connect(m_ui->cb_Vl_1_3,SIGNAL(stateChanged(int)),this,SLOT(setValve(int)));
    }
}

void OneFilter::setValve(int v)
{
    int f=sender()->objectName().mid(8,1).toInt(); // визначити номер клапана
    s.sendValue(QString("Vl_%1_%2").arg(n).arg(f),qint16(v?-1:0)); // відправити команду на клапан
}
