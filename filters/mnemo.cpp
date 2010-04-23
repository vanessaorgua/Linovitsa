
#include "mnemo.h"
#include "ui_mnemo.h"

#include <trendchart.h>
#include <IoNetClient.h>

#include <QVBoxLayout>

#include "onefilter.h"

Mnemo::Mnemo(IoNetClient &src, QWidget *p) : QLabel(p), m_ui(new Ui::mnemo),s(src)
{
    m_ui->setupUi(this);

    trc= new TrendChart(m_ui->Trend,600); // на десять хвилин

    QVBoxLayout *vbl1=new QVBoxLayout(m_ui->Trend);
    vbl1->addWidget(trc);
    m_ui->Trend->setLayout(vbl1);

    connect(&s,SIGNAL(updateData()),this,SLOT(updateData())); // при отриманні нових даних, засвітити їх на картинці

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
 << m_ui->cb_Vl_8_3;





}

Mnemo::~Mnemo()
{
    delete m_ui;
}


void Mnemo::updateData()
{
    //qDebug() << s[0]->getTags().keys();

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

    m_ui->bx_Nc_zd->setCurrentIndex(s[0]->getValue16("Nc_zd"));

    m_ui->le_Tper_zd->setText(QString("%1").arg(s[0]->getValue32("Tper_zd")/1000));
    m_ui->le_Tper->setText(QString("%1").arg(s[0]->getValue16("Tper")));

    m_ui->le_Q_zd->setText(QString("%1").arg(s[0]->getValueFloat("Q_zd")));
    m_ui->le_Q->setText(QString("%1").arg(s[0]->getValueFloat("Q")));

    m_ui->le_T_v2_zd->setText(QString("%1").arg(s[0]->getValue32("T_v2_zd")/1000));
    m_ui->le_T_v3_zd->setText(QString("%1").arg(s[0]->getValue32("T_v3_zd")/1000));

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

    foreach(QCheckBox* v,cb_all)
    {
        v->setChecked( s[0]->getValue16(v->objectName().right(v->objectName().size()-3) ) );

    }

    m_ui->pb_L_nfs->setValue(s[0]->getValue16("L_nfs"));
    m_ui->pb_L_nz->setValue(s[0]->getValue16("L_nz"));
    m_ui->pb_L_sus->setValue(s[0]->getValue16("L_sus"));
    m_ui->pb_L_cs->setValue(s[0]->getValue16("L_cs"));
    m_ui->pb_L_ho->setValue(s[0]->getValue16("L_ho"));


    // це місце треба переробляти на виведення шкальованих значень
    m_ui->le_L_nfs->setText(QString("%1").arg(s[0]->getValue16("L_nfs")));
    m_ui->le_L_nz->setText(QString("%1").arg(s[0]->getValue16("L_nz")));
    m_ui->le_L_sus->setText(QString("%1").arg(s[0]->getValue16("L_sus")));
    m_ui->le_L_cs->setText(QString("%1").arg(s[0]->getValue16("L_cs")));
    m_ui->le_L_ho->setText(QString("%1").arg(s[0]->getValue16("L_ho")));

    //m

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
    OneFilter *f=new OneFilter(*s[0],this);
    f->setFn(sender()->objectName().mid(5,1).toInt()); // встановити номер фільтра
    f->exec();


}
