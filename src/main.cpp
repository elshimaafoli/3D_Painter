#include<QtWidgets/QApplication>
#include<MainWindow.h>
int main(int argc, char* argv[]) {
	QApplication app(argc,argv);
	/*QWidget* wid = new QWidget();
	wid->show();*/
	MainWindow mainWindow;
	mainWindow.show();
	return app.exec();
}