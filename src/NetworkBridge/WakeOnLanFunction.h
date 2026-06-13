#pragma once
#include "NetworkBridgeFunction.h"

class WakeOnLanFunction : public NetworkBridgeFunction
{
public:
    explicit WakeOnLanFunction(uint8_t channelIndex);

    const char* typeName() const override { return "Wake on LAN"; }
    void setup() override;
    void processInputKo(GroupObject &ko) override;
    void loop() override;

private:
    uint8_t _mac[6] = {};
    uint8_t _packet[102] = {};

    bool parseMac(const char* str);
    void buildPacket();
    void sendMagicPacket();
};
