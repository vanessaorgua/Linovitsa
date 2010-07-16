#ifndef DLGVODACTRL_H
#define DLGVODACTRL_H

#include <QDialog>

class IoDev;

namespace Ui {
    class dlgVodaCtrl;
}

class dlgVodaCtrl : public QDialog {
    Q_OBJECT
public:
    dlgVodaCtrl(IoDev &source, QWidget *parent = 0);
    ~dlgVodaCtrl();
public slots:
    void slotSetData(bool);
    void slotSetData(int);
    void slotSetData(double);
    void slotUpdateData();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::dlgVodaCtrl *ui;
    IoDev &src;

};

#endif // DLGVODACTRL_H
