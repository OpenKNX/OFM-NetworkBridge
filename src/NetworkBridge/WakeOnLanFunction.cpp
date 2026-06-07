#include "WakeOnLanFunction.h"
#include "knxprod.h"
#include <WiFiUdp.h>

WakeOnLanFunction::WakeOnLanFunction(uint8_t channelIndex)
    : NetworkBridgeFunction(channelIndex, "WakeOnLan")
{
}

void WakeOnLanFunction::setup()
{
    OpenKNX::Channel::setup();
    readInputKos();
    if (parseMac(reinterpret_cast<const char*>(ParamNTB_CHWolMacAddress)))
        buildPacket();
}

void WakeOnLanFunction::readInputKos()
{
}

void WakeOnLanFunction::initMissingInputValues()
{
}

void WakeOnLanFunction::processInputKo(GroupObject &ko)
{
    auto index = NTB_KoCalcIndex(ko.asap());
    if (index == NTB_KoCHTrigger && ko.value(DPT_Trigger) && openknx.afterStartupDelay())
    {
        sendMagicPacket();
    }
}

void WakeOnLanFunction::loop()
{
}

bool WakeOnLanFunction::parseMac(const char* str)
{
    if (!str) return false;
    return sscanf(str, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
        &_mac[0], &_mac[1], &_mac[2], &_mac[3], &_mac[4], &_mac[5]) == 6;
}

void WakeOnLanFunction::buildPacket()
{
    for (int i = 0; i < 6; i++)
        _packet[i] = 0xFF;
    for (int i = 0; i < 16; i++)
        for (int j = 0; j < 6; j++)
            _packet[6 + i * 6 + j] = _mac[j];
}

void WakeOnLanFunction::sendMagicPacket()
{
    WiFiUDP udp;
    udp.begin(0);
    udp.beginPacket(IPAddress(255, 255, 255, 255), 9);
    udp.write(_packet, sizeof(_packet));
    if (udp.endPacket())
        logDebugP("WOL: Magic Packet sent to %02X:%02X:%02X:%02X:%02X:%02X",
            _mac[0], _mac[1], _mac[2], _mac[3], _mac[4], _mac[5]);
    else
        logErrorP("WOL: send failed");
    udp.stop();
}
