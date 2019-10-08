#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QPixmap>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::Insert(QPixmap pix, QString texto, double precio)
{
    Agregar(pix,texto, precio);
}

void MainWidget::Agregar(QPixmap pixmap, QString string, double precio)
{
    int w, h;
    //Se saca el tamaño del Label
    w = ui->lbImagen->width();
    h = ui->lbImagen->height();
    //Se asigna la imagen con su tamaño correspondiente e ignorando los espacios en blanco
    ui->lbImagen->setPixmap(pixmap.scaled(w,h, Qt::KeepAspectRatio));
    ui->lbTexto->setText(string);
    ui->lbPrice->setText("$" + QString::number(precio));
}
