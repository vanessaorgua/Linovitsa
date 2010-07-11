#ifndef KFUPLD_H
#define KFUPLD_H

#include <QLabel>

namespace Ui {
    class KfUpld;
}

class KfUpld : public QLabel {
    Q_OBJECT
public:
    KfUpld(QWidget *parent = 0);
    ~KfUpld();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::KfUpld *ui;
};

#endif // KFUPLD_H
