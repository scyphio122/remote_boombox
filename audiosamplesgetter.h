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


    AudioSamplesSender*                         audioSender         = nullptr;
    QAudioInput*                                audioInDevice       = nullptr;
    QAudioOutput*                               audioOutDevice      = nullptr;
    QBuffer*                                    capturingStream     = nullptr;
    QByteArray*                                 inputDataBuffer     = nullptr;
    bool                                        isMuted;
    bool                                        echoSound;
    bool                                        isCurrentlySampling;

    void playEchoedSamples();

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
    const static int                            AUDIO_IN_BUFFER_SIZE = 8192;

    AudioSamplesGetter();
    ~AudioSamplesGetter();

    void setAudioSamplesSender(AudioSamplesSender* newAudioSamplesSender);
    void connectDeviceWithBuf();


//    bool audioMixerDeviceInit(std::string audioDeviceName);

signals:
    void isSamplingSignal(bool isSampling);

};

#endif // AUDIOSAMPLESGETTER_H
