# AGENTS for OFM-NetworkBridge

## Ziel

Dieses Modul soll neue Netzwerk-Funktionen als separate Channel-Typen bereitstellen.

## Regeln fuer Weiterentwicklung

1. Jede neue Funktion bekommt eine eigene Klasse in `src/NetworkBridge/`.
2. Jede Funktionsklasse muss von `NetworkBridgeFunction` erben.
3. Die Auswahl der Funktionsklasse erfolgt ausschliesslich in `NetworkBridgeModule::createChannel`.
4. XML-Typauswahl in `NetworkBridge.share.xml` und Factory-Switch in C++ muessen synchron bleiben.
5. Pro Kanalparameter in XML muessen die erwarteten `ParamNB_...` Makros in C++ konsistent genutzt werden.
