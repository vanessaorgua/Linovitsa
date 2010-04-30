#ifndef FORM_H
#define FORM_H

#include <QtGui/QWidget>

namespace Ui
{
    class Form;
}

class IoDev;

class Form : public QWidget
{
    Q_OBJECT
public:
    Form(IoDev &s, QWidget *parent = 0);
    ~Form();
private slots:
    void sendData(int);
    void sendState();
    void updateData();

private:
    IoDev &src;
    Ui::Form *m_ui;
    qint16 lState;
};

#endif // FORM_H
