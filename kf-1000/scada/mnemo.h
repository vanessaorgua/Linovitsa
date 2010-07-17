#ifndef __MNEMO_H__
#define __MNEMO_H__

#include <QLabel>
#include <QVector>

class TrendChart;
class IoNetClient;
class QCheckBox;
class QLineEdit;
class QProgressBar;
class QStackedWidget;

class KfOne;
class KfUpld;
class TrendChart;

namespace Ui {
    class mnemo;
}

class Mnemo: public QLabel
{
    Q_OBJECT
public:
    Mnemo(IoNetClient &src, QWidget *p=NULL);
    ~Mnemo();

public slots:
    void updateDataRaw(); // слот обновляє дані на мнемосхемі
    void updateDataScaled(); // слот обновляє дані на мнемосхемі
    void slotCallReg();
    void slotCallKfOnePanel();
    void slotCallVodaCtrl();
    void updateTrend();


private:
    Ui::mnemo *m_ui;
    IoNetClient &s;

    TrendChart *trC;

    QVector<KfOne*> ko;
    QVector<KfUpld*> ku;

    QVector<QLineEdit*> le;
    QVector<QProgressBar*> pb;
    QVector<QCheckBox*> cb;
    QVector<QLineEdit*> State;
    QStringList tState; // текст
    QVector<QStackedWidget*> sw;



};

#endif
