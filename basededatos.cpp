
#include "basededatos.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

BaseDeDatos::BaseDeDatos()
{
}

bool BaseDeDatos::conectar()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("db_simulador");

    if (!database.open()) {
        qDebug() << "Error al abrir la base de datos:" << database.lastError().text();
        return false;
    }

    qDebug() << "Conexión a la base de datos establecida correctamente.";
    return true;
}

void BaseDeDatos::crearTablaAlberca()
{
    if (!database.isOpen()) {
        qDebug() << "Error: no se ha establecido una conexión a la base de datos.";
        return;
    }

    QSqlQuery query(database);
    query.exec("CREATE TABLE IF NOT EXISTS alberca ("
               "nombre TEXT PRIMARY KEY,"
               "nivel_max REAL,"
               "area_base REAL,"
               "nivel_init REAL,"
               "acequia_caudal_agua REAL,"
               "acequia_caudal_max REAL,"
               "lluvia_caudal REAL,"
               "valvula_radio REAL"
               ")");
    if (!query.exec()) {
        qDebug() << "Error al crear la tabla alberca:" << query.lastError().text();
        return;
    }

    qDebug() << "Tabla alberca creada correctamente.";
}

void BaseDeDatos::guardarConfiguracion(const Alberca& alberca, const Acequia& acequia, const Lluvia& lluvia, const Valvula& valvula)
{
    if (!database.isOpen()) {
        qDebug() << "Error: no se ha establecido una conexión a la base de datos.";
        return;
    }

    QSqlQuery query(database);
    query.prepare("INSERT OR REPLACE INTO alberca (nombre, nivel_max, area_base, nivel_init, acequia_caudal_agua, acequia_caudal_max, lluvia_caudal, valvula_radio) "
                  "VALUES (:nombre, :nivel_max, :area_base, :nivel_init, :acequia_caudal_agua, :acequia_caudal_max, :lluvia_caudal, :valvula_radio)");

    query.bindValue(":nombre", alberca.getNombre());
    query.bindValue(":nivel_max", alberca.getNivel_max());
    query.bindValue(":area_base", alberca.getArea_base());
    query.bindValue(":nivel_init", alberca.getNivel_init());
    query.bindValue(":acequia_caudal_agua", acequia.getACaudal_agua());
    query.bindValue(":acequia_caudal_max", acequia.getACaudal_max());
    query.bindValue(":lluvia_caudal", lluvia.getLluvia_caudal());
    query.bindValue(":valvula_radio", valvula.getValvula_radio());

    if (!query.exec()) {
        qDebug() << "Error al guardar la configuración en la base de datos:" << query.lastError().text();
        return;
    }

    qDebug() << "Configuración guardada correctamente en la base de datos.";
}

void BaseDeDatos::cargarConfiguracion(const QString& nombre, Alberca& alberca, Acequia& acequia, Lluvia& lluvia, Valvula& valvula)
{
    if (!database.isOpen()) {
        qDebug() << "Error: no se ha establecido una conexión a la base de datos.";
        return;
    }

    QSqlQuery query(database);
    query.prepare("SELECT * FROM alberca WHERE nombre = :nombre");
    query.bindValue(":nombre", nombre);

    if (!query.exec()) {
        qDebug() << "Error al cargar la configuración desde la base de datos:" << query.lastError().text();
        return;
    }

    if (query.next()) {
        alberca.setNombre(query.value("nombre").toString());
        alberca.setNivel_max(query.value("nivel_max").toDouble());
        alberca.setArea_base(query.value("area_base").toDouble());
        alberca.setNivel_init(query.value("nivel_init").toDouble());

        acequia.setACaudal_agua(query.value("acequia_caudal_agua").toDouble());
        acequia.setACaudal_max(query.value("acequia_caudal_max").toDouble());

        lluvia.setLluvia_caudal(query.value("lluvia_caudal").toDouble());

        valvula.setValvula_radio(query.value("valvula_radio").toDouble());

        qDebug() << "Configuración cargada correctamente desde la base de datos.";
    } else {
        qDebug() << "No se encontró ninguna configuración en la base de datos para el nombre:" << nombre;
    }
}

