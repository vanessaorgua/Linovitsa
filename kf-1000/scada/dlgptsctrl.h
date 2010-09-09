#ifndef DLGPTSCTRL_H
#define DLGPTSCTRL_H

#include <QDialog>

namespace Ui {
    class DlgPtsCtrl;
}

class IoDev;

class DlgPtsCtrl : public QDialog {
    Q_OBJECT
public:
    DlgPtsCtrl(IoDev &source, QWidget *parent = 0);
    ~DlgPtsCtrl();
public slots:
    void slotSetBool(int); // приймає сигнали від  комбобоксів
    void slotSetInt(int);
    void updateData();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DlgPtsCtrl *ui;
    IoDev &src;
};

#endif // DLGPTSCTRL_H
