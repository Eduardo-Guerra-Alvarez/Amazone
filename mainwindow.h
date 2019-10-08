#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "user.h"
#include "product.h"
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include "mainwidget.h"
#include <algorithm>

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

private:
    Ui::MainWindow *ui;
    vector<User> users;
    vector<Product> products;
    QMenu *fileMenu;
    QAction *openFileAction;
    QString fileName;
    QJsonArray jsonArray;
    QFile dbFile;
    MainWindow *m;

    void enableLoginPB();
    void enableCreatePB();
    void loadDB();
    void saveDB();
    void llenarWidget(int item = 0, int order = 0, QString bus = "");
    void setFileName(QString fileName);
    QString getFileName();
};

#endif // MAINWINDOW_H
