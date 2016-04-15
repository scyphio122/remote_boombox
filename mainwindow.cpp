#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QList>
#include <QAudioDeviceInfo>
#include <QString>
#include <QTimer>
#include <QPainter>
#include <QPixmap>
#include "audiosamplesgetter.h"
#include "audiosamplessender.h"
#include "commandssender.h"
#include "udpmanager.h"
#include "receiveddatagramprocessor.h"
#include "audiosamplesplayer.h"
#include "commandreceiver.h"
#include "graphicsvisualizer.h"
#include "fftcalculator.h"
#include "complex.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    audioSamplesGetter.setAudioSamplesSender(&audioSamplesSender);
    audioSamplesSender.setUdpManager(&udpManager);
    commandsSender.setUdpManager(&udpManager);
    udpManager.setDatagramProc(&receivedDataProc);

    displayAudioInDevices();

    pixmap = new QPixmap(ui->lB_visualization->width(), ui->lB_visualization->height());
    painter = new QPainter(pixmap);
    brush = new QBrush(QColor(255,255,255));
    pen = new QPen(QColor(255, 0, 255));
    painter->setBrush(*brush);
    painter->setPen(*pen);
    this->fft = new FftCalculator();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete pixmap;
}

void MainWindow::setFftCalculator(FftCalculator* fft)
{
    this->fft = fft;
}


void MainWindow::fftTest()
{
    static int cnt = 1;
    const int   size = 1024;
    double*     sampleArray = new double[size];
    fft->setInputArray(sampleArray);
    fft->setInputArraySize(size);
    fft->setOutputArraySize(fft->getInputArraySize());

    inputFreq = 10*cnt;
    generateTestSin(inputFreq, sampleArray, 0, 1, size);
    fft->runTransform();

    visualizeFFT();
    redrawFlag = true;
    drawingDelay(1000);
    if(cnt++ == 5)
        cnt = 1;
}

void MainWindow::visualizeFFT()
{
    update();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    if(redrawFlag == true)
    {

        double step = (double)(ui->lB_visualization->width() - 30)/(fft->getOutputArraySize()/2);
        ui->lB_visualization->clear();
        double maxVal = 0;
        painter->setBrush(QBrush(QColor(255,255,255)));
        painter->drawRect(0,0, ui->lB_visualization->width(), ui->lB_visualization->height());
        painter->setBrush(*brush);
        double coord = 0;
        if(fft->getOutputArray() != nullptr)
        {
            for(uint32_t i=0; i<fft->getOutputArraySize()/2; i++)
            {

                Complex fftElement = fft->getOutputElement(i);
                double  fftValue = fftElement.getMagnitude();
                if(fftElement.getMagnitude() > maxVal)
                    maxVal = fftValue;
                coord = 15 + i*step;
                painter->drawRect(15 + i*step, 0, step, 5*fftValue);
            }
        }
        coord = (double)(ui->lB_visualization->width() - 30)/2;
        int size = 1024;
        if(fft->getInputArray() != nullptr)
        {
            for(uint32_t i=0; i<size/2; i++)
            {
                double  inputSample = fft->getInputArray()[i];

                coord = coord + 1;
                painter->drawRect(coord, 300, 1, 50*inputSample);
            }
        }

        ui->lB_visualization->setPixmap(*pixmap);

        redrawFlag = false;

        delete[] fft->getOutputArray();
        delete[] fft->getInputArray();
        fft->setInputArray(nullptr);
        fft->setOutputArray(nullptr);
    }

}

void MainWindow::drawingDelay(uint32_t time_ms)
{
    QTimer::singleShot((int)time_ms, this, SLOT(fftTest()));
}

void MainWindow::runFftTest()
{
    fftTest();
}

void MainWindow::generateTestSin(double freq, double* dataOut, double xStart, double xEnd, uint32_t sampleNumber)
{
    double omega = 2*M_PI*freq;
    double x = xStart;
    double interpolator = (xEnd - xStart)/sampleNumber;
    for(uint32_t i=0; i<sampleNumber; i++)
    {
        dataOut[i] = (sin(omega*x));//+0.2*sin(10*omega*x))*0.2*sin(15*omega*x);
        x += interpolator;
    }
}



void MainWindow::displayAudioInDevices()
{
    QList<QAudioDeviceInfo> audioInDev = this->audioSamplesGetter.listAvailableDevicesIn();
    int listSize = audioInDev.size();

    ui->cb_inputAudioDevice->clear();
    for(int i=0; i < listSize; i++)
    {
        QString devName = audioInDev[i].deviceName();
        ui->cb_inputAudioDevice->addItem(devName);
    }
}

void MainWindow::on_pB_startStopSampling_clicked()
{
    if(!audioSamplesGetter.isPlaying())
    {
        audioSamplesGetter.startSampling();
        ui->pB_startStopSampling->setText("Stop Playing");
    }
    else
    {
         audioSamplesGetter.stopSampling();
         ui->pB_startStopSampling->setText("Start Playing");
    }
}

void MainWindow::on_cb_inputAudioDevice_activated(const QString &arg1)
{
    audioSamplesGetter.audioMixerDeviceInit(arg1.toStdString());
    ui->pB_startStopSampling->setEnabled(true);
}
