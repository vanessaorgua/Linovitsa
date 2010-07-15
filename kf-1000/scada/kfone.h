#ifndef KFONE_H
#define KFONE_H

#include <QLabel>
#include <QVector>

class IoDev;
class QCheckBox;

namespace Ui {
    class KfOne;
}

class KfOne : public QLabel {
    Q_OBJECT
public:
    KfOne(QWidget *parent = 0);
    ~KfOne();
public slots:
      void updateData(IoDev &src);
      void slotCallOne();
signals:
      void signalCallOne();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::KfOne *ui;
    QVector<QCheckBox*> cb;

};

#endif // KFONE_H
