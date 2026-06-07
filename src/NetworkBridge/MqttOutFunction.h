#pragma once
#include "NetworkBridgeFunction.h"
#include <string>

class MqttOutFunction : public NetworkBridgeFunction
{
public:
    explicit MqttOutFunction(uint8_t channelIndex);
    void setup() override;
    void readInputKos() override;
    void initMissingInputValues() override;
    void processInputKo(GroupObject& ko) override;
    void loop() override;

private:
    std::string _topic;
    uint8_t _dpt = 0;
    uint8_t _qos = 0;
    bool _retain = false;
    bool _devicePrefix = false;
    bool _asJson = false;
};
