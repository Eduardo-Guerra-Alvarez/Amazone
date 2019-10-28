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
    bool operator <(Product const &value) const;
private:
    QString id;
    QString description;
    double price;
    int sold = 0;
};

#endif // PRODUCT_H
