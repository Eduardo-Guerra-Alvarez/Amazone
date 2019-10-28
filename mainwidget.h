#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    void Insert(QPixmap pix, QString texto, double precio);
    void setID(const QString &value);
    QString getID() const;
    void setDescription(const QString &value);
    QString getDescription() const;
    void setPrice(const double &value);
    double getPrice() const;
    void setSold(const int &value);
    int getSold() const;

private slots:
    void on_comprarPB_clicked();
signals:
    //Señal que se envia hacia mainWindow
    void added(int sold, QString id);

private:
    Ui::MainWidget *ui;
    void Agregar(QPixmap pixmap, QString string, double precio);
    QString id;
    QString description;
    double price;
    int sold = 0;
};

#endif // MAINWIDGET_H
