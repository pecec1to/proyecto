#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "acequia.h"
#include "alberca.h"
#include "lluvia.h"
#include "valvula.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QTimer>

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

private slots:

    void paso_simulador();


    void mostrarAlberca();

    void mostrarAcequia(double nivelm);

    void mostrarValvula(bool activado);

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

private:
    Ui::MainWindow *ui;
    QTimer *timerSimulacion;

    //Escena/imagen
    QGraphicsLineItem *albercaIzq;
    QGraphicsLineItem *albercaDer;
    QGraphicsLineItem *albercaSuelo;
    QGraphicsRectItem *espacioLluvia;
    QGraphicsItemGroup *group;

    QGraphicsRectItem *acequiaDibujo;

    QGraphicsPolygonItem *valvulaTring;
    QGraphicsRectItem *valvulaRect;

    QGraphicsEllipseItem *lluviaDibujo;

    QGraphicsScene *scene;
    QGraphicsView *view;

    Acequia* acequia;
    Alberca* alberca;
    Lluvia* lluvia;
    Valvula* valvula;
};
#endif // MAINWINDOW_H
