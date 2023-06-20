#ifndef VALVULA_H
#define VALVULA_H

enum valvestate {VALVULA_ABIERTA,VALVULA_CERRADA};

class Valvula
{
private:

    double valvula_radio;
    valvestate valvula_estado;

public:
    Valvula();
    Valvula(double radio);

    double getValvula_radio() const;
    void setValvula_radio(double newValvula_radio);

    valvestate getValvula_estado() const;
    void setValvula_estado(valvestate newValvula_estado);
};




#endif // VALVULA_H
