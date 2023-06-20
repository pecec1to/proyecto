#include "lluvia.h"

double Lluvia::getLluvia_caudal() const
{
    return lluvia_caudal;
}

void Lluvia::setLluvia_caudal(double newLluvia_caudal)
{
    lluvia_caudal = newLluvia_caudal;
}

Lluvia::Lluvia()
{

}

Lluvia::Lluvia(double lluvia_caudal)
{
    setLluvia_caudal(lluvia_caudal);
}
