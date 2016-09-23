//Inventory Legacy App
//
//options to:
//input inventory item descriptions
//retrieve a list of product descriptions
//exit the console application.
//
//author: Monika Orlinski.
//date: September 23 2016.

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctime>
#include <sstream>
#include <time.h>
#include <chrono>
#include <locale>

using namespace std;

struct timeDataForFile {
	string filename;
	string timeheader;
};

string pullInventory = "LegacyInv.txt";

void initializeProgram();
void addInventory(string, string);
string shortDescription(string);
int getItemId();
void DateTimeCore(timeDataForFile&);
int getItemId();
void returnInventory();

int main()
{
	initializeProgram();

	//cout << "Make a selection from the following: " << endl;
	/*DateTimeCore();*/  //works for (rough version 3).

	/*
	chrono::system_clock::time_point tpoint = chrono::system_clock::now();

	__time64_t tt = chrono::system_clock::to_time_t(tpoint);
	tm t;
	errno_t err = localtime_s(&t,&tt);

	cout << put_time(&t, "%a %b %d %H:%M:%S %Y") << endl;
	cout << put_time(&t, "%m%d%H%M%S%Y") << endl;

	cout << endl;
	///////////////////////////////////////////////////////////////////

	ostringstream out;

	out << put_time(&t, "%a %b %d %H:%M:%S %Y");

	cout << "ostringsteam status is" << endl;
	cout << out.str();
	string dateHeader = out.str();
	cout << endl << "this." << endl;

	cout << "has it been saved to a string?" << endl;
	cout << dateHeader;
	cout << endl << "yes?" << endl;
	*/

	return 0;
}

void initializeProgram() {
	int programStatus = -1;
	string outFile = "";
	timeDataForFile temp;

	do {
		//reinitialize the member variables for file output.
		temp.filename = "";
		temp.timeheader = "";

		cout << "How would you like to procede?" << endl;
		cout << "Enter \"1\" to RETRIEVE your current inventory,   " << endl;
		cout << "Enter \"2\" to ADD to your current inventory, and   " << endl;
		cout << "Enter \"0\" to leave the program.  " << endl;

		cin >> programStatus;
		cout << endl;

		if (cin) {

			switch (programStatus) {
			case (0):
				cout << "You have chosen to ";
				cout << "EXIT" << endl;
				break;
			case (1):
				cout << "You have chosen to ";
				cout << "RETRIEVE INVENTORY" << endl;
				break;
			case (2):
				cout << "You have chosen to ";
				cout << "ADD TO INVENTORY" << endl;
				DateTimeCore(temp);
				break;
			default:
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Please Select a Valid Action,\n 0 -- exit ,\n 1 -- retrieve inventory ,\n 2 -- add to inventory." << endl;
				cout << endl;
				//exit(1);
			}
		}
		//process the selected action.
		//retrieves inventory
		if (programStatus == 1) {
			returnInventory();
		}
		//enters more inventory
		else if (programStatus == 2) {
			addInventory(temp.filename, temp.timeheader);
		}
		else {
			
		}
	}  while (programStatus != 0);

}

void addInventory(string filename, string createtime) {

	filename = filename+".txt";
	ofstream file;
	string itemDescription = "";
	static int currentItemCount= 0;

	file.open(filename.c_str());

	if (file) {
		
		cin.clear();
		cin.ignore(3000, '\n');

		cout << "You have " << currentItemCount << " products currently stored in inventory." << endl;

		while (itemDescription != "stop") {
			cout << "Enter a short description for your next product, or \"stop\" to finish." << endl;

			getline(cin, itemDescription);
			//cin >> itemDescription;

			if (cin) {
				if (itemDescription != "stop") {

					file << setfill('0') << setw(6) << getItemId();
					file << '\t';
					file << shortDescription(itemDescription);
					file << '\t';
					file << createtime;
					file << '\n';

				}
			}
			else {
				itemDescription = "";
				cin.clear();
				cin.ignore(3000, '\n');
				cout << "Invalid Description Entered." << endl;
			}
			cout << endl;
	    }
	}

	file.close();

	//Update the currently available files to access the full inventory.
	ofstream pulledInventory;
	//append the next filed list of items.
	pulledInventory.open(pullInventory.c_str(), ios::app );

	if (pulledInventory) {
		pulledInventory << filename + '\n';
	} 
	else {
		cout << "Warning: Could not persist inventory file name " + createtime + " .";
		cout << endl << endl << endl;
	}

	pulledInventory.close();

	string status = "Your INVENTORY LIST has been saved";

}

string shortDescription(string s) {
	if (s.length() >= 90) {
		return s.substr(0, 90)+"..";
	}
	else {
		int numPadding = 90 - s.length();
		return s.append(numPadding, ' ');
	}
}

int getItemId() {
	//formatting as represented in addIventory()
	//000000 to 999999
	//cout << setfill('0') << setw(6) << 0;
	static int sku = 0;
	return sku++;
}

void DateTimeCore(timeDataForFile& tdff) {

	chrono::system_clock::time_point tpoint = chrono::system_clock::now();

	__time64_t tt = chrono::system_clock::to_time_t(tpoint);

	tm t;

	errno_t err = localtime_s(&t, &tt);

	put_time(&t, "%a %b %d %H:%M:%S %Y");

	//cout << put_time(&t, "%a %b %d %H:%M:%S %Y") << endl;
	//cout << put_time(&t, "%m%d%H%M%S%Y") << endl;

	string timeHeader = "";
	string fileName = "";

	ostringstream out;

	out << put_time(&t, "%a %b %d %H:%M:%S %Y");
	string dateHeader = out.str();

	////cout << "has it been saved to a string?" << endl;
	////cout << dateHeader;
	////cout << endl << "yes?" << endl << endl;

	out.str("");
	//out.flush(); //Does not reset the string, out.str("") does.

	out << put_time(&t, "%m%d%H%M%S%Y");
	fileName = out.str();

	////cout << "has this been saved to a string?" << endl;
	////cout << fileName;
	////cout << endl << "yes?" << endl << endl;

	tdff.filename = fileName;
	tdff.timeheader = dateHeader;
}

void returnInventory() {
	ifstream pulledInventory;
	ifstream openFile;
	string openedFile = "";

	string currentItem = "";
	//6 - 30 - 24 , accomodates for the null terminator.
	char sku[8];
	char description[31];
	char date[26];

	pulledInventory.open(pullInventory.c_str());

	if (pulledInventory) {

		//Process All available files.
		while (!pulledInventory.eof()) {
			//get a file name with extension.
			getline(pulledInventory, openedFile);
			//open stream.
			openFile.open(openedFile.c_str());
			if (openFile) {
				int stopInfinity = 0;

				//Test All available file IF the following while IS commented out.
				cout << endl;
				cout << "For File: ";
				cout << openedFile.c_str() << endl;
				cout << "SKU" << '\t';
				cout << "Product Description" << "\t\t\t";
				cout << "Date Entered";
				cout << endl << endl;

				while ((!openFile.eof()) && (stopInfinity <= 10)) {

					openFile.get(sku,7);
					bool endOfFile = false;
					if (sku[0] == '\0') {
						//openFile.ignore(1000, '\n');
						//openFile.get();
						cout << " IN ";
						endOfFile = true;
					}
					if (endOfFile) {
						break;
					}
					cout << " PASSED ";
					cout << sku /*<< '\t'*/;

					//openFile.ignore(1);

					openFile.get(description, 30);
					cout << description << "..\t";
					openFile.ignore(62);
					openFile.ignore(1);

					openFile.get(date, 25, '\n');
					//consume the final \n
					openFile.get();
					cout << date << endl;

					///////////////////////////////////
					////getline(openFile, currentItem);

					//openFile.get(sku, 7);
					//cout << sku << '\t';

					//openFile.get();
					//openFile.get(description, 31);

					//cout << description << "..\t";

					//openFile.get(description, 31);
					//cout << "*--" << description << "--*";
					////openFile.get(date,100,'\n');
					////cout << date << endl;
					//getline(openFile, currentItem);
					//cout << endl << endl << "*-" + currentItem + "-*" << endl << endl;

					///*openFile.get(description,31);
					//openFile.get();
					//openFile.get();
					//openFile.get(date, 25, '\n');

					//cout << date;*/
					/////////////////////////////////
					stopInfinity++;
					//cout << endl << stopInfinity << endl;
					//cout << currentItem;
				}
			}
			else {
				cout << endl << endl;
				cout << "END OF INVENTORY.";
				cout << endl << endl << endl;
			}
			openFile.close();
		}
	}
	else {
		cout << "Could not retreive items, file corrupted.";
	}

	pulledInventory.close();
}

