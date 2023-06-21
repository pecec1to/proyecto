#ifndef ACEQUIA_H
#define ACEQUIA_H


class Acequia
{
private:
    double aCaudal_agua;
    double aCaudal_max;

public:
    Acequia();
    Acequia(double caudal_agua, double caudal_max);

    double getACaudal_agua() const;
    void setACaudal_agua(double newACaudal_agua);

    double getACaudal_max() const;
    void setACaudal_max(double newACaudal_max);
};

#endif // ACEQUIA_H
