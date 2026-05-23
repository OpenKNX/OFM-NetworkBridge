# Copilot Instructions: OFM-NetworkBridge

## Kontext

- Modul-Prefix: `NB`
- Channel-basiertes Modul analog OFM-FunctionBlocks
- Gemeinsame Basisklasse: `NetworkBridgeFunction`

## Konventionen

1. Kanaltyp-Werte in XML und C++-Factory immer synchron halten.
2. Keine Logik direkt im Modul, sondern in konkreten Funktionsklassen.
3. Bei neuen Funktionen zuerst XML-Typauswahl erweitern, dann C++-Factory und neue Klasse.
4. Fuer Trigger/Status-KOs bestehende DPT-Konventionen verwenden.

## Aktueller Scope

- Typ `0`: Deaktiviert
- Typ `1`: Ping (Template, ohne Runtime-Funktion)
