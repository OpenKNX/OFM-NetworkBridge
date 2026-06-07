<!-- SPDX-License-Identifier: AGPL-3.0-only -->

# Applikationsbeschreibung Netzwerkbrücke

<!-- DOC HelpContext="Dokumentation" -->

Die Netzwerkbrücke stellt pro Kanal eine Netzwerkfunktion bereit.
Verfügbare Kanaltypen:

- Ping
- Wake on LAN
- Webhook empfangen
- Webhook senden

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
Verfügbare Typen: `Ping`, `Wake on LAN`, `Webhook empfangen`, `Webhook senden`.

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

<!-- DOC HelpContext="WOL" -->
## Wake on LAN

Konfiguration des Kanaltyps `Wake on LAN`.
Der Kanal sendet beim Empfang eines Triggers ein Magic Packet per UDP-Broadcast an das Netzwerk, um ein Gerät per Wake on LAN aufzuwecken.

<!-- DOC HelpContext="WOL-MAC-Adresse" -->
### MAC-Adresse

MAC-Adresse des Zielgeräts im Format `AA:BB:CC:DD:EE:FF`.
Das Magic Packet wird an diese Adresse adressiert und per Broadcast versandt.

<!-- DOC HelpContext="Ping-Wiederholungen" -->
### Wiederholungen

Anzahl der Wiederholungsversuche, wenn ein Ping keine Antwort erhält (0 = kein Wiederholungsversuch).
Standardwert: 2

<!-- DOC HelpContext="Ping-Timeout" -->
### Timeout (ms)

Maximale Wartezeit in Millisekunden auf eine Ping-Antwort pro Versuch.
Nach Ablauf dieser Zeit gilt der Versuch als fehlgeschlagen und es wird ggf. ein Wiederholungsversuch gestartet.
Standardwert: 1000 ms

<!-- DOC HelpContext="WebhookIn" -->
## Webhook empfangen

Konfiguration des Kanaltyps `Webhook empfangen`.
Der Kanal registriert einen HTTP-GET-Endpunkt unter `/webhook/<Pfad>`. Sobald ein Request eingeht, wird der KNX-Statusausgang auf `1` gesetzt.

<!-- DOC HelpContext="WebhookIn-Pfad" -->
### Pfad

Pfad des Webhook-Endpunkts (ohne führenden Slash).
Der vollständige Endpunkt lautet `/webhook/<Pfad>`.

Beispiel: `licht-an` → erreichbar unter `http://<Geräte-IP>/webhook/licht-an`

<!-- DOC HelpContext="WebhookOut" -->
## Webhook senden

Konfiguration des Kanaltyps `Webhook senden`.
Der Kanal ruft beim Empfang eines KNX-Triggers eine konfigurierte URL per HTTP-GET ab und gibt das Ergebnis als KNX-Statuswert aus.

<!-- DOC HelpContext="WebhookOut-URL" -->
### URL

Ziel-URL, die per GET oder POST abgerufen wird, sobald ein Trigger empfangen wird.

Beispiel: `http://192.168.1.10/api/action`

<!-- DOC HelpContext="WebhookOut-Statusmodus" -->
### Status

Legt fest, ob und wie das Ergebnis des HTTP-Requests als KNX-Wert ausgegeben wird.

- **Aus**: Kein Statusobjekt – das Ergebnis wird ignoriert.
- **Status**: 1-Bit-Ausgang: `1` bei HTTP 200, sonst `0`.
- **Response-Code**: 2-Byte-Ausgang: der HTTP-Statuscode (z. B. `200`, `404`). Bei Verbindungsfehler wird `0` gesendet.

<!-- DOC HelpContext="WebhookOut-Methode" -->
### Methode

Legt fest, ob der Request als GET oder POST gesendet wird.

- **GET**: Einfacher Abruf der URL ohne Body.
- **POST**: Sendet einen Body aus der Datei `/webhook/<Kanalnummer>.txt` auf dem internen Dateisystem. Ist die Datei nicht vorhanden, wird ein leerer Body gesendet.

<!-- DOC HelpContext="WebhookOut-ContentType" -->
### Content-Type

Legt den MIME-Typ des Requests fest.

- **HTML**: `text/html`
- **Plain**: `text/plain`
- **XML**: `application/xml`
- **JSON**: `application/json`
