#ifndef ONEFILTER_H
#define ONEFILTER_H

#include <QtGui/QDialog>

namespace Ui {
    class OneFilter;
}

class IoDev;

class OneFilter : public QDialog {
    Q_OBJECT
public:
    OneFilter(IoDev &src,QWidget *parent = 0);
    ~OneFilter();

    void setFn(int i);

public slots:
    void updateData();
    void setAm(int v);
    void setValve(int v);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::OneFilter *m_ui;
    int n;

    IoDev &s;

};

#endif // ONEFILTER_H
