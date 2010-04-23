#ifndef TPARAMDIALOG_H
#define TPARAMDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class TParamDialog;
}

class IoDev;


class TParamDialog : public QDialog {
    Q_OBJECT
public:
    TParamDialog(IoDev &src, QWidget *parent = 0);
    ~TParamDialog();

public slots:
    void myAccept(); // реакція на кнопку Ок

protected:
    void changeEvent(QEvent *e);

private:
    Ui::TParamDialog *m_ui;
    IoDev &s;
};

#endif // TPARAMDIALOG_H
