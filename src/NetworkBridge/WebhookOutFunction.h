#pragma once
#include "NetworkBridgeFunction.h"
#include <string>

class WebhookOutFunction : public NetworkBridgeFunction
{
public:
    explicit WebhookOutFunction(uint8_t channelIndex);
    const char* typeName() const override { return "Webhook senden"; }
    void setup() override;
    void processInputKo(GroupObject &ko) override;
    void loop() override;
    uint8_t configFields(const ConfigFileField*& out) const override;
    const char* fieldEtsValue(uint8_t fieldIndex) const override;
    const char* fieldExtraHint(uint8_t fieldIndex) const override;

private:
    uint8_t _statusMode = 1;
    uint8_t _method = 0;
    uint8_t _contentType = 0;
    volatile bool _hasPendingResult = false;
    volatile uint16_t _pendingStatus = 0;

    std::string readBodyFile();
    static const char* mimeTypeFor(uint8_t contentType);
};
