#include "configuracion.h"

configuracion::configuracion()
{
    setN_init(- __DBL_MAX__);
    setN_max(__DBL_MAX__);
    setAreabase(0);
    setNombre("");
    setAc_caudal(- __DBL_MAX__);
    setAc_caudal_max(__DBL_MAX__);
    setC_lluvia(- __DBL_MAX__);
    setValvula_r(0);
}

double configuracion::getAc_caudal() const
{
    return ac_caudal;
}

void configuracion::setAc_caudal(double newAc_caudal)
{
    ac_caudal = newAc_caudal;
}

double configuracion::getAc_caudal_max() const
{
    return ac_caudal_max;
}

void configuracion::setAc_caudal_max(double newAc_caudal_max)
{
    ac_caudal_max = newAc_caudal_max;
}

double configuracion::getN_init() const
{
    return n_init;
}

void configuracion::setN_init(double newN_init)
{
    n_init = newN_init;
    if (n_init >= n_max)
    {
        n_init = n_max;
    }
}

double configuracion::getN_max() const
{
    return n_max;
}

void configuracion::setN_max(double newN_max)
{
    n_max = newN_max;
}

double configuracion::getAreabase() const
{
    return areabase;
}

void configuracion::setAreabase(double newAreabase)
{
    areabase = newAreabase;
}

QString configuracion::getNombre() const
{
    return nombre;
}

void configuracion::setNombre(const QString &newNombre)
{
    nombre = newNombre;
}

double configuracion::getC_lluvia() const
{
    return c_lluvia;
}

void configuracion::setC_lluvia(double newC_lluvia)
{
    c_lluvia = newC_lluvia;
}

double configuracion::getValvula_r() const
{
    return valvula_r;
}

void configuracion::setValvula_r(double newValvula_r)
{
    valvula_r = newValvula_r;
}
