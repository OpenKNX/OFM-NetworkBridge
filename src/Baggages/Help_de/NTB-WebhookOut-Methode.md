### Methode

Legt fest, ob der Request als GET oder POST gesendet wird.

- **GET**: Einfacher Abruf der URL ohne Body.
- **POST**: Sendet einen Body aus der Datei `/ntb/<Kanalnummer>.body.txt` auf dem internen Dateisystem. Ist die Datei nicht vorhanden, wird ein leerer Body gesendet.

Zusätzliche HTTP-Header können über die Datei `/ntb/<Kanalnummer>.header.txt` definiert werden.
Format: eine Zeile pro Header, `Schlüssel: Wert`. Die Datei ist optional; ohne sie werden keine zusätzlichen Header gesendet.

