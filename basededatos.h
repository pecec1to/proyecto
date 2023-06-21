#ifndef BASEDEDATOS_H
#define BASEDEDATOS_H

#include <QSqlDatabase>
#include "alberca.h"
#include "acequia.h"
#include "lluvia.h"
#include "valvula.h"

class basededatos
{
public:
    basededatos();

    bool conectar();
    void guardarAlberca(const Alberca &alberca, const Acequia &acequia, const Lluvia &lluvia, const Valvula &valvula);
    void cargarAlberca(Alberca &alberca, Acequia &acequia, Lluvia &lluvia, Valvula &valvula);
    void reemplazarAlberca(const Alberca &alberca, const Acequia &acequia, const Lluvia &lluvia, const Valvula &valvula);

private:
    QSqlDatabase database;
};

#endif // BASEDEDATOS_H
