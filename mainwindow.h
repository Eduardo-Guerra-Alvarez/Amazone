#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "user.h"
#include "product.h"
#include "grafo.h"
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include "mainwidget.h"
#include <algorithm> //Para el Slot ordenar la lista
#include <queue> //Para la Cola prioridad
#include <QHash>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void addGrafo();
    void showGrafo(QString id);
    bool searchGrafo(QString grafo, QString arista);

    void clear2();

private slots:
    void on_emailLE_textChanged(const QString &arg1);
    void on_passwordLE_textChanged(const QString &arg1);
    void on_loginPB_clicked();
    void on_newNameLE_textChanged(const QString &arg1);
    void on_newMailLE_textChanged(const QString &arg1);
    void on_newPasswordLE_textChanged(const QString &arg1);
    void on_newUserPB_clicked();
    void on_CreatePB_clicked();
    void openFile();
    void on_cbOpciones_activated(int index);
    void on_cbOrdenar_activated(int index);
    void on_buscarLE_textEdited(const QString &arg1);
    //Funcion que recibe parametros de la coneccion entre Widget y la ventana principal
    void addProduct(int sold, QString id);


private:
    Ui::MainWindow *ui;
    vector<User> users;
    vector<Product> products;
    QMenu *fileMenu;
    QAction *openFileAction;
    QString fileName;
    QJsonArray jsonArray;
    QJsonArray jsonArrProducts;
    QFile dbFile;
    MainWindow *m;
    User us;
    //Inicializamos la cola, indicando, de que sera y como debe comportarse
    priority_queue <Product, vector<Product>, Product::comparador> colaPri;
    //Inicializamos el grafo
    QHash <QString, QHash<QString, int> > grafo;
    QHash <QString, int> arista;

    void enableLoginPB();
    void enableCreatePB();
    void loadDB();
    void saveDB();
    void llenarWidget(int item = 0, int order = 0, QString bus = "");
    void llenarVentana2();
    QString fecha;
};

#endif // MAINWINDOW_H
