#include "WebhookInFunction.h"
#include "knxprod.h"
#if defined(OPENKNX_WEBSERVER) && (defined(KNX_IP_WIFI) || defined(KNX_IP_LAN))
    #include "NetworkModule.h"
#endif

WebhookInFunction::WebhookInFunction(uint8_t channelIndex)
    : NetworkBridgeFunction(channelIndex, "WebhookIn")
{
}

void WebhookInFunction::setup()
{
    OpenKNX::Channel::setup();
#if defined(OPENKNX_WEBSERVER) && (defined(KNX_IP_WIFI) || defined(KNX_IP_LAN))
    char path[64];
    snprintf(path, sizeof(path), "/webhook/%s", reinterpret_cast<const char*>(ParamNTB_CHWebhookInPath));

    auto handler = [this](OpenKNX::Network::WebRequest& /*req*/, OpenKNX::Network::WebResponse& res) {
        _triggered = true;
        res.setStatus(200);
        res.send("OK");
    };
    openknxNetwork.webserver.addRoute(OpenKNX::Network::WEB_GET, path, handler);
    openknxNetwork.webserver.addRoute(OpenKNX::Network::WEB_POST, path, handler);
    logDebugP("Webhook registered: GET+POST %s", path);
#endif
}

void WebhookInFunction::processInputKo(GroupObject& /*ko*/) {}

void WebhookInFunction::loop()
{
    if (_triggered)
    {
        _triggered = false;
        KoNTB_CHOut.value(true, DPT_Switch);
        KoNTB_CHOut.objectWritten();
    }
}
