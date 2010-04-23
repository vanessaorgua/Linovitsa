#include "sparamdialog.h"
#include "ui_sparamdialog.h"
#include <iodev.h>


SParamDialog::SParamDialog(IoDev &src,QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SParamDialog),
    s(src)
{
    m_ui->setupUi(this);
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
