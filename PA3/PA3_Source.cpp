//Amanda Kaufman
//CISS 350A
//Programming Assignment 3
//This program is a scheduler for a doctor's office.  It allows patients and doctors
//to check in and out, assigning doctors to rooms if they are available, and assigning
//patients to doctors based on needed specialty.

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstddef>	//For NULL
#include <new>		//For bad_alloc
#include "Scheduler.h"
#include "QueType.h"
using namespace std;

int main()
{
	//Declare variables for user input.
	char doctorPatient;	//Find out if the user is a doctor or patient
	char checkInOut;	//Find out if the user wants to check in or out
	char sure;			//In case the user enters faulty input, ask them if they're sure they want to quit

	//Open an output file stream object for program output.
	ofstream outFile;
	outFile.open("trans.out");

	//Declare an instance of the Scheduler() class for this doctor's office.
	Scheduler docOffice;

	//Output a welcome message
	cout << "Welcome to your friendly neighborhood doctor's office!\n";
	outFile << "Welcome to your friendly neighborhood doctor's office!\n";

	do
	{
		cout << "\nType D for Doctor or P for Patient.\nType any other key to quit the program. ";
		outFile << "\nType D for Doctor or P for Patient.\nType any other key to quit the program. ";

		//Debugging purposes: offer a print rooms option
		cout << "\nType R to see the a current room status list: ";

		cin >> doctorPatient;
		cin.ignore();
		outFile << doctorPatient << endl;

		//Doctor
		if (doctorPatient == 'd' || doctorPatient == 'D')
		{
			cout << "\nType I for check-in or O for check-out.\nType any other key to go back: ";
			outFile << "\nType I for check-in or O for check-out.\nType any other key to go back: ";
			cin >> checkInOut;
			cin.ignore();
			outFile << checkInOut << endl;

			//Doctor check-in
			if (checkInOut == 'i' || checkInOut == 'I')
			{
				docOffice.doctorCheckIn(outFile);
			}

			//Doctor check-out
			else if (checkInOut == 'o' || checkInOut == 'O')
			{
				docOffice.doctorCheckOut(outFile);
			}
		}

		//Patient
		else if (doctorPatient == 'p' || doctorPatient == 'P')
		{
			cout << "\nType I for check-in or O for check-out.\nType any other key to go back: ";
			outFile << "\nType I for check-in or O for check-out.\nType any other key to go back: ";
			cin >> checkInOut;
			cin.ignore();
			outFile << checkInOut << endl;

			//Patient check-in
			if (checkInOut == 'i' || checkInOut == 'I')
			{
				docOffice.patientCheckIn(outFile);
			}

			//Patient check-out
			else if (checkInOut == 'o' || checkInOut == 'O')
			{
				docOffice.patientCheckOut(outFile);
			}
		}

		//Debugging purposes: Print list of rooms
		else if (doctorPatient == 'r' || doctorPatient == 'R')
		{
			docOffice.printRoomList(outFile);
		}
		else
		{
			cout << "\nAre you sure you want to quit? (y/n): ";
			outFile << "\nAre you sure you want to quit? (y/n): ";
			cin >> sure;
			cin.ignore();
		}

	} while (doctorPatient == 'd' || doctorPatient == 'D' || doctorPatient == 'p' || 
		doctorPatient == 'P' || doctorPatient == 'r' || doctorPatient == 'R' || sure == 'n' || sure == 'N');

	cout << "\nThanks for using the program!" << endl;
	outFile << "\nThanks for using the program!" << endl;

	//Close the output file.
	outFile.close();

	system("pause");

	return 0;
}