#include "NetworkBridgeModule.h"
#include "NetworkBridge/PingFunction.h"
#include "NetworkBridge/WakeOnLanFunction.h"
#include "NetworkBridge/WebhookInFunction.h"
#include "NetworkBridge/WebhookOutFunction.h"
#include "knxprod.h"

const std::string NetworkBridgeModule::version()
{
#ifdef NTB_ModuleVersion
    return std::to_string(NTB_ModuleVersion);
#else
    return "";
#endif
}

void NetworkBridgeModule::setup(bool configured)
{
    Module::setup(configured);
    NTBChannelOwnerModule::initialize(configured ? ParamNTB_VisibleChannels : 0);
    NTBChannelOwnerModule::setup(configured);
}

OpenKNX::Channel* NetworkBridgeModule::createChannel(uint8_t _channelIndex /* this parameter is used in macros, do not rename */)
{
    if (ParamNTB_CHChannelDisabled)
    {
        logDebugP("NTB channel %d is disabled", _channelIndex);
        return nullptr;
    }

    switch (ParamNTB_CHChannelType)
    {
        case 0:
            return nullptr;
        case 1:
            return new PingFunction(_channelIndex);
        case 2:
            return new WakeOnLanFunction(_channelIndex);
        case 3:
            return new WebhookInFunction(_channelIndex);
        case 4:
            return new WebhookOutFunction(_channelIndex);
        default:
            logErrorP("NTB channel %d has unknown type %d", _channelIndex, ParamNTB_CHChannelType);
            return nullptr;
    }
}

NetworkBridgeModule openknxNetworkBridgeModule;
