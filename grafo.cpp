#include "grafo.h"

Grafo::Grafo()
{

}

QString Grafo::getId() const
{
    return id;
}

void Grafo::setId(const QString &value)
{
    id = value;
}

int Grafo::getPeso() const
{
    return peso;
}

void Grafo::setPeso(int value)
{
    peso = value;
}

bool Grafo::operator >(Grafo const &value) const
{
    return peso > value.peso;
}

bool Grafo::operator >=(const Grafo &value) const
{
    return peso >= value.peso;
}

bool Grafo::operator <(Grafo const &value) const
{
    return peso < value.peso;
}

bool Grafo::operator <=(const Grafo &value) const
{
    return peso <= value.peso;
}
