#ifndef BASEDEDATOS_H
#define BASEDEDATOS_H

#include <QtSql>
#include "alberca.h"
#include "acequia.h"
#include "lluvia.h"
#include "valvula.h"

class BaseDeDatos
{
public:
    BaseDeDatos();
    bool conectar();
    void crearTablaAlberca();
    void guardarConfiguracion(const Alberca& alberca, const Acequia& acequia, const Lluvia& lluvia, const Valvula& valvula);
    void cargarConfiguracion(const QString& nombre, Alberca& alberca, Acequia& acequia, Lluvia& lluvia, Valvula& valvula);

private:
    QSqlDatabase database;
};

#endif // BASEDEDATOS_H


