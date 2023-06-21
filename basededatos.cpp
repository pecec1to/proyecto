#include "BaseDeDatos.h"
#include "configuracion.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

BaseDeDatos::BaseDeDatos()
{
}

bool BaseDeDatos::conectar()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("db_alberca");
    return database.open();
}

void BaseDeDatos::crearTablaAlberca()
{
    if (!database.isOpen()) {
        qDebug().noquote().nospace()<< "crearTablas(): La base de datos no está conectada.";
        return;
    }

    QSqlQuery query(database);

    query.prepare("CREATE TABLE IF NOT EXISTS alberca (nombre VARCHAR(30) PRIMARY KEY, ac_caudal REAL, ac_caudal_max REAL, n_init REAL, REAL, c_lluvia REAL, valvula_r REAL);");

    if (!query.exec())
    {
        qDebug().noquote().nospace() << "Error al crear la tabla de la alberca: " <<  query.lastError().text();
    }

}


void BaseDeDatos::guardarAlberca(const configuracion &c)
{
    if (!database.isOpen()) {
        qDebug().noquote().nospace() << "guardarAlberca(): La base de datos no está conectada.";
        return;
    }

    QSqlQuery query(database);

    query.prepare("REPLACE INTO alberca (nombre, ac_caudal, ac_caudal_max, n_init, c_lluvia, valvula_r) VALUES (?, ?, ?, ?, ?, ?);");

    query.addBindValue(c.getNombre());
    query.addBindValue(c.getAc_caudal());
    query.addBindValue(c.getAc_caudal_max());
    query.addBindValue(c.getN_init());
    query.addBindValue(c.getC_lluvia());
    query.addBindValue(c.getValvula_r());

    if (!query.exec()) {
        qDebug().noquote().nospace() << "Error al insertar en la tabla alberca: " << query.lastError().text();
    }
}


void BaseDeDatos::cargarAlberca(const configuracion &c)
{
         if (!database.isOpen()) {
            qDebug().noquote().nospace() << "CargarAlberca(): La base de datos no está conectada.";
            return;
        }

        QSqlQuery query(database);

        query.prepare("UPDATE configuraciones SET ac_caudal = ?, ac_caudal_max = ?, n_init = ?, c_lluvia = ?, valvula_r = ? WHERE nombre = ?;");

        query.addBindValue(c.getAc_caudal());
        query.addBindValue(c.getAc_caudal_max());
        query.addBindValue(c.getN_init());
        query.addBindValue(c.getC_lluvia());
        query.addBindValue(c.getValvula_r());
        query.addBindValue(c.getNombre());

        if (!query.exec()) {
            qDebug().noquote().nospace() << "Error al cargar en la tabla las configuraciones: " << query.lastError().text();
        }

}

configuracion* BaseDeDatos::leerAlberca(const QString &nombre)
{
    if (!database.isOpen()) {
        qDebug().noquote().nospace() << "leerAlberca(): La base de datos no está conectada.";
        return nullptr;
    }

    QSqlQuery query(database);

    query.prepare("SELECT nombre, ac_caudal, ac_caudal_max, n_init, c_lluvia, valvula_r FROM alberca WHERE nombre = ?;");
    query.addBindValue(nombre);

    if (!query.exec()) {
        qDebug().noquote().nospace() << "Error en leerAlberca: " << query.lastError().text();
        return nullptr;
    }

    configuracion *c = nullptr;

    if (query.next()) {
        c = new configuracion();
        c->setNombre(query.value(0).toString());
        c->setAc_caudal(query.value(1).toDouble());
        c->setAc_caudal_max(query.value(2).toDouble());
        c->setN_init(query.value(3).toDouble());
        c->setC_lluvia(query.value(6).toDouble());
        c->setValvula_r(query.value(7).toDouble());
    }
    return c;
}


QList<QString>* BaseDeDatos::leerNombreAlberca()
{
    if (!database.isOpen()) {
        qDebug().noquote().nospace()<< "leerNombreAlberca(): La base de datos no está conectada.";
        return nullptr;
    }
    if (!database.isOpen()) {
        qDebug().noquote().nospace()<< "leerNombreAlberca(): La base de datos no está conectada.";
        return nullptr;
    }

    QSqlQuery query(database);

    query.prepare("SELECT nombre FROM alberca;");

    if(!query.exec())
    {
        qDebug().noquote().nospace() << "Error en leerNombreAlberca(): " <<query.lastError().text();
        return nullptr;
    }

    QList<QString> *list = new QList<QString>();

    while(query.next())
    {
        QString s;
        s = query.value(0).toString();
        list->append(s);
    }
    return list;
}
