#ifndef ALBERCA_H
#define ALBERCA_H
#include <QString>

class Alberca
{
private:
    double nivel_real;
    double nivel_max;
    double area_base;
    double nivel_init;
    QString nombre;


public:
    Alberca();
    Alberca(double nivel_init, double nivel_max, double area_base, double nivel_real);

    double getNivel_real() const;
    void setNivel_real(double newNivel_real);

    double getNivel_max() const;
    void setNivel_max(double newNivel_max);

    double getArea_base() const;
    void setArea_base(double newArea_base);

    double getNivel_init() const;
    void setNivel_init(double newNivel_init);

    QString getNombre() const;
    void setNombre(const QString &newNombre);
};

#endif // ALBERCA_H
