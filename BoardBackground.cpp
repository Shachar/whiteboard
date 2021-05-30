#include "BoardBackground.h"

#include <QDebug>

#include <iostream>

void BoardBackground::draw(QPainter &painter, const QRect &updateArea) const {
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
            int y=LineSpacing-1 + (updateArea.top() / LineSpacing) * LineSpacing;
            y<updateArea.bottom();
            y+=LineSpacing
           )
        {
            painter.drawLine( updateArea.left(), y, updateArea.right(), y );
        }
        break;
    case Type::Grid:
        for(
            int y=GridSpacing-1 + (updateArea.top() / GridSpacing) * GridSpacing;
            y<updateArea.bottom();
            y+=GridSpacing
           )
        {
            painter.drawLine( updateArea.left(), y, updateArea.right(), y );
        }
        for(
            int x=GridSpacing-1 + (updateArea.left() / GridSpacing) * GridSpacing;
            x<updateArea.right();
            x+=GridSpacing
           )
        {
            painter.drawLine( x, updateArea.top(), x, updateArea.bottom() );
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
