#ifndef SPARAMDIALOG_H
#define SPARAMDIALOG_H

#include <QtGui/QDialog>

class IoDev;

namespace Ui {
    class SParamDialog;
}

class SParamDialog : public QDialog {
    Q_OBJECT
public:
    SParamDialog(IoDev &src,QWidget *parent = 0);
    ~SParamDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SParamDialog *m_ui;
    IoDev &s;
};

#endif // SPARAMDIALOG_H
