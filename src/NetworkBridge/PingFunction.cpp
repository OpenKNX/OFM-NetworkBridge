#include "PingFunction.h"
#include "NetworkModule.h"
#include "knxprod.h"

PingFunction::PingFunction(uint8_t channelIndex)
    : NetworkBridgeFunction(channelIndex, "Ping")
{
}

void PingFunction::setup()
{
    OpenKNX::Channel::setup();

    uint32_t targetRaw = ParamNTB_CHPingTargetAddress;
    _targetAddress = IPAddress(
        static_cast<uint8_t>((targetRaw >> 24) & 0xFF),
        static_cast<uint8_t>((targetRaw >> 16) & 0xFF),
        static_cast<uint8_t>((targetRaw >> 8) & 0xFF),
        static_cast<uint8_t>(targetRaw & 0xFF));
}

void PingFunction::processInputKo(GroupObject &ko)
{
    auto index = NTB_KoCalcIndex(ko.asap());
    if (index == NTB_KoCHIn && ko.value(DPT_Trigger) && openknx.afterStartupDelay())
    {
        triggerPing();
    }
}

void PingFunction::loop()
{
    if (!ParamNTB_CHPingAutomatic || _pingRunning || !openknx.afterStartupDelay())
    {
        return;
    }

    unsigned long now = millis();
    if (_startTimeStampForNextPing == 0 || now - _startTimeStampForNextPing >= pingIntervalMs())
    {
        triggerPing();
    }
}

unsigned long PingFunction::pingIntervalMs() const
{
    unsigned long intervalMs = ParamNTB_CHPingIntervalDelayTimeMS;
    return intervalMs == 0 ? 1000 : intervalMs;
}

void PingFunction::scheduleNextAutomaticPing()
{
    _startTimeStampForNextPing = max(1ul, millis());
}

void PingFunction::onPingResult(IPAddress ip, bool reachable)
{
    logDebugP("Ping callback for %s: %s", ip.toString().c_str(), reachable ? "reachable" : "unreachable");
    _pingRunning = false;
    KoNTB_CHOut.value(reachable, DPT_Alarm);
    if (ParamNTB_CHPingAutomatic)
        scheduleNextAutomaticPing();
}

void PingFunction::triggerPing()
{
    if (_pingRunning)
    {
        return;
    }

#ifdef OPENKNX_PING
    _pingRunning = true;
    logDebugP("Ping triggered to %s", _targetAddress.toString().c_str());
    openknxNetwork.ping(_targetAddress, [this](IPAddress ip, bool reachable)
                        { onPingResult(ip, reachable); }, ParamNTB_CHPingRetries, ParamNTB_CHPingTimeout);
#endif
}
