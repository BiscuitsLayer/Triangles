#include <iostream>
#include <cmath>

const double eps = 1e-3;

struct Point {
    double x = 0, y = 0, z = 0;
};

bool operator == (const Point lhs, const Point rhs) {
    return (fabs (lhs.x - rhs.x) < eps && 
            fabs (lhs.y - rhs.y) < eps && 
            fabs (lhs.z - rhs.z) < eps );
}

class Vector {
    private:
        Point coord_ = {};
    public:
        Vector () = delete;
        explicit Vector (double x, double y, double z):     //  Чтобы был единственный вариант 
            coord_ ({ x, y, z })                            //  конструктора плоскости в случае
            {}                                              //  передачи параметров {a, b, c} и Vector
        Vector (Point start, Point finish):
            coord_ ({ finish.x - start.x,
                      finish.y - start.y,
                      finish.z - start.z })
            {}
        explicit Vector (Point p0):             //  Конструктор explicit, чтобы не происходило неявного
            coord_ (p0)                         //  построения вектора из точки, когда нам это не нужно
            {}
        Point GetCoord () const {
            return coord_;
        }
        ~Vector () = default;
};

using PairVP = std::pair <Vector, Point>;

double operator * (const Vector lhs, const Vector rhs) {
    Point lhsCoord = lhs.GetCoord ();
    Point rhsCoord = rhs.GetCoord ();
    return lhsCoord.x * rhsCoord.x +
           lhsCoord.y * rhsCoord.y +
           lhsCoord.z * rhsCoord.z;
}

Vector operator % (const Vector lhs, const Vector rhs) {
    Point lhsCoord = lhs.GetCoord ();
    Point rhsCoord = rhs.GetCoord ();
    return Vector ( lhsCoord.y * rhsCoord.z - lhsCoord.z * rhsCoord.y,
                    lhsCoord.z * rhsCoord.x - lhsCoord.x * rhsCoord.z,
                    lhsCoord.x * rhsCoord.y - lhsCoord.y * rhsCoord.x );
}

bool operator == (const Vector lhs, const Vector rhs) {
    bool flag = (lhs.GetCoord () == rhs.GetCoord ());
    return flag;
}

class Plane {
    private:
        Point p0_ = {};
        Vector n_ = { Point (), Point () };
    public:
        Plane () = delete;
        Plane (Point p0, Vector n):
            p0_ (p0),
            n_ (n)
            {}
        Plane (Vector r0, Vector n):
            p0_ (r0.GetCoord ()),
            n_ (n)
            {}
        Plane (Point p1, Point p2, Point p3):
            p0_ (p1),
            n_ ( Vector(p1, p2) % Vector(p1, p3) )
            {}
        PairVP GetPlane () const {
            return { n_, p0_ };
        }
};

bool operator == (const Plane lhs, const Plane rhs) {
    return { lhs.GetPlane () == rhs.GetPlane () };
}

int main () {
    Point ox = {1, 0, 0};
    Point oy = {0, 1, 0};
    Point oz = {0, 0, 1};
    Vector test = Vector(ox) % Vector(oy);
    std::cout << (test == Vector(oz)) << std::endl;
    Plane p1 = { {0, 0, 0}, Vector(oz) };
    Plane p2 = { {0, 0, 1}, Vector(oz) };
    std::cout << (p1 == p2) << std::endl;
    return 0;
}