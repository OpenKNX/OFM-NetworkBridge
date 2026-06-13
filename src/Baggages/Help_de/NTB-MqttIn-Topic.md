### Topic

MQTT-Topic, das abonniert wird. Das ETS-Feld ist auf 50 Zeichen begrenzt.
Wildcards sind erlaubt:

- `+` steht für genau eine Ebene (z. B. `sensoren/+/temperatur`)
- `#` steht für beliebig viele Ebenen am Ende (z. B. `sensoren/#`)

Für längere Topics kann die Datei `/ntb/<Kanalnummer>.topic.txt` auf dem Dateisystem angelegt werden.
Ist diese Datei vorhanden, wird ihr Inhalt als Topic verwendet und das ETS-Feld ignoriert.

