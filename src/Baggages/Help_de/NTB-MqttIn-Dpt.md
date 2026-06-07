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

