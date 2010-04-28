#ifndef FORM_H
#define FORM_H

#include <QtGui/QWidget>

#include "ui_form.h"

class IoDev;

class Form : public QWidget , public Ui::Form
{
    Q_OBJECT
public:
    Form(IoDev &s, QWidget *parent = 0);
    ~Form();
private slots:
    void sendData();
    void updateData();

private:
    IoDev &src;

};

#endif // FORM_H
