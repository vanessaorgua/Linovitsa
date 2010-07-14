#ifndef KFUPLD_H
#define KFUPLD_H

#include <QLabel>
#include <QVector>

class IoDev;
class QCheckBox;

namespace Ui {
    class KfUpld;
}

class KfUpld : public QLabel {
    Q_OBJECT
public:
    KfUpld(QWidget *parent = 0);
    ~KfUpld();

public slots:
    void updateData(IoDev &src);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::KfUpld *ui;
    QVector<QCheckBox*> cb;

};

#endif // KFUPLD_H
