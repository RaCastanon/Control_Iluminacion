#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;

// Global variables
QSerialPort serial;
QTimer *timerOne;
Mat inputImage;
Mat inputVideo;

unsigned int intensityLevel = 0;
bool serialFlag = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Configure timer one
    timerOne = new QTimer(this);
    connect(timerOne, SIGNAL(timeout()), this, SLOT(onTimeOut()));

    // Disable not required buttons
    ui->grayScale->setDisabled(true);
    ui->middlePoints->setDisabled(true);
    ui->getIntensity->setDisabled(true);

    ui->closeSerialPort->setEnabled(false);
    ui->openSerialPort->setEnabled(true);
    ui->setTimer->setEnabled(true);
    ui->stopTimer->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* Function name: OpenImageButton()
 * Developer:     Raul Castañon
 * Details:       Function used to open and show an image
 */
void MainWindow::on_OpenImageButton_clicked()
{
    QString inputImg = QFileDialog::getOpenFileName();
    inputImage = imread(inputImg.toLatin1().data());
    // Verify if there is an image
    if(!(inputImage.empty()))
    {
        // Show image
        namedWindow(NAMED_WINDOW, WINDOW_NORMAL);
        imshow(NAMED_WINDOW, inputImage);
        // Enable grayscale button
        ui->grayScale->setEnabled(true);
        // Disable OpenImage button
        ui->OpenImageButton->setEnabled(false);
    }
}

/* Function name: on_grayScale_clicked()
 * Developer:     Raul Castañon
 * Details:       Convert image in grayscale format
 */
void MainWindow::on_grayScale_clicked()
{
    if(false == inputImage.empty())
    {
        // Set image in grayscale format
        cvtColor(inputImage, inputImage, CV_RGB2GRAY);
        // Show result in same window
        imshow(NAMED_WINDOW, inputImage);
        ui->middlePoints->setEnabled(true);
        ui->grayScale->setEnabled(false);
    }
}

/* Function name: on_middlePoints_clicked()
 * Developer:     Raul Castañon
 * Details:       Get the middle points to process the calculation of intensity
 */
void MainWindow::on_middlePoints_clicked()
{
    // Get middle points of the img
    setMiddlePoints(inputImage.cols, inputImage.rows);
    // Put values on the GUI labels
    ui->P_one_cols->setText(QString::number(middlePoints[0][0]));
    ui->P_one_rows->setText(QString::number(middlePoints[0][1]));
    ui->P_two_cols->setText(QString::number(middlePoints[1][0]));
    ui->P_two_rows->setText(QString::number(middlePoints[1][1]));
    ui->P_three_cols->setText(QString::number(middlePoints[2][0]));
    ui->P_three_rows->setText(QString::number(middlePoints[2][1]));
    ui->P_four_cols->setText(QString::number(middlePoints[3][0]));
    ui->P_four_rows->setText(QString::number(middlePoints[3][1]));
    ui->P_five_cols->setText(QString::number(middlePoints[4][0]));
    ui->P_five_rows->setText(QString::number(middlePoints[4][1]));
    // Enable visualization of getIntensity button
    ui->getIntensity->setEnabled(true);
    // Disable current button
    ui->middlePoints->setEnabled(false);
}

/* Function name: on_getIntensity_clicked()
 * Developer:     Raul Castañon
 * Details:       Get intensity levels from the image
 */
void MainWindow::on_getIntensity_clicked()
{
    // Local variables
    Scalar promAreaOne, promAreaTwo, promAreaThree, promAreaFour, promAreaFive;
    Mat areaOne, areaTwo, areaThree, areaFour, areaFive;

    // Verify size of the area
    if((ui->widthArea->text().isEmpty())||(ui->hightArea->text().isEmpty()))
    {
        qDebug("Default size asigned");
        ui->widthArea->setText(QString::number(DEFAULT_SIZE_INTENSITY));
        ui->hightArea->setText(QString::number(DEFAULT_SIZE_INTENSITY));
    }
    // Get areas
    areaOne = inputImage(Rect(middlePoints[0][0], middlePoints[0][1], ui->widthArea->text().toInt(), ui->hightArea->text().toInt()));
    areaTwo = inputImage(Rect(middlePoints[1][0], middlePoints[1][1], ui->hightArea->text().toInt(), ui->hightArea->text().toInt()));
    areaThree = inputImage(Rect(middlePoints[2][0], middlePoints[2][1], ui->widthArea->text().toInt(), ui->hightArea->text().toInt()));
    areaFour = inputImage(Rect(middlePoints[3][0], middlePoints[3][1], ui->widthArea->text().toInt(), ui->hightArea->text().toInt()));
    areaFive = inputImage(Rect(middlePoints[4][0], middlePoints[4][1], ui->widthArea->text().toInt(), ui->hightArea->text().toInt()));
    // Get intensity level per area
    promAreaOne = mean(areaOne);
    promAreaTwo = mean(areaTwo);
    promAreaThree = mean(areaThree);
    promAreaFour = mean(areaFour);
    promAreaFive = mean(areaFive);
    // Show results
    ui->intensityLevel_1->setText(QString::number(promAreaOne[0]));
    ui->intensityLevel_2->setText(QString::number(promAreaTwo[0]));
    ui->intensityLevel_3->setText(QString::number(promAreaThree[0]));
    ui->intensityLevel_4->setText(QString::number(promAreaFour[0]));
    ui->intensityLevel_5->setText(QString::number(promAreaFive[0]));
    // Get intensity level mean
    intensityLevel = (unsigned int)((promAreaOne[0] + promAreaTwo[0] + promAreaThree[0] + promAreaFour[0] + promAreaFive[0]) / 5.0);
    ui->intensityLevel->setText(QString::number(intensityLevel));
    // Enable open Image button and disable getIntensity
    ui->OpenImageButton->setEnabled(true);
    ui->getIntensity->setEnabled(false);
}

/* Function name: openSerialPort()
 * Developer:     Raul Castañon
 * Details:       Configure and open the serialPort
 */
void MainWindow::on_openSerialPort_clicked()
{
    if(false == serialFlag)
    {
        // Set configuration conditions
        serial.setPortName(DEFAULT_COM_PORT);
        serial.setBaudRate(QSerialPort::Baud9600);
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setFlowControl(QSerialPort::NoFlowControl);
        // Check if the port has been open
        if(serial.open(QIODevice::ReadWrite))
        {
            qDebug(CONNECTION_SUCESSFUL);
            serial.write(DEFAULT_PWM_VALUE);
            serialFlag = true;
            ui->openSerialPort->setEnabled(false);
            ui->closeSerialPort->setEnabled(true);
        }
        //if not send error message
        else
        {
            qDebug(SERIAL_COMM_ERROR);
            ui->closeSerialPort->setEnabled(false);
            ui->openSerialPort->setEnabled(true);
        }
    }
    else
    {
        qDebug(ERROR_PORT_OPEN);
    }
}

/* Function name: closeSerialPort()
 * Developer:     Raul Castañon
 * Details:       Close serial port
 */
void MainWindow::on_closeSerialPort_clicked()
{
    // Verify serial port condition
    if(serial.isOpen())
    {
        serial.close();
        serialFlag = false;
        qDebug(SERIAL_CLOSED);
        ui->closeSerialPort->setEnabled(false);
        ui->openSerialPort->setEnabled(true);
    }
    else
    {
        // Do nothing
        qDebug(ERROR_PORT_OPEN);
    }
}

/* Function name: closeApp()
 * Developer:     Raul Castañon
 * Details:       Close application
 */
void MainWindow::on_closeApp_clicked()
{
    // Check if we need to stop timer
    if(true == timerOne->isActive())
    {
        timerOne->stop();
    }
    // Check if we need to close SerialPort
    if(true == serialFlag)
    {
        MainWindow::on_closeSerialPort_clicked();
    }
    close();
}

/* Function name: setTimer()
 * Developer:     Raul Castañon
 * Details:       Start timer one
 */
void MainWindow::on_setTimer_clicked()
{
    // Verify if timer one is not active
    if(!(timerOne->isActive()))
    {
        qDebug(TIMER_ENABLED);
        timerOne->start(DEFAULT_TIMER_ON);
        ui->setTimer->setEnabled(false);
        ui->stopTimer->setEnabled(true);
        ui->openSerialPort->click();
    }
    else
    {
        qDebug(ERROR_TIMER_OPEN);
    }
}

/* Function name: stopTimer()
 * Developer:     Raul Castañon
 * Details:       Stop timer one
 */
void MainWindow::on_stopTimer_clicked()
{
    // Verify timer one is active
    if(timerOne->isActive())
    {
        qDebug(TIMER_DISABLED);
        timerOne->stop();
        ui->setTimer->setEnabled(true);
        ui->stopTimer->setEnabled(false);
        ui->closeSerialPort->click();
        destroyWindow(NAMED_WINDOW);
    }
    else
    {
        qDebug(ERROR_TIMER_TOCLOSE);
    }
}

/* Function name: on_openVideoCam_clicked()
 * Developer:     Raul Castañon
 * Details:       Open video camera and show continuosly
 */
void MainWindow::on_openVideoCam_clicked()
{
    VideoCapture videoCam;
    videoCam.open(DEFAULT_VIDEO_CAM_PORT);
    if(videoCam.isOpened())
    {
        qDebug(VIDEO_ENABLED);
        do
        {
            videoCam >> inputVideo;
            imshow(DEFAULT_VIDEO_NAME, inputVideo);
            waitKey(DEFAULT_WAITKEY_TIME);
            ui->openVideoCam->setEnabled(false);
            if(waitKey(30) > 0)
            {
                videoCam.release();
                destroyWindow(DEFAULT_VIDEO_NAME);
                qDebug(VIDEO_DISABLED);
                ui->openVideoCam->setEnabled(true);
                break;
            }
        }while(true);
    }
}

/* Function name: onTimeOut()
 * Developer:     Raul Castañon
 * Details:       Linker to the timeOut signal from timerOne
 */
void MainWindow::onTimeOut()
{
    // Local variables
    VideoCapture videoCam;
    unsigned int setPoint = 0;
    unsigned int ut = 0;
    char u_t[3];

    // Open videoCamera
    videoCam.open(0);
    if(videoCam.isOpened())
    {
        // Get image
        videoCam >> inputImage;
        // Process Image
        ui->grayScale->clicked();
        ui->middlePoints->clicked();
        ui->getIntensity->clicked();
        // Verify setPoint
        if((ui->setPoint->text().toInt() > 0) && (ui->setPoint->text().toInt() <= 255))
        {
            // Get set point
            setPoint = (unsigned int) ui->setPoint->text().toInt();
        }
        else
        {
            qDebug(ERROR_SET_POINT);
            setPoint = DEFAULT_SETPOINT;
            ui->setPoint->setText(DEFAULT_SET_POINT);
        }
        // Send params to the controller
        ut = FuzzyCtl(intensityLevel, setPoint);

        /* ut is the control signal, it has been calculated in the limits of a byte
         * need to normalize to the mcu needs
         * maximum value that mcu will receive will be 100 (this will be the 100% of the PWM cycle
         * minimum value will be 5.
         * 255 -> 100
         * ut  -> x
         */
        ut = 100 - (unsigned int)((ut * 100) / 255);
        itoa(ut, u_t, 10);
        u_t[2] = '\n';
        // Verify if serial port is open before writting
        if(serial.isOpen())
        {
            serial.write(u_t);
            qDebug(u_t);
        }
        else
        {
            qDebug(ERROR_SERIAL_CLOSE);
            ui->stopTimer->clicked();
        }
    }
}
