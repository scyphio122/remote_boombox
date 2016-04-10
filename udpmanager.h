#ifndef UDPMANAGER_H
#define UDPMANAGER_H

#include "receiveddatagramprocessor.h"

class UdpManager
{
private:
    ReceivedDatagramProcessor* datagramProc;
public:
    UdpManager();
    void setDatagramProc(ReceivedDatagramProcessor* datagramProcessor);
};

#endif // UDPMANAGER_H
