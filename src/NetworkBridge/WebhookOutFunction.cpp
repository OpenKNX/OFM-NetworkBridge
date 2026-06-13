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
    _statusMode = ParamNTB_CHWebhookOutStatusMode;
    _method = ParamNTB_CHWebhookOutMethod;
    _contentType = ParamNTB_CHWebhookOutContentType;
}

void WebhookOutFunction::processInputKo(GroupObject &ko)
{
    auto index = NTB_KoCalcIndex(ko.asap());
    if (index != NTB_KoCHIn || !ko.value(DPT_Trigger) || !openknx.afterStartupDelay())
        return;

#ifdef OPENKNX_WEBCLIENT
    char url[128];
    readChannelFileOrParam("url", url, sizeof(url), reinterpret_cast<const char*>(ParamNTB_CHWebhookOutUrl));
    if (url[0] == '\0')
        return;

    auto done = [this](const OpenKNX::Network::Webclient::Response& r) {
        _pendingStatus = r.success() ? r.status() : 0;
        _hasPendingResult = true;
    };

    // parse /ntb/{ch}.header.txt on-the-fly and apply headers to request
    auto applyHeaders = [this](auto& req) -> auto& {
        char path[32];
        getChannelFilename(path, sizeof(path), "header");
        if (!LittleFS.exists(path))
            return req;
        File f = LittleFS.open(path, "r");
        if (!f)
            return req;
        char line[256];
        while (f.available())
        {
            int n = f.readBytesUntil('\n', line, sizeof(line) - 1);
            line[n] = '\0';
            if (n > 0 && line[n - 1] == '\r')
                line[--n] = '\0';
            if (n == 0)
                continue;
            char* colon = strchr(line, ':');
            if (!colon)
                continue;
            *colon = '\0';
            char* val = colon + 1;
            while (*val == ' ')
                val++;
            req.header(line, val);
        }
        f.close();
        return req;
    };

    if (_method == 1)
    {
        std::string body = readBodyFile();
        auto req = openknxNetwork.webclient.post(url)
            .body(body)
            .contentType(mimeTypeFor(_contentType));
        applyHeaders(req).ignoreBody().onDone(done).send();
        logDebugP("Webhook triggered: POST %s (%zu bytes)", url, body.size());
    }
    else
    {
        auto req = openknxNetwork.webclient.get(url)
            .header("Content-Type", mimeTypeFor(_contentType));
        applyHeaders(req).ignoreBody().onDone(done).send();
        logDebugP("Webhook triggered: GET %s", url);
    }
#endif
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
    static char bodyBuf[4096];
    bodyBuf[0] = '\0';
    readChannelFile("body", bodyBuf, sizeof(bodyBuf));
    return std::string(bodyBuf);
#else
    return "";
#endif
}

const char* WebhookOutFunction::fieldEtsValue(uint8_t /*fieldIndex*/) const
{
    return nullptr;
}

const char* WebhookOutFunction::fieldExtraHint(uint8_t fieldIndex) const
{
    // Methode unter dem URL-Feld (Feld 0) anzeigen
    if (fieldIndex == 0)
        return _method == 1 ? "Methode: POST" : "Methode: GET";
    return nullptr;
}

uint8_t WebhookOutFunction::configFields(const ConfigFileField*& out) const
{
    static const ConfigFileField fieldsPost[] = {
        {"url",    "URL",         false, "Überschreibt das ETS-Feld (max. 50 Zeichen). Leer lassen = ETS-Wert verwenden."},
        {"header", "HTTP-Header", true,  "Ein Header pro Zeile, Format: Schlüssel: Wert. Leer lassen = keine zusätzlichen Header."},
        {"body",   "POST-Body",   true,  "Wird als Body des POST-Requests gesendet. Leer lassen = leerer Body."},
    };
    static const ConfigFileField fieldsGet[] = {
        {"url",    "URL",         false, "Überschreibt das ETS-Feld (max. 50 Zeichen). Leer lassen = ETS-Wert verwenden."},
        {"header", "HTTP-Header", true,  "Ein Header pro Zeile, Format: Schlüssel: Wert. Leer lassen = keine zusätzlichen Header."},
    };
    if (_method == 0)
    {
        out = fieldsGet;
        return 2;
    }
    out = fieldsPost;
    return 3;
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
