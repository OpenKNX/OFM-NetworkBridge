#pragma once
#include "NetworkBridgeFunction.h"

class MqttOutFunction : public NetworkBridgeFunction
{
public:
    explicit MqttOutFunction(uint8_t channelIndex);
    const char* typeName() const override { return "MQTT senden"; }
    void setup() override;
    void processInputKo(GroupObject& ko) override;
    void loop() override;
    uint8_t configFields(const ConfigFileField*& out) const override;
    const char* fieldEtsValue(uint8_t fieldIndex) const override;
};
