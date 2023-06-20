#include "valvula.h"

double Valvula::getValvula_radio() const
{
    return valvula_radio;
}

void Valvula::setValvula_radio(double newValvula_radio)
{
    valvula_radio = newValvula_radio;
}

valvestate Valvula::getValvula_estado() const
{
    return valvula_estado;
}

void Valvula::setValvula_estado(valvestate newValvula_estado)
{
    if (newValvula_estado == VALVULA_ABIERTA)
    {
        valvula_estado = VALVULA_ABIERTA;
    }
    else if (newValvula_estado == VALVULA_CERRADA)
    {
        valvula_estado = VALVULA_CERRADA;
    }
}

Valvula::Valvula()
{

}

Valvula::Valvula(double radio)
{
    setValvula_radio(radio);
}
