#include "WebhookInFunction.h"
#include "knxprod.h"
#ifdef OPENKNX_WEBSERVER
    #include "NetworkModule.h"
#endif

WebhookInFunction::WebhookInFunction(uint8_t channelIndex)
    : NetworkBridgeFunction(channelIndex, "WebhookIn")
{
}

void WebhookInFunction::setup()
{
    OpenKNX::Channel::setup();
    readInputKos();
#ifdef OPENKNX_WEBSERVER
    std::string path = "/webhook/";
    path += reinterpret_cast<const char*>(ParamNTB_CHWebhookInPath);
    auto handler = [this](OpenKNX::Network::WebRequest& /*req*/, OpenKNX::Network::WebResponse& res) {
        _triggered = true;
        res.setStatus(200);
        res.send("OK");
    };
    openknxNetwork.webserver.addRoute(OpenKNX::Network::WEB_GET, path, handler);
    openknxNetwork.webserver.addRoute(OpenKNX::Network::WEB_POST, path, handler);
    logDebugP("Webhook registered: GET+POST %s", path.c_str());
#endif
}

void WebhookInFunction::readInputKos() {}
void WebhookInFunction::initMissingInputValues() {}
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
