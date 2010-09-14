#include "reportwr.h"
#include "iodev.h"
#include <QtSql>

ReportWiter::ReportWiter(QVector<IoDev*> &source):
        src(source)

{
    // ініціалізувати масив станів
    lastState.fill(0,3);

    // ініціалізувати масив даних
    QVector<double> v(8);
    data.fill(v,5) ;

}


void ReportWiter::checkState()
{
    int i=sender()->objectName().right(1).toInt()-1; // визначити хто відправив сигнал
    qint16 s=src[i]->getValue16("State");
    if(lastState[i]==9 && s!=9) //якщо була регенерація а стала не регенерація - записати звіт
    {
        qDebug() << "Report writing time !!!!";
        QSqlDatabase dbs=QSqlDatabase::database("logging",true); //знайти з’єдання logging
        if(dbs.isOpen()) // якщо база відкрита
        {
            QDateTime tm=QDateTime::currentDateTime();
            QSqlQuery query(dbs);
            // сформувати і виконати запит, перевірити результат
            if(! query.exec(QString("INSERT INTO report(Dt,Nf,Nc,Qsusp,Qvs,Qvw,Tall,Tfilt,Tvs,tvw,Tf) "
                               "VALUES (\'%1\',\'%2\',\'%3\',\'%4\',\'%5\',\'%6\',\'%7\',\'%8\',\'%9\',\'%10\',\'%11\')")
                       .arg(tm.toString("yyyy/MM/dd hh:mm:ss"))
                       .arg(i+1)
                       .arg(src[i]->getValue32("Nc"))
                       .arg(data[i][0],6,'f',2)
                       .arg(data[i][1],6,'f',2)
                       .arg(data[i][2],6,'f',2)
                       .arg(data[i][3],6,'f',2)
                       .arg(data[i][4],6,'f',2)
                       .arg(data[i][5],6,'f',2)
                       .arg(data[i][6],6,'f',2)
                       .arg(data[i][7],6,'f',2)         )

                )
            { // якщо помилка
                qDebug() << query.lastError().databaseText(); //на екран
            }
            qDebug() << query.lastQuery();
            dbs.close();
        }
    }

    if(s==8) // якщо стадія регенераці зберегти дані, бо після її закінчення контролер їх обнулить.
    {
        data[i][0]=src[i]->getValueFloat("Qsusp")*0.0625;
        data[i][1]=src[i]->getValueFloat("Qvs")*0.015;
        data[i][2]=src[i]->getValueFloat("Qvw")*0.015;
        data[i][3]=(double)src[i]->getValue32("Tall")/60000.0;
        data[i][4]=(double)src[i]->getValue32("Tfilt")/60000.0;
        data[i][5]=(double)src[i]->getValue32("Tvs")/60000.0;
        data[i][6]=(double)src[i]->getValue32("Tvw")/60000.0;
        data[i][7]=(double)src[i]->getValue32("Tf")/60000.0;
    }
    lastState[i]=s; // зберегти стан для наступного виклику

}


