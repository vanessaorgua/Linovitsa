#include "report.h"
#include "ui_report.h"
#include <QVBoxLayout>

#include "datetimedialog.h"

#include <QSqlQueryModel>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

Report::Report(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::Report)
{
    m_ui->setupUi(this);

    connect(m_ui->cf_no,SIGNAL(currentIndexChanged(int)),this,SLOT(slotChangeCfN(int)));
    connect(m_ui->bnStart,SIGNAL(clicked()),this,SLOT(slotCallCalendar()));
    connect(m_ui->bnStop,SIGNAL(clicked()),this,SLOT(slotCallCalendar()));
    connect(m_ui->cbSelZm,SIGNAL(currentIndexChanged(int)),this,SLOT(slotChangeZm(int)));
    connect(m_ui->sbStart,SIGNAL(dateTimeChanged(QDateTime)),this,SLOT(slotChangeDate()));
    connect(m_ui->sbStop,SIGNAL(dateTimeChanged(QDateTime)),this,SLOT(slotChangeDate()));


    QDateTime cdt=QDateTime::currentDateTime(); // визначити поточний час
    if(cdt.time().hour()>=8 && cdt.time().hour()<20) // якщо зараз денна зміна
    {
        m_ui->sbStart->setDate(cdt.date());
        m_ui->sbStart->setTime(QTime(8,0,0,0));
    }
    else
    {
        m_ui->sbStart->setDate(QDate(cdt.date().year(),cdt.date().month(),cdt.date().day()).addDays(cdt.time().hour()>19?0:-1));
        m_ui->sbStart->setTime(QTime(20,0,0,0));
    }
    m_ui->sbStop->setDateTime(m_ui->sbStart->dateTime().addSecs(12*3600));

    QSqlDatabase dbs=QSqlDatabase::addDatabase("QMYSQL","report");

    QSettings set;
    qDebug() << "/db/host" << set.value("/db/hostname","localhost").toString();

    dbs.setHostName(set.value("/db/hostname","localhost").toString());
    dbs.setDatabaseName(set.value("/db/dbname","centrif").toString());
    dbs.setUserName(set.value("/db/username","scada").toString());
    dbs.setPassword(set.value("/db/passwd","").toString());
    if(!dbs.open()) // спробувати відкрити
    {
        qDebug() << dbs.lastError().databaseText();
    }


    //mRepModel = new QSqlQueryModel();
    //mRepAgrModel = new QSqlQueryModel();

    mRepModel.setQuery(QString("SELECT Dt,Nf,Nc,Tall,Qsusp,Tfilt,Qvs,Tvs,Qvw,tvw,Tf FROM report WHERE Dt BETWEEN \'%1\' AND \'%2\'")
                        .arg(m_ui->sbStart->dateTime().toString("yyyy/MM/dd hh:mm:ss"))
                        .arg(m_ui->sbStop->dateTime().toString("yyyy/MM/dd hh:mm:ss")),dbs);


    mRepAgrModel.setQuery(QString("SELECT \"%0\",\" \",count(*),avg(Tall),sum(Qsusp),avg(Tfilt),sum(Qvs),avg(Tvs),sum(Qvw),avg(tvw),avg(Tf) FROM report WHERE Dt BETWEEN \'%1\' AND \'%2\'")
                           .arg(tr("Загалом"))
                        .arg(m_ui->sbStart->dateTime().toString("yyyy/MM/dd hh:mm:ss"))
                        .arg(m_ui->sbStop->dateTime().toString("yyyy/MM/dd hh:mm:ss")),dbs);

    if(mRepModel.lastError().isValid())
    {
        qDebug() << mRepModel.lastError();
    }

    mRepAgrModel.setQuery(QString("SELECT \"%0\",\" \",count(*),avg(Tall),sum(Qsusp),avg(Tfilt),sum(Qvs),avg(Tvs),sum(Qvw),avg(tvw),avg(Tf) FROM report WHERE Dt BETWEEN \'%1\' AND \'%2\'")
                           .arg(tr("Загалом"))
                        .arg(m_ui->sbStart->dateTime().toString("yyyy/MM/dd hh:mm:ss"))
                        .arg(m_ui->sbStop->dateTime().toString("yyyy/MM/dd hh:mm:ss")),dbs);

    if(mRepAgrModel.lastError().isValid())
    {
        qDebug() << mRepAgrModel.query().lastQuery();
        qDebug() << mRepAgrModel.lastError();
    }


    mRepModel.setHeaderData(0,Qt::Horizontal,tr("Час та дата"));
    mRepModel.setHeaderData(1,Qt::Horizontal,tr("№ фільтра"));
    mRepModel.setHeaderData(2,Qt::Horizontal,tr("№ циклу"));
    mRepModel.setHeaderData(3,Qt::Horizontal,tr("Час циклу (хв)"));
    mRepModel.setHeaderData(4,Qt::Horizontal,tr("Q суспензії (м3)"));
    mRepModel.setHeaderData(5,Qt::Horizontal,tr("Час фільт.(хв)"));
    mRepModel.setHeaderData(6,Qt::Horizontal,tr("Q висол. (м3)"));
    mRepModel.setHeaderData(7,Qt::Horizontal,tr("Час висол.(хв)"));
    mRepModel.setHeaderData(8,Qt::Horizontal,tr("Q пром. (м3)"));
    mRepModel.setHeaderData(9,Qt::Horizontal,tr("Час пром.(хв)"));
    mRepModel.setHeaderData(10,Qt::Horizontal,tr("Час сушіння (хв)"));


    m_ui->twReport->setModel(&mRepModel);

    m_ui->twReport->setColumnWidth(0, 160 );
    m_ui->twReport->setColumnWidth(1 , 105 );
    m_ui->twReport->setColumnWidth(2 , 105 );
    m_ui->twReport->setColumnWidth(3 , 105 );
    m_ui->twReport->setColumnWidth(4 , 105 );
    m_ui->twReport->setColumnWidth(5 , 105 );
    m_ui->twReport->setColumnWidth(6 , 105 );
    m_ui->twReport->setColumnWidth(7 , 105 );
    m_ui->twReport->setColumnWidth(8 , 105 );
    m_ui->twReport->setColumnWidth(9 , 105 );
    m_ui->twReport->setColumnWidth(10 , 105 );

    m_ui->twReportAgr->setModel(&mRepAgrModel);


    m_ui->twReportAgr->setColumnWidth(0, 166 );
    m_ui->twReportAgr->setColumnWidth(1 , 105 );
    m_ui->twReportAgr->setColumnWidth(2 , 105 );
    m_ui->twReportAgr->setColumnWidth(3 , 105 );
    m_ui->twReportAgr->setColumnWidth(4 , 105 );
    m_ui->twReportAgr->setColumnWidth(5 , 105 );
    m_ui->twReportAgr->setColumnWidth(6 , 105 );
    m_ui->twReportAgr->setColumnWidth(7 , 105 );
    m_ui->twReportAgr->setColumnWidth(8 , 105 );
    m_ui->twReportAgr->setColumnWidth(9 , 105 );
    m_ui->twReportAgr->setColumnWidth(10 , 105 );

}

Report::~Report()
{
    //delete mRepModel;
    //delete mRepAgrModel;

    {
        QSqlDatabase dbs=QSqlDatabase::database("report");
        dbs.close();
    }
    QSqlDatabase::removeDatabase("report");
    delete m_ui;
}

void Report::changeEvent(QEvent *e)
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

void  Report::slotChangeCfN(int v)
{
    QSqlDatabase dbs=QSqlDatabase::database("report",true); //знайти з’єдання logging
    if(dbs.isOpen())
    {
        if(v==0)
        {
            mRepModel.setQuery(QString("SELECT Dt,Nf,Nc,Tall,Qsusp,Tfilt,Qvs,Tvs,Qvw,tvw,Tf FROM report WHERE Dt BETWEEN \'%1\' AND \'%2\'")
                            .arg(m_ui->sbStart->dateTime().toString("yyyy/MM/dd hh:mm:ss"))
                            .arg(m_ui->sbStop->dateTime().toString("yyyy/MM/dd hh:mm:ss")),dbs);

            mRepAgrModel.setQuery(QString("SELECT \"%0\",\" \",count(*),avg(Tall),sum(Qsusp),avg(Tfilt),sum(Qvs),avg(Tvs),sum(Qvw),avg(tvw),avg(Tf) FROM report WHERE Dt BETWEEN \'%1\' AND \'%2\'")
                                   .arg(tr("Загалом"))
                                .arg(m_ui->sbStart->dateTime().toString("yyyy/MM/dd hh:mm:ss"))
                                .arg(m_ui->sbStop->dateTime().toString("yyyy/MM/dd hh:mm:ss")),dbs);

        }
        else
        {
            mRepModel.setQuery(QString("SELECT Dt,Nf,Nc,Tall,Qsusp,Tfilt,Qvs,Tvs,Qvw,tvw,Tf FROM report WHERE Dt BETWEEN \'%1\' AND \'%2\' and Nf=\'%3\'")
                        .arg(m_ui->sbStart->dateTime().toString("yyyy/MM/dd hh:mm:ss"))
                        .arg(m_ui->sbStop->dateTime().toString("yyyy/MM/dd hh:mm:ss"))
                        .arg(v),dbs);

            mRepAgrModel.setQuery(QString("SELECT \"%0\",\" \",count(*),avg(Tall),sum(Qsusp),avg(Tfilt),sum(Qvs),avg(Tvs),sum(Qvw),avg(tvw),avg(Tf) FROM report WHERE Dt BETWEEN \'%1\' AND \'%2\'  and Nf=\'%3\'")
                                   .arg(tr("Загалом"))
                                .arg(m_ui->sbStart->dateTime().toString("yyyy/MM/dd hh:mm:ss"))
                                .arg(m_ui->sbStop->dateTime().toString("yyyy/MM/dd hh:mm:ss"))
                                .arg(v),dbs);

        }
    }
    else
    {
        qDebug() << dbs.lastError().databaseText();
    }
    //qDebug() << mRepModel->query().lastQuery();
}

void Report::slotCallCalendar()
{
    QDateTime v= (sender()->objectName()=="bnStart"? m_ui->sbStart: m_ui->sbStop)->dateTime();


    myDateTimeDialog dt(this,v);
    if(dt.exec()==QDialog::Accepted)
    {
        v.setDate(dt.date->selectedDate());
        v.setTime(QTime(dt.hour->value(),dt.minute->value(),dt.second->value()));
    }
    (sender()->objectName()=="bnStart"? m_ui->sbStart: m_ui->sbStop)->setDateTime(v);


    m_ui->cbSelZm->blockSignals(true);
    m_ui->cbSelZm->setCurrentIndex(3);
    m_ui->cbSelZm->blockSignals(false);

    slotChangeCfN(m_ui->cf_no->currentIndex());

}

void Report::slotChangeZm(int v)
{
    // визначили поточну зміну
    QDateTime cdt=QDateTime::currentDateTime(); // визначити поточний час
    if(cdt.time().hour()>=8 && cdt.time().hour()<20) // якщо зараз денна зміна
    {
        m_ui->sbStart->setDate(cdt.date());
        m_ui->sbStart->setTime(QTime(8,0,0,0));
    }
    else
    {
        m_ui->sbStart->setDate(QDate(cdt.date().year(),cdt.date().month(),cdt.date().day()).addDays(cdt.time().hour()>19?0:-1));
        m_ui->sbStart->setTime(QTime(20,0,0,0));
    }
    m_ui->sbStop->setDateTime(m_ui->sbStart->dateTime().addSecs(12*3600));

    switch(v)
    {
        default:
        case 0:
            break;
        case 1:
            m_ui->sbStart->setDateTime(m_ui->sbStart->dateTime().addSecs(-12*3600));
            m_ui->sbStop->setDateTime(m_ui->sbStop->dateTime().addSecs(-12*3600));
            break;
        case 2:
            m_ui->sbStart->setDateTime(m_ui->sbStart->dateTime().addSecs(-12*3600));
            break;
    }


    slotChangeCfN(m_ui->cf_no->currentIndex());
}

void Report::slotChangeDate()
{
    slotChangeCfN(m_ui->cf_no->currentIndex());
}
