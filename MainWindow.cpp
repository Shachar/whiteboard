#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QClipboard>

#include <QtDebug>

static const QColor
    brown(130, 85, 0),
    orange(244, 165, 0),
    pink(252, 0, 235);

static const std::array BuiltinColors{
        QColor(Qt::black),
        QColor(Qt::white),
        QColor(Qt::red),
        QColor(Qt::green),
        QColor(Qt::blue),
        QColor(Qt::yellow),
        orange,
        brown,
        pink
    };

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto iconSize = ui->colorToolBar->iconSize();
    for( auto &color : BuiltinColors ) {
        QPixmap pix(iconSize);
        pix.fill(color);
        QAction *action = ui->colorToolBar->addAction(QIcon( std::move(pix) ), "Color");

        action->setData( color );
        connect( action, SIGNAL(triggered()), ui->board, SLOT(setPenColor()) );
    }

    for( auto &action : ui->menuBar->actions() ) {
        adoptMenuActions(action);
    }
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

void MainWindow::toggleFullScreen() {
    qDebug()<<"Previous state "<<ui->board->windowState();

    if( isFullScreen() ) {
        unimportantWidgetsVisibility(true);
        showNormal();
    } else {
        unimportantWidgetsVisibility(false);
        showFullScreen();
    }
}

void MainWindow::unimportantWidgetsVisibility(bool show) {
    ui->menuBar->setVisible(show);
    ui->statusBar->setVisible(show);
    ui->statusBar->setVisible(show);
}

void MainWindow::adoptMenuActions(QAction *action) {
    if( action->isSeparator() ) {
        // Not adoping a separator
    } else if(action->menu()) {
        // Action menu
        for( auto &innerAction : action->menu()->actions() )
            adoptMenuActions(innerAction);
    } else {
        addAction(action);
    }
}
