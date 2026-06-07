#include "MqttInFunction.h"
#include "knxprod.h"
#ifdef OPENKNX_MQTT
    #include "NetworkModule.h"
    #include "OpenKNX/Format/JSON/Reader.h"
#endif

MqttInFunction::MqttInFunction(uint8_t channelIndex)
    : NetworkBridgeFunction(channelIndex, "MqttIn")
{
}

void MqttInFunction::setup()
{
    OpenKNX::Channel::setup();
    readInputKos();
#ifdef OPENKNX_MQTT
    _dpt = ParamNTB_CHMqttInDpt;
    _jsonEnabled = ParamNTB_CHMqttInJsonEnabled;
    if (_jsonEnabled)
        _jsonPath = reinterpret_cast<const char*>(ParamNTB_CHMqttInJsonPath);

    std::string topic = reinterpret_cast<const char*>(ParamNTB_CHMqttInTopic);
    if (topic.empty())
        return;

    openknxNetwork.mqtt.subscribe(topic, [this](const char*, const void* payload, size_t len) {
        char raw[256] = {};
        if (_jsonEnabled && !_jsonPath.empty()) {
            char jsonBuf[512] = {};
            size_t jsonLen = len < 511 ? len : 511;
            memcpy(jsonBuf, payload, jsonLen);
            OpenKNX::Format::JSON::Reader r(jsonBuf);
            r.select(_jsonPath.c_str()).toString(raw, sizeof(raw));
        } else {
            size_t copyLen = len < 255 ? len : 255;
            memcpy(raw, payload, copyLen);
        }
        memcpy(_pendingPayload, raw, sizeof(_pendingPayload));
        _hasPendingValue = true;
    }, 0);

    logDebugP("MQTT subscribed: %s (DPT %d, JSON %s)", topic.c_str(), _dpt, _jsonEnabled ? _jsonPath.c_str() : "off");
#endif
}

void MqttInFunction::readInputKos() {}
void MqttInFunction::initMissingInputValues() {}
void MqttInFunction::processInputKo(GroupObject& /*ko*/) {}

void MqttInFunction::loop()
{
    if (!_hasPendingValue)
        return;
    _hasPendingValue = false;
    applyValue(_pendingPayload);
}

void MqttInFunction::applyValue(const char* v)
{
    switch (_dpt)
    {
        case 0: KoNTB_CHOut.value(strcmp(v, "1") == 0 || strcmp(v, "true") == 0, Dpt(1, 2)); break;
        case 1: KoNTB_CHOut.value((uint8_t)atoi(v), Dpt(5, 5)); break;
        case 2: KoNTB_CHOut.value((uint8_t)(atof(v) / 100.0f * 255.0f + 0.5f), Dpt(5, 1)); break;
        case 3: KoNTB_CHOut.value((int8_t)atoi(v), Dpt(6, 10)); break;
        case 4: KoNTB_CHOut.value((uint16_t)atoi(v), Dpt(7, 1)); break;
        case 5: KoNTB_CHOut.value((int16_t)atoi(v), Dpt(8, 1)); break;
        case 6: KoNTB_CHOut.value((float)atof(v), Dpt(9, 1)); break;
        case 7: KoNTB_CHOut.value((uint32_t)atol(v), Dpt(12, 1)); break;
        case 8: KoNTB_CHOut.value((int32_t)atol(v), Dpt(13, 1)); break;
        case 9: KoNTB_CHOut.value((float)atof(v), Dpt(14, 0)); break;
        default: return;
    }
    KoNTB_CHOut.objectWritten();
    logDebugP("MQTT value applied: '%s' as DPT %d", v, _dpt);
}
