
#ifndef BASEDEDATOS_H
#define BASEDEDATOS_H
#include "configuracion.h"
#include <QSqlDatabase>

class BaseDeDatos
{
public:
    BaseDeDatos();
    bool conectar();

    void crearTablaAlberca();

    void guardarAlberca(const configuracion &c); //inserta el sensor en la base de datos
    void cargarAlberca(const configuracion &c);  //actualiza los datos del sensor si ya está creado

    configuracion *leerAlberca(const QString &nombre);   //lee los valores del sensor


    QList<QString> *leerNombreAlberca(); // lee el nombre de los sensores en la base de datos

private:
    QSqlDatabase database;
};

#endif // BASEDEDATOS_H


