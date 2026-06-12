### JSON-Filter

Wenn aktiviert, wird der MQTT-Payload als JSON interpretiert und ein einzelner Wert per Pfad extrahiert.
Der Pfad folgt dem Standard RFC 6901 (JSON Pointer) und wird aus der Datei `/ntb/<Kanalnummer>.select.txt` gelesen.

Beispiele für den Dateiinhalt (eine Zeile):

- `/temp` → extrahiert aus `{"temp":22.5}` den Wert `22.5`
- `/sensors/0/val` → extrahiert aus `{"sensors":[{"val":10}]}` den Wert `10`

Ist die Checkbox deaktiviert oder die Datei nicht vorhanden, wird der gesamte Payload als Wert verwendet.

