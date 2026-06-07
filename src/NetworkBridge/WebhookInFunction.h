#pragma once
#include "NetworkBridgeFunction.h"

class WebhookInFunction : public NetworkBridgeFunction
{
public:
    explicit WebhookInFunction(uint8_t channelIndex);
    void setup() override;
    void readInputKos() override;
    void initMissingInputValues() override;
    void processInputKo(GroupObject &ko) override;
    void loop() override;

private:
    volatile bool _triggered = false;
};
