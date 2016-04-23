#ifndef AUDIOSAMPLESGETTER_H
#define AUDIOSAMPLESGETTER_H

#include <QtMultimedia/QAudioInput>
#include <QtMultimedia/QAudioOutput>
#include <QBuffer>
#include "audiosamplessender.h"

class AudioSamplesSender;
class AudioSamplesGetter : public QObject
{
    Q_OBJECT

private:
    const static int                            AUDIO_IN_BUFFER_SIZE = 8192;

    AudioSamplesSender*                         audioSender;
    QAudioInput*                                audioInDevice;
    QAudioOutput*                               audioOutDevice;
    QBuffer*                                    capturingStream;
    QByteArray*                                 inputDataBuffer;
    bool                                        isMuted;
    bool                                        echoSound;
    bool                                        isCurrentlySampling;

    void playEchoedSamples(int leftSample, int rightSample);

public slots:
    void init();
    void setInputAudioDevice(QAudioInput* newAudioInputDev);
    void setOutputAudioDevice(QAudioOutput* newAudioOutputDev);
    void onSamplesCaptured();
    void setMuteEnabled(bool isMuted);
    void setEchoEnabled(bool isOn);
    void startSampling(bool value);
    void isSampling();
public:
    AudioSamplesGetter();
    ~AudioSamplesGetter();

    void setAudioSamplesSender(AudioSamplesSender* newAudioSamplesSender);
    void connectDeviceWithBuf();


//    bool audioMixerDeviceInit(std::string audioDeviceName);

signals:
    void isSamplingSignal(bool isSampling);

};

#endif // AUDIOSAMPLESGETTER_H
