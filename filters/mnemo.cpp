
#include "mnemo.h"
#include "ui_mnemo.h"


Mnemo::Mnemo(QWidget *p) : QLabel(p), m_ui(new Ui::mnemo)
{
    m_ui->setupUi(this);

}

Mnemo::~Mnemo()
{
    delete m_ui;
}

