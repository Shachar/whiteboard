#ifndef BOARDBACKGROUND_H
#define BOARDBACKGROUND_H

#include <QMetaType>
#include <QPainter>

class BoardBackground
{
public:
    enum class Type {
        Plain,
        Lines,
        Grid,
    };

    explicit constexpr BoardBackground(Type type = Type::Plain) : _type(type)
    {}

    void draw(QRectF sourceRect, QPainter &painter, const QRect &updateArea) const;

    Type type() const {
        return _type;
    }

private:
    Type _type;
};

std::ostream &operator<<(std::ostream &out, BoardBackground::Type type);

Q_DECLARE_METATYPE(BoardBackground)

#endif // BOARDBACKGROUND_H
