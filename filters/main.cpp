#include <QApplication>
#include <QTranslator>
#include <QtSql>

#include <IoNetClient.h>

#include "mainform.h"

#ifdef WIN32
#include <QtPlugin>
Q_IMPORT_PLUGIN(qsvg)
Q_IMPORT_PLUGIN(qsqlmysql)
#endif

int main(int argc,char **argv)
{
    QApplication app(argc,argv);

    QCoreApplication::setOrganizationName("Rabitsa");
    QCoreApplication::setApplicationName("filters");
    QApplication::setStyle(new QPlastiqueStyle);

    QSettings s;
    
    IoNetClient net( s.value("/ioserv/hostname","localhost").toString());
    net.setCmode(
#ifdef WIN32
                false
#else
                s.value("/ioserv/cmode",true).toBool()
#endif
    );
    QStackedWidget main;
    QColor cl;
    QPalette pal;
    cl.setRgb(90,132,201);
    pal.setColor(QPalette::Background,cl);
    main.setPalette(pal);

    QTranslator translator;
    translator.load("filters_"+QLocale::system().name(),":/translate");
    app.installTranslator(&translator);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    
    mMainForm *mainForm = new mMainForm(net);
    
    main.addWidget(mainForm);
    main.setCurrentWidget(mainForm);


    if(QApplication::desktop()->size().width()>1366 && QApplication::desktop()->size().height()> 768)
    {
        main.resize(1366,768);
        main.show();
    }
    else
    {
        main.showFullScreen();
    }

    return app.exec();
}


