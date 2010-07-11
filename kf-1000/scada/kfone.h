#ifndef KFONE_H
#define KFONE_H

#include <QLabel>

namespace Ui {
    class KfOne;
}

class KfOne : public QLabel {
    Q_OBJECT
public:
    KfOne(QWidget *parent = 0);
    ~KfOne();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::KfOne *ui;
};

#endif // KFONE_H
