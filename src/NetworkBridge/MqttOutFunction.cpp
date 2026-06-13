#include "MqttOutFunction.h"
#include "knxprod.h"
#include <cstring>
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
}

void MqttOutFunction::processInputKo(GroupObject& ko)
{
    auto index = NTB_KoCalcIndex(ko.asap());
    if (index != NTB_KoCHIn)
        return;

#ifdef OPENKNX_MQTT
    char topic[128];
    readChannelFileOrParam("topic", topic, sizeof(topic), reinterpret_cast<const char*>(ParamNTB_CHMqttOutTopic));
    if (topic[0] == '\0')
        return;

    const uint8_t dpt          = ParamNTB_CHMqttOutDpt;
    const uint8_t qos          = ParamNTB_CHMqttOutQos;
    const bool    retain       = ParamNTB_CHMqttOutRetain;
    const bool    devicePrefix = ParamNTB_CHMqttOutDevicePrefix;
    const bool    asJson       = ParamNTB_CHMqttOutAsJson;

    char buf[32] = {};
    switch (dpt)
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
    if (asJson)
    {
        const char* jsonVal = (dpt == 0) ? (buf[0] == '1' ? "true" : "false") : buf;
        payload = std::string("{\"value\":") + jsonVal + "}";
    }
    else
    {
        payload = std::string(buf);
    }

    if (devicePrefix)
        openknxNetwork.mqtt.publishP(topic, payload, qos, retain);
    else
        openknxNetwork.mqtt.publish(topic, payload, qos, retain);
    logDebugP("MQTT published: %s = '%s'", topic, payload.c_str());
#endif
}

void MqttOutFunction::loop() {}

const char* MqttOutFunction::fieldEtsValue(uint8_t fieldIndex) const
{
    if (fieldIndex == 0)
    {
        const char* topic = reinterpret_cast<const char*>(ParamNTB_CHMqttOutTopic);
        // "bridge/test" ist der ETS-Platzhalter-Default → nicht als Wert anzeigen
        if (topic && topic[0] && strcmp(topic, "bridge/test") != 0)
            return topic;
    }
    return nullptr;
}

uint8_t MqttOutFunction::configFields(const ConfigFileField*& out) const
{
    static const ConfigFileField fields[] = {
        {"topic", "Topic", false, "Überschreibt das ETS-Feld (max. 50 Zeichen). Leer lassen = ETS-Wert verwenden."},
    };
    out = fields;
    return 1;
}
