#include "form.h"
#include "ui_form.h"
#include "../iodev.h"
#include <QDebug>

Form::Form(IoDev &s,QWidget *parent) :
    QWidget(parent),
    src(s)
{
    setupUi(this);
}

Form::~Form()
{

}

void Form::sendData()
{

}


void Form::updateData()
{

}
