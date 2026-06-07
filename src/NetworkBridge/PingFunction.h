#pragma once
#include "NetworkBridgeFunction.h"
#include <IPAddress.h>

class PingFunction : public NetworkBridgeFunction
{
public:
    explicit PingFunction(uint8_t channelIndex);

    void setup() override;
    void readInputKos() override;
    void initMissingInputValues() override;
    void processInputKo(GroupObject &ko) override;
    void loop() override;

private:
    bool _pingRunning = false;
    IPAddress _targetAddress;
    unsigned long _startTimeStampForNextPing = 0;

    unsigned long pingIntervalMs() const;
    void triggerPing();
    void onPingResult(IPAddress ip, bool reachable);
    void scheduleNextAutomaticPing();
};
