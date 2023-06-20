#include "acequia.h"

double Acequia::getACaudal_agua() const
{
    return aCaudal_agua;
}

void Acequia::setACaudal_agua(double newACaudal_agua)
{
    aCaudal_agua = newACaudal_agua;
}

double Acequia::getACaudal_max() const
{
    return aCaudal_max;
}

void Acequia::setACaudal_max(double newACaudal_max)
{
    aCaudal_max = newACaudal_max;
}

Acequia::Acequia()
{

}

Acequia::Acequia(double caudal_agua, double caudal_max)
{
    setACaudal_agua(caudal_agua);
    setACaudal_max(caudal_max);
}
