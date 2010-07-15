#ifndef DLGKFMENU_H
#define DLGKFMENU_H

#include <QDialog>

namespace Ui {
    class dlgKfMenu;
}

class QString;

class dlgKfMenu : public QDialog {
    Q_OBJECT
public:
    dlgKfMenu(QString text,QWidget *parent = 0);
    ~dlgKfMenu();
    inline qint16 getRes() {return res;}

public slots:
    void slotSetVal();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::dlgKfMenu *ui;
    qint16 res;

};

#endif // DLGKFMENU_H
