#pragma once
#include "ChannelOwnerModule.h"
#include "ModuleVersionCheck.h"
#include "MemoryAllocator.h"
#if defined(OPENKNX_WEBSERVER) && (defined(KNX_IP_WIFI) || defined(KNX_IP_LAN))
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

#if defined(OPENKNX_WEBSERVER) && (defined(KNX_IP_WIFI) || defined(KNX_IP_LAN))
    NetworkBridgeConfigPage _configPage;
#endif
};

extern NetworkBridgeModule openknxNetworkBridgeModule;
