### Status

Legt fest, ob und wie das Ergebnis des HTTP-Requests als KNX-Wert ausgegeben wird.

- **Aus**: Kein Statusobjekt – das Ergebnis wird ignoriert.
- **Status**: 1-Bit-Ausgang: `1` bei HTTP 200, sonst `0`.
- **Response-Code**: 2-Byte-Ausgang: der HTTP-Statuscode (z. B. `200`, `404`). Bei Verbindungsfehler wird `0` gesendet.

