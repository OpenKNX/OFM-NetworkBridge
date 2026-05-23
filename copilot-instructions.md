# Copilot Instructions: OFM-NetworkBridge

## Kontext

- Modul-Prefix: `NTB`
- Channel-basiertes Modul analog OFM-FunctionBlocks
- Gemeinsame Basisklasse: `NetworkBridgeFunction`

## Konventionen

1. Kanaltyp-Werte in XML und C++-Factory immer synchron halten.
2. Keine Logik direkt im Modul, sondern in konkreten Funktionsklassen.
3. Bei neuen Funktionen zuerst XML-Typauswahl erweitern, dann C++-Factory und neue Klasse.
4. Fuer Trigger/Status-KOs bestehende DPT-Konventionen verwenden.
5. Sichtbare Parameter in der ETS muessen ueber `ParameterRefRef` mit `IndentLevel` und `HelpContext` versehen werden.
6. `HelpContext`-Namen fuer dieses Modul verwenden den Prefix `NTB-`.

## Help/Doku Workflow

1. Fuer jede neue oder geaenderte `HelpContext`-Id einen passenden Abschnitt in `doc/Applikationsbeschreibung-NetworkBridge.md` mit `<!-- DOC HelpContext="..." -->` pflegen.
2. Hilfe-Dateien mit `./createDoc.ps1` im Modulverzeichnis neu erzeugen.
3. Sicherstellen, dass `src/NetworkBridge.share.xml` den `Baggages`-Block fuer `Help_de.zip` und `Icons.zip` enthaelt.
4. Gesamtpruefung immer aus dem Workspace-Root starten: `OpenKNXproducer create --Debug -h include/knxprod.h src/InternetServices-Dev`.

## Sprachqualitaet fuer Doku

1. In der Applikationsbeschreibung und den daraus generierten Help-Dateien deutsche Umlaute normal schreiben (`ä`, `ö`, `ü`, `Ä`, `Ö`, `Ü`, `ß`) und nicht als `ae/oe/ue/ss` ersetzen.
2. Nach Textaenderungen in `doc/Applikationsbeschreibung-NetworkBridge.md` immer `./createDoc.ps1` ausfuehren und mindestens stichprobenartig die generierten Dateien unter `src/Baggages/Help_de` pruefen.
3. Wenn der Benutzer explizit bestimmte Schreibweisen vorgibt, haben diese Vorrang.

## Aktuell dokumentierte HelpContext-Ids

- `NTB-Dokumentation`
- `NTB-Verfuegbare-Kanaele`
- `NTB-Bezeichnung`
- `NTB-Kanaltyp`
- `NTB-Kanal-deaktivieren`
- `NTB-Ping-Ziel-IP-Adresse`
- `NTB-Ping-Automatisch`
- `NTB-Ping-Intervall-Zeitbasis`
- `NTB-Ping-Intervall-Zeit`

## Aktueller Scope

- Typ `0`: Deaktiviert
- Typ `1`: Ping (Template, ohne Runtime-Funktion)

## Ping Runtime Verhalten

1. Bei aktiviertem Automatik-Ping wird in `PingFunction::setup()` sofort ein erster Ping mit `triggerPing(false)` gestartet.
2. Der zyklische Folgetimer verwendet `unsigned long` mit Delta-Vergleich im Stil von FunctionBlocks:
	`now - _startTimeStampForNextPing >= pingIntervalMs()`.
3. `scheduleNextAutomaticPing()` speichert den Startzeitpunkt (`max(1ul, millis())`), nicht einen absoluten Zielzeitpunkt.
4. Die ETS-IP aus `ParamNTB_CHPingTargetAddress` ist Big-Endian codiert und muss fuer `IPAddress(a,b,c,d)` als `>>24, >>16, >>8, >>0` gemappt werden.
5. Logs fuer IP-Adressen sollen `IPAddress::toString().c_str()` verwenden.
