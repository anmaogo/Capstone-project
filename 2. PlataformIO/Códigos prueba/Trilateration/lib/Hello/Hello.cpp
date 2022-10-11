#include <Hello.h>
#include <Arduino.h>

Hello::Hello()
{
    
}

Hello::~Hello()
{
    
}

void Hello::Mensaje(String mensaje)
{
    _mensaje = mensaje;
    Serial.println(mensaje);
}




