#include "audiosamplesgetter.h"
#include <QMessageBox>
#include <QObject>
#include <string>
#include <QAudioInput>
#include <QAudioOutput>

#include <QTime>
#include <QDebug>

AudioSamplesGetter::AudioSamplesGetter()
{
    this->isCurrentlyPlaying    = false;
    this->isMuted               = true;
    this->echoSound             = false;

    inputDataBuffer = new QByteArray(AUDIO_IN_BUFFER_SIZE, 0);
    capturingStream = new QBuffer(this->inputDataBuffer);
    capturingStream->open(QIODevice::ReadWrite);
    audioInDevice = new QAudioInput();
    audioOutDevice = new QAudioOutput();

    memset(inputDataBuffer->data(), 0, AUDIO_IN_BUFFER_SIZE);
}

AudioSamplesGetter::~AudioSamplesGetter()
{
    delete inputDataBuffer;
    delete capturingStream;
    delete audioInDevice;
    delete audioOutDevice;
}



void AudioSamplesGetter::setAudioSamplesSender(AudioSamplesSender* newAudioSamplesSender)
{
    audioSender = newAudioSamplesSender;
}


void AudioSamplesGetter::setInputAudioDevice(QAudioInput* newAudioInputDev)
{
    this->audioInDevice = newAudioInputDev;
}

void AudioSamplesGetter::connectDeviceWithBuf()
{
    connect(capturingStream, SIGNAL(readyRead()), this, SLOT(onSamplesCaptured()));
}

void AudioSamplesGetter::setOutputAudioDevice(QAudioOutput* newAudioOutputDev)
{
    this->audioOutDevice = (newAudioOutputDev);
}

void AudioSamplesGetter::setMuteEnabled(bool isMuted)
{
    this->isMuted = isMuted;
}

void AudioSamplesGetter::setEchoEnabled(bool isOn)
{
    this->echoSound = isOn;
}

void AudioSamplesGetter::startSampling()
{
    if(!isCurrentlyPlaying)
    {
        inputDataBuffer->fill(0, inputDataBuffer->length());
        audioInDevice->setBufferSize(AUDIO_IN_BUFFER_SIZE);
        audioInDevice->start(capturingStream);
        isCurrentlyPlaying = true;
    }
}

void AudioSamplesGetter::stopSampling()
{
    if(isCurrentlyPlaying)
    {
        audioInDevice->stop();
        isCurrentlyPlaying = false;
    }
}


void AudioSamplesGetter::onSamplesCaptured()
{
    this->capturingStream->seek(0);
    qDebug()<<QTime::currentTime()<<this->capturingStream->bytesAvailable() <<this->capturingStream->size();
    this->audioSender->sendSamples(this->capturingStream);
}

void AudioSamplesGetter::playEchoedSamples(int leftSample, int rightSample)
{

}

bool AudioSamplesGetter::isPlaying()
{
    return this->isCurrentlyPlaying;
}

