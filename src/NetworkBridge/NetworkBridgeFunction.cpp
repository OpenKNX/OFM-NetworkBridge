#include "NetworkBridgeFunction.h"
#include "knxprod.h"
#include <LittleFS.h>

NetworkBridgeFunction::NetworkBridgeFunction(uint8_t channelIndex, const char* name)
{
    _channelIndex = channelIndex;
    _name = name;
}

void NetworkBridgeFunction::setup()
{
    OpenKNX::Channel::setup();
}

const std::string NetworkBridgeFunction::name()
{
    return _name;
}

bool NetworkBridgeFunction::processCommand(const std::string cmd, bool diagnoseKo)
{
    (void)cmd;
    (void)diagnoseKo;
    return false;
}

void NetworkBridgeFunction::channelFilePath(uint8_t channelIndex, const char* type, char* out, size_t outLen)
{
    snprintf(out, outLen, "/ntb/%d.%s.txt", channelIndex, type);
}

void NetworkBridgeFunction::getChannelFilename(char* out, size_t outLen, const char* type) const
{
    channelFilePath(_channelIndex, type, out, outLen);
}

bool NetworkBridgeFunction::readChannelFile(const char* type, char* buf, size_t bufLen) const
{
    char path[32];
    getChannelFilename(path, sizeof(path), type);
    if (!LittleFS.exists(path))
        return false;
    File f = LittleFS.open(path, "r");
    if (!f)
        return false;
    size_t n = f.readBytes(buf, bufLen - 1);
    buf[n] = '\0';
    f.close();
    return true;
}

void NetworkBridgeFunction::readChannelFileOrParam(const char* type, char* buf, size_t bufLen, const char* paramValue) const
{
    if (!readChannelFile(type, buf, bufLen))
    {
        strncpy(buf, paramValue, bufLen - 1);
        buf[bufLen - 1] = '\0';
    }
}
