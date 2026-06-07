### Methode

Legt fest, ob der Request als GET oder POST gesendet wird.

- **GET**: Einfacher Abruf der URL ohne Body.
- **POST**: Sendet einen Body aus der Datei `/webhook/<Kanalnummer>.txt` auf dem internen Dateisystem. Ist die Datei nicht vorhanden, wird ein leerer Body gesendet.

