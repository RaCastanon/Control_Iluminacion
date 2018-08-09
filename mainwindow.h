#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//Included libraries
#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <QFileDialog>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <qstring.h>
#include "methods.h"

// Definitions
#define DEFAULT_TEST_POINT "180"
#define DEFAULT_COM_PORT "COM10"
#define DEFAULT_PWM_VALUE "50\n"
#define DEFAULT_SIZE_INTENSITY 50
#define DEFAULT_TIMER_ON 2000
#define DEFAULT_VIDEO_NAME "Ventana de Video"
#define DEFAULT_WAITKEY_TIME 100
#define DEFAULT_VIDEO_CAM_PORT 0
#define DEFAULT_SETPOINT 120
#define DEFAULT_SET_POINT "120"
#define TIMER_ENABLED "Temporalizador UNO habilitado"
#define TIMER_DISABLED "Temporalizador UNO deshabilitado"
#define VIDEO_ENABLED "Camara de video Habilitada"
#define VIDEO_DISABLED "Camara de video Deshabilitada"
#define CONNECTION_SUCESSFUL "Comunicacion serial establecida"
#define SERIAL_CLOSED "Puerto de comunicacion cerrado"
#define NAMED_WINDOW "Imagen Seleccionada"

#define SERIAL_COMM_ERROR "Error 0: No se logró abrir el puerto de comunicacion serial"
#define ERROR_PORT_OPEN "Error 1: Puerto de comunicacion serial en uso"
#define ERROR_TIMER_OPEN "Error 2: El temporalizador UNO esta Habilidato"
#define ERROR_TIMER_TOCLOSE "Error 3: El temporalizador UNO no esta Habilitado"
#define ERROR_SET_POINT "Error 4: Valor de referencia asignado por default"
#define ERROR_SERIAL_CLOSE "Error 5: El puerto de comunicacion serial no esta Habilitado"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_OpenImageButton_clicked();

    void on_closeApp_clicked();

    void on_grayScale_clicked();

    void on_middlePoints_clicked();

    void on_getIntensity_clicked();

    void on_openSerialPort_clicked();

    void on_closeSerialPort_clicked();

    void on_setTimer_clicked();

    void on_stopTimer_clicked();

    void on_openVideoCam_clicked();

    void onTimeOut();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
