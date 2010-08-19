#include "sparamdialog.h"
#include "ui_sparamdialog.h"
#include <iodev.h>
#include <QSettings>

#include <QDebug>


SParamDialog::SParamDialog(IoDev &src,QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SParamDialog),
    s(src)
{
    m_ui->setupUi(this);

    foreach(QString tag,s.getTags().keys())
    {
        if(s.isScaleChange(tag))
            tag_name << tag;
    }

    foreach(QString str,tag_name)
    {
        m_ui->bx_Teg->addItem(s.getText()[str]);
    }

    connect(m_ui->bn_Ok,SIGNAL(clicked()),this,SLOT(myAccept()));
    connect(m_ui->bx_Teg,SIGNAL(currentIndexChanged(int)),this,SLOT(selectTeg(int)));

    connect(m_ui->sb_Fs,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));
    connect(m_ui->sb_Zs,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));
    connect(m_ui->sb_f,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));
    connect(m_ui->sb_Scope,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));
    //connect(m_ui->sb_m,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));

    m_ui->bx_Teg->setCurrentIndex(0);
    selectTeg(0); // в цьому місці є проблема із запуском коли немає тегів для шкалювання

    QSettings set;
    m_ui->le_ioserv->setText(set.value("/ioserv/hostname","localhost").toString());

    m_ui->le_dbserv->setText(set.value("/db/hostname","localhost").toString());
    m_ui->le_dbname->setText(set.value("/db/dbname","test").toString());
    m_ui->le_username->setText(set.value("/db/username","scada").toString());
    m_ui->le_passwd->setText(set.value("/db/passwd","").toString());

   m_ui->sb_Lsusp_max->setValue(s.getValueScaled("Lsusp_max"));
   m_ui->sb_Ls_start->setValue(s.getValueScaled("Ls_start"));
   m_ui->sb_Ls_min->setValue(s.getValueScaled("Ls_min"));

   m_ui->sb_Tw_1->setValue(s.getValue32("Tw_1")/1000);
   m_ui->sb_Tw_2->setValue(s.getValue32("Tw_2")/1000);
   m_ui->sb_Tw_3->setValue(s.getValue32("Tw_3")/1000);

   m_ui->sb_Xv_zd_min->setValue(s.getValueFloat("Xv_zd_min")/40.0);
   m_ui->sb_Xs_zd_min->setValue(s.getValueFloat("Xs_zd_min")/40.0);
   m_ui->sb_Xs_np_zd->setValue(s.getValueFloat("Xv_np_zd")/40.0);
}

SParamDialog::~SParamDialog()
{
    delete m_ui;
}

void SParamDialog::changeEvent(QEvent *e)
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


void SParamDialog::myAccept()
{
    s.sendValueScaled("Lsusp_max",m_ui->sb_Lsusp_max->value());
    s.sendValueScaled("Ls_start",m_ui->sb_Ls_start->value());
    s.sendValueScaled("Ls_min",m_ui->sb_Ls_min->value());

    s.sendValue("Tw_1",m_ui->sb_Tw_1->value()*1000);
    s.sendValue("Tw_2",m_ui->sb_Tw_2->value()*1000);
    s.sendValue("Tw_3",m_ui->sb_Tw_3->value()*1000);

    s.sendValue("Xv_zd_min",m_ui->sb_Xv_zd_min->value()*40.0);
    s.sendValue("Xs_zd_min",m_ui->sb_Xs_zd_min->value()*40.0);
    s.sendValue("Xs_np_zd",m_ui->sb_Xs_np_zd->value()*40.0);

    s.sendValue("Save",qint16(-1));
    s.sendValue("Run",qint16(0));

    QSettings set;
    set.setValue("/ioserv/hostname",m_ui->le_ioserv->text());

    set.setValue("/db/hostname",m_ui->le_dbserv->text());
    set.setValue("/db/dbname",m_ui->le_dbname->text());
    set.setValue("/db/username",m_ui->le_username->text());
    set.setValue("/db/passwd",m_ui->le_passwd->text());


    accept();
}



void SParamDialog::slotSet(QString v )
{
    //qDebug() << sender()->objectName();

    if(sender()->objectName()=="sb_Zs")
        s.setScaleZero(tag_name[m_ui->bx_Teg->currentIndex()],v.toDouble());
    else if(sender()->objectName()=="sb_Fs")
        s.setScaleFull(tag_name[m_ui->bx_Teg->currentIndex()],v.toDouble());
    else if(sender()->objectName()=="sb_f")
        s.sendValue(QString("Kf_%1").arg((s.getAddress(tag_name[m_ui->bx_Teg->currentIndex()])-38)/2+1 ,2,10,QChar('0')),qint16(v.toInt()));
    else if(sender()->objectName()=="sb_Scope")
    {
        s.sendValue(QString("Fs_%1").arg((s.getAddress(tag_name[m_ui->bx_Teg->currentIndex()])-38)/2+1,2,10,QChar('0')),v.toDouble());

      //  qDebug() << QString("Fs_%1").arg((s.getAddress(tag_name[m_ui->bx_Teg->currentIndex()])-38)/2+1,2,10,QChar('0'))<< v.toDouble() ;
    }

}

void SParamDialog::selectTeg(int v)
{
    m_ui->sb_Zs->setValue(s.scaleZero(tag_name[v]));
    m_ui->sb_Fs->setValue(s.scaleFull(tag_name[v]));
    m_ui->sb_f->setValue(s.getValue16(QString("Kf_%1").arg((s.getAddress(tag_name[v])-38)/2+1 ,2,10,QChar('0'))));
    m_ui->sb_Scope->setValue(s.getValueFloat(QString("Fs_%1").arg((s.getAddress(tag_name[v])-38)/2+1 ,2,10,QChar('0'))));

    //qDebug() << "tag" << tag_name[v] << "Index" << s.getIndex(tag_name[v]) << QString("kf_%1").arg((s.getAddress(tag_name[v])-38)/2+1 ,2,10,QChar('0')) << QString("Fs_%1").arg((s.getAddress(tag_name[v])-38)/2+1 ,2,10,QChar('0'));

    //m_ui->sb_m->setValue((double)s.getValue16(QString("m_%1").arg(tag_name[v]))/4000.0*(s.scaleFull(tag_name[v])-s.scaleZero(tag_name[v]))+s.scaleZero(tag_name[v]));
}



