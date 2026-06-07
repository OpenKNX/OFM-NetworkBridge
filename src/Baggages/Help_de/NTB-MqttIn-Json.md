### JSON-Filter

Wenn aktiviert, wird der MQTT-Payload als JSON interpretiert und ein einzelner Wert per Pfad extrahiert.
Der Pfad folgt dem Standard RFC 6901 (JSON Pointer).

Beispiele:

- Payload `{"temp":22.5}`, Pfad `/temp` → Wert `22.5`
- Payload `{"sensors":[{"val":10}]}`, Pfad `/sensors/0/val` → Wert `10`

Ist die Checkbox deaktiviert, wird der gesamte Payload als Wert verwendet.

