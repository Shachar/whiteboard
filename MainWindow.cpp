#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QClipboard>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::performPaste() {
    QPixmap pixmap = qGuiApp->clipboard()->pixmap();

    if( !pixmap.isNull() ) {
        ui->board->newBackground( std::move(pixmap) );
    }
}

void MainWindow::resetBoard() {
    ui->board->clearBoard();

}
