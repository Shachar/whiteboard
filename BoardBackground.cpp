#include "BoardBackground.h"

#include <QDebug>

#include <iostream>

void BoardBackground::draw(QRectF sourceRect, QPainter &painter, const QRect &updateArea) const {
    static const QColor LinesColor(Qt::cyan);

    QPen pen = painter.pen();
    pen.setColor( LinesColor );
    pen.setWidth(0);
    painter.setPen(pen);

    static constexpr unsigned LineSpacing = 30;
    static constexpr unsigned GridSpacing = 15;

    painter.fillRect(updateArea, Qt::white);

    switch( _type ) {
    case Type::Plain:
        break;
    case Type::Lines:
        for(
            int y=LineSpacing-1 + (sourceRect.top() / LineSpacing) * LineSpacing;
            y<sourceRect.bottom();
            y+=LineSpacing
           )
        {
            qreal realY = y/sourceRect.height()*updateArea.height();
            painter.drawLine( updateArea.left(), realY, updateArea.right(), realY );
        }
        break;
    case Type::Grid:
        for(
            int y=GridSpacing-1 + (updateArea.top() / GridSpacing) * GridSpacing;
            y<sourceRect.bottom();
            y+=GridSpacing
           )
        {
            qreal realY = y/sourceRect.height()*updateArea.height();
            painter.drawLine( updateArea.left(), realY, updateArea.right(), realY );
        }
        for(
            int x=GridSpacing-1 + (updateArea.left() / GridSpacing) * GridSpacing;
            x<sourceRect.right();
            x+=GridSpacing
           )
        {
            qreal realX = x/sourceRect.width()*updateArea.width();
            painter.drawLine( realX, updateArea.top(), realX, updateArea.bottom() );
        }
        break;
    }
}

std::ostream &operator<<(std::ostream &out, BoardBackground::Type type) {
#define CASE(a) \
    case BoardBackground::Type::a: return out<<#a

    switch(type) {
    CASE(Plain);
    CASE(Lines);
    CASE(Grid);
    }
#undef CASE

    return out<<"Type("<<static_cast<int>(type)<<")";
}
