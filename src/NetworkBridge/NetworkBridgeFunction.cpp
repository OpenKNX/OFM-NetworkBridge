#include "NetworkBridgeFunction.h"

NetworkBridgeFunction::NetworkBridgeFunction(uint8_t channelIndex, const char* name)
{
    _channelIndex = channelIndex;
    _name = name;
}

void NetworkBridgeFunction::setup(bool configured)
{
    OpenKNX::Channel::setup(configured);
    readInputKos();
}

const std::string NetworkBridgeFunction::name()
{
    return _name;
}

void NetworkBridgeFunction::processInputKo(GroupObject &ko)
{
    handleKo(ko);
}

bool NetworkBridgeFunction::processCommand(const std::string cmd, bool diagnoseKo)
{
    (void)cmd;
    (void)diagnoseKo;
    return false;
}
