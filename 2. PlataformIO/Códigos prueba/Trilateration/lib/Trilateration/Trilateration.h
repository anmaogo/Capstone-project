#ifndef __TRILATERATION_H__
#define __TRILATERATION_H__

#include <Arduino.h>


class Trilateration
{
    public:
        Trilateration();
        ~Trilateration();
        void Localizate(double x1, double y1, double x2, double y2, double x3, double y3, double r1, double r2, double r3);
    private:


    protected:
};


#endif // __TRILATERATION_H__