#ifndef CONFIGURACION_H
#define CONFIGURACION_H
#include <QString>
#include <QDebug>

//fichero con los parámetros a definir en la configuración

class configuracion
{
private:

    //acequia
    double ac_caudal;
    double ac_caudal_max;

    //alberca
    double n_init;
    double n_max;
    double areabase;
    QString nombre;

    //lluvia
    double c_lluvia;

    //valvula
    double valvula_r;


public:
    configuracion();

    //acequia
    double getAc_caudal() const;
    void setAc_caudal(double newAc_caudal);

    double getAc_caudal_max() const;
    void setAc_caudal_max(double newAc_caudal_max);

    //alberca
    double getN_init() const;
    void setN_init(double newN_init);


    double getN_max() const;
    void setN_max(double newN_max);


    double getAreabase() const;
    void setAreabase(double newAreabase);

    QString getNombre() const;
    void setNombre(const QString &newNombre);

    //lluvia
    double getC_lluvia() const;
    void setC_lluvia(double newC_lluvia);

    //valvula
    double getValvula_r() const;
    void setValvula_r(double newValvula_r);
};

#endif // CONFIGURACION_H
