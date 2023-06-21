#include "basededatos.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

basededatos::basededatos()
{

}

bool basededatos::conectar()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("db_alberca");

    if (!database.open()) {
        qDebug() << "Error al abrir la base de datos:" << database.lastError().text();
        return false;
    }

    qDebug() << "Conexión a la base de datos establecida correctamente.";
    return true;
}

void basededatos::cargarAlberca(Alberca &alberca, Acequia &acequia, Lluvia &lluvia, Valvula &valvula)
{
    if (!database.isOpen()) {
        qDebug() << "Error: no se ha establecido una conexión a la base de datos.";
        return;
    }

    QSqlQuery query("SELECT * FROM db_alberca");

    if (!query.exec()) {
        qDebug() << "Error al cargar la configuración desde la base de datos:" << query.lastError().text();
        return;
    }

    if (query.next()) {
        alberca.setNombre(query.value("nombre").toString());
        alberca.setNivel_init(query.value("nivel_init").toDouble());
        alberca.setNivel_max(query.value("nivel_max").toDouble());
        acequia.setACaudal_agua(query.value("aCaudal_agua").toDouble());
        acequia.setACaudal_max(query.value("aCaudal_max").toDouble());
        lluvia.setLluvia_caudal(query.value("lluvia_caudal").toDouble());
        valvula.setValvula_radio(query.value("valvula_radio").toDouble());

        qDebug() << "Configuración cargada correctamente desde la base de datos.";
    } else {
        qDebug() << "No se encontró ninguna configuración en la base de datos.";
    }
}

void basededatos::guardarAlberca(const Alberca &alberca, const Acequia &acequia, const Lluvia &lluvia, const Valvula &valvula)
{
    if (!database.isOpen()) {
        qDebug() << "Error: no se ha establecido una conexión a la base de datos.";
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT OR REPLACE INTO configuracion (nombre, nivel_init, nivel_max, aCaudal_agua, aCaudal_max, lluvia_caudal, valvula_radio) "
                  "VALUES (:nombre, :nivel_init, :nivel_max, :aCaudal_agua, :aCaudal_max, :lluvia_caudal, :valvula_radio)");
    query.bindValue(":nombre", alberca.getNombre());
    query.bindValue(":nivel_init", alberca.getNivel_init());
    query.bindValue(":nivel_max", alberca.getNivel_max());
    query.bindValue(":aCaudal_agua", acequia.getACaudal_agua());
    query.bindValue(":aCaudal_max", acequia.getACaudal_max());
    query.bindValue(":lluvia_caudal", lluvia.getLluvia_caudal());
    query.bindValue(":valvula_radio", valvula.getValvula_radio());

    if (!query.exec()) {
        qDebug() << "Error al guardar la configuración en la base de datos:" << query.lastError().text();
        return;
    }

    qDebug() << "Configuración guardada correctamente en la base de datos.";
}

void basededatos::reemplazarAlberca(const Alberca &alberca, const Acequia &acequia, const Lluvia &lluvia, const Valvula &valvula)
{
    if (!database.isOpen()) {
        qDebug() << "Error: no se ha establecido una conexión a la base de datos.";
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE configuracion SET nivel_init = :nivel_init, nivel_max = :nivel_max, "
                  "aCaudal_agua = :aCaudal_agua, aCaudal_max = :aCaudal_max, "
                  "lluvia_caudal = :lluvia_caudal, valvula_radio = :valvula_radio WHERE nombre = :nombre");
    query.bindValue(":nombre", alberca.getNombre());
    query.bindValue(":nivel_init", alberca.getNivel_init());
    query.bindValue(":nivel_max", alberca.getNivel_max());
    query.bindValue(":aCaudal_agua", acequia.getACaudal_agua());
    query.bindValue(":aCaudal_max", acequia.getACaudal_max());
    query.bindValue(":lluvia_caudal", lluvia.getLluvia_caudal());
    query.bindValue(":valvula_radio", valvula.getValvula_radio());

    if (!query.exec()) {
        qDebug() << "Error al reemplazar la configuración en la base de datos:" << query.lastError().text();
        return;
    }

    qDebug() << "Configuración reemplazada correctamente en la base de datos.";
}
