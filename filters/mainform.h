#include <QtGui>

#include "ui_mainform.h"

//#include "../../lib/trend.h"

class RIoNetClient;

class mMainForm: public QWidget // ,public Ui::Form
{
    Q_OBJECT
public:
	mMainForm(RIoNetClient *source=NULL,QWidget *p=NULL);
	~mMainForm();
	
public slots:
    void timeOut();
    void about();
    void trRun();
    void showMe();            
    void setupParm();
    
    void slotExit();
    
private:
    
    RIoNetClient *src; // вказівник на джерело даних

    //TrendWindow *pTrw; // вказівник на вікно трендів
    //struct trendinfo tp;
    Ui::Form *m_ui;
};

