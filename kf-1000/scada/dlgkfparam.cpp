#include "dlgkfparam.h"
#include "ui_dlgkfparam.h"

#include "iodev.h"

dlgKfParam::dlgKfParam(IoDev &source, QWidget *parent) :
    QDialog(parent),
    src(source),
    ui(new Ui::dlgKfParam)
{
    ui->setupUi(this);
    // технологічні налаштування
    ui->sb_Gsusp_zd->setValue(src.getValueScaled("Gsusp_zd"));
    ui->sb_Psusp_zd->setValue(src.getValueScaled("Psusp_zd"));

    ui->sb_Qvs_zd->setValue(src.getValueScaled("Qvs_zd"));
    qDebug() << src.getValueScaled("Qvs_zd");

    ui->sb_Qvw_zd->setValue(src.getValueScaled("Qvw_zd"));
    qDebug() << src.getValueScaled("Qvw_zd");

    ui->sb_Tnp_zd->setValue(src.getValue32("Tnp_zd")/1000);
    ui->sb_Tp_zd->setValue(src.getValue32("Tp_zd")/1000);
    ui->sb_Tf_zd->setValue(src.getValue32("Tf_zd")/1000);

    // інженерні налаштування
    ui->sb_Tb_zd->setValue(src.getValue32("Tb_zd")/1000);
    ui->sb_Trp_zd->setValue((double)src.getValue32("Trp_zd")/1000.0);
    ui->sb_Ta_zd->setValue(src.getValue32("Ta_zd")/1000);
    ui->sb_Tbw_zd->setValue(src.getValue32("Tbw_zd")/1000);
    ui->sb_Tpo_zd->setValue(src.getValue32("Tpo_zd")/1000);
    ui->sb_Ttoff_zd->setValue(src.getValue32("Ttoff_zd")/1000);

    ui->sb_kf_Psusp->setValue(src.getValue16("kf_Psusp"));
    ui->sb_kf_Poil->setValue(src.getValue16("kf_Poil"));

    ui->sb_k_Qv->setValue(src.getValueScaled("k_Qv"));

        connect(ui->bnExit,SIGNAL(clicked()),this,SLOT(slotAccept()));
}

dlgKfParam::~dlgKfParam()
{
    delete ui;
}

void dlgKfParam::changeEvent(QEvent *e)
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

void dlgKfParam::slotAccept()
{
    // технологія
    src.sendValueScaled("Psusp_zd",ui->sb_Psusp_zd->value());
    src.sendValueScaled("Gsusp_zd",ui->sb_Gsusp_zd->value());
    src.sendValueScaled("Qvs_zd",ui->sb_Qvs_zd->value());
    src.sendValueScaled("Qvw_zd",ui->sb_Qvw_zd->value());

    src.sendValue("Tnp_zd",qint32(ui->sb_Tnp_zd->value()*1000));
    src.sendValue("Tp_zd",qint32(ui->sb_Tp_zd->value()*1000));
    src.sendValue("Tf_zd",qint32(ui->sb_Tf_zd->value()*1000));

   // інженерні налаштування
    src.sendValue("Tb_zd",qint32(ui->sb_Tb_zd->value()*1000));
    src.sendValue("Trp_zd",qint32(ui->sb_Trp_zd->value()*1000.0));
    src.sendValue("Ta_zd",qint32(ui->sb_Ta_zd->value()*1000));
    src.sendValue("Tbw_zd",qint32(ui->sb_Tbw_zd->value()*1000));
    src.sendValue("Tpo_zd",qint32(ui->sb_Tpo_zd->value()*1000));
    src.sendValue("Ttoff_zd",qint32(ui->sb_Ttoff_zd->value()*1000));

    src.sendValue("kf_Psusp",qint16(ui->sb_kf_Psusp->value()));
    src.sendValue("kf_Poil",qint16(ui->sb_kf_Poil->value()));

    src.sendValueScaled("k_Qv",ui->sb_k_Qv->value());
    accept();
}



