#include "SecondaryWindow.h"
#include "ui_SecondaryWindow.h"

SecondaryWindow::SecondaryWindow(QWidget *parent, WhiteBoardWidget *board) : QMainWindow(parent), _ui(new Ui::SecondaryWindow)
{
    _ui->setupUi(this);

    _ui->slaveBoard->setMaster(board);
}
