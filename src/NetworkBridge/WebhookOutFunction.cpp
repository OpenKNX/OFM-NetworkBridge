#include "WebhookOutFunction.h"
#include "knxprod.h"
#ifdef OPENKNX_WEBCLIENT
    #include "NetworkModule.h"
    #include <LittleFS.h>
#endif

WebhookOutFunction::WebhookOutFunction(uint8_t channelIndex)
    : NetworkBridgeFunction(channelIndex, "WebhookOut")
{
}

void WebhookOutFunction::setup()
{
    OpenKNX::Channel::setup();
    readInputKos();
    _url = reinterpret_cast<const char*>(ParamNTB_CHWebhookOutUrl);
    _statusMode = ParamNTB_CHWebhookOutStatusMode;
    _method = ParamNTB_CHWebhookOutMethod;
    _contentType = ParamNTB_CHWebhookOutContentType;
}

void WebhookOutFunction::readInputKos() {}
void WebhookOutFunction::initMissingInputValues() {}

void WebhookOutFunction::processInputKo(GroupObject &ko)
{
    auto index = NTB_KoCalcIndex(ko.asap());
    if (index == NTB_KoCHIn && ko.value(DPT_Trigger) && openknx.afterStartupDelay())
    {
#ifdef OPENKNX_WEBCLIENT
        auto done = [this](const OpenKNX::Network::Webclient::Response& r) {
            _pendingStatus = r.success() ? r.status() : 0;
            _hasPendingResult = true;
        };

        if (_method == 1)
        {
            std::string body = readBodyFile();
            openknxNetwork.webclient.post(_url)
                .body(body)
                .contentType(mimeTypeFor(_contentType))
                .ignoreBody()
                .onDone(done)
                .send();
            logDebugP("Webhook triggered: POST %s (%zu bytes)", _url.c_str(), body.size());
        }
        else
        {
            openknxNetwork.webclient.get(_url)
                .header("Content-Type", mimeTypeFor(_contentType))
                .ignoreBody()
                .onDone(done)
                .send();
            logDebugP("Webhook triggered: GET %s", _url.c_str());
        }
#endif
    }
}

void WebhookOutFunction::loop()
{
    if (!_hasPendingResult)
        return;

    _hasPendingResult = false;
    if (_statusMode == 1)
    {
        KoNTB_CHOut.value(_pendingStatus == 200, DPT_Switch);
        KoNTB_CHOut.objectWritten();
    }
    else if (_statusMode == 2)
    {
        KoNTB_CHOut.value((uint16_t)_pendingStatus, Dpt(7, 1));
        KoNTB_CHOut.objectWritten();
    }
}

std::string WebhookOutFunction::readBodyFile()
{
#ifdef OPENKNX_WEBCLIENT
    char path[32];
    snprintf(path, sizeof(path), "/webhook/%d.txt", _channelIndex);
    if (!LittleFS.exists(path))
        return "";
    File f = LittleFS.open(path, "r");
    if (!f)
        return "";
    std::string content(f.size(), '\0');
    f.readBytes(&content[0], f.size());
    f.close();
    return content;
#else
    return "";
#endif
}

const char* WebhookOutFunction::mimeTypeFor(uint8_t contentType)
{
    switch (contentType)
    {
        case 0: return "text/html";
        case 1: return "text/plain";
        case 2: return "application/xml";
        case 3: return "application/json";
        default: return "text/plain";
    }
}
