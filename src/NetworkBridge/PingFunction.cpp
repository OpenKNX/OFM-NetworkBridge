#include "PingFunction.h"
#include "NetworkModule.h"
#include "knxprod.h"

PingFunction::PingFunction(uint8_t channelIndex)
    : NetworkBridgeFunction(channelIndex, "Ping")
{
}

void PingFunction::setup(bool configured)
{
    NetworkBridgeFunction::setup(configured);
    _automaticPing = ParamNTB_CHPingAutomatic;
    KoNTB_CHPingStatus.value(false, DPT_Alarm);
    if (_automaticPing)
    {
        scheduleNextAutomaticPing();
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

void PingFunction::handleKo(GroupObject &ko)
{
    auto index = NTB_KoCalcIndex(ko.asap());
    if (index == NTB_KoCHPingTrigger && ko.value(DPT_Trigger))
    {
        triggerPing(true);
    }
}

void PingFunction::loop(bool configured)
{
    (void)configured;
    if (!_automaticPing || _pingRunning)
    {
        return;
    }

    uint32_t now = millis();
    if (_nextAutomaticPingAt == 0 || static_cast<int32_t>(now - _nextAutomaticPingAt) >= 0)
    {
        triggerPing(false);
    }
}

void PingFunction::loop()
{
    if (!_automaticPing || _pingRunning)
    {
        return;
    }

    uint32_t now = millis();
    if (_nextAutomaticPingAt == 0 || static_cast<int32_t>(now - _nextAutomaticPingAt) >= 0)
    {
        triggerPing(false);
    }
}

uint32_t PingFunction::pingIntervalMs() const
{
    uint32_t intervalMs = ParamNTB_CHPingIntervalDelayTimeMS;
    return intervalMs == 0 ? 1000 : intervalMs;
}

void PingFunction::scheduleNextAutomaticPing()
{
    _nextAutomaticPingAt = millis() + pingIntervalMs();
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
    IPAddress target = IPAddress(ParamNTB_CHPingTargetAddress);
    _pingRunning = true;
    openknxNetwork.ping(target,
        [this](IPAddress ip, bool reachable) {
            (void)ip;
            _pingRunning = false;
            if (_automaticPing)
            {
                KoNTB_CHPingStatus.valueCompare(reachable, DPT_Alarm);
                scheduleNextAutomaticPing();
            }
            else
            {
                KoNTB_CHPingStatus.value(reachable, DPT_Alarm);
            }
        },
        2);
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
