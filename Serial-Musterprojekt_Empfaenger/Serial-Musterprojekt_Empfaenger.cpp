/*
  EMPFÄNGER (Receiver)

  Voraussetzungen:
    - Ein virtuelles COM-Port-Paar (z. B. mit HHD Virtual Serial Port Tool).
      Beispiel: COM1 <-> COM2, wobei der SENDER auf COM1, der EMPFÄNGER auf COM2 liegt.

  Wichtige Hinweise:
    - Diese Beispiel-„main“ nutzt eine Serial-Klasse (Header liegt im Projekt unter Serial/Serial.h).
    - In unserer empfohlenen Serial-Implementierung sind Lese-Operationen NICHT blockierend:
        read()  -> gibt -1 zurück, wenn aktuell kein Byte vorliegt
        readLine() -> gibt die bis dahin gelesenen Zeichen zurück, wenn noch kein '\n' kam
    - Handshake: Der Empfänger setzt DTR=true, der Sender sieht das als DSR=true.
*/

#include "Serial/Serial.h"   // Serial-Klasse liegt im Ordner Serial/Serial  (siehe VS-Projektordner)
#include <iostream>
using namespace std;

// Übertragungs-Steuerzeichen (für spätere Protokoll-Erweiterungen nützlich)
char ETX = 0x03; // End of Text (Ende eines Datenblocks)
char ACK = 0x06; // Acknowledge (fehlerfreie Übertragung)
char NAK = 0x15; // No Acknowledge (fehlerhafte Übertragung)
char SOH = 0x01; // Start of Heading (Start eines Blockes)
char EOT = 0x04; // End of Transmission (Ende der Übertragung)
char CAN = 0x18; // Cancel (Abbruch der Übertragung)

int main()
{
    // COM-Port-Nummer: hier zur Demo fest verdrahtet (statt cin >> port_nr)
    string port_nr = "2";   // Beispiel: Empfänger auf COM2
    cout << "COM Port Nummer (Empfaenger): " << port_nr << endl;

    // "COM" + Nummer ergibt den Portnamen (z. B. "COM2")
    string serieller_port("COM");
    serieller_port += port_nr;

    Serial* com = new Serial((string)serieller_port, 9600, 8, ONESTOPBIT, NOPARITY);

    if (!com->open()) {
        cout << "Schnittstelle Empfaenger konnte NICHT geoeffnet werden. Programmabbruch!" << endl;
        return -1;
    }
    cout << "Schnittstelle Empfaenger geoeffnet" << endl;

    // Signalisiere mit DTR=true, dass der Empfänger bereit ist.
    // (HHD setzt DTR oft automatisch auf true; hier dennoch explizit.)
    cout << "Stelle DTR auf true, um Empfangsbereitschaft zu signalisieren." << endl;
    com->setDTR(true);

    // --- Lese-Beispiele ----------------------------------------------------
    // Hinweis: read() ist nicht blockierend (siehe Serial-Implementierung). Wenn gerade nichts anliegt, kommt -1.
    cout << "Lese ein Zeichen (Integer-Wert): " << com->read() << endl; // z. B. 65 für 'A'

    // Zeichenketten werden Zeichen für Zeichen gesendet; je nach Timing kommt -1,
    // wenn noch nichts vorliegt. Beispiele:
    cout << "Lese ein Zeichen (als Integer): " << com->read() << endl;
    cout << "Lese ein Zeichen (als char):    " << (char)com->read() << endl;
    cout << "Lese ein Zeichen (als char):    " << (char)com->read() << endl;

    // readLine(): Liest bis '\n'. Falls noch kein Newline anliegt, gibt sie den
    // bisherigen Inhalt zurück (nicht-blockierend).
    cout << "Lese eine Zeile: " << com->readLine() << endl;

    // Puffer-Lese-Beispiel:
    // Dynamische, uninitialisierte char*-Puffer führen leicht zu Speicherfehlern.
    // Ein statischer Puffer wie unten ist sicher:
    char buffer[6] = { 0 };
    cout << "Wie viele Zeichen wurden gelesen? --> " << com->read(buffer, 6) << endl;
    cout << "Was wurde gelesen: \"" << buffer << "\"" << endl;
    // -----------------------------------------------------------------------

    com->close();
    return 0;
}
