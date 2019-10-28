#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QDebug>
#include "mainwidget.h"
#include <QGridLayout>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fileMenu = ui->menuBar->addMenu(tr("&Archivo"));
    openFileAction = new QAction(tr("Abrir Archivo"), this);
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));
    fileMenu->addAction(openFileAction);
    fecha = QDateTime::currentDateTime().toString("dd/MM/yy HH:mm:ss");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    fileName = QFileDialog::getOpenFileName(this, "DataBase", "", "Archivos JSON (*.json)");
    if (fileName.length() > 0)
    {
        dbFile.setFileName(fileName);
        loadDB();
    }
}



void MainWindow::loadDB()
{
    QByteArray data;
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;

    //Abrimos el archivo
    dbFile.open(QIODevice::ReadOnly);
    //Leemos los bytes
    data = dbFile.readAll();
    //Creamos el documento json a partir de los bytes
    jsonDoc = QJsonDocument(QJsonDocument::fromJson(data));
    //Creamos el objeto json a partir del documento
    jsonObj = jsonDoc.object();
    //Asignamos informacion al arreglo de objetos json
    jsonArray = jsonObj["users"].toArray();
    jsonArrProducts = jsonObj["producs"].toArray();
    //Cerramos el archivo
    dbFile.close();
}

void MainWindow::saveDB()
{
      QJsonObject jsonObj;
      QJsonDocument jsonDoc;
      //Arbir el archivo como modo escritura
      dbFile.open(QIODevice::WriteOnly);
      //Guarda el arreglo de objetos en un objeto Json
      jsonObj["users"] = jsonArray;
      jsonObj["products"] = jsonArrProducts;
      //Crea un documento Json a partir de un objeto Json
      jsonDoc = QJsonDocument(jsonObj);
      //Escribe al archivo el documento en formato json
      dbFile.write(jsonDoc.toJson());
      dbFile.close();
}

void MainWindow::enableLoginPB()
{
    /// Nota en C++ una '\' diagonal invertida se debe poner doble '\\'
    /// para que no se tome como cadena
    QRegularExpression re("^[a-zA-Z0-9._-]+@[a-zA-Z]+(\\.[a-z]{2,4}){1,3}$");

    //Comparamos que tenga texto tanto en la caja email como password
    if (ui->emailLE->text().length() > 0 && ui->passwordLE->text().length() > 0)
    {
        //comparamos el correo que coincida con la exprecion regular
        QRegularExpressionMatch match = re.match(ui->emailLE->text());
        //si retorna verdadero habilitamos el boton
        if (match.hasMatch() == true)
            ui->loginPB->setEnabled(true);
        else
            ui->loginPB->setEnabled(false);
    }
    else {
        //en otro caso no
        ui->loginPB->setEnabled(false);
    }
}

//Funcion para habilitar el boton de crear usuario
void MainWindow::enableCreatePB()
{
    QRegularExpression re("^[a-zA-Z0-9._-]+@[a-zA-Z]+(\\.[a-z]{2,4}){1,3}$");
    if (ui->newMailLE->text().length() > 0 && ui->newPasswordLE->text().length() > 0
            && ui->newNameLE->text().length() > 0)
    {
        QRegularExpressionMatch match = re.match(ui->newMailLE->text());
        if (match.hasMatch() == true)
            ui->CreatePB->setEnabled(true);
        else
           ui->CreatePB->setEnabled(false);
    }
    else
        ui->CreatePB->setEnabled(false);
}


void MainWindow::on_emailLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableLoginPB();
}

void MainWindow::on_passwordLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableLoginPB();
}

void MainWindow::on_newNameLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableCreatePB();
}

void MainWindow::on_newMailLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableCreatePB();
}

void MainWindow::on_newPasswordLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableCreatePB();
}

void MainWindow::on_newUserPB_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

//Funcon para comprobar o agregar un usuario
void MainWindow::on_CreatePB_clicked()
{
    QByteArray data;
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    //Abrimos el archivo
    dbFile.open(QIODevice::ReadOnly);
    //Leemos los bytes
    data = dbFile.readAll();
    //Creamos el documento json a partir de los bytes leidos
    jsonDoc = QJsonDocument(QJsonDocument::fromJson(data));
    //Creamos el objeto json a partir del documento
    jsonObj = jsonDoc.object();
    //Asignamos informacion al arreglo de objetos json
    jsonArray = jsonObj["users"].toArray();
    QMessageBox message;
    int i = 0;
    for (;i < jsonArray.size(); i++) {
        QJsonObject jobj = jsonArray.at(i).toObject();
        //correo ya existe
        if (jobj["email"].toString() == ui->newMailLE->text())
        {
            message.setText("El usuario ya existe"); //caja de texto
            message.setIcon(QMessageBox::Warning);//icono en la caja de texto
            break;//rompe
        }
    }
    dbFile.close();

    if (i == jsonArray.size()) // si no existe
    {
        QJsonObject jsonObj;
        jsonObj["name"] = ui->newNameLE->text();
        jsonObj["email"] = ui->newMailLE->text();
        jsonObj["password"] = ui->newPasswordLE->text();
        jsonArray.append(jsonObj);
        saveDB();

        ui->newNameLE->clear();
        ui->newMailLE->clear();
        ui->newPasswordLE->clear();
        ui->stackedWidget->setCurrentIndex(0); //Mandamos a la pagina de inicio
        message.setText("Usuario registrado con éxito");
        message.setIcon(QMessageBox::Information);
    }
    message.exec(); //para ejecutarlo
}

//Funcion para entrar en caso de que si estes registrado
void MainWindow::on_loginPB_clicked()
{
    QMessageBox message;
    QByteArray data;
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    //Abrimos el archivo
    dbFile.open(QIODevice::ReadOnly);
    //Leemos los bytes
    data = dbFile.readAll();
    //Creamos el documento json a partir de los bytes leidos
    jsonDoc = QJsonDocument(QJsonDocument::fromJson(data));
    //Creamos el objeto json a partir del documento
    jsonObj = jsonDoc.object();
    //Asignamos informacion al arreglo de objetos json
    jsonArray = jsonObj["users"].toArray();

    int i = 0;

    //Recorre el array
    for (; i < jsonArray.size(); i++)
    {
        //Se crea un nuevo objeto y se pasan los datos del array
        QJsonObject jobj = jsonArray.at(i).toObject();
        //Se muestra el valor del ojeto y se compara con lo escrito
        if  (jobj["email"].toString() == ui->emailLE->text())
        {
            if  (jobj["password"].toString() == ui->passwordLE->text())
            {
                us.setEmail(ui->emailLE->text());
                ui->emailLE->clear();
                ui->passwordLE->clear();
                ui->stackedWidget->setCurrentIndex(2);
                llenarWidget();
                break;
            }
            else {
                message.setText("Usuario y/o contraseña incorrectos");
                message.setIcon(QMessageBox::Critical);
                message.exec();
                break;
            }
        }
    }
    //Cerramos el archivo
    dbFile.close();

    //Si llega al final es que el usuario no existe
    if (i == jsonArray.size())
    {
        message.setText("El usuario no existe");
        message.setIcon(QMessageBox::Warning);
        message.exec();
    }
}
//Funcion para llenar el scrollArea con las imagenes y descripciones
void MainWindow::llenarWidget(int item, int order, QString bus)
{
    //Se cierra el archivo en caso de que este abierto
    dbFile.close();
    QByteArray data;
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    //Abrimos el archivo
    dbFile.open(QIODevice::ReadOnly);
    //Leemos los bytes
    data = dbFile.readAll();
    //Creamos el documento json a partir de los bytes leidos
    jsonDoc = QJsonDocument(QJsonDocument::fromJson(data));
    //Creamos el objeto json a partir del documento
    jsonObj = jsonDoc.object();
    //Asignamos informacion al arreglo de objetos json
    jsonArrProducts = jsonObj["products"].toArray();

    for (int i = 0;i < jsonArrProducts.size();i++) {
        QJsonObject jobj = jsonArrProducts.at(i).toObject();
        Product p;
        p.setID(jobj["id"].toString());
        p.setDescription(jobj["name"].toString());
        p.setPrice(jobj["price"].toDouble());
        p.setSold(jobj["sold"].toInt());
        //Comprueba que la palabra a buscar coincida con la descripcion
        //sin importar mayusculas o minusculas
        if (p.getDescription().contains(bus, Qt::CaseInsensitive) == true){
            if (item == 0)
            {
                //Guarda los productos filtrados en el vector
                products.push_back(p);
            }
            //Si el combobox esta en la posicion 1
            else if (item == 1){
                //En el JsonArray buscara aquellas que inicien con "AB"
                if(p.getID().startsWith("AB") == true){
                    products.push_back(p);
                }
            }
            //Si esta en la posicion 2
            else if (item == 2){
                //Si inicia con "L"
                if(p.getID().startsWith("L") == true){
                    products.push_back(p);
                }
            }
            //Si esta en la posicion 3
            else if (item == 3){
                //Si inicia con "E"
                if(p.getID().startsWith("E") == true){
                    products.push_back(p);
                }
            }
            //Si esta en la posicion 4
            else if (item == 4){
                //Si inicia con "HC"
                if(p.getID().startsWith("HC") == true){
                    products.push_back(p);
                }
            }
            //Si esta en la posicion 5
            else if (item == 5){
                //Si inicia con "D"
                if(p.getID().startsWith("D") == true){
                    products.push_back(p);
                }
            }
        }
    }

    //En caso de ser 1 se ordena de menor a mayor
    if (order == 1)
        //Con la funcion sort se ordena el vector
        sort(products.begin(), products.end());
    //En caso de ser 2 se ordena de mayor a menor
    else if (order == 2)
        sort(products.begin(), products.end(), greater<Product>());

    QString imagen, texto;
    double precio;
    int sold;
    //Se recorre el vector
    size_t i = 0;
    for (;i < products.size();i++) {
        imagen = products.at(i).getID();
        texto = products.at(i).getDescription();
        precio = products.at(i).getPrice();
        sold = products.at(i).getSold();
        //Se creaa un nuevo QWidged cada vez
        MainWidget *mv = new MainWidget;
        mv->setID(imagen);
        mv->setDescription(texto);
        mv->setPrice(precio);
        mv->setSold(sold);
        //Se le asigna el tamaño minimo
        mv->setMinimumSize(250,250);
        //Se le da la ruta de la imagen a insertar
        QPixmap pix("/Users/lalo/Desktop/Amazone/imgs/" + imagen);
        //Se mandan los datos al widget
        mv->Insert(pix,texto, precio);
        ui->auxGrild->minimumSize();
        /*
         * Antes de llenar el Widget se conecta la señal que se envio
         * con el slot de mainWindow
         */
        connect(mv, SIGNAL(added(int,QString)), this, SLOT(addProduct(int,QString)));
        //Se establece en que posicion ira
        ui->auxGrild->addWidget(mv, i/2, i%2, Qt::Alignment());
    }
    //Se cierra el archivo
    dbFile.close();
    //Limpia el vector
    products.clear();
}

//Funcion que detecta cual opcion esta seleccionada
void MainWindow::on_cbOpciones_activated(int index)
{
    //Se limpia el Gridlayout
    QLayoutItem* item;
    while ( ( item = ui->auxGrild->takeAt(0)) != nullptr )
    {
        delete item->widget();
        delete item;
    }
    //Se mandan los parametros para llenar el widget
    llenarWidget(index,ui->cbOrdenar->currentIndex(),ui->buscarLE->text());
}

void MainWindow::on_cbOrdenar_activated(int index)
{
    QLayoutItem* item;
    while ( ( item = ui->auxGrild->takeAt(0)) != nullptr )
    {
        delete item->widget();
        delete item;
    }
    llenarWidget(ui->cbOpciones->currentIndex(),index,ui->buscarLE->text());
}

void MainWindow::on_buscarLE_textEdited(const QString &arg1)
{
    QLayoutItem* item;
    while ( ( item = ui->auxGrild->takeAt(0)) != nullptr )
    {
        delete item->widget();
        delete item;
    }
    llenarWidget(ui->cbOpciones->currentIndex(),ui->cbOrdenar->currentIndex(),arg1);
}

//Slot que recibe la señal
void MainWindow::addProduct(int sold, QString id)
{
    QByteArray data;
    QJsonObject jsonObj;
    QJsonObject jsonObjTem;
    QJsonObject jsonOb;
    QJsonObject jsonObjF;
    QJsonDocument jsonDoc;
    QJsonArray jsonTemp;
    QJsonArray jsonArrFecha;
    bool encontrado = false;

    //Abrimos el archivo
    dbFile.open(QIODevice::ReadOnly);
    //Leemos los bytes
    data = dbFile.readAll();
    //Creamos el documento json a partir de los bytes leidos
    jsonDoc = QJsonDocument(QJsonDocument::fromJson(data));
    //Creamos el objeto json a partir del documento
    jsonObj = jsonDoc.object();
    //Asignamos informacion al arreglo de objetos json
    jsonArrProducts = jsonObj["products"].toArray();
    jsonArray = jsonObj["users"].toArray();
    dbFile.close();
    //Recorremos el arreglo de productos
    for (int i = 0; i < jsonArrProducts.size(); i++) {
        //Pasamos el objeto que esta en cierta posicion
        QJsonObject jsonObj = jsonArrProducts.at(i).toObject();
        //Si el id del objeto coincide con el recibido por la señal
        if  (jsonObj["id"].toString() == id){
            //Sumamos la cantidad con la ya establecida
            jsonObj["sold"] = jsonObj["sold"].toInt() + sold;
            //Remplazamos el objeto con los datos ya modificados
            jsonArrProducts.replace(i,jsonObj);
            saveDB();
        }
    }
    //Abrimos el archivo
    dbFile.open(QIODevice::ReadOnly);
    //Recorremos el arreglo de usuarios
    for (int i = 0; i < jsonArray.size();i++) {
        //Pasamoes el objeto que esta en cierta posicion
        QJsonObject jsonObj = jsonArray.at(i).toObject();
        //Comprobamos el email con el que se inicio sesion
        if  (jsonObj["email"].toString() == us.getEmail()) {
            //Asignamos informacion al arreglo de objetos json de shopping
            jsonTemp = jsonObj["purchase"].toArray();
            int j = 0;
            //Recorremos el arreglo shopping
            for (; j < jsonTemp.size(); i++) {
                //Pasamos los datos a un objeto
                QJsonObject jsonOb = jsonTemp.at(j).toObject();
                //Verificamos si el objeto esta vacio
                if (jsonOb.isEmpty() == false) {
                    //Si no esta vacio pasamos el arreglo de objetos json de fecha
                    jsonArrFecha = jsonOb[fecha].toArray();
                    int k = 0;
                    //Recorremos el arreglo de fecha
                    for (;k < jsonArrFecha.size(); k++) {
                        //Pasamos los datos a un objeto
                        QJsonObject jsonObjF = jsonArrFecha.at(k).toObject();
                        //Comparamos que el id del objeto coincida con el enviado por el slot
                        if (jsonObjF["id"] == id) {
                            //Modificamos en el array el objeto modificado
                            jsonArrFecha.replace(k,jsonObjF);
                            //Decimos que fue verdadero
                            encontrado = true;
                            //Rompemos el bucle
                            break;
                        }
                    }
                    //En caso de no ser encontrado
                    if (encontrado == false) {
                        //Pasamos el id y sold al objeto
                        jsonObjF["id"] = id;
                        //Agregamos el objeto al final
                        jsonArrFecha.append(jsonObjF);
                    }
                    //Guarda el arreglo de objetos en un objeto Json Fecha
                    jsonOb[fecha] = jsonArrFecha;
                    //Modificamos en elarray el objeto modificado
                    jsonTemp.replace(j,jsonOb);
                    //Guarda el arreglo de objetos en un objeto Json shopping
                    jsonObj["purchase"] = jsonTemp;
                    //Modificamos en elarray el objeto modificado
                    jsonArray.replace(i,jsonObj);
                    dbFile.close();
                    saveDB();
                    break;
                }
            }
            //En caso de que el temporal llegue al final
            if (j == jsonTemp.size()) {
                jsonArrFecha = jsonOb[fecha].toArray();
                int k = 0;
                for (;k < jsonArrFecha.size(); k++) {
                    QJsonObject jsonObjF = jsonArrFecha.at(k).toObject();
                    if (jsonObjF["id"] == id) {
                        jsonArrFecha.replace(k,jsonObjF);
                        encontrado = true;
                        break;
                    }
                }
                if (encontrado == false) {
                    jsonObjF["id"] = id;
                    jsonArrFecha.append(jsonObjF);
                }
                jsonOb[fecha] = jsonArrFecha;
                jsonTemp.append(jsonOb);
                jsonObj["purchase"] = jsonTemp;
                jsonArray.replace(i,jsonObj);
                dbFile.close();
                saveDB();
                break;
            }
        }
        else {
            dbFile.close();
        }
    }

}
