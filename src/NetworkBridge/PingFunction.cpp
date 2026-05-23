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
    readInputKos();

    uint32_t targetRaw = ParamNTB_CHPingTargetAddress;
    _targetAddress = IPAddress(
        static_cast<uint8_t>((targetRaw >> 24) & 0xFF),
        static_cast<uint8_t>((targetRaw >> 16) & 0xFF),
        static_cast<uint8_t>((targetRaw >> 8) & 0xFF),
        static_cast<uint8_t>(targetRaw & 0xFF));

    _automaticPing = ParamNTB_CHPingAutomatic;
    KoNTB_CHPingStatus.value(false, DPT_Alarm);
    if (_automaticPing)
    {
        triggerPing(false);
    }
}

void PingFunction::readInputKos()
{
    // Inputs are handled via processInputKo/handleKo.
}

void PingFunction::initMissingInputValues()
{
    // No additional startup initialization required.
}

void PingFunction::processInputKo(GroupObject &ko)
{
    auto index = NTB_KoCalcIndex(ko.asap());
    if (index == NTB_KoCHPingTrigger && ko.value(DPT_Trigger))
    {
        triggerPing(true);
    }
}


void PingFunction::loop()
{
    if (!_automaticPing || _pingRunning)
    {
        return;
    }

    unsigned long now = millis();
    if (_startTimeStampForNextPing == 0 || now - _startTimeStampForNextPing >= pingIntervalMs())
    {
        triggerPing(false);
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

void PingFunction::triggerPing(bool manualTrigger)
{
    _automaticPing = ParamNTB_CHPingAutomatic;
    if (_pingRunning)
    {
        return;
    }

    if (!_automaticPing && manualTrigger)
    {
        // Manual mode: clear status immediately on each trigger.
        KoNTB_CHPingStatus.value(false, DPT_Alarm);
    }

#ifdef OPENKNX_PING
    _pingRunning = true;
    logDebugP("Ping triggered (%s) to %s", manualTrigger ? "manual" : "automatic", _targetAddress.toString().c_str());
    openknxNetwork.ping(_targetAddress,
        [this, manualTrigger](IPAddress ip, bool reachable) {
            logDebugP("Ping callback for %s: %s", ip.toString().c_str(), reachable ? "reachable" : "unreachable");  
            _pingRunning = false;
            if (!manualTrigger)
            {
                bool changed = KoNTB_CHPingStatus.valueCompare(reachable, DPT_Alarm);
                if (changed)
                {
                    logDebugP("Auto ping status changed for %s: %s", ip.toString().c_str(), reachable ? "reachable" : "unreachable");
                }
                scheduleNextAutomaticPing();
            }
            else
            {
                KoNTB_CHPingStatus.value(reachable, DPT_Alarm);
                logDebugP("Manual ping result for %s: %s", ip.toString().c_str(), reachable ? "reachable" : "unreachable");
            }
        },
        manualTrigger ? 0 : 2);
#else
    // If OPENKNX_PING is not available, report failure state.
    KoNTB_CHPingStatus.value(false, DPT_Alarm);
    _pingRunning = false;
    if (_automaticPing)
    {
        scheduleNextAutomaticPing();
    }
#endif
}
