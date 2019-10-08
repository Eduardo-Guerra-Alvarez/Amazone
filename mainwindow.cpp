#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QDebug>
#include "mainwidget.h"
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fileMenu = ui->menuBar->addMenu(tr("&Archivo"));
    openFileAction = new QAction(tr("Abrir Archivo"), this);
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));
    fileMenu->addAction(openFileAction);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    fileName = QFileDialog::getOpenFileName(this, "DataBase", "", "Archivos JSON (*.json)");
    setFileName(fileName);
    if (fileName.length() > 0)
    {
        dbFile.setFileName(fileName);
        loadDB();
    }
}

void MainWindow::setFileName(QString fileName)
{
    this->fileName = fileName;
}

QString MainWindow::getFileName()
{
    return fileName;
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
      jsonObj["user"] = jsonArray;
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
                ui->emailLE->clear();
                ui->passwordLE->clear();
                ui->stackedWidget->setCurrentIndex(2);
                llenarWidget(0);
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
    jsonArray = jsonObj["products"].toArray();

    for (int i = 0;i < jsonArray.size();i++) {
        QJsonObject jobj = jsonArray.at(i).toObject();
        Product p;
        p.setID(jobj["id"].toString());
        p.setDescription(jobj["name"].toString());
        p.setPrice(jobj["price"].toDouble());
        if (item == 0)
        {
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

    if (order == 1)
        sort(products.begin(), products.end());

    else if (order == 2)
        sort(products.begin(), products.end(), greater<Product>());

    QString imagen, texto;
    double precio;
    for (int i = 0;i < products.size();i++) {
        imagen = products.at(i).getID();
        texto = products.at(i).getDescription();
        precio = products.at(i).getPrice();
        //Se creaa un nuevo QWidged cada vez
        MainWidget *mv = new MainWidget;
        //Se le asigna el tamaño minimo
        mv->setMinimumSize(250,250);
        QPixmap pix("/Users/lalo/Desktop/Amazone/imgs/" + imagen);
        mv->Insert(pix,texto, precio);
        ui->auxGrild->minimumSize();
        ui->auxGrild->addWidget(mv, i/2, i%2, Qt::Alignment());

        /*if (texto.contains(bus, Qt::CaseInsensitive) == true) {


        }*/

    }
    //Se recorre el array
    /*for (int i = 0;i < jsonArray.size(); i++) {
        //Se crea un nuevo objeto y se pasan los datos del array
        QJsonObject jobj = jsonArray.at(i).toObject();
        imagen = jobj["id"].toString();
        //Se creaa un nuevo QWidged cada vez
        MainWidget *mv = new MainWidget();
        //Se le asigna el tamaño minimo
        mv->setMinimumSize(250,250);
        //Compara si el combobox esta en la posicion 0
        //En este caso mostrara todos
        if (item == 0)
        {
            texto = jobj["name"].toString();
            precio = jobj["price"].toDouble();
            //Se crea un pixmap con la direccion de las imagenes
            QPixmap pix("/Users/lalo/Desktop/Amazone/imgs/" + imagen);
            //Se le agregan los datos al objeto
            mv->Insert(pix,texto, precio);
            //Se establece que acepte el tamaño minimo
            ui->auxGrild->minimumSize();
            //Se agregan los widget en la posicion indicada
            ui->auxGrild->addWidget(mv, i/2, i%2, Qt::Alignment());
        }
        //Si el combobox esta en la posicion 1
        else if (item == 1){
            //En el JsonArray buscara aquellas que inicien con "AB"
            if(imagen.startsWith("AB") == true){
                //Si es asi pasa los datos
                texto = jobj["name"].toString();
                precio = jobj["price"].toDouble();
                QPixmap pix("/Users/lalo/Desktop/Amazone/imgs/" + imagen);
                mv->Insert(pix,texto, precio);
                ui->auxGrild->minimumSize();
                ui->auxGrild->addWidget(mv, i/2, i%2, Qt::Alignment());
            }
        }
        //Si esta en la posicion 2
        else if (item == 2){
            //Si inicia con "L"
            if(imagen.startsWith("L") == true){
                texto = jobj["name"].toString();
                precio = jobj["price"].toDouble();
                QPixmap pix("/Users/lalo/Desktop/Amazone/imgs/" + imagen);
                mv->Insert(pix,texto, precio);
                ui->auxGrild->minimumSize();
                ui->auxGrild->addWidget(mv, i/2, i%2, Qt::Alignment());
            }
        }
        //Si esta en la posicion 3
        else if (item == 3){
            //Si inicia con "E"
            if(imagen.startsWith("E") == true){
                texto = jobj["name"].toString();
                precio = jobj["price"].toDouble();
                QPixmap pix("/Users/lalo/Desktop/Amazone/imgs/" + imagen);
                mv->Insert(pix,texto, precio);
                ui->auxGrild->minimumSize();
                ui->auxGrild->addWidget(mv, i/2, i%2, Qt::Alignment());
            }
        }
        //Si esta en la posicion 4
        else if (item == 4){
            //Si inicia con "HC"
            if(imagen.startsWith("HC") == true){
                texto = jobj["name"].toString();
                precio = jobj["price"].toDouble();
                QPixmap pix("/Users/lalo/Desktop/Amazone/imgs/" + imagen);
                mv->Insert(pix,texto, precio);
                ui->auxGrild->minimumSize();
                ui->auxGrild->addWidget(mv, i/2, i%2, Qt::Alignment());
            }
        }
        //Si esta en la posicion 5
        else if (item == 5){
            //Si inicia con "D"
            if(imagen.startsWith("D") == true){
                texto = jobj["name"].toString();
                precio = jobj["price"].toDouble();
                QPixmap pix("/Users/lalo/Desktop/Amazone/imgs/" + imagen);
                mv->Insert(pix,texto, precio);
                ui->auxGrild->minimumSize();
                ui->auxGrild->addWidget(mv, i/2, i%2, Qt::Alignment());
            }
        }
    }*/
    //Se cierra el archivo
    dbFile.close();
}

//Funcion que detecta cual opcion esta seleccionada
void MainWindow::on_cbOpciones_activated(int index)
{
    //Se crea un nuevo MainWindow
    m = new MainWindow();
    //Se le asigna la direccion del archivo
    m->setFileName(getFileName());
    //Se le asigna el index en el que esta actualmente el combobox
    //Esto para que al abrir la nueva ventana este seleccionado ese
    m->ui->cbOpciones->setCurrentIndex(index);
    m->ui->cbOrdenar->setCurrentIndex(ui->cbOrdenar->currentIndex());
    //Se le indica donde debe abrir
    m->ui->stackedWidget->setCurrentIndex(2);
    //Se le pasan los datos del archivo
    m->dbFile.setFileName(m->getFileName());
    //Se llama a la funcion y manda el index en el que esta seleccionado
    m->llenarWidget(index, ui->cbOrdenar->currentIndex());
    //Se abre la ventana nueva
    m->show();
    //Se cierra la actual
    close();
}



void MainWindow::on_cbOrdenar_activated(int index)
{
    //Se crea un nuevo MainWindow
    m = new MainWindow();
    //Se le asigna la direccion del archivo
    m->setFileName(getFileName());
    //Se le asigna el index en el que esta actualmente el combobox
    //Esto para que al abrir la nueva ventana este seleccionado ese
    m->ui->cbOpciones->setCurrentIndex(ui->cbOpciones->currentIndex());
    m->ui->cbOrdenar->setCurrentIndex(index);
    //Se le indica donde debe abrir
    m->ui->stackedWidget->setCurrentIndex(2);
    //Se le pasan los datos del archivo
    m->dbFile.setFileName(m->getFileName());
    //Se llama a la funcion y manda el index en el que esta seleccionado
    m->llenarWidget(ui->cbOpciones->currentIndex(),index);
    //Se abre la ventana nueva
    m->show();
    //Se cierra la actual
    close();
}

void MainWindow::on_buscarLE_textEdited(const QString &arg1)
{
    //Se crea un nuevo MainWindow
    m = new MainWindow();
    //Se le asigna la direccion del archivo
    m->setFileName(getFileName());
    //Se le asigna el index en el que esta actualmente el combobox
    //Esto para que al abrir la nueva ventana este seleccionado ese
    m->ui->cbOpciones->setCurrentIndex(ui->cbOpciones->currentIndex());
    m->ui->cbOrdenar->setCurrentIndex(ui->cbOrdenar->currentIndex());
    m->ui->buscarLE->setText(arg1);
    //Se le indica donde debe abrir
    m->ui->stackedWidget->setCurrentIndex(2);
    //Se le pasan los datos del archivo
    m->dbFile.setFileName(m->getFileName());
    //Se llama a la funcion y manda el index en el que esta seleccionado
    m->llenarWidget(ui->cbOpciones->currentIndex(),ui->cbOrdenar->currentIndex(), arg1);
    //Se abre la ventana nueva
    m->show();
    //Se cierra la actual
    close();
}
