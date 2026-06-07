#pragma once
#include "NetworkBridgeFunction.h"
#include <string>

class MqttInFunction : public NetworkBridgeFunction
{
public:
    explicit MqttInFunction(uint8_t channelIndex);
    void setup() override;
    void readInputKos() override;
    void initMissingInputValues() override;
    void processInputKo(GroupObject& ko) override;
    void loop() override;

private:
    uint8_t _dpt = 0;
    bool _jsonEnabled = false;
    std::string _jsonPath;
    volatile bool _hasPendingValue = false;
    char _pendingPayload[256] = {};

    void applyValue(const char* valueStr);
};
