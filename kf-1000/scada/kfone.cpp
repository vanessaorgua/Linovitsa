#include "kfone.h"
#include "ui_kfone.h"

#include <QWidget>

KfOne::KfOne(QWidget *parent) :
    QLabel(parent),
    ui(new Ui::KfOne)
{
    ui->setupUi(this);
}

KfOne::~KfOne()
{
    delete ui;
}

void KfOne::changeEvent(QEvent *e)
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
