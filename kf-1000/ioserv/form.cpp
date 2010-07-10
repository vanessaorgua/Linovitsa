#include "form.h"
#include "ui_form.h"
#include <iodev.h>

Form::Form(IoDev &source, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::Form),
    src(source)
{
    m_ui->setupUi(this);
/*    connect(m_ui->M_1,SIGNAL(toggled(bool)),this,SLOT(slotSend(bool)));
    connect(m_ui->M_2,SIGNAL(toggled(bool)),this,SLOT(slotSend(bool)));
    connect(m_ui->M_3,SIGNAL(toggled(bool)),this,SLOT(slotSend(bool)));
    connect(m_ui->M_4,SIGNAL(toggled(bool)),this,SLOT(slotSend(bool)));
    connect(m_ui->M_5,SIGNAL(toggled(bool)),this,SLOT(slotSend(bool)));*/

}

Form::~Form()
{
    delete m_ui;
}

void Form::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void Form::updateData()
{
    QString r;
    foreach(QString s,src.getTags().keys())
    {
        r+=QString("%1=%2; ").arg(s).arg(src.getValue16(s));

    }
    m_ui->tagView->setText(r);
}

void Form::slotSend(bool v)
{

    src.sendValue(sender()->objectName(),qint16(v?-1:0));
}
