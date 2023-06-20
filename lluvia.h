#ifndef LLUVIA_H
#define LLUVIA_H


class Lluvia
{
private:
    double lluvia_caudal;

public:
    Lluvia();
    Lluvia(double lluvia_caudal);

    double getLluvia_caudal() const;
    void setLluvia_caudal(double newLluvia_caudal);
};


#endif // LLUVIA_H
