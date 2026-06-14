<!-- SPDX-License-Identifier: AGPL-3.0-only -->

# Applikationsbeschreibung Netzwerkbrücke

<!-- DOC HelpContext="Dokumentation" -->

Die Netzwerkbrücke stellt pro Kanal eine Netzwerkfunktion bereit.
Verfügbare Kanaltypen:

- Ping
- Wake on LAN
- Webhook empfangen
- Webhook senden
- MQTT empfangen
- MQTT senden

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
Verfügbare Typen: `Ping`, `Wake on LAN`, `Webhook empfangen`, `Webhook senden`, `MQTT empfangen`, `MQTT senden`.

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

> **Voraussetzung:** Der Webserver muss im Netzwerk-Modul aktiviert sein.


<!-- DOC HelpContext="WebhookIn-Pfad" -->
### Pfad

Pfad des Webhook-Endpunkts (ohne führenden Slash), maximal 50 Zeichen.
Der vollständige Endpunkt lautet `/webhook/<Pfad>`.

Beispiel: `licht-an` → erreichbar unter `http://<Geräte-IP>/webhook/licht-an`

<!-- DOC HelpContext="WebhookOut" -->
## Webhook senden

Konfiguration des Kanaltyps `Webhook senden`.
Der Kanal ruft beim Empfang eines KNX-Triggers eine konfigurierte URL per HTTP-GET ab und gibt das Ergebnis als KNX-Statuswert aus.

<!-- DOC HelpContext="WebhookOut-URL" -->
### URL

Ziel-URL, die per GET oder POST abgerufen wird, sobald ein Trigger empfangen wird. Das ETS-Feld ist auf 50 Zeichen begrenzt.

Beispiel: `http://192.168.1.10/api/action`

Für längere URLs kann die Datei `/ntb/<Kanalnummer>.url.txt` auf dem Dateisystem angelegt werden.
Ist diese Datei vorhanden, wird ihr Inhalt als URL verwendet und das ETS-Feld ignoriert.

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
- **POST**: Sendet einen Body aus der Datei `/ntb/<Kanalnummer>.body.txt` auf dem internen Dateisystem. Ist die Datei nicht vorhanden, wird ein leerer Body gesendet.

Zusätzliche HTTP-Header können über die Datei `/ntb/<Kanalnummer>.header.txt` definiert werden.
Format: eine Zeile pro Header, `Schlüssel: Wert`. Die Datei ist optional; ohne sie werden keine zusätzlichen Header gesendet.

<!-- DOC HelpContext="WebhookOut-ContentType" -->
### Content-Type

Legt den MIME-Typ des Requests fest.

- **HTML**: `text/html`
- **Plain**: `text/plain`
- **XML**: `application/xml`
- **JSON**: `application/json`

<!-- DOC HelpContext="MqttIn" -->
## MQTT empfangen

Konfiguration des Kanaltyps `MQTT empfangen`.
Der Kanal abonniert ein MQTT-Topic und schreibt den empfangenen Wert auf ein KNX-Kommunikationsobjekt.
Der Datentyp des Kommunikationsobjekts wird über den Parameter `Datentyp` festgelegt.

> **Voraussetzung:** MQTT muss im Netzwerk-Modul aktiviert und eingerichtet sein.

<!-- DOC HelpContext="MqttIn-Topic" -->
### Topic

MQTT-Topic, das abonniert wird. Das ETS-Feld ist auf 50 Zeichen begrenzt.

Für längere Topics kann die Datei `/ntb/<Kanalnummer>.topic.txt` auf dem Dateisystem angelegt werden.
Ist diese Datei vorhanden, wird ihr Inhalt als Topic verwendet und das ETS-Feld ignoriert.

<!-- DOC HelpContext="MqttIn-Dpt" -->
### Datentyp

Legt fest, wie der empfangene MQTT-Payload in einen KNX-Wert umgewandelt wird.

- **DPT-1 (Bool)**: `1` oder `true` → `1`, alles andere → `0`
- **DPT-5 (8 Bit)**: Ganzzahl 0–255
- **DPT-5.001 (Prozent)**: Prozentwert 0–100, wird auf 0–255 skaliert (100 Prozent → 255)
- **DPT-6**: Vorzeichenbehaftete 8-Bit-Ganzzahl
- **DPT-7**: Vorzeichenlose 16-Bit-Ganzzahl
- **DPT-8**: Vorzeichenbehaftete 16-Bit-Ganzzahl
- **DPT-9**: 16-Bit-Gleitkommazahl
- **DPT-12**: Vorzeichenlose 32-Bit-Ganzzahl
- **DPT-13**: Vorzeichenbehaftete 32-Bit-Ganzzahl
- **DPT-14**: 32-Bit-Gleitkommazahl

<!-- DOC HelpContext="MqttIn-Json" -->
### JSON-Filter

Wenn aktiviert, wird der MQTT-Payload als JSON interpretiert und ein einzelner Wert per Pfad extrahiert.
Der Pfad folgt dem Standard RFC 6901 (JSON Pointer) und wird aus der Datei `/ntb/<Kanalnummer>.select.txt` gelesen.

Beispiele für den Dateiinhalt (eine Zeile):

- `/temp` → extrahiert aus `{"temp":22.5}` den Wert `22.5`
- `/sensors/0/val` → extrahiert aus `{"sensors":[{"val":10}]}` den Wert `10`

Ist die Checkbox deaktiviert oder die Datei nicht vorhanden, wird der gesamte Payload als Wert verwendet.

<!-- DOC HelpContext="MqttOut" -->
## MQTT senden

Konfiguration des Kanaltyps `MQTT senden`.
Der Kanal veröffentlicht den empfangenen KNX-Wert als MQTT-Nachricht auf dem konfigurierten Topic.

> **Voraussetzung:** MQTT muss im Netzwerk-Modul aktiviert und eingerichtet sein.

<!-- DOC HelpContext="MqttOut-Topic" -->
### Topic

MQTT-Topic, auf dem der Wert veröffentlicht wird. Das ETS-Feld ist auf 50 Zeichen begrenzt.

Für längere Topics kann die Datei `/ntb/<Kanalnummer>.topic.txt` auf dem Dateisystem angelegt werden.
Ist diese Datei vorhanden, wird ihr Inhalt als Topic verwendet und das ETS-Feld ignoriert.

<!-- DOC HelpContext="MqttOut-Dpt" -->
### Datentyp

Legt fest, wie der KNX-Wert in einen MQTT-Payload umgewandelt wird.
Die verfügbaren Typen entsprechen denen des Kanaltyps `MQTT empfangen`.

<!-- DOC HelpContext="MqttOut-Qos" -->
### QoS

Quality of Service für die MQTT-Nachricht.

- **QoS 0 (At most once)**: Nachricht wird einmal gesendet, keine Bestätigung.
- **QoS 1 (At least once)**: Nachricht wird mindestens einmal zugestellt, Bestätigung durch Broker.
- **QoS 2 (Exactly once)**: Nachricht wird genau einmal zugestellt, aufwendigeres Handshake.

<!-- DOC HelpContext="MqttOut-Retain" -->
### Retain

Wenn aktiviert, speichert der Broker die letzte Nachricht auf dem Topic und sendet sie sofort an neu verbundene Empfänger.

<!-- DOC HelpContext="MqttOut-DevicePrefix" -->
### Geräte-Präfix

Wenn aktiviert, wird dem Topic automatisch der gerätespezifische Präfix des Geräts vorangestellt (z. B. `openknx/<SerialNr>/`).
Damit lassen sich mehrere Geräte mit identischen Topics eindeutig unterscheiden.

<!-- DOC HelpContext="MqttOut-AsJson" -->
### Als JSON senden

Wenn aktiviert, wird der Wert als JSON-Objekt versendet: `{"value": <Wert>}`.
Ist die Option deaktiviert, wird der Wert direkt als Zeichenkette gesendet.

## Konfigurationsdateien (`/ntb/`)

Lange Zeichenketten, die nicht in das 50-Byte-ETS-Feld passen, sowie Daten ohne ETS-Fallback werden als Dateien auf dem internen LittleFS-Dateisystem abgelegt.
Alle Dateien liegen unter `/ntb/` und folgen dem Schema `/ntb/<Kanalnummer>.<Typ>.txt`.
Die Kanalnummer entspricht dem nullbasierten Kanalindex (Kanal 1 → `0`, Kanal 2 → `1`, usw.).

| Datei | Inhalt | ETS-Fallback |
|---|---|---|
| `/ntb/<ch>.url.txt` | URL (WebhookOut) | Ja – ETS-Feld (≤ 50 Zeichen) |
| `/ntb/<ch>.body.txt` | HTTP-POST-Body (WebhookOut) | Nein – leerer Body |
| `/ntb/<ch>.header.txt` | Zusätzliche HTTP-Header, Format `Schlüssel: Wert` (eine Zeile pro Header) | Nein – keine zusätzlichen Header |
| `/ntb/<ch>.topic.txt` | MQTT-Topic (MQTT empfangen + senden) | Ja – ETS-Feld (≤ 50 Zeichen) |
| `/ntb/<ch>.select.txt` | JSON-Pfad (RFC 6901) für den JSON-Filter (MQTT empfangen) | Nein – gesamter Payload wird verwendet |

**Fallback-Logik:** Ist eine Datei vorhanden, hat sie Vorrang vor dem ETS-Feld.
Ist sie nicht vorhanden, gilt (sofern vorhanden) der ETS-Wert.

**Hinweis:** Die Dateien können über die Web-Seite **Netzwerkbrücke** (siehe unten), den integrierten Web-Dateimanager oder per KNX-Dateitransfer auf das Gerät übertragen werden. Änderungen werden erst nach einem Neustart oder einem erneuten Kanalaufruf (KO-Trigger) aktiv.

## Web-Konfiguration (`/ntb`)

Erfordert einen aktiven Webserver (Compile-Flag `OPENKNX_WEBSERVER`). Im Web-Menü erscheint der Eintrag **Netzwerkbrücke** (`/ntb`).

Die Seite bietet einen komfortablen Editor für die Konfigurationsdateien unter `/ntb/`, ohne den generischen Dateimanager bemühen zu müssen:

- Über ein **Dropdown** wird genau **ein** Kanal ausgewählt. Es werden nur Kanäle angeboten, die konfiguriert sind **und** bearbeitbare Datei-Felder besitzen (MQTT empfangen/senden, Webhook senden). Inaktive Kanäle sowie Typen ohne Datei-Daten (Ping, Wake on LAN, Webhook empfangen) erscheinen nicht.
- Für den gewählten Kanal werden **typ-abhängig** die passenden Felder angezeigt und mit dem aktuellen Dateiinhalt vorbefüllt:
  - **MQTT empfangen**: Topic, JSON-Selektor
  - **MQTT senden**: Topic
  - **Webhook senden**: URL, HTTP-Header, POST-Body
- **Speichern** schreibt den Wert in die zugehörige `/ntb/`-Datei. Ein **leeres Feld** löscht die Datei und stellt damit den ETS-Wert bzw. das Standardverhalten wieder her.

Die Seite verwaltet ausschließlich die `/ntb/`-Dateien; ETS-Parameter selbst sind hier nicht editierbar.
