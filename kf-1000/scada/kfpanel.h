#ifndef KFPANEL_H
#define KFPANEL_H

#include <QDialog>
#include <QVector>

class IoNetClient;
class QLineEdit;

namespace Ui {
    class KfPanel;
}

class KfOne;
class KfUpld;

class KfPanel : public QDialog {
    Q_OBJECT
public:
    KfPanel(IoNetClient &source, int KfNum, QWidget *parent = 0);
    ~KfPanel();
public slots:
    void updateDataRaw();
    void updateDataScaled();
    void slotAlarm();
    void slotStart();
    void slotMenu();
    void slotParam();
    void slotTrend();
    void slotUpload();


protected:
    void changeEvent(QEvent *e);

private:
    Ui::KfPanel *ui;

    KfOne *kfo;
    KfUpld *kfu;

    IoNetClient &src;
    int Nf;
    QStringList tState; // текст

};

#endif // KFPANEL_H
