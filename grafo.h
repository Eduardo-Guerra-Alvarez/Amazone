#ifndef GRAFO_H
#define GRAFO_H

#include <QString>

class Grafo
{
public:
    Grafo();

    bool operator >(Grafo const &value) const;
    bool operator >= (Grafo const &value) const;
    bool operator <(Grafo const &value) const;
    bool operator <=(Grafo const &value) const;

    //Para la cola prioridad
    struct comparador {
        bool operator()(Grafo &p1, Grafo &p2) {
            return p1.getPeso() < p2.getPeso();
        }
    };
    QString getId() const;
    void setId(const QString &value);

    int getPeso() const;
    void setPeso(int value);

private:
    QString id;
    int peso;
};

#endif // GRAFO_H
