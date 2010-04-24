#ifndef __MNEMO_H__
#define __MNEMO_H__

#include <QLabel>
#include <QVector>

class TrendChart;
class IoNetClient;
class QCheckBox;

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
    void updateTrChart(); // слот , який обновляє графіки
    void slotFilter(); // реакція на кнопку-натискання фільтра
    void slotTParam(); // кнопка завдання технологічних параметрів

private:
    Ui::mnemo *m_ui;

    TrendChart  *trc; // поточний графік параметрів
    IoNetClient &s;


    QVector<QCheckBox*> cb_all;
//    QStringList cb_tag;

};

#endif
