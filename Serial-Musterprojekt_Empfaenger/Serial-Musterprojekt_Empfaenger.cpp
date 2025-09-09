#include "Serial/Serial.h" // Beachte! Die Klasse Serial.h ist im Ordner Serial -> Siehe VS Projekt Ordner.
#include <iostream>
using namespace std;

// Übertragungs-Steuerzeichen
char ETX = 0x03; // End of Text (Ende eines Datenblocks);
char ACK = 0x06; // Acknowledge (fehlerfreie Übertragung);
char NAK = 0x15; // No Acknowledge (fehlerhafte Übertragung);
char SOH = 0x01; // Start of Heading (Start eines Blockes)
char EOT = 0x04;  // End of Transmission (Ende der Übertragung);
char CAN = 0x18;  //Cancel (Abbruch der Übertragung);Medium

/************
* EMPFÄNGER
*************/

int main()
{

	string port_nr = "";
	cout << "COM Port Nummer: ";

	//cin >> port_nr;
	port_nr = "2";

	string serieller_port("COM");
	serieller_port += port_nr;

	Serial* com1 = new Serial((string)serieller_port, 9600, 8, NOPARITY, ONESTOPBIT);

	if (!com1->open()) {
		cout << "Schnittstelle Empfaenger konnte NICHT geoeffnet werden. Programmabbruch!" << endl;
		return -1;
	}
	cout << "Schnittstelle Empfaenger geoeffnet" << endl;

	cout << "Stelle DTR auf true, um Enpfangsbereitschaft zu signalisieren." << endl;
	com1->setDTR(true); // Diese Zeile kann auch auskommentiert werden, da die HHD Software DTR am Anfang immer auf true setzt.

	// read() blockiert bzw. wartet so lange, bis es etwas empfängt. 
	// es wird ein Binärcode empfangen, hier:  0100 0001, der als Integer interpretiert wird. 
	cout << "Lese ein Zeichen " << com1->read() << endl; // statt 'A' wird 65 ausgegeben.

	cout << "Lese eine Zeichenkette " << com1->read() << endl;			//Ausgabe: 66 >> Beachte: Es wird Zeichen für Zeichen gesendet.
	cout << "Lese eine Zeichenkette " << (char)com1->read() << endl;	//Ausgabe: 'a' >> 0100 0010 wird hier als char interpretiert
	cout << "Lese eine Zeichenkette " << (char)com1->read() << endl;	//Ausgabe: 'a' >> 0100 0010 wird hier als char interpretiert
	cout << "Lese eine Zeichenkette " << com1->readLine() << endl;		//Ausgabe: "llBall" >> readLine(9 ließt bis zum '\n'. Fehlt dieses, blockiert die Methode.

	// char* buffer geht nicht! --> Speicherzugriffsverletzung
	char buffer[6] = {0}; 
	cout << "Wie viele Zeichen wurden gesendet? --> " << com1->read(buffer, 6) << endl;
	cout << "Was wurde gesendet: " << buffer << endl;
	
	com1->close();
	return 0;
}