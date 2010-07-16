#ifndef DLGKFPARAM_H
#define DLGKFPARAM_H

#include <QDialog>

namespace Ui {
    class dlgKfParam;
}
class IoDev;

class dlgKfParam : public QDialog {
    Q_OBJECT
public:
    dlgKfParam(IoDev &source, QWidget *parent = 0);
    ~dlgKfParam();

protected:
    void changeEvent(QEvent *e);
public slots:
    void slotAccept();

private:
    Ui::dlgKfParam *ui;
    IoDev &src;
};

#endif // DLGKFPARAM_H
