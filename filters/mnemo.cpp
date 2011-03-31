
#include "mnemo.h"
#include "ui_mnemo.h"

#include <trendchart.h>
#include <IoNetClient.h>

#include <QVBoxLayout>

#include "onefilter.h"
#include "tparamdialog.h"
#include "panelReg.h"


Mnemo::Mnemo(IoNetClient &src, QWidget *p) : QLabel(p), m_ui(new Ui::mnemo),s(src)
{
    m_ui->setupUi(this);

    trc= new TrendChart(m_ui->Trend,600); // на десять хвилин

    QVBoxLayout *vbl1=new QVBoxLayout(m_ui->Trend);
    vbl1->addWidget(trc);
    m_ui->Trend->setLayout(vbl1);

    QVector<QColor> clr;
    clr << QColor(255,24,237) << QColor(66,168,255)<< QColor(255,39,0)<< QColor(100,255,0)<< QColor(Qt::yellow)<< QColor(90,0,113)<< QColor(0,9,137)<< QColor(0,89,0);
    trc->setColor(clr);

    connect(&s,SIGNAL(updateDataRaw()),this,SLOT(updateDataRaw())); // при отриманні нових даних, засвітити їх на картинці
    connect(&s,SIGNAL(updateDataScaled()),this,SLOT(updateDataScaled())); // при отриманні нових даних, засвітити їх на картинці

    QTimer* t=new QTimer(this);
    t->setInterval(5000);
    t->start();
    connect(t,SIGNAL(timeout()),this,SLOT(updateTrChart()));


    connect(m_ui->bn_F_1,SIGNAL(clicked()),this,SLOT(slotFilter()));
    connect(m_ui->bn_F_2,SIGNAL(clicked()),this,SLOT(slotFilter()));
    connect(m_ui->bn_F_3,SIGNAL(clicked()),this,SLOT(slotFilter()));
    connect(m_ui->bn_F_4,SIGNAL(clicked()),this,SLOT(slotFilter()));
    connect(m_ui->bn_F_5,SIGNAL(clicked()),this,SLOT(slotFilter()));
    connect(m_ui->bn_F_6,SIGNAL(clicked()),this,SLOT(slotFilter()));
    connect(m_ui->bn_F_7,SIGNAL(clicked()),this,SLOT(slotFilter()));
    connect(m_ui->bn_F_8,SIGNAL(clicked()),this,SLOT(slotFilter()));

    connect(m_ui->bn_TParam,SIGNAL(clicked()),this,SLOT(slotTParam()));

    connect(m_ui->bnX_01,SIGNAL(clicked()),this,SLOT(slotCallReg()));

    cb_all   // ініціалізувати масив, для того щоб потім за раз все завантажити
 << m_ui->cb_Cl_1_1
 << m_ui->cb_Cl_1_2
 << m_ui->cb_Cl_1_3
 << m_ui->cb_Cl_2_1
 << m_ui->cb_Cl_2_2
 << m_ui->cb_Cl_2_3
 << m_ui->cb_Cl_3_1
 << m_ui->cb_Cl_3_2
 << m_ui->cb_Cl_3_3
 << m_ui->cb_Cl_4_1
 << m_ui->cb_Cl_4_2
 << m_ui->cb_Cl_4_3
 << m_ui->cb_Cl_5_1
 << m_ui->cb_Cl_5_2
 << m_ui->cb_Cl_5_3
 << m_ui->cb_Cl_6_1
 << m_ui->cb_Cl_6_2
 << m_ui->cb_Cl_6_3
 << m_ui->cb_Cl_7_1
 << m_ui->cb_Cl_7_2
 << m_ui->cb_Cl_7_3
 << m_ui->cb_Cl_8_1
 << m_ui->cb_Cl_8_2
 << m_ui->cb_Cl_8_3
 << m_ui->cb_Vl_1_1
 << m_ui->cb_Vl_1_2
 << m_ui->cb_Vl_1_3
 << m_ui->cb_Vl_2_1
 << m_ui->cb_Vl_2_2
 << m_ui->cb_Vl_2_3
 << m_ui->cb_Vl_3_1
 << m_ui->cb_Vl_3_2
 << m_ui->cb_Vl_3_3
 << m_ui->cb_Vl_4_1
 << m_ui->cb_Vl_4_2
 << m_ui->cb_Vl_4_3
 << m_ui->cb_Vl_5_1
 << m_ui->cb_Vl_5_2
 << m_ui->cb_Vl_5_3
 << m_ui->cb_Vl_6_1
 << m_ui->cb_Vl_6_2
 << m_ui->cb_Vl_6_3
 << m_ui->cb_Vl_7_1
 << m_ui->cb_Vl_7_2
 << m_ui->cb_Vl_7_3
 << m_ui->cb_Vl_8_1
 << m_ui->cb_Vl_8_2
 << m_ui->cb_Vl_8_3
 << m_ui->cb_Alm_1_6
 << m_ui->cb_Alm_2_6
 << m_ui->cb_Alm_3_6
 << m_ui->cb_Alm_4_6
 << m_ui->cb_Alm_5_6
 << m_ui->cb_Alm_6_6
 << m_ui->cb_Alm_7_6
 << m_ui->cb_Alm_8_6;


    le_State << m_ui->le_State_1
            << m_ui->le_State_2
            << m_ui->le_State_3
            << m_ui->le_State_4
            << m_ui->le_State_5
            << m_ui->le_State_6
            << m_ui->le_State_7
            << m_ui->le_State_8;

    le_T << m_ui->le_T_1
            << m_ui->le_T_2
            << m_ui->le_T_3
            << m_ui->le_T_4
            << m_ui->le_T_5
            << m_ui->le_T_6
            << m_ui->le_T_7
            << m_ui->le_T_8;
}

Mnemo::~Mnemo()
{
    delete m_ui;
}


void Mnemo::updateDataRaw()
{
    //qDebug() << "updateDataRaw()";

    if(s[0]->getValue16("r_l"))
    {
        m_ui->le_r_l->setText(tr("Full"));
    }
    else
    {
        m_ui->le_r_l->setText(tr("Half"));
    }

    qint16 nc=s[0]->getValue16("Nom_Cur");

    m_ui->le_NomCur->setText(QString("%1").arg(nc));

    QString leNc_zd;
    switch(s[0]->getValue16("Nc_zd"))
    {
    case 0:
        leNc_zd="1";
        break;
    case 1:
        leNc_zd="1/2";
        break;
    case 2:
        leNc_zd="1/3";
        break;
    case 3:
        leNc_zd=tr("Not partial");
        break;
    default:
        leNc_zd="?";
    }
    m_ui->leNc_zd->setText(leNc_zd);

    m_ui->le_Tper_zd->setText(QString("%1").arg(s[0]->getValue32("Tper_zd")/1000));
    m_ui->le_Tper->setText(QString("%1").arg(s[0]->getValue16("Tper")));

    m_ui->le_Q_zd->setText(QString("%1").arg(s[0]->getValueFloat("Q_zd")));
    m_ui->le_Q->setText(QString("%1").arg(s[0]->getValueFloat("Q"),4,'f',0));

    m_ui->le_T_v2_zd->setText(QString("%1").arg(s[0]->getValue32("T_v2_zd")/1000));
    m_ui->le_T_v3_zd->setText(QString("%1").arg(s[0]->getValue32("T_v3_zd")/1000));
    m_ui->cAM_01->setChecked(!s[0]->getValue16("Am_01"));

    if(nc)
    {
        m_ui->le_T_v2->setText(QString("%1").arg(s[0]->getValue16(QString("T_v2_%1_e").arg(nc,1) )));
        m_ui->le_T_v3->setText(QString("%1").arg(s[0]->getValue16(QString("T_v3_%1_e").arg(nc,1) )));
    }
    else
    {
        m_ui->le_T_v2->setText("");
        m_ui->le_T_v3->setText("");
    }
    m_ui->di_1->setChecked(s[0]->getValue16("Di_0")?Qt::Checked:Qt::Unchecked);
    m_ui->di_2->setChecked(s[0]->getValue16("Di_1")?Qt::Checked:Qt::Unchecked);
    m_ui->di_3->setChecked(s[0]->getValue16("Di_2")?Qt::Checked:Qt::Unchecked);
    m_ui->di_4->setChecked(s[0]->getValue16("Di_3")?Qt::Checked:Qt::Unchecked);
    m_ui->di_5->setChecked(s[0]->getValue16("Di_4")?Qt::Checked:Qt::Unchecked);

    foreach(QCheckBox* v,cb_all)
    {
        v->setChecked( s[0]->getValue16(v->objectName().right(v->objectName().size()-3) ) );

    }

    m_ui->pb_L_nfs->setValue(s[0]->getValue16("L_nfs"));
    m_ui->pb_L_nz->setValue(s[0]->getValue16("L_nz"));
    m_ui->pb_L_sus->setValue(s[0]->getValue16("L_sus"));
    m_ui->pb_L_cs->setValue(s[0]->getValue16("L_cs"));
    m_ui->pb_L_ho->setValue(s[0]->getValue16("L_ho"));


    int i=1;
    foreach(QLineEdit *le,le_State)
    {
        if(! s[0]->getValue16(QString("Dio_%1_").arg(i))) // перевірити зв’язок
        {
            le->setText(tr("Off-line"));
        }
        else
        {
            if(s[0]->getValue16(QString("Amr_%1_").arg(i))) // перевірити зв’язок
            {

                if(! s[0]->getValue16(QString("Am_%1_").arg(i))) // перевірити зв’язок
                {
                    le->setText(tr("Manual"));
                }
                else
                {
                    switch(s[0]->getValue16(QString("State_%1_").arg(i)))
                    {
                        case 0: // ST_OFF
                            le->setText(tr("Off"));
                            break;
                        case 1: // ST_START
                            le->setText(tr("Start"));
                            break;
                        case 2: // ST_START
                            le->setText(tr("Filtering"));
                            break;
                        case 3:
                        case 4:
                        case 5:
                            le->setText(tr("Upload"));
                            break;
                        default:
                            le->setText(tr("Unknown"));
                            break;
                    }
                }

            }
            else
            {
                le->setText(tr("Local"));
            }
        }
        ++i;
    }

    i=1;
    foreach(QLineEdit *le,le_T)
    {
        le->setText(QString("%1:%2").arg(s[0]->getValue16(QString("Tf_h_%1_").arg(i)),2,10,QChar('0')).arg(s[0]->getValue16(QString("Tf_m_%1_").arg(i)),2,10,QChar('0')));
        ++i;
    }
   //m_ui->lbKpr->setText(QString("%1").arg(s[0]->getValueFloat("Kp_01")));
}

void Mnemo::updateDataScaled() // слот обновляє дані на мнемосхемі
{
    // це місце треба переробляти на виведення шкальованих значень
    m_ui->le_L_nfs->setText(QString("%1").arg(s[0]->getValueScaled("L_nfs"),3,'f',0));
    m_ui->le_L_nz->setText(QString("%1").arg(s[0]->getValueScaled("L_nz"),3,'f',0));
    m_ui->le_L_sus->setText(QString("%1").arg(s[0]->getValueScaled("L_sus"),3,'f',0));
    m_ui->le_L_cs->setText(QString("%1").arg(s[0]->getValueScaled("L_cs"),3,'f',0));
    m_ui->le_L_ho->setText(QString("%1").arg(s[0]->getValueScaled("L_ho"),3,'f',0));

    m_ui->ld_T_s->display(s[0]->getValueScaled("T_s"));
    m_ui->ld_G_cs->display(s[0]->getValueScaled("G_cs"));

    m_ui->lX_01->display(s[0]->getValueScaled("X_01"));

}

void Mnemo::updateTrChart() // малюваня графіка раз в 5 секунд
{
    QVector<double> v;

    v << s[0]->getValue16("L_nfs")
      << s[0]->getValue16("L_nz")
      << s[0]->getValue16("L_ho")
      << s[0]->getValue16("L_cs")
      << s[0]->getValue16("L_sus")
      << s[0]->getValue16("G_cs")
      << s[0]->getValue16("T_s");

    trc->addPoint(v);


}

void Mnemo::slotFilter()
{
    OneFilter f(*s[0],this);
    f.setFn(sender()->objectName().mid(5,1).toInt()); // встановити номер фільтра
    f.exec();

}

void Mnemo::slotTParam() // кнопка завдання технологічних параметрів
{

    TParamDialog p(*s[0],this);
    p.exec();


}


void Mnemo::slotCallReg()
{
    RpanelReg p(*s[0],0,this);
    p.exec();
}
