#pragma once

#if defined(OPENKNX_WEBSERVER) && (defined(KNX_IP_WIFI) || defined(KNX_IP_LAN))

    #include "NetworkModule.h"

// Web-Konfigurationsseite /ntb: typ-abhängiger Editor für die /ntb/-Datei-Overrides
// eines per Dropdown gewählten Kanals. Verwaltet ausschließlich LittleFS-Dateien.
class NetworkBridgeConfigPage
{
  public:
    void setup();

  private:
    void handlePage(OpenKNX::Network::WebRequest& req, OpenKNX::Network::WebResponse& res);
    void handleSave(OpenKNX::Network::WebRequest& req, OpenKNX::Network::WebResponse& res);
};

#endif // OPENKNX_WEBSERVER && KNX_IP
