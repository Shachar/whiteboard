#include "WhiteBoardWidget.h"

#include <QPainter>
#include <QResizeEvent>

WhiteBoardWidget::WhiteBoardWidget(QWidget *parent) : QWidget(parent)
{
}

void WhiteBoardWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap( event->rect(), background, event->rect() );
    painter.drawPixmap( event->rect(), underlyingImage, event->rect() );
}

void WhiteBoardWidget::resizeEvent(QResizeEvent *event) {
    background = QPixmap( event->size() );
    background.fill( Qt::white );
    underlyingImage = QPixmap( event->size() );
    underlyingImage.fill( QColor(255, 255, 255, 0) );
}

void WhiteBoardWidget::mousePressEvent(QMouseEvent *event) {
    lastPoint = event->pos();
}

void WhiteBoardWidget::mouseMoveEvent(QMouseEvent *event) {
    QPainter painter( &underlyingImage );
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen = painter.pen();
    pen.setWidthF(2);
    painter.setPen(pen);
    painter.drawLine( lastPoint, event->pos() );
    lastPoint = event->pos();
    update();
}

// Slots
void WhiteBoardWidget::clearBoard() {
    underlyingImage.fill( Qt::white );
    update();
}
