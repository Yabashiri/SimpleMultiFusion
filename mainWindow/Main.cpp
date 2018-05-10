#include <QApplication>
#include <QtGui>
#include <QDir>
#include <QWidget>
#include <QBoxLayout>
#include <QTranslator>
#include <QStyleFactory>
#include <QSplashScreen>
#include "MainWindow.h"

// ----------------------------------------------------------------------

class I : public QThread
{
public:
static void sleep(unsigned long secs) {
    QThread::sleep(secs);
    }
};
int main( int argC, char** argV )
{
    QApplication app( argC, argV );

    QPixmap pixmap(":/main/images/splash.png"); //Insert your splash page image here
    QSplashScreen splash(pixmap);
    splash.setPixmap(pixmap);
    splash.setEnabled(false); //Prevent user from closing the splash
    splash.show();

    //splash.showMessage(QObject::tr("Initiating your program now..."),
                   // Qt::AlignLeft | Qt::AlignTop, Qt::black);  //This line represents the alignment of text, color and position

    app.processEvents();

	QTranslator *translator = 0;

    QStringList files = QDir( "./lang" ).entryList( QDir::Files );
    files.sort();
    int countFiles = files.size();
    app.setWindowIcon(QIcon(":/main/images/splash.png"));
   // QSplashScreen splash(QPixmap(":/main/images/cut.png"));

  //  splash.show();



    for( int i = 0; i < countFiles; i++ )
    {
		translator = new QTranslator( &app );
		QString fileName = "./lang/" + files[i];

		if( !translator->load( fileName ) )
		{
			delete translator;
			continue;
		}

        app.installTranslator( translator );
    }
    app.addLibraryPath( "./plugins" );




    QApplication::setStyle( QStyleFactory::create("Skulpture") );
    QFile file( "./style/style.qss" );
    file.open( QFile::ReadOnly );
    QString styleSheet = QLatin1String( file.readAll() );
    app.setStyleSheet( styleSheet );
    mainWindow main;
   // QTimer::singleShot(2500, splash, SLOT(close()));
    //    QTimer::singleShot(2600, &main, SLOT(showMaximized()));
   // I::sleep(5); // splash is shown for 5 seconds

    I::sleep(5);

    main.showMaximized();
    splash.finish(&main);

    splash.raise();

    return app.exec();
}
