#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "acequia.h"
#include "alberca.h"
#include "lluvia.h"
#include "valvula.h"
#include <QtMath>
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>


int T = 1000;
//double acequia_caudal;
//double acequia_caudalMax;
//double acequia_caudalInit;
//int alberca_nivel=alberca.getNivelReal;
//int alberca_nivelMax;
//double alberca_areaBase;
//double lluvia_caudal;
//double valvula_radio;

double Qdesague;
double Vacequia;
double Qacequia;
double Vlluvia;
double Qlluvia;
double Ventrada;
double Vsalida;
int retardo;

float T2;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

   alberca = new Alberca(ui->doubleSpinBox_albercaInit->value(),
                         ui->doubleSpinBox_albercaMax->value(),
                         ui->doubleSpinBox_albercaArea->value());

   lluvia = new Lluvia(ui->doubleSpinBox_lluviaInit->value());

   valvula = new Valvula(ui->doubleSpinBox_valvulaRadio->value());

   acequia = new Acequia(ui->horizontalSlider_acequia->value(),
                         ui->doubleSpinBox_acequiaMax->value());

    connect(ui->horizontalSlider_lluvia, &QSlider::valueChanged, this, &MainWindow::on_horizontalSlider_lluvia_sliderMoved);
    connect(ui->horizontalSlider_acequia, &QSlider::valueChanged, this, &MainWindow::on_horizontalSlider_acequia_sliderMoved);
    connect(ui->pushButton_abrir, SIGNAL(clicked()), this, SLOT(on_pushButton_abrir_clicked()));
    connect(ui->pushButton_cerrar, SIGNAL(clicked()), this, SLOT(on_pushButton_cerrar_clicked()));

    scene = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene);

    timerSimulacion = new QTimer();
    timerSimulacion->setInterval(T);
    connect(timerSimulacion, SIGNAL(timeout()), this, SLOT(paso_simulador()));


}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::paso_simulador()
{

    retardo++;
    Qacequia = acequia->getACaudal_agua();
    Qlluvia = lluvia->getLluvia_caudal();

    Vacequia = T * Qacequia;
    Vlluvia = T * Qlluvia;
    Ventrada = Vacequia + Vlluvia;

    if (valvula->getValvula_estado() == VALVULA_ABIERTA && retardo > 15)
    {        //se divide el nivel de la alberca entre 100 para pasarlo a metros
        Qdesague = (2 * 3.1415 * (valvula->getValvula_radio())) * sqrt(2 * 9.8 * (static_cast<double>(alberca->getNivel_real())/100.0));
    } else {
        Qdesague = 0;
    }
    Vsalida = T * Qdesague;

    //alberca nivel está en centimetros
    double nuevoNivel_real = alberca->getNivel_real() + 100 * ((Ventrada - Vsalida) / (alberca->getArea_base()));
    alberca->setNivel_real(nuevoNivel_real);

   // scene->clear();
    mostrarAcequia(nuevoNivel_real);
    mostrarAlberca();
    mostrarValvula(valvula->getValvula_estado());

    qreal waterLevel = static_cast<qreal>(nuevoNivel_real) / (alberca->getNivel_max());
    qreal waterHeight = waterLevel * (alberca->getArea_base());
    qreal viewHeight = ui->graphicsView->viewport()->size().height();
    qreal waterY = viewHeight - waterHeight;
    QRectF waterRect(0, waterY, (alberca->getArea_base()), waterHeight);
    QGraphicsRectItem* waterItem = new QGraphicsRectItem(waterRect);
    waterItem->setBrush(Qt::blue);
    waterItem->setPen(Qt::NoPen);

    //scene->addItem(waterItem);
    if (nuevoNivel_real<240){
    ui->label_nivelTotal->setText("Nivel: " + QString::number(nuevoNivel_real) + "cm");
    }else{
     ui->label_nivelTotal->setText("Nivel: 240cm");

    }

    ui->label_caudalsalida->setText("Caudal de salida: " + QString::number(Qdesague) + "m^3/s");
    ui->Qcaudal->setText("Vsalida: " + QString::number(Vsalida) + "m^3/s");
    ui->Qdesague->setText("Ventrada: " + QString::number(Ventrada) + "m^3/s");
    ui->Qacequia->setText("Qacequiamax: " + QString::number(acequia->getACaudal_max()) + "m^3/s");

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

    //alberca
    double Spinbox_albercamax = ui->doubleSpinBox_albercaMax->value();
    alberca->setNivel_max(Spinbox_albercamax);
    double Spinbox_albercaint = ui->doubleSpinBox_albercaInit->value();
    alberca->setNivel_init(Spinbox_albercaint);

    //acequia
    double Spinbox_acequiainit= ui->doubleSpinBox_acequiaInit->value();
    acequia->setACaudal_agua(Spinbox_acequiainit);

    double Spinbox_acequiamax= ui->doubleSpinBox_acequiaMax->value();
    acequia->setACaudal_max(Spinbox_acequiamax);


   // mostrarAlberca();
   // mostrarAcequia(0);
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

void MainWindow::on_pushButton_reset_clicked()
{
    ui->pushButton_reset->setEnabled(1);
    T2=0;
    double valorSpinBoxvalvula= ui-> doubleSpinBox_valvulaRadio->value();
    valvula->setValvula_radio(valorSpinBoxvalvula);

    double valorSpinBoxbase= ui->doubleSpinBox_albercaArea->value();
    alberca->setArea_base(valorSpinBoxbase);

    double valorSpinBoxacaudal = ui->doubleSpinBox_acequiaMax->value();
    acequia->setACaudal_max(valorSpinBoxacaudal);

    double valorSpinboxamax = ui->doubleSpinBox_albercaMax->value();
    alberca->setNivel_max(valorSpinboxamax);

    Qdesague=0;
    Qacequia=0;
    Qlluvia=0;

    alberca->setNivel_real(0);
    scene->clear();
    mostrarAlberca();
    mostrarValvula(valvula->getValvula_estado());
}

void MainWindow::on_horizontalSlider_acequia_sliderMoved(int position)
{
    Qacequia = (acequia->getACaudal_max())* static_cast<double>(position) /100000.0;
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
    retardo = 0;
    ui->label_valvula->setText("Válvula abierta");
}

void MainWindow::on_pushButton_cerrar_clicked()
{
    valvula->setValvula_estado(VALVULA_CERRADA);
    ui->label_valvula->setText("Válvula cerrada");

}

void MainWindow::mostrarAlberca()
{

    QPen contorno;
    QBrush relleno;

    contorno.setColor(QColor(0,0,0));
    contorno.setWidth(1); // grosor
    contorno.setStyle(Qt::PenStyle::SolidLine);

    relleno.setColor(QColor(255,255,255));
    relleno.setStyle(Qt::BrushStyle::SolidPattern);

    albercaIzq = new QGraphicsLineItem;
    albercaDer = new QGraphicsLineItem;
    albercaSuelo = new QGraphicsLineItem;

    albercaIzq->setPen(contorno);
    albercaDer->setPen(contorno);
    albercaSuelo->setPen(contorno);


    albercaIzq->setLine(0.0,0.0,0.0,-(alberca->getNivel_max()));
    albercaDer->setLine((alberca->getArea_base()),0.0,(alberca->getArea_base()),-(alberca->getNivel_max()));
    albercaSuelo->setLine(0.0,0.0,alberca->getArea_base(),0.0);

    contorno.setColor(QColor(255,255,255));

    espacioLluvia = new QGraphicsRectItem();
    espacioLluvia->setPen(contorno);
    espacioLluvia->setBrush(relleno);


  //  espacioLluvia->setRect(0.0,-251.0, 300.0,-50.0);
    // Create a group and add the rectangles to it
       QGraphicsItemGroup *group = new QGraphicsItemGroup();
       group->addToGroup(albercaIzq);
       group->addToGroup(albercaDer);
        group->addToGroup(albercaSuelo);
       // Add the group to the scene
       scene->addItem(group);

}

void MainWindow::mostrarAcequia(double nivelm)
{
    QPen contorno;
    QBrush relleno;

    contorno.setColor(QColor(173,216,230));
    contorno.setWidth(1); // grosor
    contorno.setStyle(Qt::PenStyle::NoPen);


    //QColor(173,216,230)
    relleno.setColor(QColor(173,216,230));
    relleno.setStyle(Qt::BrushStyle::SolidPattern);

    QGraphicsRectItem *acequiaDibujo = new QGraphicsRectItem();

    acequiaDibujo->setPen(contorno);
    acequiaDibujo->setBrush(relleno);

if ( nivelm<=240){
    acequiaDibujo->setRect(1.0,0.0, (alberca->getArea_base())-1,-nivelm);
}else{
    acequiaDibujo->setRect(1.0,0.0, (alberca->getArea_base())-1,-240);
}

    QGraphicsItemGroup *group2 = new QGraphicsItemGroup();
    group2->addToGroup(acequiaDibujo);

    // Add the group to the scene
    scene->addItem(group2);


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


    //QColor(173,216,230)

    QPolygonF forma;

    if  (activado)  relleno.setColor(QColor(199,0,57)); // si esta activado se pone en verde
    else  relleno.setColor(QColor(39,174,96));  // si no se pone en rojo

    relleno.setStyle(Qt::BrushStyle::SolidPattern);

    //QpointF

   //forma.setPolygon();

    //forma.append(QPointF(int x, int y));


    forma << QPointF((alberca->getArea_base())+35.0, 10.0) << QPointF((alberca->getArea_base())+65.0, 10.0) << QPointF((alberca->getArea_base())+35.0, -20.0) << QPointF((alberca->getArea_base())+65.0, -20.0);
    //forma << QPointF(335.0, 10.0) << QPointF(365.0, 10.0) << QPointF(335.0, -20.0) << QPointF(365.0, -20.0);

    forma.isClosed();
   //~QAbstractGraphicsShapeItem( ¿?);

    valvulaTring = new QGraphicsPolygonItem;

    valvulaTring->setPen(contorno);
    valvulaTring->setBrush(relleno);
    valvulaTring->setPolygon(forma);   //para decirle que sea un triangulo



    scene->addItem(valvulaRect);
    scene->addItem(valvulaTring);

}

