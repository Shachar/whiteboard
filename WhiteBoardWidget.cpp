#include "WhiteBoardWidget.h"

#include <QAction>
#include <QPainter>
#include <QResizeEvent>
#include <QtDebug>

static constexpr QPointF InvalidPoint = QPointF(-1, -1);
static constexpr qreal HighlightAlpha = 0.05;
static constexpr int HighlightSizeFactor = 10;

WhiteBoardWidget::WhiteBoardWidget(QWidget *parent) : QWidget(parent), lastPoint(InvalidPoint)
{
}

void WhiteBoardWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    if( backgroundImage.isNull() )
        backgroundType.draw(painter, event->rect());
    else
        painter.drawPixmap( event->rect(), backgroundImage, event->rect() );

    painter.drawPixmap( event->rect(), underlyingImage, event->rect() );
}

void WhiteBoardWidget::resizeEvent(QResizeEvent *event) {
    internalClearBackground(event->size());
}

void WhiteBoardWidget::mousePressEvent(QMouseEvent *event) {
    lastPoint = event->pos();
}

void WhiteBoardWidget::mouseMoveEvent(QMouseEvent *event) {
    DrawType type;

    auto buttons = event->buttons();

    if( (buttons & Qt::MiddleButton) != 0 ) {
        type = DrawType::Highlighter;
    } else if( (buttons & (Qt::LeftButton | Qt::RightButton)) == (Qt::LeftButton | Qt::RightButton) ) {
        type = DrawType::Highlighter;
    } else if( (buttons & Qt::RightButton) != 0 ) {
        type = DrawType::Eraser;
    } else if( (buttons & Qt::LeftButton) != 0 ) {
        type = DrawType::Pen;
    } else {
        qDebug()<<"Mouse move with no relevant button pressed";
        return;
    }

    draw(event->pos(), 1, type);
}

void WhiteBoardWidget::tabletEvent(QTabletEvent *event) {
    event->accept();

    if( (event->buttons() & Qt::LeftButton)==0 ) {
        lastPoint=InvalidPoint;
        return;
    }

    if( lastPoint==InvalidPoint ) {
        lastPoint=event->posF();
        return;
    }

    DrawType type;

    if( event->pointerType() == QTabletEvent::PointerType::Eraser ) {
        type = DrawType::Highlighter;
    } else if( (event->buttons() & Qt::MiddleButton) != 0 ) {
        type = DrawType::Eraser;
    } else {
        type = DrawType::Pen;
    }

    draw(event->posF(), event->pressure(), type);
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
    backgroundImage = std::move(pixmap);
    clearDrawing();
    resize(pixmap.size());
}

void WhiteBoardWidget::newBackground(BoardBackground background) {
    backgroundType = background;
    update();
}

void WhiteBoardWidget::internalClearBackground(QSize size) {
    backgroundImage = QPixmap();
    underlyingImage = QPixmap( size );
    clearDrawing();
}

void WhiteBoardWidget::draw(QPointF pos, qreal pressure, DrawType drawType) {
    QPainter painter( &underlyingImage );
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen = painter.pen();

    switch(drawType) {
    case DrawType::Pen:
        pen.setColor(penColor);
        pen.setWidthF(penWidth * pressure);
        break;
    case DrawType::Highlighter:
        {
            QColor color = penColor;
            color.setAlphaF( HighlightAlpha );
            pen.setColor( color );
            pen.setWidthF(penWidth * pressure * HighlightSizeFactor);
        }
        break;
    case DrawType::Eraser:
        //pen.setColor( QColor(255, 255, 255, 0) );
        pen.setWidthF(penWidth * pressure * HighlightSizeFactor);
        painter.setCompositionMode( QPainter::CompositionMode_Clear );
        break;
    }

    painter.setPen(pen);
    painter.drawLine( lastPoint, pos );
    lastPoint = pos;
    update();
}
