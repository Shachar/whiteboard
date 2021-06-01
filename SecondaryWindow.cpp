#include "SecondaryWindow.h"

#include "ui_SecondaryWindow.h"
#include "Support.h"

SecondaryWindow::SecondaryWindow(QWidget *parent, WhiteBoardWidget *board) : QMainWindow(parent), _ui(new Ui::SecondaryWindow)
{
    _ui->setupUi(this);

    _ui->slaveBoard->setMaster(board);

    for( auto &action : _ui->menubar->actions() ) {
        adoptMenuActions(this, action);
    }
}

// Public slots
void SecondaryWindow::toggleFullScreen() {
    if( isFullScreen() ) {
        unimportantWidgetsVisibility(true);
        showNormal();
    } else {
        unimportantWidgetsVisibility(false);
        showFullScreen();
    }
}

void SecondaryWindow::exitFullScreen() {
    if( !isFullScreen() )
        return;

    toggleFullScreen();
}

// Private methods
void SecondaryWindow::unimportantWidgetsVisibility(bool show) {
    _ui->menubar->setVisible(show);
}
