# AGENTS for OFM-NetworkBridge

## Ziel

Dieses Modul soll neue Netzwerk-Funktionen als separate Channel-Typen bereitstellen.

## Regeln fuer Weiterentwicklung

1. Jede neue Funktion bekommt eine eigene Klasse in `src/NetworkBridge/`.
2. Jede Funktionsklasse muss von `NetworkBridgeFunction` erben.
3. Die Auswahl der Funktionsklasse erfolgt ausschliesslich in `NetworkBridgeModule::createChannel`.
4. XML-Typauswahl in `NetworkBridge.share.xml` und Factory-Switch in C++ muessen synchron bleiben.
5. Pro Kanalparameter in XML muessen die erwarteten `ParamNB_...` Makros in C++ konsistent genutzt werden.

## Regeln fuer ETS-Hilfe und Doku

1. Jede sichtbare `ParameterRefRef` in `src/NetworkBridge.share.xml` und `src/NetworkBridge.templ.xml` bekommt `IndentLevel` und einen passenden `HelpContext` mit Prefix `NTB-`.
2. Jede verwendete `HelpContext`-Id muss in `doc/Applikationsbeschreibung-NetworkBridge.md` als `<!-- DOC HelpContext="..." -->` dokumentiert sein.
3. Die Hilfe-Dateien werden mit `createDoc.ps1` erzeugt (Befehl: `OpenKNXproducer baggages -d doc/Applikationsbeschreibung-NetworkBridge.md -b src/Baggages/Help_de -p NTB`).
4. In `src/NetworkBridge.share.xml` muss der `Baggages`-Block fuer `Help_de.zip` und `Icons.zip` vorhanden bleiben.
5. Nach Doku-/HelpContext-Aenderungen zuerst `createDoc.ps1` im Modul ausfuehren und danach `OpenKNXproducer create --Debug -h include/knxprod.h src/InternetServices-Dev` aus dem Workspace-Root (`OAM-InternetServices`) validieren.
