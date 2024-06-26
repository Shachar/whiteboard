#include "WhiteBoardWidget.h"

#include "qt_version_compat.h"

#include <QAction>
#include <QPainter>
#include <QResizeEvent>
#include <QtDebug>

static constexpr QPointF InvalidPointF = QPointF(-1, -1);
static constexpr QPoint InvalidPoint = QPoint(-1, -1);
static constexpr qreal HighlightAlpha = 0.05;
static constexpr int HighlightSizeFactor = 10;

WhiteBoardWidget::WhiteBoardWidget(QWidget *parent) : QWidget(parent), lastPoint(InvalidPointF), scrollAnchor(InvalidPoint)
{
}

void WhiteBoardWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    drawOnMe(painter, event->rect());
}

void WhiteBoardWidget::drawOnMe(QPainter &painter, QRect regionOfInterest) {
    painter.save();

    QSize deviceSize( painter.device()->width(), painter.device()->height() );
    QRectF sourceRect(
                regionOfInterest.left()/double(deviceSize.width())*size().width(),
                regionOfInterest.top()/double(deviceSize.height())*size().height(),
                regionOfInterest.width()/double(deviceSize.width())*size().width(),
                regionOfInterest.height()/double(deviceSize.height())*size().height()
            );
    if( backgroundImage.isNull() )
        backgroundType.draw(sourceRect, painter, regionOfInterest);
    else
        painter.drawPixmap( regionOfInterest, backgroundImage, sourceRect );

    painter.drawPixmap( regionOfInterest, underlyingImage, sourceRect );

    painter.restore();
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
        qDebug()<<"Mouse move with no relevant button pressed "<<buttons;
        return;
    }

    draw(event->pos(), 1, type);
}

void WhiteBoardWidget::tabletEvent(QTabletEvent *event) {
    event->accept();

    if( (event->buttons() & (Qt::LeftButton | Qt::RightButton))!= (Qt::LeftButton | Qt::RightButton) ) {
        scrollAnchor = InvalidPoint;
    }

    if( (event->buttons() & Qt::LeftButton)==0 ) {
        lastPoint=InvalidPointF;
        return;
    }

    if( lastPoint==InvalidPointF ) {
        lastPoint=event->POSITION();
        return;
    }

    DrawType type;

    if( event->pointerType() == POINTER_TYPE::Eraser ) {
        type = DrawType::Highlighter;
    } else if( (event->buttons() & Qt::MiddleButton) != 0 ) {
        type = DrawType::Eraser;
    } else if( (event->buttons() & Qt::RightButton) != 0 ) {
        if( scrollAnchor!=InvalidPoint ) {
            scrollEvent( event->GLOBAL_POSITION() - scrollAnchor );
        }
        scrollAnchor = event->GLOBAL_POSITION();
        type = DrawType::Scroll;
    } else {
        type = DrawType::Pen;
    }

    draw(event->POSITION(), event->pressure(), type);
}

void WhiteBoardWidget::wheelEvent(QWheelEvent *event) {
    static constexpr qreal WholePoint = 240;

    penWidth -= event->angleDelta().y() / WholePoint;
    if( penWidth<=0 )
        penWidth = 0.5;

    event->accept();

    emit penSizeChanged(penWidth);
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
    drawingChanged();
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
    drawingChanged();
}

const QPixmap &WhiteBoardWidget::getForeground() const {
    return underlyingImage;
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
        pen.setColor( QColor(255, 255, 255, 0) );
        pen.setWidthF(penWidth * pressure * HighlightSizeFactor);
        painter.setCompositionMode( QPainter::CompositionMode_Source );
        break;
    case DrawType::Scroll:
        return;
    }

    painter.setPen(pen);
    painter.drawLine( lastPoint, pos );
    lastPoint = pos;
    drawingChanged();
}

void WhiteBoardWidget::drawingChanged() {
    update();
    emit imageUpdated();
}
