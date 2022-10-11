#ifndef __HELLO_H__
#define __HELLO_H__
#include <Arduino.h>

class Hello
{
    public: 
        Hello();
        ~Hello();
        void Mensaje(String mensaje);
    private:
        String _mensaje;

    protected:
};

#endif // __HELLO_H__