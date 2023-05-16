#include "mainwindow.h"
#include <QApplication>
#include <QFile>

#include <signal.h>


static void catch_sig(int signum)
{
   (void)signum;
   SphinxObject::global_done = 1;
}
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	a.setApplicationName("Papagayo");
	a.setOrganizationName("Lost Marble");
	a.setOrganizationDomain("lostmarble.com");

	QCoreApplication::setAttribute(Qt::AA_DontShowIconsInMenus, true);

    signal(SIGINT,catch_sig);

	MainWindow w;
	w.show();

	const QStringList arguments = a.arguments();
	if (arguments.size() > 1 && QFile::exists(arguments.last()))
	{
		w.OpenFile(arguments.last());
	}



	return a.exec();
}
