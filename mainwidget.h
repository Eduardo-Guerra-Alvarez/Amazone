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

private:
    Ui::MainWidget *ui;
    void Agregar(QPixmap pixmap, QString string, double precio);
};

#endif // MAINWIDGET_H
