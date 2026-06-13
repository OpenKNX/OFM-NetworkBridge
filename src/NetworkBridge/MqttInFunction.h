#pragma once
#include "NetworkBridgeFunction.h"
#include <string>

class MqttInFunction : public NetworkBridgeFunction
{
public:
    explicit MqttInFunction(uint8_t channelIndex);
    const char* typeName() const override { return "MQTT empfangen"; }
    void setup() override;
    void processInputKo(GroupObject& ko) override;
    void loop() override;
    uint8_t configFields(const ConfigFileField*& out) const override;
    const char* fieldEtsValue(uint8_t fieldIndex) const override;

private:
    uint8_t _dpt = 0;
    bool _jsonEnabled = false;
    volatile bool _hasPendingValue = false;
    char _pendingPayload[256] = {};

    void applyValue(const char* valueStr);
};
