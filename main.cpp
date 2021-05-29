#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("Shachar Shemesh");
    app.setOrganizationDomain("shemesh.biz");
    app.setApplicationName("Whiteboard");

    MainWindow win;
    win.show();

    return app.exec();
}
