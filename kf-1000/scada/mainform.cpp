#include "mainform.h"

#include "mnemo.h"

#include <QDebug>

#include <QMessageBox>
#include <IoNetClient.h>

#include "sparamdialog.h"
#include "about.h"
#include "history.h"
#include "alertviewdialog.h"
#include "dlgptsctrl.h"
#include "report.h"

mMainForm::mMainForm(IoNetClient &source,QWidget *p): QWidget(p)
,src(source),m_ui(new Ui::Form)
{

    m_ui->setupUi(this);
    

    QTimer *t=new QTimer(this);
    t->setInterval(1000);
    t->setObjectName("Clock_timer");
    
    connect(t,SIGNAL(timeout()),this,SLOT(timeOut()));
    connect(m_ui->About,SIGNAL(clicked()),this,SLOT(about()));
    connect(m_ui->Trend,SIGNAL(clicked()),this,SLOT(trRun()));
    connect(m_ui->bnParm,SIGNAL(clicked()),this,SLOT(setupParm()));

    connect (m_ui->Exit,SIGNAL(clicked()),this,SLOT(slotExit()));

    connect(m_ui->bn_showAlert,SIGNAL(clicked()),this,SLOT(showAlert()));

    connect(m_ui->TrCtrl,SIGNAL(clicked()),this,SLOT(slotTrCtrl()));
    // відобразити мнемосхему
    //m_ui->scrollArea->setWidget(new Mnemo(src,this));
    m_ui->sW->addWidget(new Mnemo(src,this));

    // відображення свіжих алертів
    connect(&src,SIGNAL(Alert(QString)),this,SLOT(slotAlert(QString)));

     // відображення звітів
    connect(m_ui->bnReport,SIGNAL(clicked()),this,SLOT(showReport()));
    
    t->start();


}

mMainForm::~mMainForm()
{
    delete m_ui;
}


void mMainForm::timeOut()
{
    QDateTime tm;
    

    tm=QDateTime::currentDateTime();
    
    m_ui->lineEDate->setText(tm.toString("dd:MM:yyyy"));
    m_ui->lineETime->setText(tm.toString("hh:mm:ss"));
    
}

void mMainForm::about()
{
    aboutDlg *ab=new aboutDlg(this);
    ab->exec();
    delete ab;
}


void mMainForm::trRun()
{

    int nHeight=4000;

//    qDebug() << sender()->objectName();
    RHistorySelect *trd = new RHistorySelect(src,&tp,this);

    if(trd->exec()==QDialog::Accepted)
    {
	pTrw = new TrendWindow(NULL,&tp,nHeight);
        //pTrw->setAttribute( Qt::WA_DeleteOnClose);

        connect(pTrw,SIGNAL(finished()),this,SLOT(showMe()));

	((QStackedWidget*)parentWidget())->addWidget(pTrw);       // показати вікно графіків
        ((QStackedWidget*)parentWidget())->setCurrentWidget(pTrw);
    }
    delete trd;
    //qDebug() << "End Init";
}

void mMainForm::showMe()
{
    ((QStackedWidget*)parentWidget())->setCurrentWidget(this) ;
    ((QStackedWidget*)parentWidget())->removeWidget(pTrw);
    delete pTrw;
}

void mMainForm::setupParm()
{
    SParamDialog d(*src[3],this);
    d.exec();
}

void mMainForm::slotExit()
{
    if(QMessageBox::warning(this,tr("Попередження"),tr("Роботу програми буде завершено\nПродовжити?"),
	QMessageBox::Yes|QMessageBox::No,QMessageBox::No) == QMessageBox::Yes)
    {
	qobject_cast<QWidget*>(parent())->close() ; // тоді закритися 
    }
}

void mMainForm::slotAlert(QString v)
{
    QString a=QString("%1 | %2").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss")).arg(v);
    m_ui->AlertBrowser->clear();
    m_ui->AlertBrowser->insertPlainText( a );


    alertList.push(a);          // зберегти поточний
    for(;alertList.size()>100;) // якщо більше 100
    {
        alertList.pop_front();  // видалити зайві
    }
}

void mMainForm::showAlert()
{
    AlertViewDialog d(alertList,this);
    d.exec();
}

void mMainForm::slotTrCtrl()
{
    DlgPtsCtrl p(*src[3],this);
    p.exec();
}

void mMainForm::showReport() // показати сторінку звітів
{

    if(m_ui->sW->currentIndex()==1) // якщо кнопка повернення схована тоді
    {
        m_ui->sW->setCurrentIndex(0);
        m_ui->sW->removeWidget(rep);
        delete rep;
    }
    else
    {
        // створити та відобразити сторінку звітів... тможе треба б додати менеджер розміщення
        rep=new  Report(this);
        m_ui->sW->addWidget(rep);
        m_ui->sW->setCurrentWidget(rep);
    }
}
