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
