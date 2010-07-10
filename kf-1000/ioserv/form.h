#ifndef FORM_H
#define FORM_H

#include <QtGui/QWidget>

class  IoDev;

namespace Ui {
    class Form;
}

class Form : public QWidget {
    Q_OBJECT
public:
    Form(IoDev &source, QWidget *parent = 0);
    ~Form();

protected:
    void changeEvent(QEvent *e);
public slots:
    void updateData();
    void slotSend(bool);

private:
    Ui::Form *m_ui;
    IoDev &src;
};

#endif // FORM_H
