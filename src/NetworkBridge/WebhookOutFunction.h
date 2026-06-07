#pragma once
#include "NetworkBridgeFunction.h"
#include <string>

class WebhookOutFunction : public NetworkBridgeFunction
{
public:
    explicit WebhookOutFunction(uint8_t channelIndex);
    void setup() override;
    void readInputKos() override;
    void initMissingInputValues() override;
    void processInputKo(GroupObject &ko) override;
    void loop() override;

private:
    std::string _url;
    uint8_t _statusMode = 1;
    uint8_t _method = 0;
    uint8_t _contentType = 0;
    volatile bool _hasPendingResult = false;
    volatile uint16_t _pendingStatus = 0;

    std::string readBodyFile();
    static const char* mimeTypeFor(uint8_t contentType);
};
