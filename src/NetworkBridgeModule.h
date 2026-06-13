#pragma once
#include "ChannelOwnerModule.h"
#include "ModuleVersionCheck.h"
#include "MemoryAllocator.h"
#ifdef OPENKNX_WEBSERVER
    #include "NetworkBridge/NetworkBridgeConfigPage.h"
#endif

class NetworkBridgeModule : public NTBChannelOwnerModule
{
public:
 static void* operator new(size_t size)
    {
        return HS_MALLOC(size);
    }
    NetworkBridgeModule() = default;
    const std::string name() override { return "NetworkBridgeModule"; }
    const std::string version() override;
    void setup(bool configured) override;

protected:
    OpenKNX::Channel* createChannel(uint8_t _channelIndex /* this parameter is used in macros, do not rename */) override;

#ifdef OPENKNX_WEBSERVER
    NetworkBridgeConfigPage _configPage;
#endif
};

extern NetworkBridgeModule openknxNetworkBridgeModule;
