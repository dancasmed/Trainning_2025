#ifndef POSITION
#define POSITION

struct Position {
    int x;
    int y;
    Position(int x, int y) : x(x), y(y) { }
    Position() : x(-1), y(-1) { }

    friend bool operator==(const Position& leftValue, const Position& rightValue) {
        return (leftValue.x == rightValue.x && leftValue.y == rightValue.y);
    }

    friend bool operator!=(const Position& leftValue, const Position& rightValue) {
        return !(leftValue == rightValue);
    }
};

#endif