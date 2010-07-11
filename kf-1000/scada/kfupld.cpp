#include "kfupld.h"
#include "ui_kfupld.h"
#include <QWidget>

KfUpld::KfUpld(QWidget *parent) :
    QLabel(parent),
    ui(new Ui::KfUpld)
{
    ui->setupUi(this);
}

KfUpld::~KfUpld()
{
    delete ui;
}

void KfUpld::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
