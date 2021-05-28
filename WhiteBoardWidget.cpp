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
    internalClearBackground(event->size());
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
    setMinimumSize(QSize(0,0));
    setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));

    internalClearBackground(size());
}

void WhiteBoardWidget::clearDrawing() {
    underlyingImage.fill( QColor(255, 255, 255, 0) );
    update();
}

void WhiteBoardWidget::newBackground(QPixmap pixmap) {
    setMinimumSize(pixmap.size());
    setMaximumSize(pixmap.size());
    background = std::move(pixmap);
    clearDrawing();
    resize(pixmap.size());
}

void WhiteBoardWidget::internalClearBackground(QSize size) {
    background = QPixmap( size );
    background.fill( Qt::white );
    underlyingImage = QPixmap( size );
    clearDrawing();
}
