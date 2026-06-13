#pragma once
#include "OpenKNX.h"
#include <string>

// Beschreibt ein editierbares /ntb/-Datei-Feld eines Kanals (für die Web-Konfigurationsseite).
struct ConfigFileField
{
    const char* type;     // Datei-Suffix: "topic","select","url","header","body"
    const char* label;    // UI-Label, z. B. "Topic", "JSON-Selektor"
    bool multiline;       // true → <textarea> (header, body), sonst <input>
    const char* hint;     // optionaler Hinweistext unter dem Feld, nullptr = kein Hinweis
};

class NetworkBridgeFunction : public OpenKNX::Channel
{
    std::string _name;

protected:
    NetworkBridgeFunction(uint8_t channelIndex, const char* name);

    void getChannelFilename(char* out, size_t outLen, const char* type) const;
    bool readChannelFile(const char* type, char* buf, size_t bufLen) const;
    void readChannelFileOrParam(const char* type, char* buf, size_t bufLen, const char* paramValue) const;

public:
    void setup() override;
    const std::string name() override;

    virtual bool processCommand(const std::string cmd, bool diagnoseKo);
    virtual const char* typeName() const = 0;

    // Datei-Felder dieses Kanals für die Web-Konfiguration. Basis: keine.
    virtual uint8_t configFields(const ConfigFileField*& out) const
    {
        out = nullptr;
        return 0;
    }

    // ETS-Rohwert für Feld fieldIndex (nullptr = kein ETS-Fallback für dieses Feld).
    virtual const char* fieldEtsValue(uint8_t /*fieldIndex*/) const { return nullptr; }

    // Zusätzliche Infozeile unter Feld fieldIndex (nullptr = keine).
    virtual const char* fieldExtraHint(uint8_t /*fieldIndex*/) const { return nullptr; }

    // Einheitliches Pfad-Schema /ntb/<channelIndex>.<type>.txt (Single Source of Truth).
    static void channelFilePath(uint8_t channelIndex, const char* type, char* out, size_t outLen);
};
