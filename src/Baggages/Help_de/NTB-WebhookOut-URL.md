### URL

Ziel-URL, die per GET oder POST abgerufen wird, sobald ein Trigger empfangen wird. Das ETS-Feld ist auf 50 Zeichen begrenzt.

Beispiel: `http://192.168.1.10/api/action`

Für längere URLs kann die Datei `/ntb/<Kanalnummer>.url.txt` auf dem Dateisystem angelegt werden.
Ist diese Datei vorhanden, wird ihr Inhalt als URL verwendet und das ETS-Feld ignoriert.

