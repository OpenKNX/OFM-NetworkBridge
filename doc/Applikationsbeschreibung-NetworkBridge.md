<!-- SPDX-License-Identifier: AGPL-3.0-only -->

# Applikationsbeschreibung Network Bridge

<!-- DOC HelpContext="Dokumentation" -->

Die Network Bridge stellt pro Kanal eine Netzwerkfunktion bereit.
Aktuell steht als Kanaltyp ein Ping-Check zur Verfuegung.

## ETS Konfiguration

<!-- DOC HelpContext="Verfuegbare-Kanaele" -->
### Verfuegbare Kanaele

Legt fest, wie viele Kanaele in der ETS sichtbar und konfigurierbar sind.

<!-- DOC HelpContext="Bezeichnung" -->
### Bezeichnung

Frei waehlbarer Name des Kanals.
Der Name wird in der ETS zur besseren Zuordnung von Kanal und Kommunikationsobjekten verwendet.

<!-- DOC HelpContext="Kanaltyp" -->
### Typ

Bestimmt die Funktion des Kanals.
Aktuell ist der Typ `Ping` verfuegbar.

<!-- DOC HelpContext="Kanal-deaktivieren" -->
### Kanal deaktivieren (Test)

Deaktiviert den Kanal fuer Testzwecke, ohne die restliche Konfiguration zu loeschen.

<!-- DOC HelpContext="Ping-Ziel-IP-Adresse" -->
### Ziel IP-Adresse

IP-Adresse des Hosts, der vom Kanal angepingt wird.

<!-- DOC HelpContext="Ping-Automatisch" -->
### Automatischer Ping

Wenn aktiviert, wird der Ping zyklisch ausgefuehrt.
Wenn deaktiviert, erfolgt der Ping nur durch den Trigger ueber das Kommunikationsobjekt.

<!-- DOC HelpContext="Ping-Intervall-Zeitbasis" -->
### Intervall Zeitbasis

Waehlt die Zeiteinheit fuer das automatische Ping-Intervall.

<!-- DOC HelpContext="Ping-Intervall-Zeit" -->
### Intervall Zeit

Definiert die Intervalllaenge fuer den automatischen Ping in der ausgewaehlten Zeitbasis.
