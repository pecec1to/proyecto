#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "acequia.h"
#include "alberca.h"
#include "lluvia.h"
#include "valvula.h"
#include "basededatos.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QTimer>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    void valuesConfig();

    double surface(double radius);

    void albercaCambio(double nivel);

    void mostrarAcequia();

    void mostrarAlberca();

    void mostrarValvula(bool activado);

    void CrearAlberca();

    void AlbercaCambiada(QString nombre);

private slots:

    void paso_simulador();

    void on_pushButton_start_clicked();

    void on_pushButton_pause_clicked();

    void on_radioButton_1x_clicked();

    void on_radioButton_10x_clicked();

    void on_radioButton_100x_clicked();

    void on_radioButton_250x_clicked();

    void on_pushButton_reset_clicked();

    void on_horizontalSlider_acequia_sliderMoved(int position);

    void on_horizontalSlider_lluvia_sliderMoved(int position);

    void on_pushButton_abrir_clicked();

    void on_pushButton_cerrar_clicked();

    void on_doubleSpinBox_albercaMax_valueChanged(double arg1);

    void on_doubleSpinBox_albercaInit_valueChanged(double arg1);

    void on_doubleSpinBox_albercaArea_valueChanged(double arg1);

    void on_doubleSpinBox_acequiaMax_valueChanged(double arg1);

    void on_doubleSpinBox_acequiaInit_valueChanged(double arg1);

    void on_doubleSpinBox_lluviaInit_valueChanged(double arg1);

    void on_doubleSpinBox_valvulaRadio_valueChanged(double arg1);

    void on_pushButton_Cargar_clicked();

    void on_pushButton_Guardar_clicked();

    void albercaCreada(configuracion &c);

private:
    Ui::MainWindow *ui;
    QTimer *timerSimulacion;

    //Escena/imagen
    QGraphicsLineItem *acequiaIzq;
    QGraphicsLineItem *acequiaDer;
    QGraphicsLineItem *acequiaSuelo;
    QGraphicsRectItem *espacioLluvia;
    QGraphicsItemGroup *group;

    QGraphicsRectItem *albercaDibujo;

    QGraphicsPolygonItem *valvulaTring;
    QGraphicsRectItem *valvulaRect;

    QGraphicsEllipseItem *lluviaDibujo;

    QGraphicsScene *scene;
    QGraphicsView *view;

    Acequia* acequia;
    Alberca* alberca;
    Lluvia* lluvia;
    Valvula* valvula;

    BaseDeDatos db;

    QList<configuracion *> listaAlberca;

    configuracion * albercaActiva = nullptr;

     bool dbConectada = false;
};
#endif // MAINWINDOW_H
