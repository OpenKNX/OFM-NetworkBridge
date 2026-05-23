#pragma once
#include "OpenKNX.h"
#include <string>

class NetworkBridgeFunction : public OpenKNX::Channel
{
    std::string _name;

protected:
    NetworkBridgeFunction(uint8_t channelIndex, const char* name);

public:
    void setup() override;
    const std::string name() override;

    virtual void readInputKos() = 0;
    virtual void initMissingInputValues() = 0;
    virtual bool processCommand(const std::string cmd, bool diagnoseKo);
};
