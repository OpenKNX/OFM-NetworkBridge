#pragma once
#include "ChannelOwnerModule.h"
#include "ModuleVersionCheck.h"

class NetworkBridgeModule : public NTBChannelOwnerModule
{
public:
    NetworkBridgeModule() = default;
    const std::string name() override { return "NetworkBridgeModule"; }
    const std::string version() override;
    void setup(bool configured) override;

protected:
    OpenKNX::Channel* createChannel(uint8_t _channelIndex /* this parameter is used in macros, do not rename */) override;
};

extern NetworkBridgeModule openknxNetworkBridgeModule;
