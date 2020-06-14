#ifndef __P2POINT_H__
#define __P2POINT_H__

#include "Globals.h"

template<class TYPE>
class P2Point {
public:
    TYPE x, y;

    P2Point() : x(0), y(0) {}

    P2Point(const P2Point& _v) {
        x = _v.x;
        y = _v.y;
    }

    P2Point(const TYPE& _x, const TYPE& _y) {
        x = _x;
        y = _y;
    }

    P2Point& create(const TYPE& _x, const TYPE& _y) {
        x = _x;
        y = _y;

        return(*this);
    }

    // Math ------------------------------------------------
    P2Point operator -(const P2Point& _v) const {
        P2Point r;

        r.x = x - _v.x;
        r.y = y - _v.y;

        return(r);
    }

    P2Point operator + (const P2Point& _v) const {
        P2Point r;

        r.x = x + _v.x;
        r.y = y + _v.y;

        return(r);
    }

    const P2Point& operator -=(const P2Point& _v) {
        x -= _v.x;
        y -= _v.y;

        return(*this);
    }

    const P2Point& operator +=(const P2Point& _v) {
        x += _v.x;
        y += _v.y;

        return(*this);
    }

    bool operator ==(const P2Point& _v) const { return (x == _v.x && y == _v.y); }

    bool operator !=(const P2Point& _v) const { return (x != _v.x || y != _v.y); }

    // Utils ------------------------------------------------
    bool IsZero() const { return (x == 0 && y == 0); }

    P2Point& SetToZero() {
        x = 0;
        y = 0;
        return(*this);
    }

    P2Point& Negate() {
        x = -x;
        y = -y;

        return(*this);
    }

    TYPE sqrtf(float _x) {
        float number;
        float sqrt;
        sqrt = _x / 2;
        number = 0;
        if (_x <= 0) { return number; }
        while (sqrt != number) {
            number = sqrt;
            sqrt = (_x / number + number) / 2;
        }
        return sqrt;
    }

    // Distances ---------------------------------------------
    TYPE DistanceTo(const P2Point& _v) const {
        TYPE fx = x - _v.x;
        TYPE fy = y - _v.y;

        return sqrtf(float(fx * fx) + float(fy * fy));
    }

    TYPE DistanceNoSqrt(const P2Point& _v) const {
        TYPE fx = x - _v.x;
        TYPE fy = y - _v.y;

        return (fx * fx) + (fy * fy);
    }

    TYPE DistanceManhattan(const P2Point& _v) const { return abs(_v.x - x) + abs(_v.y - y); }
};

typedef P2Point<int> iPoint;
typedef P2Point<float> fPoint;

#endif // __P2POINT_H__