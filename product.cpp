#include "product.h"

Product::Product()
{

}

void Product::setID(const QString &value)
{
    id = value;
}

QString Product::getID() const
{
    return id;
}

void Product::setDescription(const QString &value)
{
    description = value;
}

QString Product::getDescription() const
{
    return description;
}

void Product::setPrice(const double &value)
{
    price = value;
}

double Product::getPrice() const
{
    return price;
}

void Product::setSold(const int &value)
{
    sold = value;
}

int Product::getSold() const
{
    return sold;
}

bool Product::operator >(Product const &value) const
{
    return price > value.price;
}

bool Product::operator >=(const Product &value) const
{
    return price >= value.price;
}

bool Product::operator <(Product const &value) const
{
    return price < value.price;
}

bool Product::operator <=(const Product &value) const
{
    return price <= value.price;
}




