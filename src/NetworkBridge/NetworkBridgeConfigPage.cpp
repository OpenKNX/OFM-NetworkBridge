#include "NetworkBridgeConfigPage.h"

#if defined(OPENKNX_WEBSERVER) && (defined(KNX_IP_WIFI) || defined(KNX_IP_LAN))

    #include "NetworkBridgeFunction.h"
    #include "NetworkBridgeModule.h"
    #include <LittleFS.h>
    #include <cstdlib>
    #include <string>

using namespace OpenKNX::Network;

namespace
{
    const char ntbCss[] =
        ".ntb-sel{height:34px;padding:0 14px;box-sizing:border-box;margin:0;border:1px solid #d0d0d0;max-width:420px;width:100%}"
        ".ntb-edit{display:flex;gap:10px;align-items:flex-start;flex-wrap:wrap}"
        ".ntb-edit input[type=text]{width:100%;height:34px;padding:0 14px;box-sizing:border-box;margin:0;border:1px solid #d0d0d0}"
        ".ntb-edit textarea{width:100%;min-height:120px;padding:6px;box-sizing:border-box;margin:0;border:1px solid #d0d0d0;resize:vertical}"
        ".ntb-edit button{height:34px;padding:0 14px;box-sizing:border-box;margin:0;border:1px solid #d0d0d0;white-space:nowrap;flex-shrink:0}"
        ".ntb-edit-wrap{display:flex;flex-direction:column;gap:4px;flex:1;min-width:180px}"
        ".ntb-status{font-size:.9em;color:#555;white-space:nowrap;padding-top:7px}"
        ".ntb-hint{color:#888}"
        ".ntb-field-hint{font-size:.82em;color:#888;margin:0}"
        ".ntb-table td{vertical-align:top;padding-top:8px}";

    const char ntbJs[] =
        "function ntbSelect(v){location=v===''?'/ntb':'/ntb?ch='+encodeURIComponent(v);}"
        "async function ntbSave(ch,type,elId){"
        "const el=document.getElementById(elId);"
        "const st=document.getElementById('s_'+type);"
        "st.textContent='\\u2026';"
        "try{"
        "const r=await fetch('/ntb/save?ch='+ch+'&type='+type,{method:'POST',body:el.value});"
        "if(!r.ok)throw new Error(await r.text());"
        "st.textContent='\\u2713 gespeichert';"
        "}catch(e){st.textContent='Fehler: '+e.message;}"
        "}";

    std::string htmlEscape(const std::string& s)
    {
        std::string r;
        r.reserve(s.size());
        for (char c : s)
        {
            switch (c)
            {
                case '&': r += "&amp;"; break;
                case '<': r += "&lt;"; break;
                case '>': r += "&gt;"; break;
                case '"': r += "&quot;"; break;
                default: r += c; break;
            }
        }
        return r;
    }

    std::string readChannelFileContent(uint8_t ch, const char* type)
    {
        char path[32];
        NetworkBridgeFunction::channelFilePath(ch, type, path, sizeof(path));
        if (!LittleFS.exists(path))
            return "";
        File f = LittleFS.open(path, "r");
        if (!f)
            return "";
        size_t sz = f.size();
        std::string content;
        content.resize(sz);
        if (sz)
            f.readBytes(&content[0], sz);
        f.close();
        return content;
    }

    // Liefert die NTB-Funktion eines Kanals oder nullptr (disabled/none).
    NetworkBridgeFunction* channelFunction(uint8_t ch)
    {
        if (ch >= openknxNetworkBridgeModule.getNumberOfChannels())
            return nullptr;
        return static_cast<NetworkBridgeFunction*>(openknxNetworkBridgeModule.getChannel(ch));
    }
} // namespace

void NetworkBridgeConfigPage::setup()
{
    openknxNetwork.webserver.addMenuItem("Netzwerkbrücke", "/ntb", 40);

    openknxNetwork.webserver.addRoute(WEB_GET, "/assets/ntb.css",
                                      Webserver::Static("text/css", ntbCss));
    openknxNetwork.webserver.addRoute(WEB_GET, "/assets/ntb.js",
                                      Webserver::Static("application/javascript", ntbJs));
    openknxNetwork.webserver.addStylesheet("/assets/ntb.css");
    openknxNetwork.webserver.addJavaScript("/assets/ntb.js");

    openknxNetwork.webserver.addRoute(WEB_GET, "/ntb",
                                      [this](WebRequest& req, WebResponse& res) { handlePage(req, res); });
    openknxNetwork.webserver.addRoute(WEB_POST, "/ntb/save",
                                      [this](WebRequest& req, WebResponse& res) { handleSave(req, res); });
}

void NetworkBridgeConfigPage::handlePage(WebRequest& req, WebResponse& res)
{
    std::string chParam = req.getQueryParam("ch");
    int selectedCh = chParam.empty() ? -1 : atoi(chParam.c_str());

    uint8_t count = openknxNetworkBridgeModule.getNumberOfChannels();

    std::string html;
    html.reserve(2048);
    html += "<div class='container'><h1>Netzwerkbrücke</h1>";

    // Dropdown nur relevanter Kanäle (konfiguriert + mit Datei-Feldern)
    html += "<table class='attribute-table'><tbody><tr><td>Kanal</td><td>"
            "<select class='ntb-sel' onchange='ntbSelect(this.value)'>"
            "<option value=''>&mdash; Kanal w&auml;hlen &mdash;</option>";

    bool selectedValid = false;
    for (uint8_t i = 0; i < count; i++)
    {
        NetworkBridgeFunction* fn = channelFunction(i);
        if (!fn)
            continue;
        const ConfigFileField* fields = nullptr;
        if (fn->configFields(fields) == 0)
            continue;

        std::string opt = "Kanal " + std::to_string(i + 1) + " (" + fn->typeName() + ")";

        html += "<option value='" + std::to_string(i) + "'";
        if ((int)i == selectedCh)
        {
            html += " selected";
            selectedValid = true;
        }
        html += ">" + htmlEscape(opt) + "</option>";
    }
    html += "</select></td></tr></tbody></table>";

    if (!selectedValid)
    {
        html += "<p class='ntb-hint'>Bitte einen Kanal w&auml;hlen.</p></div>";
        res.setContentType("text/html");
        res.setLayout(true);
        res.setActiveMenu("/ntb");
        res.send(html.c_str());
        return;
    }

    // Felder des gewählten Kanals
    NetworkBridgeFunction* fn = channelFunction((uint8_t)selectedCh);
    const ConfigFileField* fields = nullptr;
    uint8_t n = fn->configFields(fields);

    std::string heading = "Kanal " + std::to_string(selectedCh + 1) + " (" + fn->typeName() + ")";
    html += "<h2>" + htmlEscape(heading) + "</h2>";
    html += "<table class='attribute-table ntb-table'><tbody>";

    for (uint8_t f = 0; f < n; f++)
    {
        const ConfigFileField& field = fields[f];
        std::string content = readChannelFileContent((uint8_t)selectedCh, field.type);
        std::string elId = std::string("f_") + field.type;

        html += "<tr><td>" + htmlEscape(field.label) + "</td><td><div class='ntb-edit'>";
        html += "<div class='ntb-edit-wrap'>";
        if (field.multiline)
            html += "<textarea id='" + elId + "'>" + htmlEscape(content) + "</textarea>";
        else
            html += "<input type='text' id='" + elId + "' value='" + htmlEscape(content) + "'>";
        const char* etsVal = fn->fieldEtsValue(f);
        if (etsVal && etsVal[0])
            html += "<p class='ntb-field-hint'>ETS-Wert: <code>" + htmlEscape(etsVal) + "</code></p>";
        const char* extraHint = fn->fieldExtraHint(f);
        if (extraHint && extraHint[0])
            html += "<p class='ntb-field-hint'>" + htmlEscape(extraHint) + "</p>";
        if (field.hint)
            html += "<p class='ntb-field-hint'>" + htmlEscape(field.hint) + "</p>";
        html += "</div>";
        html += "<button onclick=\"ntbSave(" + std::to_string(selectedCh) + ",'" + field.type + "','" + elId + "')\">Speichern</button>";
        html += "<span class='ntb-status' id='s_" + std::string(field.type) + "'></span>";
        html += "</div></td></tr>";
    }

    html += "</tbody></table></div>";

    res.setContentType("text/html");
    res.setLayout(true);
    res.setActiveMenu("/ntb");
    res.send(html.c_str());
}

void NetworkBridgeConfigPage::handleSave(WebRequest& req, WebResponse& res)
{
    res.setContentType("text/plain");

    std::string chParam = req.getQueryParam("ch");
    std::string type = req.getQueryParam("type");
    if (chParam.empty() || type.empty())
    {
        res.setStatus(400);
        res.send("Bad request");
        return;
    }

    int ch = atoi(chParam.c_str());
    NetworkBridgeFunction* fn = (ch >= 0) ? channelFunction((uint8_t)ch) : nullptr;
    if (!fn)
    {
        res.setStatus(404);
        res.send("Channel not found");
        return;
    }

    // type gegen Allowlist der Felder genau dieses Kanaltyps prüfen
    const ConfigFileField* fields = nullptr;
    uint8_t n = fn->configFields(fields);
    bool allowed = false;
    for (uint8_t f = 0; f < n; f++)
    {
        if (type == fields[f].type)
        {
            allowed = true;
            break;
        }
    }
    if (!allowed)
    {
        res.setStatus(403);
        res.send("Field not allowed");
        return;
    }

    char path[32];
    NetworkBridgeFunction::channelFilePath((uint8_t)ch, type.c_str(), path, sizeof(path));

    size_t len = req.bodyLength();

    // Leerer Wert → Datei löschen (Reset auf ETS-Wert/Fallback)
    if (len == 0)
    {
        LittleFS.remove(path);
        res.send("OK");
        return;
    }

    LittleFS.mkdir("/ntb");
    File f = LittleFS.open(path, "w");
    if (!f)
    {
        res.setStatus(500);
        res.send("Cannot open file");
        return;
    }
    f.write(req.body(), len);
    f.close();

    // LittleFS schreibt erst beim close() — write() meldet keinen Platzmangel.
    // Verlässlich nur über Dateigröße nach dem Schließen prüfbar.
    File verify = LittleFS.open(path, "r");
    size_t actual = verify ? verify.size() : 0;
    if (verify)
        verify.close();
    if (actual != len)
    {
        LittleFS.remove(path);
        res.setStatus(413);
        res.send("Dateisystem voll");
        return;
    }

    res.send("OK");
}

#endif // OPENKNX_WEBSERVER && KNX_IP
