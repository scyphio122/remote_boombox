#include "audiosamplesplayer.h"

AudioSamplesPlayer::AudioSamplesPlayer()
{
    this->audioOutputBuffer = new QBuffer(new QByteArray(new char[AUDIO_OUT_BUF_SIZE], AUDIO_OUT_BUF_SIZE));
}

AudioSamplesPlayer::~AudioSamplesPlayer()
{
    delete this->audioOutputBuffer;
}


void AudioSamplesPlayer::setAudioPlayer(QAudioOutput audioOutput)
{
    this->audioOutput.s
}

void AudioSamplesPlayer::onDataReceived(QByteArray* data)
{
    while(data->size() != 0)
    {
        audioOutput.
    }
}
