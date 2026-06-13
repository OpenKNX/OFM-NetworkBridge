#pragma once
#include "NetworkBridgeFunction.h"

class WebhookInFunction : public NetworkBridgeFunction
{
public:
    explicit WebhookInFunction(uint8_t channelIndex);
    const char* typeName() const override { return "Webhook empfangen"; }
    void setup() override;
    void processInputKo(GroupObject &ko) override;
    void loop() override;

private:
    volatile bool _triggered = false;
};
