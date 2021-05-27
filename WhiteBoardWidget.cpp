#include "WhiteBoardWidget.h"

#include <QPainter>
#include <QResizeEvent>

WhiteBoardWidget::WhiteBoardWidget(QWidget *parent) : QWidget(parent)
{
}

void WhiteBoardWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap( event->rect(), underlyingImage, event->rect() );
}

void WhiteBoardWidget::resizeEvent(QResizeEvent *event) {
    underlyingImage = QPixmap( event->size() );
    underlyingImage.fill( Qt::white );
}

void WhiteBoardWidget::mousePressEvent(QMouseEvent *event) {
    lastPoint = event->pos();
}

void WhiteBoardWidget::mouseMoveEvent(QMouseEvent *event) {
    QPainter painter( &underlyingImage );
    painter.drawLine( lastPoint, event->pos() );
    lastPoint = event->pos();
    update();
}

// Slots
void WhiteBoardWidget::clearBoard() {
    underlyingImage.fill( Qt::white );
    update();
}
