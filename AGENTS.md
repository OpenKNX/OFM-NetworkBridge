# AGENTS for OFM-NetworkBridge

## Ziel

Dieses Modul soll neue Netzwerk-Funktionen als separate Channel-Typen bereitstellen.

## Versionierungs-Schema (templ.xml)

| Platzhalter | Bedeutung | Einschränkung |
|-------------|-----------|---------------|
| `%T%` | ModuleType | **Einstellig (0–9)** |
| `%CCC%` | Kanal-Nummer (3-stellig) | Ermöglicht mehr als 99 Kanäle |
| `%C%` | Kanal-Nummer (für Anzeige) | – |

Parameter- und ComObject-IDs folgen dem Schema `%AID%_UP-%T%%CCC%NNN` (`NNN` = dreistellige Nummer im Kanal).
Standalone `%T%` (ohne `%CCC%`) wird als `%T%0` geschrieben. `%TT%` und `%CC%` entfallen vollständig.

## Filesystem-Fallback-Konvention

ETS-Stringfelder sind auf **50 Byte** begrenzt (`SizeInBit="400"`). Längere Werte und Daten ohne ETS-Feld liegen als `/ntb/<channelIndex>.<typ>.txt` auf LittleFS.

| Typ | Inhalt | ETS-Fallback |
|-----|--------|--------------|
| `url` | URL (WebhookOut) | Ja |
| `body` | HTTP-POST-Body | Nein |
| `header` | HTTP-Header (`Key: Value`, eine Zeile pro Header) | Nein |
| `topic` | MQTT-Topic | Ja |
| `select` | JSON-Selektor (RFC 6901 Pointer) | Nein |

Lesereihenfolge: Datei vorhanden → Datei; sonst ETS-Parameter.
Helfer `readChannelFile`, `readChannelFileOrParam`, `getChannelFilename` sind in `NetworkBridgeFunction` definiert — keine String-Member für ETS/Datei-Daten in abgeleiteten Klassen.
Stack-Buffer für ETS-Strings: immer **Feldlänge + 1** reservieren (z. B. 50-Zeichen-Feld → `char buf[51]`), damit das `\0`-Terminierungszeichen immer Platz hat.

## Regeln fuer Weiterentwicklung

1. Jede neue Funktion bekommt eine eigene Klasse in `src/NetworkBridge/`.
2. Jede Funktionsklasse muss von `NetworkBridgeFunction` erben.
3. Die Auswahl der Funktionsklasse erfolgt ausschliesslich in `NetworkBridgeModule::createChannel`.
4. XML-Typauswahl in `NetworkBridge.share.xml` und Factory-Switch in C++ muessen synchron bleiben.
5. Pro Kanalparameter in XML muessen die erwarteten `ParamNB_...` Makros in C++ konsistent genutzt werden.

## Regeln fuer ETS-Hilfe und Doku

1. Jede sichtbare `ParameterRefRef` in `src/NetworkBridge.share.xml` und `src/NetworkBridge.templ.xml` bekommt `IndentLevel` und einen passenden `HelpContext` mit Prefix `NTB-`.
2. Jede verwendete `HelpContext`-Id muss in `doc/Applikationsbeschreibung-NetworkBridge.md` als `<!-- DOC HelpContext="..." -->` dokumentiert sein.
3. Die Hilfe-Dateien werden mit `createDoc.ps1` erzeugt (Befehl: `OpenKNXproducer baggages -d doc/Applikationsbeschreibung-NetworkBridge.md -b src/Baggages/Help_de -p NTB`).
4. In `src/NetworkBridge.share.xml` muss der `Baggages`-Block fuer `Help_de.zip` und `Icons.zip` vorhanden bleiben.
5. Nach Doku-/HelpContext-Aenderungen zuerst `createDoc.ps1` im Modul ausfuehren und danach `OpenKNXproducer create --Debug -h include/knxprod.h src/InternetServices-Dev` aus dem Workspace-Root (`OAM-InternetServices`) validieren.
6. Deutsche Texte in `doc/Applikationsbeschreibung-NetworkBridge.md` mit echten Umlauten schreiben (`ä`, `ö`, `ü`, `ß`) und keine Umschreibungen wie `ae/oe/ue` verwenden, sofern nicht explizit anders gefordert.
7. Nach jeder Textaenderung an der Applikationsbeschreibung mindestens eine kurze Sichtpruefung der erzeugten Dateien in `src/Baggages/Help_de` durchfuehren.
8. Im Ping-Kanal (`src/NetworkBridge.templ.xml`) die Zeitparameter immer als Tabelle (Inline-Grid) umsetzen, analog OFM-FunctionBlocks (Zeitwert und Zeitbasis in einer gemeinsamen Zeile).

## Regeln fuer Ping Runtime

1. Bei aktiviertem Automatik-Ping in `PingFunction::setup()` sofort `triggerPing(false)` aufrufen.
2. Zeitmessung fuer den zyklischen Ping immer mit `unsigned long` und Delta-Vergleich umsetzen (`now - start >= interval`), analog OFM-FunctionBlocks.
3. Als Zeitmarke den Startzeitpunkt speichern (`_startTimeStampForNextPing = max(1ul, millis())`).
4. Die ETS-IP aus `ParamNTB_CHPingTargetAddress` ist Big-Endian codiert; Mapping nach `IPAddress(a,b,c,d)` erfolgt als `>>24, >>16, >>8, >>0`.
5. IP-Logging ueber `toString().c_str()` ausgeben.
