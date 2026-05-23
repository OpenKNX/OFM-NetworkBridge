#pragma once
#include "NetworkBridgeFunction.h"

class PingFunction : public NetworkBridgeFunction
{
public:
    explicit PingFunction(uint8_t channelIndex);

    void setup(bool configured) override;
    void readInputKos() override;
    void initMissingInputValues() override;
    void handleKo(GroupObject &ko) override;
    void loop(bool configured) override;
    void loop() override;

private:
    bool _automaticPing = false;
    bool _pingRunning = false;
    uint32_t _nextAutomaticPingAt = 0;

    uint32_t pingIntervalMs() const;
    void triggerPing(bool manualTrigger);
    void scheduleNextAutomaticPing();
};
