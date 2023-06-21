#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "acequia.h"
#include "alberca.h"
#include "lluvia.h"
#include "valvula.h"
#include "basededatos.h"

#include <QtMath>
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>

int T = 1000;

double Qdesague;
double Vacequia;
double Qacequia;
double Vlluvia;
double Qlluvia;
double Ventrada;
double Vsalida;

float T2;

bool desborde = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

   alberca = new Alberca(ui->doubleSpinBox_albercaInit->value(),
                         ui->doubleSpinBox_albercaMax->value(),
                         ui->doubleSpinBox_albercaArea->value(),
                         ui->doubleSpinBox_albercaInit->value());

   lluvia = new Lluvia(ui->doubleSpinBox_lluviaInit->value());

   valvula = new Valvula(ui->doubleSpinBox_valvulaRadio->value());

   acequia = new Acequia(ui->horizontalSlider_acequia->value(),
                         ui->doubleSpinBox_acequiaMax->value());
   valvula->setValvula_estado(VALVULA_CERRADA);

    connect(ui->horizontalSlider_lluvia, &QSlider::valueChanged, this, &MainWindow::on_horizontalSlider_lluvia_sliderMoved);
    connect(ui->horizontalSlider_acequia, &QSlider::valueChanged, this, &MainWindow::on_horizontalSlider_acequia_sliderMoved);
    connect(ui->pushButton_abrir, SIGNAL(clicked()), this, SLOT(on_pushButton_abrir_clicked()));
    connect(ui->pushButton_cerrar, SIGNAL(clicked()), this, SLOT(on_pushButton_cerrar_clicked()));

    timerSimulacion = new QTimer();
    timerSimulacion->setInterval(T);
    connect(timerSimulacion, SIGNAL(timeout()), this, SLOT(paso_simulador()));
    Qacequia = acequia->getACaudal_agua();
    Qlluvia = lluvia->getLluvia_caudal();

    scene = new QGraphicsScene(this);

    albercaDibujo = new QGraphicsRectItem();

    ui->graphicsView->setScene(scene);
    mostrarAcequia();
    mostrarAlberca();
    mostrarValvula(valvula->getValvula_estado());


    dbConectada = db.conectar();
    if (dbConectada)
    {
        db.crearTablaAlberca();
        CrearAlberca();
    }
    else
    {
        QMessageBox::warning(this, "Base de datos no conectada.", "No ha sido posible conectarse a la base de datos.");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paso_simulador()
{
    desborde = false;
    Qacequia = (acequia->getACaudal_max()) * static_cast<double>(ui->horizontalSlider_acequia->value()) /100000.0;

    Vacequia = T * Qacequia;
    Vlluvia = T * Qlluvia;
    Ventrada = Vacequia + Vlluvia;
    Qdesague = (surface(valvula->getValvula_radio()) * (sqrt(2 * 9.8 * ((alberca->getNivel_real())/100.0))));

    if (alberca->getNivel_real() <= 0.0)
    {
        Qdesague = 0.0;
    }

    if (valvula->getValvula_estado() == VALVULA_ABIERTA)
    {
    Vsalida = T * Qdesague;
    }
    else
    {
        Vsalida = 0.0;
    }

    double nuevoNivel_real = alberca->getNivel_real() + 100 * ((Ventrada - Vsalida) / (alberca->getArea_base()));

    if (nuevoNivel_real <= 0)
    {
        nuevoNivel_real = 0;
    }
    else if (nuevoNivel_real > (alberca->getNivel_max()))
    {
        desborde = true;
        nuevoNivel_real=alberca->getNivel_max();
    }

    ui->label_nivelTotal->setText("Nivel: " + QString::number(nuevoNivel_real) + "cm");

    alberca->setNivel_real(nuevoNivel_real);

    ui->label_caudalsalida->setText("Caudal de salida: " + QString::number(Qdesague) + "m^3/s");
    ui->labelVsalida->setText("Vsalida: " + QString::number(Vsalida) + "m^3/s");
    ui->labelVentrada->setText("Ventrada: " + QString::number(Ventrada) + "m^3/s");
    ui->labelQacequia->setText("Qacequiamax: " + QString::number(acequia->getACaudal_max()) + "m^3/s");

    T2 = T2 + T/1000.0;
    ui->label_21->setText("Tiempo simulado: " + QString::number(T2)+ " s");

    if (valvula->getValvula_estado() == VALVULA_CERRADA)
    {
        ui->label_valvula->setText("Válvula cerrada");
    }
    else if (valvula->getValvula_estado() == VALVULA_ABIERTA)
    {
        ui->label_valvula->setText("Válvula abierta");
    }

    mostrarAcequia();
    albercaCambio(nuevoNivel_real);
    mostrarValvula(valvula->getValvula_estado());

    if (desborde == true)
    {
        QMessageBox::warning(this, "Alerta", "La alberca se está desbordando");
    }
}

void MainWindow::on_pushButton_start_clicked()
{
    timerSimulacion->start();
    ui->pushButton_start->setEnabled(false);
    ui->doubleSpinBox_acequiaInit->setEnabled(false);
    ui->doubleSpinBox_acequiaMax->setEnabled(false);
    ui->doubleSpinBox_albercaArea->setEnabled(false);
    ui->doubleSpinBox_albercaMax->setEnabled(false);
    ui->doubleSpinBox_albercaInit->setEnabled(false);
    ui->doubleSpinBox_lluviaInit->setEnabled(false);
    ui->doubleSpinBox_valvulaRadio->setEnabled(false);
    ui->pushButton_pause->setEnabled(true);
    ui->pushButton_reset->setEnabled(true);
    ui->horizontalSlider_acequia->setEnabled(true);
    ui->horizontalSlider_lluvia->setEnabled(true);

    double Spinboxvalvula= ui-> doubleSpinBox_valvulaRadio->value();
    valvula->setValvula_radio(Spinboxvalvula);

    double Spinboxbase= ui->doubleSpinBox_albercaArea->value();
    alberca->setArea_base(Spinboxbase);

    double valorSpinboxlluvia= ui->doubleSpinBox_lluviaInit->value();
    lluvia->setLluvia_caudal(valorSpinboxlluvia);

    double Spinbox_albercamax = ui->doubleSpinBox_albercaMax->value();
    alberca->setNivel_max(Spinbox_albercamax);
    double Spinbox_albercaint = ui->doubleSpinBox_albercaInit->value();
    alberca->setNivel_init(Spinbox_albercaint);
    ui->label_nivelTotal->setText("Nivel: " + QString::number(alberca->getNivel_real()) + "cm");

    double Spinbox_acequiainit= ui->doubleSpinBox_acequiaInit->value();
    acequia->setACaudal_agua(Spinbox_acequiainit);

    double Spinbox_acequiamax= ui->doubleSpinBox_acequiaMax->value();
    acequia->setACaudal_max(Spinbox_acequiamax);

    scene->clear();
    mostrarAcequia();
    mostrarAlberca();
    albercaCambio(alberca->getNivel_real());
    mostrarValvula(valvula->getValvula_estado());

}

void MainWindow::on_pushButton_pause_clicked()
{
    timerSimulacion->stop();
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_pause->setEnabled(false);
    ui->doubleSpinBox_acequiaInit->setEnabled(true);
    ui->doubleSpinBox_acequiaMax->setEnabled(true);
    ui->doubleSpinBox_albercaArea->setEnabled(true);
    ui->doubleSpinBox_albercaMax->setEnabled(true);
    ui->doubleSpinBox_albercaInit->setEnabled(true);
    ui->doubleSpinBox_lluviaInit->setEnabled(true);
    ui->doubleSpinBox_valvulaRadio->setEnabled(true);
    ui->horizontalSlider_acequia->setEnabled(false);
    ui->horizontalSlider_lluvia->setEnabled(false);
    albercaCambio(alberca->getNivel_real());
}

void MainWindow::on_pushButton_reset_clicked()
{
    ui->pushButton_reset->setEnabled(1);
    T2=0;
    Qdesague=0;
    Qacequia=0;
    Qlluvia=0;

    valuesConfig();
    scene->clear();
    ui->label_nivelTotal->setText("Nivel: " + QString::number(ui->doubleSpinBox_albercaInit->value()) + "cm");
    alberca->setNivel_init(ui->doubleSpinBox_albercaInit->value());
    mostrarAcequia();

    mostrarValvula(valvula->getValvula_estado());
}

void MainWindow::on_radioButton_1x_clicked()
{
    T = 1000;
}

void MainWindow::on_radioButton_10x_clicked()
{
    T = 100;
}

void MainWindow::on_radioButton_100x_clicked()
{
    T = 10;
}

void MainWindow::on_radioButton_250x_clicked()
{
    T = 4;
}

void MainWindow::on_horizontalSlider_acequia_sliderMoved(int position)
{
    Qacequia = (acequia->getACaudal_max()) * static_cast<double>(position) /100000.0;
    ui->label_acequiaPorcent->setText("Porcentaje: " + QString::number(position) + "%");
    ui->label_acequiaCaudal->setText("Caudal: " + QString::number(Qacequia) + " m^3/s");
}

void MainWindow::on_horizontalSlider_lluvia_sliderMoved(int position)
{
    Qlluvia = static_cast<double>(position) / 100000.0;
    ui->label_lluviaCaudal->setText("Caudal: " + QString::number(Qlluvia) + " m^3/s");
}

void MainWindow::on_pushButton_abrir_clicked()
{
    valvula->setValvula_estado(VALVULA_ABIERTA);
    ui->label_valvula->setText("Válvula abierta");
}

void MainWindow::on_pushButton_cerrar_clicked()
{
    valvula->setValvula_estado(VALVULA_CERRADA);
    ui->label_valvula->setText("Válvula cerrada");

}

void MainWindow::mostrarAcequia()
{
    QPen contorno;
    QBrush relleno;

    contorno.setColor(QColor(0,0,0));
    contorno.setWidth(1); // grosor
    contorno.setStyle(Qt::PenStyle::SolidLine);

    relleno.setColor(QColor(255,255,255));
    relleno.setStyle(Qt::BrushStyle::SolidPattern);

    acequiaIzq = new QGraphicsLineItem;
    acequiaDer = new QGraphicsLineItem;
    acequiaSuelo = new QGraphicsLineItem;

    acequiaIzq->setPen(contorno);
    acequiaDer->setPen(contorno);
    acequiaSuelo->setPen(contorno);

    acequiaIzq->setLine(0.0,0.0,0.0,-(alberca->getNivel_max()));
    acequiaDer->setLine((alberca->getArea_base()),0.0,(alberca->getArea_base()),-(alberca->getNivel_max()));
    acequiaSuelo->setLine(0.0,0.0,alberca->getArea_base(),0.0);

    contorno.setColor(QColor(255,255,255));

    espacioLluvia = new QGraphicsRectItem();
    espacioLluvia->setPen(contorno);
    espacioLluvia->setBrush(relleno);

    QGraphicsItemGroup *group = new QGraphicsItemGroup();
    group->addToGroup(acequiaIzq);
    group->addToGroup(acequiaDer);
    group->addToGroup(acequiaSuelo);

    scene->addItem(group);

}

void MainWindow::mostrarAlberca()
{
    QPen contorno;
    QBrush relleno;

    contorno.setColor(QColor(173,216,230));
    contorno.setWidth(1); // grosor
    contorno.setStyle(Qt::PenStyle::NoPen);


    //QColor(173,216,230)
    relleno.setColor(QColor(173,216,230));
    relleno.setStyle(Qt::BrushStyle::SolidPattern);

    albercaDibujo = new QGraphicsRectItem();

    albercaDibujo->setPen(contorno);
    albercaDibujo->setBrush(relleno);

    albercaDibujo->setRect(1.0,0.0, (alberca->getArea_base()-1),0);

    QGraphicsItemGroup* group2 = new QGraphicsItemGroup();
    group2->addToGroup(albercaDibujo);
    scene->addItem(group2);

}

void MainWindow::albercaCambio(double nivel)
{
    if ((nivel <= alberca->getNivel_max()) && (nivel > 0))
    {
        albercaDibujo->setRect(1.0, 0.0, (alberca->getArea_base() - 1), -nivel);
    }
    else if (nivel <= 0)
    {
        albercaDibujo->setRect(1.0, 0.0, (alberca->getArea_base() - 1), 0);
    }
    scene->update();
}

void MainWindow::mostrarValvula(bool activado)
{
    QPen contorno;
    QBrush relleno;

    contorno.setColor(QColor(5,5,5));

    relleno.setColor(QColor(102,51,0));
    relleno.setStyle(Qt::BrushStyle::SolidPattern);

    valvulaRect = new QGraphicsRectItem();
    valvulaRect->setPen(contorno);
    valvulaRect->setBrush(relleno);

    valvulaRect->setRect((alberca->getArea_base()),0.0, 100.0,-10.0);

    contorno.setColor(QColor(80,16,110));
    contorno.setWidth(1); // grosor
    contorno.setStyle(Qt::PenStyle::SolidLine);

    QPolygonF forma;

    if  (activado)  relleno.setColor(QColor(199,0,57)); // si esta activado se pone en verde
    else  relleno.setColor(QColor(39,174,96));  // si no se pone en rojo

    relleno.setStyle(Qt::BrushStyle::SolidPattern);

    forma << QPointF((alberca->getArea_base())+35.0, 10.0) << QPointF((alberca->getArea_base())+65.0, 10.0) << QPointF((alberca->getArea_base())+35.0, -20.0) << QPointF((alberca->getArea_base())+65.0, -20.0);

    forma.isClosed();

    valvulaTring = new QGraphicsPolygonItem;

    valvulaTring->setPen(contorno);
    valvulaTring->setBrush(relleno);
    valvulaTring->setPolygon(forma);   //para decirle que sea un triangulo

    scene->addItem(valvulaRect);
    scene->addItem(valvulaTring);

}

void MainWindow::on_doubleSpinBox_albercaMax_valueChanged(double arg1)
{
    alberca->setNivel_max(arg1);
}


void MainWindow::on_doubleSpinBox_albercaInit_valueChanged(double arg1)
{
    alberca->setNivel_init(arg1);
    alberca->setNivel_real(arg1);
    ui->label_nivelTotal->setText("Nivel: " + QString::number(alberca->getNivel_init()) + "cm");
}


void MainWindow::on_doubleSpinBox_albercaArea_valueChanged(double arg1)
{
    alberca->setArea_base(arg1);
}


void MainWindow::on_doubleSpinBox_acequiaMax_valueChanged(double arg1)
{
    acequia->setACaudal_max(arg1);
}


void MainWindow::on_doubleSpinBox_acequiaInit_valueChanged(double arg1)
{
    acequia->setACaudal_agua(arg1);
}


void MainWindow::on_doubleSpinBox_lluviaInit_valueChanged(double arg1)
{
    lluvia->setLluvia_caudal(arg1);
}


void MainWindow::on_doubleSpinBox_valvulaRadio_valueChanged(double arg1)
{
    valvula->setValvula_radio(arg1);
}

void MainWindow::valuesConfig()
{
    alberca->setArea_base(ui->doubleSpinBox_albercaArea->value());
    alberca->setNivel_init(ui->doubleSpinBox_albercaInit->value());
    alberca->setNivel_max(ui->doubleSpinBox_albercaMax->value());
    alberca->setNivel_real(ui->doubleSpinBox_albercaInit->value());
    acequia->setACaudal_agua(ui->doubleSpinBox_acequiaInit->value());
    acequia->setACaudal_max(ui->doubleSpinBox_acequiaMax->value());
    lluvia->setLluvia_caudal(ui->doubleSpinBox_lluviaInit->value());
    valvula->setValvula_radio(ui->doubleSpinBox_valvulaRadio->value());
    valvula->setValvula_estado(VALVULA_CERRADA);
}

double MainWindow::surface(double radius)
{
    return (3.1415 * radius *2);
}

void MainWindow::on_pushButton_Cargar_clicked()
{
    QString nombre = ui->lineEdit_nombre->text();

    configuracion* c = db.leerAlberca(nombre);

    if (c == nullptr) {
        return;
    }

    if (albercaActiva != nullptr) {
        delete albercaActiva;
    }

    albercaActiva = c;

    AlbercaCambiada(c->getNombre());
    QMessageBox::information(this, "Éxito", "La alberca se ha cargado correctamente desde la base de datos.");
}

void MainWindow::on_pushButton_Guardar_clicked()
{
    configuracion c;

    c.setNombre(ui->lineEdit_nombre->text());
    c.setAc_caudal(ui->doubleSpinBox_acequiaInit->value());
    c.setAc_caudal_max(ui->doubleSpinBox_acequiaMax->value());
    c.setN_init(ui->doubleSpinBox_albercaInit->value());
    c.setC_lluvia(ui->doubleSpinBox_lluviaInit->value());
    c.setValvula_r(ui->doubleSpinBox_valvulaRadio->value());

    albercaCreada(c);
    qDebug().noquote().nospace() << "Configuración guardada correctamente en la base de datos.";

    QMessageBox::information(this, "Éxito", "La configuración se ha guardado correctamente.");
}

void MainWindow::CrearAlberca()
{
    if (!dbConectada) {
        return;
    }

    QList<QString>* lista = db.leerNombreAlberca();

    ui->lineEdit_nombre->setText("");
    ui->doubleSpinBox_acequiaInit->setValue(0);
    ui->doubleSpinBox_acequiaMax->setValue(0);
    ui->doubleSpinBox_albercaInit->setValue(0);
    ui->doubleSpinBox_lluviaInit->setValue(0);
    ui->doubleSpinBox_valvulaRadio->setValue(0);

    delete lista;
}

void MainWindow::albercaCreada(configuracion& c)
{
    if (!dbConectada) {
        return;
    }

    db.guardarAlberca(c);
    CrearAlberca();
}

void MainWindow::AlbercaCambiada(QString nombre)
{
    if (!dbConectada) {
        return;
    }
    if (albercaActiva == nullptr) {
        return;
    }
    configuracion* cn = db.leerAlberca(nombre);

    if (cn == nullptr) {
        return;
    }

    delete albercaActiva;
    albercaActiva = cn;

    ui->doubleSpinBox_acequiaInit->setValue(albercaActiva->getAc_caudal());
    ui->doubleSpinBox_acequiaMax->setValue(albercaActiva->getAc_caudal_max());
    ui->doubleSpinBox_albercaInit->setValue(albercaActiva->getN_init());
    ui->doubleSpinBox_lluviaInit->setValue(albercaActiva->getC_lluvia());
    ui->doubleSpinBox_valvulaRadio->setValue(albercaActiva->getValvula_r());
}
