#include "sparamdialog.h"
#include "ui_sparamdialog.h"
#include <iodev.h>

#include <QDebug>

SParamDialog::SParamDialog(IoDev &src,QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SParamDialog),
    s(src),ch(-1)
{
    m_ui->setupUi(this);

    m_ui->sb_Taval_1->setValue(s.getValue32("Tavl_1")/1000);
    m_ui->sb_Taval_2->setValue(s.getValue32("Tavl_2")/1000);
    m_ui->sb_Taval_3->setValue(s.getValue32("Tavl_3")/1000);


    tag_name << "L_nfs"
            << "L_nz"
            << "L_sus"
            << "L_cs"
            << "L_ho"
            << "T_s"
            << "G_cs";
    foreach(QString str,tag_name)
    {
        m_ui->bx_Teg->addItem(s.getText()[str]);
    }

    connect(m_ui->bn_Ok,SIGNAL(clicked()),this,SLOT(myAccept()));
    connect(m_ui->bx_Teg,SIGNAL(currentIndexChanged(int)),this,SLOT(selectTeg(int)));

    connect(m_ui->sb_Fs,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));
    connect(m_ui->sb_Zs,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));
    connect(m_ui->sb_f,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));
    connect(m_ui->sb_m,SIGNAL(valueChanged(QString)),this,SLOT(slotSet(QString)));

    m_ui->bx_Teg->setCurrentIndex(0);

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
    s.sendValue("Tavl_1",qint32(m_ui->sb_Taval_1->value()*1000));
    s.sendValue("Tavl_2",qint32(m_ui->sb_Taval_2->value()*1000));
    s.sendValue("Tavl_3",qint32(m_ui->sb_Taval_3->value()*1000));

    s.sendValue("Save",qint16(-1));
    accept();
}



void SParamDialog::slotSet(QString v )
{
    ch=m_ui->bx_Teg->currentIndex();
    qDebug() << sender()->objectName() << v;
}

void SParamDialog::selectTeg(int v)
{
    // відправити змінені дані

    if(-1<ch)
    {
        s.setScaleZero(tag_name[ch],m_ui->sb_Zs->value());
        s.setScaleFull(tag_name[ch],m_ui->sb_Fs->value());
        s.sendValue(QString("f_%1").arg(tag_name[v]),qint16(m_ui->sb_f->value()));
        s.sendValue(QString("m_%1").arg(tag_name[v]),qint16( ((double)m_ui->sb_m->value()-s.scaleZero(tag_name[v]))/(s.scaleFull(tag_name[v])+s.scaleZero(tag_name[v]))*4000.0 ));

        // смикнут контролер
        s.sendValue("Run",qint16(0));
        s.sendValue("Save",qint16(-1));

    }

    m_ui->sb_Zs->setValue(s.scaleZero(tag_name[v]));
    m_ui->sb_Fs->setValue(s.scaleFull(tag_name[v]));
    m_ui->sb_f->setValue(s.getValue16(QString("f_%1").arg(tag_name[v])));

    m_ui->sb_m->setValue((double)s.getValue16(QString("m_%1").arg(tag_name[v]))/4000.0*(s.scaleFull(tag_name[v])-s.scaleZero(tag_name[v]))+s.scaleZero(tag_name[v]));
    ch=-1;
    qDebug() << "ch " << ch;
}



