//Amanda Kaufman
//CISS 350A
//Programming Assignment 3
//This is a scheduler data structure that holds a list of exam rooms in a doctor's office
//and has functions to check doctors and patients in and out of the clinic.

#pragma once

#include <iomanip>
#include <string>
#include <fstream>
#include <cstddef>	//For NULL
#include <new>		//For bad_alloc
#include "QueType.h"
using namespace std;

//Structure to contain a doctor's information.
struct Doctor
{
	string name;
	string specialty;
};

//For some reason, the program wanted the Patient struct in QueType.h and not here

//Structure to hold an exam room's information and the waitlist of patients.
struct ExamRoom
{
	int roomNum;
	bool available;
	Doctor roomDoctor;
	Patient currentPatient;
	int waitLength;
	QueType WaitList;
};

//Constant for the number of rooms at the doctor's office.
const int NUM_ROOMS = 100;

//Scheduler class that performs essential functions for a doctor's office, such as
//doctor and patient check-in and check-out, and assigning patients to doctors.
class Scheduler
{
private:
	//List of exam rooms at the doctor's office.
	ExamRoom roomList[NUM_ROOMS];
public:
	//###########################################################################################
	//Constructor/Destructor.																	#
	//###########################################################################################
	Scheduler()
	{
		//Initialize a blank Doctor object for use in the list.
		Doctor tempDoctor;		
		tempDoctor.name = "";
		tempDoctor.specialty = "";

		//Initialize a blank Patient object for use in the list.
		Patient tempPatient;
		tempPatient.name = "";
		tempPatient.age = 0;
		tempPatient.specialty = "";
		tempPatient.emergency = false;

		//Initialize a set of defaults for the exam rooms in the doctor's office.
		ExamRoom tempRoom;
		tempRoom.available = true;
		tempRoom.roomDoctor = tempDoctor;
		tempRoom.currentPatient = tempPatient;
		tempRoom.waitLength = 0;

		//Assign the defaults to each ExamRoom item in the array.
		for (int index = 0; index < NUM_ROOMS; index++)
		{
			roomList[index] = tempRoom;
			roomList[index].roomNum = (index + 1);
		}
	}

	~Scheduler()
	{
		//Make all wait lists empty.
		for (int index = 0; index < NUM_ROOMS; index++)
		{
			roomList[index].WaitList.makeEmpty();	//Deallocates any memory used by the waitlists.
		}
	}

	//###########################################################################################
	//Print functions.																			#
	//###########################################################################################
	
	//Function to print information about all rooms in the doctor's office.
	//Primarily used for debugging purposes.
	void printRoomList(ofstream &outFile)
	{
		//Output to screen
		cout << "Room List\n==============================================================================\n";
		cout << left << setw(8) << "Room #";
		cout << left << setw(10) << "Available";
		cout << left << setw(20) << "Doctor's Name";
		cout << left << setw(10) << "Specialty";
		cout << left << setw(20) << "Current Patient";
		cout << left << setw(10) << "Wait List";
		cout << "\n------------------------------------------------------------------------------\n";

		for (int index = 0; index < NUM_ROOMS; index++)
		{
			cout << left << setw(8) << roomList[index].roomNum;
			if (roomList[index].available == true)
				cout << left << setw(10) << "YES";
			else
				cout << left << setw(10) << "NO";
			cout << left << setw(20) << roomList[index].roomDoctor.name;
			cout << left << setw(10) << roomList[index].roomDoctor.specialty;
			cout << left << setw(20) << roomList[index].currentPatient.name;
			cout << left << setw(10) << roomList[index].waitLength;
			cout << endl;
		}
		cout << endl;

		//Echo to output file
		outFile << "Room List\n==========================================================\n";
		outFile << left << setw(8) << "Room #";
		outFile << left << setw(10) << "Available";
		outFile << left << setw(20) << "Doctor's Name";
		outFile << left << setw(10) << "Specialty";
		outFile << left << setw(10) << "Wait List";
		outFile << "----------------------------------------------------------\n";

		for (int index = 0; index < NUM_ROOMS; index++)
		{
			outFile << left << setw(8) << roomList[index].roomNum;
			if (roomList[index].available == true)
				outFile << left << setw(10) << "YES";
			else
				outFile << left << setw(10) << "NO";
			outFile << left << setw(20) << roomList[index].roomDoctor.name;
			outFile << left << setw(10) << roomList[index].roomDoctor.specialty;
			outFile << left << setw(10) << roomList[index].waitLength;
			outFile << endl;
		}
		outFile << endl;

		system("pause");
	}

	//Function to print waitlist information for a particular room passed as an argument.
	//Primarily used for debugging purposes.
	void printWaitList(int index, ofstream &outFile)
	{
		//Temporary patient object for display purposes.
		Patient temp;

		cout << "Room #" << (index + 1) << " waitlist currently has " << roomList[index].waitLength << " patients.\n";
		cout << "They are:\n";
		for (int count = 0; count < roomList[index].waitLength; count++)
		{
			temp = roomList[index].WaitList.getPatient(count);
			cout << temp.name << ", age " << temp.age << ", specialty code " << temp.specialty << endl;
		}

		system("pause");
	}

	
	//###########################################################################################
	//Accessor functions.																		#
	//###########################################################################################

	//###########################################################################################
	//Mutator functions.																		#
	//###########################################################################################

	//Function to check in a doctor.  Gets doctor's information and checks to see if the
	//preferred room is available.  Assigns doctor to that room if it is free.
	void doctorCheckIn(ofstream &outFile)
	{
		//Temporary variables to hold information for check-in
		string doctorName;
		int specialtyInput;
		string specialtyCode;
		int prefRoomNum;

		//Get the doctor's name for check-in.
		cout << "\nEnter your name: ";
		outFile << "\nEnter your name: ";
		getline(cin, doctorName, '\n');
		outFile << doctorName << endl;

		//Validate the doctor's name is not too long.
		while (doctorName.length() > 20)
		{
			cout << "Error! Name must be 20 characters or less.  Please try again.\n";
			outFile << "Error! Name must be 20 characters or less.  Please try again.\n";
			cout << "Enter your name: ";
			outFile << "Enter your name: ";
			getline(cin, doctorName, '\n');
			outFile << doctorName << endl;
		}

		//Get the doctor's specialty code for check-in.
		cout << "What is your specialty code? Select from the list below:\n\n";
		outFile << "What is your specialty code? Select from the list below:\n\n";
		cout << "\t1. PED\t\tPediatrics\n"
			<< "\t2. GEN\t\tGeneral Practice\n"
			<< "\t3. INT\t\tInternal Medicine\n"
			<< "\t4. CAR\t\tCardiology\n"
			<< "\t5. SUR\t\tSurgeon\n"
			<< "\t6. OBS\t\tObstetrics\n"
			<< "\t7. PSY\t\tPsychiatry\n"
			<< "\t8. NEU\t\tNeurology\n"
			<< "\t9. ORT\t\tOrthopedics\n"
			<< "\t10. DET\t\tDermatology\n"
			<< "\t11. OPT\t\tOpthalmology\n"
			<< "\t12. ENT\t\tEar, Nose, and Throat\n\n";
		outFile << "\t1. PED\t\tPediatrics\n"
			<< "\t2. GEN\t\tGeneral Practice\n"
			<< "\t3. INT\t\tInternal Medicine\n"
			<< "\t4. CAR\t\tCardiology\n"
			<< "\t5. SUR\t\tSurgeon\n"
			<< "\t6. OBS\t\tObstetrics\n"
			<< "\t7. PSY\t\tPsychiatry\n"
			<< "\t8. NEU\t\tNeurology\n"
			<< "\t9. ORT\t\tOrthopedics\n"
			<< "\t10. DET\t\tDermatology\n"
			<< "\t11. OPT\t\tOpthalmology\n"
			<< "\t12. ENT\t\tEar, Nose, and Throat\n\n";
		cout << "Enter your selection: ";
		outFile << "Enter your selection: ";
		cin >> specialtyInput;
		outFile << specialtyInput << endl;

		//Validate the user's input.
		while (specialtyInput < 1 || specialtyInput > 12)
		{
			cout << "Error! That is not a valid menu choice.  Please try again: ";
			outFile << "Error! That is not a valid menu choice.  Please try again: ";
			cin >> specialtyInput;
			outFile << specialtyInput << endl;
		}

		//Assign a specialty code based on the number entered above.
		switch (specialtyInput)
		{
		case 1: specialtyCode = "PED";
			break;
		case 2: specialtyCode = "GEN";
			break;
		case 3: specialtyCode = "INT";
			break;
		case 4: specialtyCode = "CAR";
			break;
		case 5: specialtyCode = "SUR";
			break;
		case 6: specialtyCode = "OBS";
			break;
		case 7: specialtyCode = "PSY";
			break;
		case 8: specialtyCode = "NEU";
			break;
		case 9: specialtyCode = "ORT";
			break;
		case 10: specialtyCode = "DET";
			break;
		case 11: specialtyCode = "OPT";
			break;
		case 12: specialtyCode = "ENT";
			break;
		default: specialtyCode = "GEN";
			break;
		}

		//Get the doctor's preferred room number.
		cout << "What room number do you prefer? (1-100): ";
		outFile << "What room number do you prefer? (1-100): ";
		cin >> prefRoomNum;
		outFile << prefRoomNum << endl;

		//Validate the user's input.
		while (prefRoomNum < 1 || prefRoomNum > 100)
		{
			cout << "Error! Must enter a room number between 1 and 100.  Please try again.\n";
			outFile << "Error! Must enter a room number between 1 and 100.  Please try again.\n";
			cout << "What room number do you prefer? (1-100): ";
			outFile << "What room number do you prefer? (1-100): ";
			cin >> prefRoomNum;
			outFile << prefRoomNum << endl;
		}

		//Check to see if the doctor's preferred room is available.
		//If it isn't, output an error message and ask for a new room number.
		while (roomList[prefRoomNum - 1].available == false)
		{
			cout << "\nThat room is currently taken.  Please enter a different room number.\n";
			outFile << "\nThat room is currently taken.  Please enter a different room number.\n";
			cout << "\nCurrently available rooms are:\n";
			outFile << "\nCurrently available rooms are:\n";
			for(int index = 0; index < NUM_ROOMS; index++)
			{
				if (roomList[index].available == true)
				{
					cout << roomList[index].roomNum << " ";
					outFile << roomList[index].roomNum << " ";
				}
			}
			cout << "\n\nEnter your next room choice: ";
			outFile << "\n\nEnter your next room choice: ";
			cin >> prefRoomNum;
			outFile << prefRoomNum << endl;
		}

		//Assign the doctor to the room in question.
		//Set the room's availability to "false" in the roomList array.
		roomList[prefRoomNum - 1].available = false;

		//Create a temporary doctor object and assign the input from the user to its fields.
		Doctor tempDoctor;
		tempDoctor.name = doctorName;
		tempDoctor.specialty = specialtyCode;

		//Assign the Doctor object to the room in the roomList array.
		roomList[prefRoomNum - 1].roomDoctor = tempDoctor;

		//Output a message telling the doctor that he/she was successfully checked in.
		cout << "\nYou have been successfully checked in.\n\n";
		outFile << "\nYou have been successfully checked in.\n\n";

		system("pause");
	}

	//Function to check in a patient.  Gets patient's information and calls a function to
	//find a doctor for the patient.
	void patientCheckIn(ofstream &outFile)
	{
		//Temporary variables to hold information for check-in
		string patientName;
		int patientAge;
		int specialtyInput;
		string specialtyCode;
		char emergency;
		bool emergencyStatus;

		//Get the patient's name for check-in.
		cout << "\nEnter your name: ";
		outFile << "\nEnter your name: ";
		getline(cin, patientName, '\n');
		outFile << patientName << endl;

		//Validate the patient's name is not too long.
		while (patientName.length() > 20)
		{
			cout << "Error! Name must be 20 characters or less.  Please try again.\n";
			outFile << "Error! Name must be 20 characters or less.  Please try again.\n";
			cout << "Enter your name: ";
			outFile << "Enter your name: ";
			getline(cin, patientName, '\n');
			outFile << patientName << endl;
		}

		//Get the patient's age
		cout << "Enter your age: ";
		outFile << "Enter your age: ";
		cin >> patientAge;
		outFile << patientAge << endl;

		//Validate the input.
		while (patientAge < 0)
		{
			cout << "Error! You must enter a positive number.  Please try again.\n";
			outFile << "Error! You must enter a positive number.  Please try again.\n";
			cout << "Enter your age: ";
			outFile << "Enter your age: ";
			cin >> patientAge;
			outFile << patientAge << endl;
		}

		//Get the patient's specialty code for check-in.
		//If the patient is under 16 years of age, specialty is automatically PED.
		if (patientAge < 16)
		{
			specialtyInput = 1;
			cout << "Your specialty is: PED.\n";
			outFile << "Your specialty is: PED.\n";
		}
		//If the patient is 16 or older, find out what specialty they need.
		else
		{
			cout << "What specialty do you need? Select from the list below:\n\n";
			outFile << "What specialty do you need? Select from the list below:\n\n";
			cout << "\t1. PED\t\tPediatrics\n"
				<< "\t2. GEN\t\tGeneral Practice\n"
				<< "\t3. INT\t\tInternal Medicine\n"
				<< "\t4. CAR\t\tCardiology\n"
				<< "\t5. SUR\t\tSurgeon\n"
				<< "\t6. OBS\t\tObstetrics\n"
				<< "\t7. PSY\t\tPsychiatry\n"
				<< "\t8. NEU\t\tNeurology\n"
				<< "\t9. ORT\t\tOrthopedics\n"
				<< "\t10. DET\t\tDermatology\n"
				<< "\t11. OPT\t\tOpthalmology\n"
				<< "\t12. ENT\t\tEar, Nose, and Throat\n\n";
			outFile << "\t1. PED\t\tPediatrics\n"
				<< "\t2. GEN\t\tGeneral Practice\n"
				<< "\t3. INT\t\tInternal Medicine\n"
				<< "\t4. CAR\t\tCardiology\n"
				<< "\t5. SUR\t\tSurgeon\n"
				<< "\t6. OBS\t\tObstetrics\n"
				<< "\t7. PSY\t\tPsychiatry\n"
				<< "\t8. NEU\t\tNeurology\n"
				<< "\t9. ORT\t\tOrthopedics\n"
				<< "\t10. DET\t\tDermatology\n"
				<< "\t11. OPT\t\tOpthalmology\n"
				<< "\t12. ENT\t\tEar, Nose, and Throat\n\n";
			cout << "Enter your selection: ";
			outFile << "Enter your selection: ";
			cin >> specialtyInput;
			outFile << specialtyInput << endl;

			//Validate the user's input.
			while (specialtyInput < 1 || specialtyInput > 12)
			{
				cout << "Error! That is not a valid menu choice.  Please try again: ";
				outFile << "Error! That is not a valid menu choice.  Please try again: ";
				cin >> specialtyInput;
				outFile << specialtyInput << endl;
			}
		}

		//Assign a specialty code based on the number entered above.
		switch (specialtyInput)
		{
		case 1: specialtyCode = "PED";
			break;
		case 2: specialtyCode = "GEN";
			break;
		case 3: specialtyCode = "INT";
			break;
		case 4: specialtyCode = "CAR";
			break;
		case 5: specialtyCode = "SUR";
			break;
		case 6: specialtyCode = "OBS";
			break;
		case 7: specialtyCode = "PSY";
			break;
		case 8: specialtyCode = "NEU";
			break;
		case 9: specialtyCode = "ORT";
			break;
		case 10: specialtyCode = "DET";
			break;
		case 11: specialtyCode = "OPT";
			break;
		case 12: specialtyCode = "ENT";
			break;
		default: specialtyCode = "GEN";
			break;
		}

		cin.ignore();	//Clear the keyboard buffer

		//Get the patient's emergency status.
		cout << "Is this an emergency? (y/n): ";
		outFile << "Is this an emergency? (y/n): ";
		emergency = cin.get();

		//Assign an emergency status to the patient if they entered a y or Y.
		if (emergency == 'y' || emergency == 'Y')
			emergencyStatus = true;
		else
			emergencyStatus = false;

		//Create a temporary patient object to add to the waitlist.
		Patient tempPatient;
		tempPatient.name = patientName;
		tempPatient.age = patientAge;
		tempPatient.specialty = specialtyCode;
		tempPatient.emergency = emergencyStatus;

		//Pass the tempPatient object to a function that will assign it to a doctor and exam room
		//based on the information entered.
		assignDoctor(tempPatient, outFile);
	}

	//Function to assign a patient to a doctor and exam room.  If there are no doctors available
	//with the patient's specialty, function outputs a message.  If there are multiple doctors
	//with the patient's specialty, function assigns the patient to the doctor/exam room with the
	//shortest wait list.
	void assignDoctor(Patient newPatient, ofstream &outFile)
	{
		int shortestWaitIndex = 0;	//Holds the index of the room with the shortest wait
		bool foundDoctor = false;	//Flag to indicate when doctor is found

		//If patient is less than 16 years of age, assign them to a pediatrician.
		if (newPatient.age < 16)
		{
			//Search for the first pediatrician in the list, and assign shortestWaitIndex
			//to the index of that doctor.
			for (int index = 0; index < NUM_ROOMS; index++)
			{
				if (roomList[index].roomDoctor.specialty == "PED")
				{
					// Assign the index of that room to the shortestWaitIndex variable for comparison
					shortestWaitIndex = index;

					foundDoctor = true;	//Flag that we found at least one doctor
					index = NUM_ROOMS;	//Stop the for loop from continuing
				}
			}

			//If no doctor was found, output a message to the user.
			if (!foundDoctor)
			{
				cout << "\nSorry, " << newPatient.name << ", there are currently no pediatricians available.\n"
					<< "Please check back soon.\n\n";
				outFile << "\nSorry, " << newPatient.name << ", there are currently no pediatricians available.\n"
					<< "Please check back soon.\n\n";

				system("pause");
			}

			//Otherwise, if a doctor was found, go through the rest of the list and find other pediatricians,
			//using the shortestWaitIndex to find the index of the room with the shortest wait list.
			//Assign the patient to the room with the shortest wait list.
			else
			{
				for (int count = shortestWaitIndex; count < NUM_ROOMS; count++)
				{
					//If the doctor assigned to this room is a pediatrician and the wait list length of
					//the room is shorter than the list of the room at shortestWaitIndex, set the
					//shortestWaitIndex variable to this room.
					if (roomList[count].roomDoctor.specialty == "PED" && roomList[count].waitLength < roomList[shortestWaitIndex].waitLength)
					{
						shortestWaitIndex = count;
					}
				}

				//If the current patient's name in the room with the shortest wait list is blank, there is
				//not currently a patient in the room, so assign this patient as the current one and keep the
				//wait list length at 0.
				if (roomList[shortestWaitIndex].currentPatient.name == "")
				{
					roomList[shortestWaitIndex].currentPatient = newPatient;

					//Inform the patient that they can go get seen right away.
					cout << endl << newPatient.name << ", you can get seen right away by Dr. " << roomList[shortestWaitIndex].roomDoctor.name << "\n"
						<< "in room " << (shortestWaitIndex + 1) << "!  Please go there now.\n\n";
					outFile << endl << newPatient.name << ", you can get seen right away by Dr. " << roomList[shortestWaitIndex].roomDoctor.name << "\n"
						<< "in room " << (shortestWaitIndex + 1) << "!  Please go there now.\n\n";

					system("pause");

					//Debugging purposes: print the wait list
					//printWaitList(shortestWaitIndex, outFile);
				}

				//Otherwise, add this patient to the wait list and increment the wait list length.
				else
				{
					try
					{
						roomList[shortestWaitIndex].WaitList.enqueue(newPatient);
						roomList[shortestWaitIndex].waitLength += 1;

						//Inform the user that they are on the wait list.
						cout << endl << newPatient.name << ", you will be number " << roomList[shortestWaitIndex].waitLength << " on the wait list to see\n"
							<< "Dr. " << roomList[shortestWaitIndex].roomDoctor.name << " in room " << (shortestWaitIndex + 1) << ". "
							<< "Please go there now.\n\n";
						outFile << endl << newPatient.name << ", you will be number " << roomList[shortestWaitIndex].waitLength << " on the wait list to see\n"
							<< "Dr. " << roomList[shortestWaitIndex].roomDoctor.name << " in room " << (shortestWaitIndex + 1) << ". "
							<< "Please go there now.\n\n";

						system("pause");
					}
					catch (FullQueue exception)
					{
						cout << "Error! There is no more room in memory for another patient.\n\n";
						outFile << "Error! There is no more room in memory for another patient.\n\n";
					}
				}
			}
		} //End of if patient is younger than 16

		//For all other patients, search for a doctor according to the specialty requested.
		//If no doctor is available, search for a general practitioner (GEN).
		//If there is no general practitioner, the patient is assigned to any doctor
		//with the shortest wait list.
		else
		{
			//Search for the first doctor in the list that matches the patient's specialty,
			//and assign shortestWaitIndex to the index of that doctor.
			for (int index = 0; index < NUM_ROOMS; index++)
			{
				if (roomList[index].roomDoctor.specialty == newPatient.specialty)
				{
					// Assign the index of that room to the shortestWaitIndex variable for comparison
					shortestWaitIndex = index;

					foundDoctor = true;	//Flag that we found at least one doctor
					index = NUM_ROOMS;	//Stop the for loop from continuing
				}
			}

			//If a doctor was found, go through the rest of the list and search for any additional doctors
			//of the same specialty and find the one with the shortest wait list.
			if (foundDoctor)
			{
				for (int count = shortestWaitIndex; count < NUM_ROOMS; count++)
				{
					//If the doctor assigned to this room matches the patient's specialaty and the wait list length of
					//the room is shorter than the list of the room at shortestWaitIndex, set the shortestWaitIndex
					//variable to this room.
					if (roomList[count].roomDoctor.specialty == newPatient.specialty && 
						(roomList[count].waitLength < roomList[shortestWaitIndex].waitLength || roomList[count].currentPatient.name == ""))
					{
						shortestWaitIndex = count;
					}
				}
			}

			//Otherwise, if no doctor matching the specialty of the patient was found, attempt to find
			//a general practitioner for the patient.
			else
			{
				//Search for the first general practitioner in the list, and assign shortestWaitIndex to the index of that doctor.
				for (int index = 0; index < NUM_ROOMS; index++)
				{
					if (roomList[index].roomDoctor.specialty == "GEN")
					{
						// Assign the index of that room to the shortestWaitIndex variable for comparison
						shortestWaitIndex = index;

						foundDoctor = true;	//Flag that we found at least one doctor
						index = NUM_ROOMS;	//Stop the for loop from continuing
					}
				}

				//If a general practitioner was found, go through the rest of the list and search for any additional general
				//practitioners and find the one with the shortest wait list.
				if (foundDoctor)
				{
					for (int count = shortestWaitIndex; count < NUM_ROOMS; count++)
					{
						//If the doctor assigned to this room is a general practitioner and the wait list length of
						//the room is shorter than the list of the room at shortestWaitIndex, set the shortestWaitIndex
						//variable to this room.
						if (roomList[count].roomDoctor.specialty == "GEN" && 
							(roomList[count].waitLength < roomList[shortestWaitIndex].waitLength || roomList[count].currentPatient.name == ""))
						{
							shortestWaitIndex = count;
						}
					}
				}

				//Otherwise, if no general practitioner was found, go through the entire list (regardless of specialty)
				//and find the doctor of any type with the shortest wait list to serve the patient.
				else
				{
					for (int index = 0; index < NUM_ROOMS; index++)
					{
						if (roomList[index].roomDoctor.specialty != "")
						{
							// Assign the index of that room to the shortestWaitIndex variable for comparison
							shortestWaitIndex = index;

							foundDoctor = true;	//Flag that we found at least one doctor
							index = NUM_ROOMS;	//Stop the for loop from continuing
						}
					}

					//If any doctor was found, go through the rest of the list and search for any additional doctors
					//and find the one with the shortest wait list.
					if (foundDoctor)
					{
						for (int count = shortestWaitIndex; count < NUM_ROOMS; count++)
						{
							//If the wait list length of this room is shorter than the list of the room at shortestWaitIndex,
							//set the shortestWaitIndex variable to this room.
							if (roomList[count].roomDoctor.specialty != "" && 
								(roomList[count].waitLength < roomList[shortestWaitIndex].waitLength || roomList[count].currentPatient.name == ""))
							{
								shortestWaitIndex = count;
							}
						}
					}
				}
			}

			//If a doctor was found, assign the patient to that room and doctor.
			if (foundDoctor)
			{
				//If the current patient's name in the room with the shortest wait list is blank, there is
				//not currently a patient in the room, so assign this patient as the current one and keep the
				//wait list length at 0.
				if (roomList[shortestWaitIndex].currentPatient.name == "")
				{
					roomList[shortestWaitIndex].currentPatient = newPatient;

					//Inform the patient that they can go get seen right away.
					cout << endl << newPatient.name << ", you can get seen right away by Dr. " << roomList[shortestWaitIndex].roomDoctor.name << "\n"
						<< "in room " << (shortestWaitIndex + 1) << "!  Please go there now.\n\n";
					outFile << endl << newPatient.name << ", you can get seen right away by Dr. " << roomList[shortestWaitIndex].roomDoctor.name << "\n"
						<< "in room " << (shortestWaitIndex + 1) << "!  Please go there now.\n\n";

					system("pause");
				}

				//Otherwise, add this patient to the wait list and increment the wait list length.
				else
				{
					try
					{
						roomList[shortestWaitIndex].WaitList.enqueue(newPatient);
						roomList[shortestWaitIndex].waitLength += 1;

						//Inform the user that they are on the wait list.
						cout << endl << newPatient.name << ", you will be number " << roomList[shortestWaitIndex].waitLength << " on the wait list to see\n"
							<< "Dr. " << roomList[shortestWaitIndex].roomDoctor.name << " in room " << (shortestWaitIndex + 1) << ". "
							<< "Please go there now.\n\n";
						outFile << endl << newPatient.name << ", you will be number " << roomList[shortestWaitIndex].waitLength << " on the wait list to see\n"
							<< "Dr. " << roomList[shortestWaitIndex].roomDoctor.name << " in room " << (shortestWaitIndex + 1) << ". "
							<< "Please go there now.\n\n";

						system("pause");
					}
					catch (FullQueue exception)
					{
						cout << "Error! There is no more room in memory for another patient.\n\n";
						outFile << "Error! There is no more room in memory for another patient.\n\n";
					}

					//Debugging purposes: print the wait list.
					//printWaitList(shortestWaitIndex, outFile);
				}
			}

			//Otherwise, if no doctor was found, inform the patient that there are currently no doctors in the clinic.
			else
			{
				cout << "\nSorry, " << newPatient.name << ", there are currently no doctors available.\n"
					<< "Please check back soon.\n\n";
				outFile << "\nSorry, " << newPatient.name << ", there are currently no doctors available.\n"
					<< "Please check back soon.\n\n";

				system("pause");
			}
		}

		/*
		Debugging purposes:
		cout << "Debugging purposes:\n";
		cout << "Room index " << shortestWaitIndex << " was modified.\n";
		cout << "The wait list for room #" << (shortestWaitIndex + 1) << " now has " << roomList[shortestWaitIndex].waitLength << " patients.\n\n";
		system("pause");
		*/
	}

	//Function to check out a patient.  Checks the room's wait list to see if there are any other
	//patients waiting.  If there are, function dequeues the next patient from the front of the
	//wait list and assigns it to the current room, then subtracts 1 from waitlist length.
	void patientCheckOut(ofstream &outFile)
	{
		//String for patient's name input
		string patientName;

		cout << "\nEnter your name: ";
		outFile << "\nEnter your name: ";
		getline(cin, patientName, '\n');
		outFile << patientName << endl;

		bool patientFound = false;

		for (int index = 0; index < NUM_ROOMS; index++)
		{
			if (roomList[index].currentPatient.name == patientName)
			{
				patientFound = true;	//Flag that we found a patient
				
				//If there is a wait list for the room, dequeue the next patient from the front
				//of the wait list and assign it to the currentPatient object in the room.
				//Decrement the length of the wait list.
				if (roomList[index].waitLength > 0)
				{
					try
					{
						roomList[index].WaitList.dequeue(roomList[index].currentPatient);
						roomList[index].waitLength -= 1;
					}
					catch (EmptyQueue exception)
					{
						cout << "Error! List is empty.\n\n";
						outFile << "Error! List is empty.\n\n";
					}
				}

				//Otherwise, if there is no wait list, clear all the fields in currentPatient.
				else
				{
					roomList[index].currentPatient.name = "";
					roomList[index].currentPatient.age = 0;
					roomList[index].currentPatient.specialty = "";
					roomList[index].currentPatient.emergency = false;
				}

				/*
				Debugging purposes
				cout << "Room index " << index << " was modified.\n";
				cout << "The wait list for room #" << (index + 1) << " is now " << roomList[index].waitLength << " patients.\n\n";
				//Debugging purposes: Print the wait list.
				printWaitList(index, outFile);
				*/

				//Output a message to the user saying that their check-out was successful.
				cout << endl << patientName << ", you have successfully checked out from room #" << (index + 1) << ".\n\n";
				outFile << endl << patientName << ", you have successfully checked out from room #" << (index + 1) << ".\n\n";

				index = NUM_ROOMS;		//To stop the for loop from continuing
			}
		}

		if (!patientFound)
		{
			cout << "\nSorry, no patient with that name is currently being seen.\n"
				<< "If you have checked in, please wait to be seen before you check out.\n\n";
			outFile << "\nSorry, no patient with that name is currently being seen.\n"
				<< "If you have checked in, please wait to be seen before you check out.\n\n";
		}

		system("pause");
	}

	//Function to check out a doctor.  If the doctor has any patients on the wait list for his room,
	//those patients are assigned to other doctors.
	void doctorCheckOut(ofstream &outFile)
	{
		//Temporary holder for patients that need to get reassigned to new doctors.
		Patient reassignPatient;

		//String for doctor's name input
		string doctorName;

		cout << "\nEnter your name: ";
		outFile << "\nEnter your name: ";
		getline(cin, doctorName, '\n');
		outFile << doctorName << endl;

		bool foundDoctor = false;	//Flag for when doctor is found

		//Search the list of rooms for the doctor that is checking out.
		for (int index = 0; index < NUM_ROOMS; index++)
		{
			if (roomList[index].roomDoctor.name == doctorName)
			{
				foundDoctor = true;

				//Inform the user that the doctor was found and that they are being checked out.
				cout << "\nDoctor found. Now initiating doctor check-out process.\n\n";
				outFile << "\nDoctor found. Now initiating doctor check-out process.\n\n";

				//Clear the roomDoctor object so this doctor won't pop up when reassigning patients.
				roomList[index].roomDoctor.name = "";
				roomList[index].roomDoctor.specialty = "";

				//Clear any information on a patient currently being seen by the doctor.
				roomList[index].currentPatient.name = "";
				roomList[index].currentPatient.age = 0;
				roomList[index].currentPatient.specialty = "";
				roomList[index].currentPatient.emergency = false;

				//Set the room's status back to available
				roomList[index].available = true;

				//Pop each patient from the wait list and reassign them to other doctors in the clinic.
				for (int count = 0; count < roomList[index].waitLength; count++)
				{
					try
					{
						roomList[index].WaitList.dequeue(reassignPatient);
						cout << "Note: Patient named " << reassignPatient.name << " has been reassigned.";
						outFile << "Note: Patient named " << reassignPatient.name << " has been reassigned.";
						assignDoctor(reassignPatient, outFile);
					}
					catch (EmptyQueue exception)
					{
						cout << "Error! List is empty.\n\n";
						outFile << "Error! List is empty.\n\n";
					}
				}

				//Output a message to the doctor saying that their check-out was successful.
				cout << endl << doctorName << ", you have successfully checked out from room #" << (index + 1) << ".\n\n";
				outFile << endl << doctorName << ", you have successfully checked out from room #" << (index + 1) << ".\n\n";
				
				index = NUM_ROOMS;	//Stop the for loop from continuing
			}
		}

		if (!foundDoctor)
		{
			cout << "\nSorry, we couldn't find a doctor with that name in the clinic right now.\n\n";
			outFile << "\nSorry, we couldn't find a doctor with that name in the clinic right now.\n\n";
		}

		system("pause");
	}
};