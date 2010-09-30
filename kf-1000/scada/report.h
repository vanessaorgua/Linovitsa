#ifndef REPORT_H
#define REPORT_H

#include <QtGui/QWidget>
#include <QSqlQueryModel>

//class QSqlQueryModel;

namespace Ui {
    class Report;
}

class Report : public QWidget {
    Q_OBJECT
public:
    Report(QWidget *parent = 0);
    ~Report();
public slots:
       void slotChangeCfN(int);
       void slotCallCalendar();
       void slotChangeZm(int);
       void slotChangeDate();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Report *m_ui;
    //QSqlQueryModel *mRepModel,*mRepAgrModel;
    QSqlQueryModel mRepModel,mRepAgrModel;

};

#endif // REPORT_H
