<!-- SPDX-License-Identifier: AGPL-3.0-only -->

# Applikationsbeschreibung Network Bridge

<!-- DOC HelpContext="Dokumentation" -->

Die Network Bridge stellt pro Kanal eine Netzwerkfunktion bereit.
Aktuell steht als Kanaltyp ein Ping-Check zur Verfügung.

## ETS Konfiguration

<!-- DOC HelpContext="Verfuegbare-Kanaele" -->
### Verfügbare Kanäle

Legt fest, wie viele Kanäle in der ETS sichtbar und konfigurierbar sind.

<!-- DOC HelpContext="Bezeichnung" -->
### Bezeichnung

Frei wählbarer Name des Kanals.
Der Name wird in der ETS zur besseren Zuordnung von Kanal und Kommunikationsobjekten verwendet.

<!-- DOC HelpContext="Kanaltyp" -->
### Typ

Bestimmt die Funktion des Kanals.
Aktuell ist der Typ `Ping` verfügbar.

<!-- DOC HelpContext="Kanal-deaktivieren" -->
### Kanal deaktivieren (Test)

Deaktiviert den Kanal für Testzwecke, ohne die restliche Konfiguration zu löschen.

<!-- DOC HelpContext="Ping" -->
## Ping

Konfiguration des Kanaltyps `Ping`.
Der Kanal prüft die Erreichbarkeit einer Ziel-IP-Adresse und stellt Trigger- und Statusobjekte bereit.

<!-- DOC HelpContext="Ping-Ziel-IP-Adresse" -->
### Ziel IP-Adresse

IP-Adresse des Hosts, der vom Kanal angepingt wird.

<!-- DOC HelpContext="Ping-Automatisch" -->
### Automatischer Ping

Wenn aktiviert, wird der Ping zyklisch ausgeführt.
Wenn deaktiviert, erfolgt der Ping nur durch den Trigger über das Kommunikationsobjekt.

<!-- DOC HelpContext="Ping-Intervall-Zeitbasis" -->
### Intervall Zeitbasis

Wählt die Zeiteinheit für das automatische Ping-Intervall.

<!-- DOC HelpContext="Ping-Intervall-Zeit" -->
### Intervall Zeit

Definiert die Intervalllänge für den automatischen Ping in der ausgewählten Zeitbasis.

<!-- DOC HelpContext="Ping-Wiederholungen" -->
### Wiederholungen

Anzahl der Wiederholungsversuche, wenn ein Ping keine Antwort erhält (0 = kein Wiederholungsversuch).
Standardwert: 2

<!-- DOC HelpContext="Ping-Timeout" -->
### Timeout (ms)

Maximale Wartezeit in Millisekunden auf eine Ping-Antwort pro Versuch.
Nach Ablauf dieser Zeit gilt der Versuch als fehlgeschlagen und es wird ggf. ein Wiederholungsversuch gestartet.
Standardwert: 1000 ms
