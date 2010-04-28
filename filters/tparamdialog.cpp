#include "tparamdialog.h"
#include "ui_tparamdialog.h"
#include <iodev.h>
#include <QTime>


TParamDialog::TParamDialog(IoDev &src,QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::TParamDialog),
    s(src)
{
    m_ui->setupUi(this);

   m_ui->sb_T_v2_zd->setValue(s.getValue32("T_v2_zd")/1000);
   m_ui->sb_T_v3_zd->setValue(s.getValue32("T_v3_zd")/1000);

   m_ui->sp_Tper_zd->setTime(QTime().addMSecs((s.getValue32("Tper_zd")/1000)*1000));
   m_ui->sb_Tst_zd->setTime(QTime().addMSecs(s.getValue32("Tst_zd")));

   m_ui->sb_Q_zd->setValue(s.getValueFloat("Q_zd"));

   m_ui->bx_Nc_zd->setCurrentIndex(s.getValue16("Nc_zd"));

   connect(m_ui->bn_Ok,SIGNAL(clicked()),this,SLOT(myAccept()));

}

TParamDialog::~TParamDialog()
{
    delete m_ui;
}

void TParamDialog::changeEvent(QEvent *e)
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

void TParamDialog::myAccept() // реакція на кнопку Ок
{
    s.sendValue("T_v2_zd",qint32(m_ui->sb_T_v2_zd->value()*1000));
    s.sendValue("T_v3_zd",qint32(m_ui->sb_T_v3_zd->value()*1000));


    s.sendValue("Tper_zd",qint32(QTime().msecsTo(m_ui->sp_Tper_zd->time())));
    s.sendValue("Tst_zd",qint32(QTime().msecsTo(m_ui->sb_Tst_zd->time())));

    s.sendValue("Q_zd",m_ui->sb_Q_zd->value());

    s.sendValue("Nc_zd",qint16(m_ui->bx_Nc_zd->currentIndex()));

    // смикнут контролер
    //s.sendValue("Run",qint16(0));
    s.sendValue("Save",qint16(-1));

    accept();
}
