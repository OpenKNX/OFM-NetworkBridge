#pragma once
#include "NetworkBridgeFunction.h"
#include <IPAddress.h>

class PingFunction : public NetworkBridgeFunction
{
public:
    explicit PingFunction(uint8_t channelIndex);

    const char* typeName() const override { return "Ping"; }
    void setup() override;
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
