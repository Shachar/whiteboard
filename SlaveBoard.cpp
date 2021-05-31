#include "SlaveBoard.h"

#include <QPainter>
#include <QPaintEvent>

SlaveBoard::SlaveBoard(QWidget *parent) : QWidget(parent)
{

}

void SlaveBoard::setMaster(WhiteBoardWidget *master) {
    Q_ASSERT(_master == nullptr);

    _master = master;
    connect( master, SIGNAL(imageUpdated()), this, SLOT(refreshImage()) );
    connect( master, SIGNAL(destroyed()), parent(), SLOT(close()) );
}

void SlaveBoard::paintEvent(QPaintEvent *event) {
    if( !_master )
        return;

    QPainter painter(this);
    _master->drawOnMe( painter, event->rect() );
}

// Slots
void SlaveBoard::refreshImage() {
    update();
}
