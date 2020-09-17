#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>

//  D   -- dot        -- точка          (не Point, чтобы не пересекалось с Plane по первой букве)
//  V   -- vector     -- вектор
//  T   -- triangle   -- треугольник
//  P   -- plane      -- плоскость

const double eps = 1e-3;

struct Dot {
    double x = 0, y = 0, z = 0;
};

bool operator == (const Dot lhs, const Dot rhs) {
    return (fabs (lhs.x - rhs.x) < eps && 
            fabs (lhs.y - rhs.y) < eps && 
            fabs (lhs.z - rhs.z) < eps );
}

class Vector {
    private:
        Dot coord_ = {};
    public:
        Vector () = delete;
        explicit Vector (double x, double y, double z):     //  Чтобы был единственный вариант 
            coord_ ({ x, y, z })                            //  конструктора плоскости в случае
            {}                                              //  передачи параметров {a, b, c} и Vector
        Vector (Dot start, Dot finish):
            coord_ ({ finish.x - start.x,
                      finish.y - start.y,
                      finish.z - start.z })
            {}
        explicit Vector (Dot d0):             //  Конструктор explicit, чтобы не происходило неявного
            coord_ (d0)                         //  построения вектора из точки, когда нам это не нужно
            {}
        Dot GetCoord () const {
            return coord_;
        }
        ~Vector () = default;
};

using PairVD = std::pair <Vector, Dot>;

double operator * (const Vector lhs, const Vector rhs) {
    Dot lhsCoord = lhs.GetCoord ();
    Dot rhsCoord = rhs.GetCoord ();
    return lhsCoord.x * rhsCoord.x +
           lhsCoord.y * rhsCoord.y +
           lhsCoord.z * rhsCoord.z;
}

Vector operator % (const Vector lhs, const Vector rhs) {
    Dot lhsCoord = lhs.GetCoord ();
    Dot rhsCoord = rhs.GetCoord ();
    return Vector (lhsCoord.y * rhsCoord.z - lhsCoord.z * rhsCoord.y,
                   lhsCoord.z * rhsCoord.x - lhsCoord.x * rhsCoord.z,
                   lhsCoord.x * rhsCoord.y - lhsCoord.y * rhsCoord.x);
}

bool operator == (const Vector lhs, const Vector rhs) {
    return (lhs.GetCoord () == rhs.GetCoord ());
}

struct Triangle {
    Dot d0 = {}, d1 = {}, d2 = {};
};

class Plane {
    private:
        Dot d0_ = {};
        Vector n_ = { Dot (), Dot () };
    public:
        Plane () = delete;
        Plane (Dot d0, Vector n):
            d0_ (d0),
            n_ (n)
            {}
        Plane (Vector r0, Vector n):
            d0_ (r0.GetCoord ()),
            n_ (n)
            {}
        Plane (Dot d0, Dot d1, Dot d2):
            d0_ (d0),
            n_ ( Vector (d0, d1) % Vector (d0, d2) )
            {}
        Plane (Triangle t0):
            Plane (t0.d0, t0.d1, t0.d2)
            {}
        PairVD GetPlane () const {
            return { n_, d0_ };
        }
};

bool operator == (const Plane lhs, const Plane rhs) {
    return (lhs.GetPlane () == rhs.GetPlane ());
}

bool AreParallel (const Plane lhs, const Plane rhs) {
    return (lhs.GetPlane ().first == rhs.GetPlane ().first);
}

double DistanceDP (Dot d0, Plane p0) {
    Dot nCoord = p0.GetPlane ().first.GetCoord ();                          //  A, B, C в формуле
    double D =  - (p0.GetPlane ().first * Vector (p0.GetPlane ().second));  //  D в формуле
    return ((nCoord.x * d0.x + nCoord.y * d0.y + nCoord.z * d0.z + D) /     //  Ax_0 + By_0 + Cz_0 + D
             (sqrt (p0.GetPlane ().first * p0.GetPlane ().first)));         //  sqrt (A^2 + B^2 + C^2)
}

int main () {
    /*  TESTS   */
    #ifdef DEBUG
    Dot ox = {1, 0, 0};
    Dot oy = {0, 1, 0};
    Dot oz = {0, 0, 1};
    Vector test = Vector(ox) % Vector(oy);
    std::cout << "TEST 1: 1 == " << (test == Vector(oz)) << std::endl;
    /* ??? (конструктор Vector явный, но при этом ambiguous conversation)
    Plane p1 = { {0, 0, 0}, Vector(oz) };
    Plane p2 = { {0, 0, 1}, Vector(oz) };
    */
    Plane p1 = { Vector (0., 0., 0.), Vector(oz) };
    Plane p2 = { Vector (0.0001, 0., 1.), Vector(oz) };
    std::cout << "TEST 2: 0 == " << (p1 == p2) << std::endl;
    std::cout << "TEST 3: 1 == " << areParallel (p1, p2) << std::endl;
    #endif
    /*   CODE   */
    unsigned q = 0;
    /* std::cin >> q; */ q = 2;
    std::vector <Triangle> triangles = {};
    Dot d0 = {}, d1 = {}, d2 = {};
    while (q--) {
        std::cin >> d0.x >> d0.y >> d0.z;
        std::cin >> d1.x >> d1.y >> d1.z;
        std::cin >> d2.x >> d2.y >> d2.z;
        triangles.push_back ({ d0, d1, d2 });
    }
    /*
    for (unsigned i = 0; i < q; ++i) {
        for (unsigned j = std::min (i + 1, q - 1); j < q; ++j) {
            cool stuff...
        }
    }
    */
    /*
        Проверка треугольников на вырожденность...
    */
    Triangle T0 = triangles[0];
    Triangle T1 = triangles[1];
    Plane P0 (T0), P1 (T1);

    double dist_T1d0_P0 = DistanceDP (T1.d0, P0),
           dist_T1d1_P0 = DistanceDP (T1.d1, P0),
           dist_T1d2_P0 = DistanceDP (T1.d2, P0);

    if (dist_T1d0_P0 * dist_T1d1_P0 > 0 &&        //  Одинаковые знаки у всех расстояний
        dist_T1d1_P0 * dist_T1d2_P0 > 0 &&        //  от вершин до плоскостей
        dist_T1d2_P0 * dist_T1d0_P0 > 0) {
            std::cout << "No intersection" << std::endl;
            return 0;
    }

    if (P0 == P1) {
        /*
            Тест для треугольников в одной плоскости...
        */
    }
    else if (AreParallel (P0, P1)) {
        std::cout << "No intersection" << std::endl;
        return 0;
    }

    double dist_T0d0_P1 = DistanceDP (T0.d0, P1),
           dist_T0d1_P1 = DistanceDP (T0.d1, P1),
           dist_T0d2_P1 = DistanceDP (T0.d2, P1);
    
    if (dist_T0d0_P1 * dist_T0d1_P1 > 0 &&
        dist_T0d1_P1 * dist_T0d2_P1 > 0 &&
        dist_T0d2_P1 * dist_T0d0_P1 > 0) {
            std::cout << "No intersection" << std::endl;
            return 0;
    }
    /*
        Линия пересечения треугольников, тест интервалов...
    */
    return 0;
}