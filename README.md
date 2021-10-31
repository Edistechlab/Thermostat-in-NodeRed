# Thermostat-in-NodeRed
In diesem Projekt ersetellen wir eine Heizungsteuerung mit zwei ESPs. 
Ein ESP hat einen Temperatursensor DS18B20 verbaut der alle 10 Sekunden mittels MQTT die
Temperatur an den Server sendet.
Der zweite ESP hat ein Relay verbaut das über MQTT gesteuert werden kann und die Heizung an bzw. aus schaltet.
