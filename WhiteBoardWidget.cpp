#include "WhiteBoardWidget.h"

#include <QAction>
#include <QPainter>
#include <QResizeEvent>
#include <QtDebug>

static constexpr QPointF InvalidPoint = QPointF(-1, -1);

WhiteBoardWidget::WhiteBoardWidget(QWidget *parent) : QWidget(parent), lastPoint(InvalidPoint)
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
    draw(event->pos(), 1);
}

void WhiteBoardWidget::tabletEvent(QTabletEvent *event) {
    event->accept();

    if( (event->buttons() & Qt::LeftButton)==0 ) {
        lastPoint=InvalidPoint;
    } else {
        if( lastPoint==InvalidPoint )
            lastPoint=event->posF();
        else
            draw(event->posF(), event->pressure());
    }
}

QColor WhiteBoardWidget::getPenColor() const {
    return penColor;
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

void WhiteBoardWidget::setPenColor(QColor color) {
    penColor = color;
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

void WhiteBoardWidget::draw(QPointF pos, qreal pressure) {
    QPainter painter( &underlyingImage );
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen = painter.pen();
    pen.setColor( penColor );
    pen.setWidthF(penWidth * pressure);
    painter.setPen(pen);
    painter.drawLine( lastPoint, pos );
    lastPoint = pos;
    update();
}
