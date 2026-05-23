# OFM-NetworkBridge

Dieses Modul stellt kanalbasierte Netzwerk-Funktionen bereit.

## Aktueller Stand

- Grundgeruest fuer kanalbasierte Funktionen vorhanden
- Basisklasse fuer Funktionen vorhanden
- Erste Funktion: Ping (Template ohne Laufzeitlogik)
- Pro Kanal sind Trigger- und Status-Kommunikationsobjekte vorbereitet

## Architektur

- `NetworkBridgeModule`: Modul mit Channel-Factory
- `NBChannelOwnerModule`: Verwaltet Channel-Lebenszyklus
- `NetworkBridgeFunction`: Gemeinsame Basisklasse fuer konkrete Funktionen
- `PingFunction`: Erste konkrete Funktion als Template

## Hinweise

- Die eigentliche Ping-Logik ist absichtlich noch nicht implementiert.
- Neue Funktionen sollen als eigene Klassen unter `src/NetworkBridge/` angelegt und in der Factory registriert werden.

## Hilfe und Doku regenerieren

Wenn `HelpContext`-Eintraege, ETS-Parameter oder die Applikationsbeschreibung angepasst wurden, sollten die Hilfe-Dateien und der Producer-Lauf neu geprueft werden.

1. Im Modulordner `lib/OFM-NetworkBridge` Hilfe-Dateien neu erzeugen:

	`./createDoc.ps1`

2. Optional pruefen, ob Dateien unter `src/Baggages/Help_de` erzeugt wurden.

3. Danach aus dem Workspace-Root `OAM-InternetServices` den Gesamtlauf pruefen:

	`OpenKNXproducer create --Debug -h include/knxprod.h src/InternetServices-Dev`

Wichtig:

- Der Producer muss aus dem Workspace-Root gestartet werden (nicht aus `lib/OFM-NetworkBridge`).
- In `src/NetworkBridge.share.xml` muss der `Baggages`-Block fuer `Help_de.zip` und `Icons.zip` vorhanden sein.
