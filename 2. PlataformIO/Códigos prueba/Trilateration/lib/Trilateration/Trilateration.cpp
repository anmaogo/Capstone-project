
#include <Trilateration.h>
#include <Arduino.h>
#include <math.h>


#ifndef max
  #define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
  #define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

Trilateration::Trilateration()
{
    
}

Trilateration::~Trilateration()
{
    
}

class Point2d{
public:
    Point2d() {}
    Point2d(double x, double y)
        : X(x), Y(y) {}

    double x() const { return X; }
    double y() const { return Y; }

    /**
     * Returns the norm of this vector.
     * @return the norm
    */
    double norm() const {
        return sqrt( X * X + Y * Y );
    }

    void setCoords(double x, double y) {
        X = x; Y = y;
    }

    // Print point

private:
    double X;
    double Y;
};

class Circle{
public:
    /**
     * @param R - radius
     * @param C - center
     */
    Circle(double R, Point2d& C)
        : r(R), c(C) {}

    /**
     * @param R - radius
     * @param X - center's x coordinate
     * @param Y - center's y coordinate
     */
    Circle(double R, double X, double Y)
        : r(R), c(X, Y) {}

    Point2d getC() const { return c; }
    double getR() const { return r; }

    size_t intersect(const Circle& C2, Point2d& i1, Point2d& i2) {
        // distance between the centers
        double d = Point2d(c.x() - C2.c.x(),
                c.y() - C2.c.y()).norm();

        // find number of solutions
        if(d > r + C2.r) // circles are too far apart, no solution(s)
        {
            //std::cout << "Circles are too far apart\n";
            return 0;
        }
        else if(d == 0 && r == C2.r) // circles coincide
        {
            //std::cout << "Circles coincide\n";
            return 0;
        }
        // one circle contains the other
        else if(d + min(r, C2.r) < max(r, C2.r))
        {
            //std::cout << "One circle contains the other\n";
            return 0;
        }
        else
        {
            double a = (r*r - C2.r*C2.r + d*d)/ (2.0*d);
            double h = sqrt(r*r - a*a);

            // find p2
            Point2d p2( c.x() + (a * (C2.c.x() - c.x())) / d,
                    c.y() + (a * (C2.c.y() - c.y())) / d);

            // find intersection points p3
            i1.setCoords( p2.x() + (h * (C2.c.y() - c.y())/ d),
                    p2.y() - (h * (C2.c.x() - c.x())/ d)
            );
            i2.setCoords( p2.x() - (h * (C2.c.y() - c.y())/ d),
                    p2.y() + (h * (C2.c.x() - c.x())/ d)
            );

            if(d == r + C2.r)
                return 1;
            return 2;
        }
    }

    // Print circle

private:
    // radius
    double r;
    // center
    Point2d c;

};
//Esta funncion no funciona para el arduino. Borrar las impresiones
int checkCoordinates(size_t point, Point2d i1, Point2d i2)
{

  if(point == 2)
  {
      //std::cout << i1 << "\n";
      //std::cout << i2 << "\n";
  }
  else if(point)
      //std::cout << i1 << "\n";

  return 0;
}

Point2d findMin(double d1, double d2, double d3, double d4, Point2d i1, Point2d i2)
{
  Point2d d;

  if(d1 < d2 && d1 < d3 && d1 < d4){
    d = i1;
  }else if(d2 < d1 && d2 < d3 && d2 < d4){
    d = i1;
  }else if(d3 < d1 && d3 < d2 && d4 < d4){
    d = i2;
  }else{
    d = i2;
  }

  return d;
}

Point2d centroidPoint(Point2d p1, Point2d p2, Point2d p3)
{
    // coordinate of the vertices
  double x1 = p1.x(), x2 = p2.x(), x3 = p3.x();
  double y1 = p1.y(), y2 = p2.y(), y3 = p3.y();
  Point2d trilateration;

  // Formula to calculate centroid
  double x = (x1 + x2 + x3) / 3;
  double y = (y1 + y2 + y3) / 3;

  trilateration.setCoords(x, y);

  return trilateration;
}

Point2d closedPoints(Point2d i1, Point2d i2, Point2d i3, Point2d i4, Point2d i5, Point2d i6)
{
  //Disances closed-i_points
  double d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12;
  Point2d p1, p2, p3, trilateration;
  // distances
  d1 = (pow( i1.x() - i6.x() ,2) + pow( i1.y() - i6.y() ,2));
  d2 = (pow( i1.x() - i5.x() ,2) + pow( i1.y() - i5.y() ,2));
  d3 = (pow( i2.x() - i6.x() ,2) + pow( i2.y() - i6.y() ,2));
  d4 = (pow( i2.x() - i5.x() ,2) + pow( i2.y() - i5.y() ,2));

  d5 = (pow( i3.x() - i1.x() ,2) + pow( i3.y() - i1.y() ,2));
  d6 = (pow( i3.x() - i2.x() ,2) + pow( i3.y() - i2.y() ,2));
  d7 = (pow( i4.x() - i1.x() ,2) + pow( i4.y() - i1.y() ,2));
  d8 = (pow( i4.x() - i2.x() ,2) + pow( i4.y() - i2.y() ,2));

  d9 = (pow( i5.x() - i3.x() ,2) + pow( i5.y() - i3.y() ,2));
  d10 = (pow( i5.x() - i4.x() ,2) + pow( i5.y() - i4.y() ,2));
  d11 = (pow( i6.x() - i3.x() ,2) + pow( i6.y() - i3.y() ,2));
  d12 = (pow( i6.x() - i4.x() ,2) + pow( i6.y() - i4.y() ,2));

  p1 = findMin(d1, d2, d3, d4, i1, i2);
  p2 = findMin(d5, d6, d7, d8, i3, i4);
  p3 =findMin(d9, d10, d11, d12, i5, i6);


  trilateration = centroidPoint(p1, p2, p3);




  return trilateration;
}

Point2d intersectCircles(Circle circle_1, Circle circle_2, Circle circle_3)
{
  //Intersection of AP1 & AP2
  Circle c1 = circle_1;
  Circle c2 = circle_2;
  Circle c3 = circle_3;
  Point2d i1, i2, i3, i4, i5, i6, trilateration;




  // intersections point(s) AP1 & AP2
  size_t i_points = c1.intersect(c2, i1, i2);
  // intersections point(s) AP2 & AP3
  size_t i_points_2 = c2.intersect(c3, i3, i4);
  // intersections point(s) AP2 & AP3
  size_t i_points_3 = c1.intersect(c3, i5, i6);

  //std::cout << "Intersection AP1 & AP2\n";
  //checkCoordinates(i_points, i1, i2);
  //std::cout << "Intersection AP2 & AP3\n";
  //checkCoordinates(i_points_2, i3, i4);
  //std::cout << "Intersection AP1 & AP3\n";
  //checkCoordinates(i_points_3, i5, i6);

  trilateration = closedPoints(i1, i2, i3, i4, i5, i6);



  return trilateration;
}



// -----------------------  FIN MANEJO DE VECTORES

void Trilateration::Localizate(double x1, double y1, double x2, double y2, double x3, double y3, double r1, double r2, double r3)
{

  Point2d trilateration;
  Circle c1(r1, x1, y1);
  Circle c2(r2, x2, y2);
  Circle c3(r3, x3, y3);
  double x_t;
  double y_t;


  trilateration = intersectCircles(c1, c2, c3);
  x_t = trilateration.x();
  y_t = trilateration.y();

  Serial.printf("Ubicación en x: %lf  en y: %lf \n", x_t,y_t);


}
