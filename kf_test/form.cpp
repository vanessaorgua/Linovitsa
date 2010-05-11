#include "form.h"
#include "ui_form.h"
#include "../iodev.h"
#include <QDebug>

Form::Form(IoDev &s,QWidget *parent) :
    QWidget(parent),
    src(s),
    m_ui(new Ui::Form),
    lState(-1)
{
    m_ui->setupUi(this);

    //кнопки
    connect(m_ui->bn_StopAll,SIGNAL(clicked()),this,SLOT(sendState()));
    connect(m_ui->bn_Preshed,SIGNAL(clicked()),this,SLOT(sendState()));
    connect(m_ui->bn_Free,SIGNAL(clicked()),this,SLOT(sendState()));
    connect(m_ui->bn_Start,SIGNAL(clicked()),this,SLOT(sendState()));
    connect(m_ui->bn_Pause,SIGNAL(clicked()),this,SLOT(sendState()));

}

Form::~Form()
{
    delete m_ui;
}

void Form::sendData(int v)
{
    src.sendValue(sender()->objectName().split("_")[1],qint16(v?-1:0));
    qDebug() << sender()->objectName().split("_")[1] << qint16(v?-1:0);

}


void Form::updateData()
{
    qint16 nState=src.getValue16("State");

    if(nState==1 && nState!=lState)
    {
        connect(m_ui->cb_gz,SIGNAL(stateChanged(int)),this,SLOT(sendData(int)));
        connect(m_ui->cb_gp,SIGNAL(stateChanged(int)),this,SLOT(sendData(int)));
        connect(m_ui->cb_mf,SIGNAL(stateChanged(int)),this,SLOT(sendData(int)));
        connect(m_ui->cb_mb,SIGNAL(stateChanged(int)),this,SLOT(sendData(int)));
    }
    lState=nState;
    switch(nState)
    {
        case 0:
            m_ui->le_State->setText(tr("Вимикання всього"));
            break;
        case 1:
            m_ui->le_State->setText(tr("Ручне управління"));
            break;
        case 2:
            m_ui->le_State->setText(tr("Стискання пресу"));
            break;
        case 3:
            m_ui->le_State->setText(tr("Розтискання пресу"));
            break;
        case 4:
        case 5:
            m_ui->le_State->setText(tr("Позиціонування каретки"));
            break;
        case 6:
        case 7:
            m_ui->le_State->setText(tr("Захват рами"));
            break;
        case 8:
            m_ui->le_State->setText(tr("Переміщення рами"));
            break;
        default:
            m_ui->le_State->setText(tr("Невідомо"));
    }

    m_ui->cb_c_mf->setCheckState(src.getValue16("mf")?Qt::Checked:Qt::Unchecked);
    m_ui->cb_c_mb->setCheckState(src.getValue16("mb")?Qt::Checked:Qt::Unchecked);

    m_ui->cb_c_kt->setCheckState(src.getValue16("c_kt")?Qt::Checked:Qt::Unchecked);
    m_ui->cb_c_kh->setCheckState(src.getValue16("c_kh")?Qt::Checked:Qt::Unchecked);
    m_ui->cb_c_hu->setCheckState(src.getValue16("c_hu")?Qt::Checked:Qt::Unchecked);

    if(src.getValue16("gp"))
    {
        if(src.getValue16("gz"))
        {
            m_ui->cb_fp->setCheckState(Qt::Unchecked);
            m_ui->cb_fr->setCheckState(Qt::Checked);
        }
        else
        {
            m_ui->cb_fp->setCheckState(Qt::Checked);
            m_ui->cb_fr->setCheckState(Qt::Unchecked);
        }
    }
    else
    {
        m_ui->cb_fp->setCheckState(Qt::Unchecked);
        m_ui->cb_fr->setCheckState(Qt::Unchecked);
    }

    m_ui->cb_c_p->setCheckState(src.getValue16("c_p")?Qt::Checked:Qt::Unchecked);

    m_ui->cb_c_ff->setCheckState(src.getValue16("c_ff")?Qt::Checked:Qt::Unchecked);
    m_ui->cb_c_fa->setCheckState(src.getValue16("c_fa")?Qt::Checked:Qt::Unchecked);



}

 void Form::sendState()
 {
    qint16 i=0;
    QString s=sender()->objectName();
    if(s=="bn_Preshed")
        i=2;
    else if(s=="bn_Free")
        i=3;
    else if(s=="bn_Start")
        i=4;
    else if(s=="bn_Pause")
        i=6;

    src.sendValue("State",i);

    // на час виконнаня всіх дій відключити кнопки і сигнали
    disconnect(m_ui->cb_gz,SIGNAL(stateChanged(int)),this,SLOT(sendData(int)));
    disconnect(m_ui->cb_gp,SIGNAL(stateChanged(int)),this,SLOT(sendData(int)));
    disconnect(m_ui->cb_mf,SIGNAL(stateChanged(int)),this,SLOT(sendData(int)));
    disconnect(m_ui->cb_mb,SIGNAL(stateChanged(int)),this,SLOT(sendData(int)));

    // та скинути всі гачки
    m_ui->cb_gz->setCheckState(Qt::Unchecked);
    m_ui->cb_gp->setCheckState(Qt::Unchecked);
    m_ui->cb_c_mf->setCheckState(Qt::Unchecked);
    m_ui->cb_c_mb->setCheckState(Qt::Unchecked);

 }



