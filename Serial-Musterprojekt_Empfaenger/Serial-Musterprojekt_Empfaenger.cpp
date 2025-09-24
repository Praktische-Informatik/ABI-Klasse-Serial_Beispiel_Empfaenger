/*
  EMPFÄNGER (Receiver) 

  Voraussetzungen:
    - Ein virtuelles COM-Port-Paar (z. B. mit HHD Virtual Serial Port Tool).
      Beispiel: COM1 <-> COM2, wobei der SENDER auf COM1, der EMPFÄNGER auf COM2 liegt.

  Wichtige Hinweise:
    - Diese Beispiel-„main“ nutzt eine Serial-Klasse (Header: Serial/Serial.h).
    - In dieser Version sind Lese-Operationen KLASSISCH BLOCKIEREND:
        read()     -> wartet, bis mindestens 1 Byte empfangen wurde (oder Fehler)
        read(buf,n)-> liest mindestens 1 Byte und anschließend alle sofort verfügbaren Bytes
        readLine() -> wartet, bis ein '\n' empfangen wurde und liefert die Zeile ohne '\n'

    - Handshake: Der Empfänger setzt DTR=true, der Sender sieht das als DSR=true.

  Tipp:
    - Da read()/readLine() blockieren, kehrt das Programm an diesen Stellen erst zurück,
      wenn tatsächlich Daten angekommen sind. Wenn gerade niemand sendet, "hängt" es dort
      absichtlich und verschwendet keine CPU-Zeit – klassisches serielles Verhalten.
*/

#include "Serial/Serial.h"   // Serial-Klasse liegt im Ordner Serial/Serial  (siehe VS-Projektordner)
#include <iostream>
#include <cstring>
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
    string portNr = "2";  // Beispiel: Empfänger auf COM2
    //cin >> portNr;
    cout << "COM Port Nummer (Empfaenger): " << portNr << endl;

    // "COM" + Nummer ergibt den Portnamen (z. B. "COM2")
    string port = "COM" + portNr;

    Serial com(port, 9600, 8, ONESTOPBIT, NOPARITY);

    if (!com.open()) {
        cout << "Schnittstelle Empfaenger konnte NICHT geoeffnet werden. Programmabbruch!" << endl;
        return -1;
    }
    cout << "Schnittstelle Empfaenger geoeffnet" << endl;

    // Signalisiere mit DTR=true, dass der Empfänger bereit ist.
    // (HHD setzt DTR oft automatisch auf true; hier dennoch explizit.)
    cout << "Stelle DTR auf true, um Empfangsbereitschaft zu signalisieren." << endl;
    com.setDTR(true);

    // --- Lese-Beispiele (BLOCKIEREND) ---------------------------------
    // 1) Ein einzelnes Zeichen lesen (als int). read() wartet, bis 1 Byte da ist.
    int val = com.read();
    cout << "Lese ein Zeichen (Integer-Wert): " << val << endl; // z. B. 65 für 'A'

    // 2) Mehrere Einzel-Reads – jede read()-Anweisung blockiert, bis ein Byte ankommt.
    int v1 = com.read();
    cout << "Lese ein Zeichen (als Integer): " << v1 << endl;

    int v2 = com.read();
    cout << "Lese ein Zeichen (als char):    " << (char)v2 << endl;

    int v3 = com.read();
    cout << "Lese ein Zeichen (als char):    " << (char)v3 << endl;

    // 3) Eine komplette Zeile lesen – readLine() kehrt erst mit '\n' zurück.
    cout << "Warte auf eine komplette Zeile (endet mit LF =\\n)..." << endl;
    string zeile = com.readLine();
    cout << "Empfangene Zeile: \"" << zeile << "\"" << endl;

    // 4) Puffer-Lese-Beispiel: Liest mindestens 1 Byte und schnappt dann alles,
    //    was sofort verfügbar ist (bis zur angegebenen Größe).
    char buffer[6] = { 0 };
    int gelesen = com.read(buffer, 5); //Alternativ: com->read(buffer, (int)sizeof(buffer) - 1); Platz für 0-Terminator lassen
    buffer[gelesen] = '\0';
    cout << "Wie viele Zeichen wurden gelesen? --> " << gelesen << endl;
    cout << "Was wurde gelesen: \"" << buffer << "\"" << endl;
    // -----------------------------------------------------------------------

    com.close();
    return 0;
}
