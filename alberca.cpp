#include "alberca.h"

double Alberca::getNivel_real() const
{
    return nivel_real;
}

void Alberca::setNivel_real(double newNivel_real)
{
    //if(nivel_real>nivel_inicial)
    nivel_real = newNivel_real;
}

double Alberca::getNivel_max() const
{
    return nivel_max;
}

void Alberca::setNivel_max(double newNivel_max)
{
    //if(nivel_max>nivel_inicial)
    nivel_max = newNivel_max;
}

double Alberca::getArea_base() const
{
    return area_base;
}

void Alberca::setArea_base(double newArea_base)
{
    area_base = newArea_base;
}

double Alberca::getNivel_init() const
{
    return nivel_init;
}

void Alberca::setNivel_init(double newNivel_init)
{
    nivel_init = newNivel_init;
}

Alberca::Alberca()
{

}

Alberca::Alberca(double nivel_init, double nivel_max, double area_base)
{
    setNivel_init(nivel_init);
    setNivel_max(nivel_max);
    setArea_base(area_base);
}
