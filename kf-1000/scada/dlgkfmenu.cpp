#include "dlgkfmenu.h"
#include "ui_dlgkfmenu.h"

dlgKfMenu::dlgKfMenu(QString text, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgKfMenu),
    res(0)
{
    ui->setupUi(this);

    ui->le_State->setText(text);
    connect(ui->bn_13,SIGNAL(clicked()),this,SLOT(slotSetVal()));
    connect(ui->bn_05,SIGNAL(clicked()),this,SLOT(slotSetVal()));
    connect(ui->bn_06,SIGNAL(clicked()),this,SLOT(slotSetVal()));
    connect(ui->bn_07,SIGNAL(clicked()),this,SLOT(slotSetVal()));
    connect(ui->bn_08,SIGNAL(clicked()),this,SLOT(slotSetVal()));
    connect(ui->bn_09,SIGNAL(clicked()),this,SLOT(slotSetVal()));

}

dlgKfMenu::~dlgKfMenu()
{
    delete ui;
}

void dlgKfMenu::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void dlgKfMenu::slotSetVal()
{
    res=sender()->objectName().right(2).toInt();
    accept();
}

