#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>

class Product
{
public:
    Product();
    void setID(const QString &value);
    QString getID() const;
    void setDescription(const QString &value);
    QString getDescription() const;
    void setPrice(const double &value);
    double getPrice() const;
    void setSold(const int &value);
    int getSold() const;


    bool operator >(Product const &value) const;
    bool operator >= (Product const &value) const;
    bool operator <(Product const &value) const;
    bool operator <=(Product const &value) const;

    //Para la cola prioridad
    struct comparador {
        bool operator()(Product &p1, Product &p2) {
            return p1.getSold() < p2.getSold();
        }
    };
private:
    QString id;
    QString description;
    double price;
    int sold = 0;
};

#endif // PRODUCT_H
