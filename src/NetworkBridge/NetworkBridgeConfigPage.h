#pragma once

#ifdef OPENKNX_WEBSERVER

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

#endif // OPENKNX_WEBSERVER
