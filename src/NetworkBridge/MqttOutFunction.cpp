#include "MqttOutFunction.h"
#include "knxprod.h"
#ifdef OPENKNX_MQTT
    #include "NetworkModule.h"
#endif

MqttOutFunction::MqttOutFunction(uint8_t channelIndex)
    : NetworkBridgeFunction(channelIndex, "MqttOut")
{
}

void MqttOutFunction::setup()
{
    OpenKNX::Channel::setup();
    readInputKos();
#ifdef OPENKNX_MQTT
    _topic         = reinterpret_cast<const char*>(ParamNTB_CHMqttOutTopic);
    _dpt           = ParamNTB_CHMqttOutDpt;
    _qos           = ParamNTB_CHMqttOutQos;
    _retain        = ParamNTB_CHMqttOutRetain;
    _devicePrefix  = ParamNTB_CHMqttOutDevicePrefix;
    _asJson        = ParamNTB_CHMqttOutAsJson;
    logDebugP("MQTT Out configured: topic=%s DPT=%d QoS=%d retain=%d prefix=%d json=%d", _topic.c_str(), _dpt, _qos, _retain, _devicePrefix, _asJson);
#endif
}

void MqttOutFunction::readInputKos() {}
void MqttOutFunction::initMissingInputValues() {}

void MqttOutFunction::processInputKo(GroupObject& ko)
{
    auto index = NTB_KoCalcIndex(ko.asap());
    if (index != NTB_KoCHIn)
        return;

#ifdef OPENKNX_MQTT
    if (_topic.empty())
        return;

    char buf[32] = {};
    switch (_dpt)
    {
        case 0: snprintf(buf, sizeof(buf), "%d", (int)(bool)ko.value(Dpt(1, 2))); break;
        case 1: snprintf(buf, sizeof(buf), "%u", (uint8_t)ko.value(Dpt(5, 5))); break;
        case 2: snprintf(buf, sizeof(buf), "%u", (uint8_t)(float)ko.value(Dpt(5, 1))); break;
        case 3: snprintf(buf, sizeof(buf), "%d", (int8_t)ko.value(Dpt(6, 10))); break;
        case 4: snprintf(buf, sizeof(buf), "%u", (uint16_t)ko.value(Dpt(7, 1))); break;
        case 5: snprintf(buf, sizeof(buf), "%d", (int16_t)ko.value(Dpt(8, 1))); break;
        case 6: snprintf(buf, sizeof(buf), "%g", (float)ko.value(Dpt(9, 1))); break;
        case 7: snprintf(buf, sizeof(buf), "%lu", (uint32_t)ko.value(Dpt(12, 1))); break;
        case 8: snprintf(buf, sizeof(buf), "%ld", (int32_t)ko.value(Dpt(13, 1))); break;
        case 9: snprintf(buf, sizeof(buf), "%g", (float)ko.value(Dpt(14, 0))); break;
        default: return;
    }

    std::string payload;
    if (_asJson)
    {
        const char* jsonVal = (_dpt == 0) ? (buf[0] == '1' ? "true" : "false") : buf;
        payload = std::string("{\"value\":") + jsonVal + "}";
    }
    else
    {
        payload = std::string(buf);
    }

    if (_devicePrefix)
        openknxNetwork.mqtt.publishP(_topic, payload, _qos, _retain);
    else
        openknxNetwork.mqtt.publish(_topic, payload, _qos, _retain);
    logDebugP("MQTT published: %s = '%s' (json=%d prefix=%d)", _topic.c_str(), payload.c_str(), _asJson, _devicePrefix);
#endif
}

void MqttOutFunction::loop() {}
