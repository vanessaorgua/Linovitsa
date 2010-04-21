#ifndef __MNEMO_H__
#define __MNEMO_H__

#include <QLabel>

namespace Ui {
    class mnemo;
}

class Mnemo: public QLabel
{
    Q_OBJECT
public:
    Mnemo(QWidget *p=NULL);
    ~Mnemo();

private:
    Ui::mnemo *m_ui;

};

#endif