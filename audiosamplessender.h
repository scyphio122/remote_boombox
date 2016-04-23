#ifndef AUDIOSAMPLESSENDER_H
#define AUDIOSAMPLESSENDER_H

#include "udpmanager.h"
#include <QBuffer>
#include "audiosamplesgetter.h"

class AudioSamplesGetter;

class AudioSamplesSender : public QObject
{
    Q_OBJECT
private:
    const QHostAddress ip       = QHostAddress::LocalHost;
    QString            ipText;
    const int receiverPort      = 8002;
    UdpManager* udpManager;

public:
    AudioSamplesSender();
    AudioSamplesSender(AudioSamplesGetter* parent = 0);
    void setUdpManager(UdpManager* udpManager);
    void sendSamples(QBuffer *sampleArray);


signals:
    void emitSendSamplesSignal(UdpDatagram* datagram);
};

#endif // AUDIOSAMPLESSENDER_H
