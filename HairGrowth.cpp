#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include "boost/date_time/gregorian/gregorian.hpp"
using namespace std;
using namespace boost::gregorian;

struct EntryType{
	date Edate;
	double length;
	bool cut;
};

struct MarkerType{
	double length;
	string marker;
};

int NumEntry(string file, int entries);
void LDataIn(vector<EntryType>& lengthData, string file, int entries);
void LDataInput(vector<EntryType>& lengthData, int& entries);
void LDataOut(vector<EntryType>& lengthData, string file, int entries);
void LPrintData(vector<EntryType>& lengthData, int entries);
double GrowthRate(vector<EntryType> lengthData, int entries);
date ProjDate(EntryType lengthData, double growthAvg, double goalLength);
void ProjLength(EntryType lengthData, double growthAvg);
void MDataIn(vector<MarkerType>& markerData, string file, int entries);
void MDataInput(vector<MarkerType>& markerData, int& entries);
void MDataOut(vector<MarkerType>& markerData, string file, int entries);
void MPrintData(vector<MarkerType>& markerData, int entries);
void GoldenRatio(EntryType lengthData, double growthAvg);
bool compareLengths(MarkerType &a, MarkerType &b) {return a.length < b.length;}
bool compareDates(EntryType &a, EntryType &b) {return a.Edate < b.Edate;}

string mSys="\" ";

int main() {
	int Lentries=-1;
	string Lfile="length.dat";
	double growthAvg=0;
	int Mentries=-1;
	string Mfile="markers.dat";
	double goalLength=0;
	int selection=1;
	
	system("cls");
	
	Lentries=NumEntry(Lfile, Lentries);
	
	vector<EntryType> lengthData(Lentries+1);
	
	LDataIn(lengthData, Lfile, Lentries);
	growthAvg=GrowthRate(lengthData, Lentries);

	Mentries=NumEntry(Mfile, Mentries);
	vector<MarkerType> markerData(Mentries+1);
	MDataIn(markerData, Mfile, Mentries);
	
	while (selection!=0) {
		cout<<"Select an option: "<<endl
			<<"1 - Projected date of input length"<<endl
			<<"2 - Projected length on input date"<<endl
			<<"3 - Input a data entry"<<endl
			<<"4 - Display all data entries"<<endl
			<<"5 - Calculate Golden Ratio lengths"<<endl
			<<"6 - Display projected growth for markers"<<endl
			<<"7 - Input a marker entry"<<endl
			<<"8 - Display all markers"<<endl;
			if (mSys=="\" ") {cout<<"9 - Use Metric system"<<endl;}
			else {cout<<"9 - Use Imperial system"<<endl;}
			cout<<"0 - Exit program"<<endl;
		cin>>selection;
		system("cls");
		
		switch (selection) {
		case 1:
			cout<<"Please enter a goal length: ";
			cin>>goalLength;
			
			cout<<"--------------------------"<<endl;
			cout<<"Date		Days until"<<endl;
			cout<<ProjDate(lengthData[Lentries], growthAvg, goalLength)<<"	"<<ProjDate(lengthData[Lentries], growthAvg, goalLength)-day_clock::local_day()<<endl;
			cout<<"--------------------------"<<endl<<endl;	
			break;
		case 2:
			ProjLength(lengthData[Lentries], growthAvg);
			break;
		case 3:
			LDataInput(lengthData, Lentries);
			sort(lengthData.begin(), lengthData.end(), compareDates);
			growthAvg=GrowthRate(lengthData, Lentries);
			LDataOut(lengthData, Lfile, Lentries);
			break;
		case 4:
			LPrintData(lengthData, Lentries);
			break;
		case 5:
			GoldenRatio(lengthData[Lentries], growthAvg);
			break;
		case 6:
			cout<<"Length   Marker        Date              Days"<<endl;
			cout<<"---------------------------------------------"<<endl;
			for (int i=0; i<=Mentries; ++i) {
				cout<<fixed<<setprecision(2)<<markerData[i].length<<mSys<<"  "
					<<setw(12)<<left<<markerData[i].marker<<"  "
					<<setw(7)<<ProjDate(lengthData[Lentries], growthAvg, markerData[i].length)<<"  "
					<<setw(5)<<ProjDate(lengthData[Lentries], growthAvg, markerData[i].length)-day_clock::local_day()<<endl;
			}
			cout<<"---------------------------------------------"<<endl<<endl;
			break;
		case 7:
			MDataInput(markerData, Mentries);
			sort(markerData.begin(), markerData.end(), compareLengths);
			MDataOut(markerData, Mfile, Mentries);
			break;
		case 8:
			MPrintData(markerData, Mentries);
			break;
		case 9:
			if (mSys=="\" ") {mSys="cm";}
			else {mSys="\" ";}
			break;
		case 0:
			return 0;
			break;
		default:
			cout<<"Incorrect input, please try again"<<endl<<endl;
			break;
		}
	}
return 0;}


int NumEntry(string file, int entries) {
	ifstream fin;
	fin.open(file.c_str());
	string line;
	
	while (!fin){
		cout<<"File does not exist"<<endl;
		return -1;
	}
	while (std::getline(fin, line)) {++entries;}
	fin.close();
	return entries;
}

void LDataIn(vector<EntryType>& lengthData, string file, int entries) {
	if (entries>0) {
		ifstream fin;
		fin.open(file.c_str() );
		
		for (int i=0; i<=entries; ++i) {
			fin>>lengthData[i].Edate>>lengthData[i].length>>lengthData[i].cut;
		}
		fin.close();
	}
	else {cout<<"0 entries"<<endl;}
}

void LDataInput(vector<EntryType>& lengthData, int& entries) {
	char another='1';
	while (another=='1') {
		lengthData.push_back(EntryType());
		entries++;
		
		cout<<endl<<"Please input entry date (YYYY-MM-DD): "<<endl;
		string s;
		cin>>s;
		date entrydate(from_simple_string(s));
		lengthData[entries].Edate=entrydate;
		
		cout<<endl<<"Please input entry length (##.##): "<<endl;
		double length=00.00;
		cin>>length;
		lengthData[entries].length=length;
		
		char cut='0';
		while (cut=='0') {
			cout<<endl<<"Was hair cut for this entry? (Y|N): "<<endl;
			cin>>cut;
			switch (cut) {
				case ('Y'):
				case ('y'):
					lengthData[entries].cut=1;
					break;
				case ('N'):
				case ('n'):
					lengthData[entries].cut=0;
					break;
				default:
					cout<<"Invalid input, please try again. "<<endl;
					cut='0';
					break;
			}
		}
		do {
			cout<<"Would you like to input another entry? (Y|N): "<<endl;
			cin>>another;
			cout<<endl;
			switch (another) {
				case ('Y'):
				case ('y'):
					another='1';
					break;
				case ('N'):
				case ('n'):
					another='2';
					break;
				default:
					cout<<"Invalid input, please try again. "<<endl;
					another='0';
					break;
			}
		}
		while (another=='0');
	}
}

void LDataOut(vector<EntryType>& lengthData, string file, int entries) {
	ofstream fout;
	fout.open(file.c_str() );
	
	for (int i=0; i<=entries; ++i) {
		fout<<lengthData[i].Edate<<"	"<<lengthData[i].length<<"	"<<lengthData[i].cut<<endl;
	}
	fout.close();
}

void LPrintData(vector<EntryType>& lengthData, int entries) {
	cout<<"Entry Date      Length     Cut"<<endl;
	cout<<"--------------------------------"<<endl;
	for (int i=0; i<=entries; ++i) {
		cout<<lengthData[i].Edate<<"     "
			<<fixed<<setprecision(2)<<lengthData[i].length<<mSys<<"     ";
		if (lengthData[i].cut==0) {cout<<"No"<<endl;}
		else {cout<<"Yes"<<endl;}
	}
	cout<<"--------------------------------"<<endl<<endl;
}

double GrowthRate(vector<EntryType> lengthData, int entries) {
	if (entries>1) {
		days noncut;
		days dateDiff;
		double difLength=0;
		double growth=0;	
		double growthAvg=0.0;
		
		for (int i=1; i<=entries; ++i) {
			dateDiff=lengthData[i].Edate-lengthData[i-1].Edate;
			if (!lengthData[i].cut) {		
				difLength=lengthData[i].length-lengthData[i-1].length;
				growth=growth+difLength;
				noncut=noncut+dateDiff;
			}
		}
		growthAvg=growth/noncut.days();
		//cout<<"Growth average per month: "<<setprecision(5)<<growthAvg*30<<endl;
		return growthAvg;
	}
	else {
		cout<<"Not enough data entries."<<endl;
		return 0;
	}
}

date ProjDate(EntryType lengthData, double growthAvg, double goalLength) {
	double remLength=0;
	int remDays=0;	
	date projDate=lengthData.Edate;

	remLength=goalLength-lengthData.length;
	remDays=remLength/growthAvg;
	date_duration remDur(remDays);
	projDate=lengthData.Edate+remDur;
	return projDate;
}

void ProjLength(EntryType lengthData, double growthAvg) {
	string s;
	double remLength=0;
	double projLength=0;
	
	cout<<"Please enter future date (YYYY-MM-DD): ";
	cin>>s;
	
	date futuredate(from_simple_string(s));
	days remTime = -(lengthData.Edate - futuredate);
	remLength=remTime.days()*growthAvg;
	projLength=remLength+lengthData.length;
	
	cout<<"------------------------------"<<endl;
	cout<<"Projected length: "<<setprecision(4)<<projLength<<mSys<<endl;
	cout<<"------------------------------"<<endl<<endl;
}

void GoldenRatio(EntryType lengthData, double growthAvg) {
	double height;
	double large, small;
	if (mSys=="\" ") {cout<<"Please enter your height in inches: ";}
	else {cout<<"Please enter your height in centimeters: ";}
	cin>>height;
	small=(height*.38196601);
	large=(height*.61803399);
	if (mSys=="\" ") {
		small+=5;
		large+=5;
	}
	else {
		small+=13;
		large+=13;
	}
	system("cls");
	cout<<"Lengths adjusted for LHC style hair measuring"<<endl<<endl<<endl;
	cout<<"Length   Marker        Date              Days"<<endl;
	cout<<"-----------------------------------------------"<<endl;
	cout<<fixed<<setprecision(2)<<small<<mSys<<"  "
		<<setw(12)<<left<<"Shorter"<<"  "
		<<setw(7)<<ProjDate(lengthData, growthAvg, small)<<"  "
		<<setw(5)<<ProjDate(lengthData, growthAvg, small)-day_clock::local_day()<<endl;
	cout<<fixed<<setprecision(2)<<large<<mSys<<"  "
		<<setw(12)<<left<<"Longer"<<"  "
		<<setw(7)<<ProjDate(lengthData, growthAvg, large)<<"  "
		<<setw(5)<<ProjDate(lengthData, growthAvg, large)-day_clock::local_day()<<endl;
	
	cout<<"-----------------------------------------------"<<endl<<endl;
			
}

void MDataIn(vector<MarkerType>& markerData, string file, int entries) {
	if (entries>0) {
		ifstream fin;
		fin.open(file.c_str() );

		for (int i=0; i<=entries; ++i) {
			fin>>markerData[i].length>>markerData[i].marker;
		}	
		fin.close();
	}
	else {cout<<"0 entries"<<endl;}
}

void MDataInput(vector<MarkerType>& markerData, int& entries) {
	char another='1';
	while (another=='1') {
		markerData.push_back(MarkerType());
		entries++;
		
		cout<<endl<<"Please input marker length (##.##): "<<endl;
		double length=00.00;
		cin>>length;
		markerData[entries].length=length;
		
		cout<<endl<<"Please input marker name: "<<endl;
		string marker;
		cin>>marker;
		markerData[entries].marker=marker;
		
		do {
			cout<<"Would you like to input another entry? (Y|N): "<<endl;
			cin>>another;
			cout<<endl;
			switch (another) {
				case ('Y'):
				case ('y'):
					another='1';
					break;
				case ('N'):
				case ('n'):
					another='2';
					break;
				default:
					cout<<"Invalid input, please try again. "<<endl;
					another='0';
					break;
			}
		}
		while (another=='0');
	}
}

void MDataOut(vector<MarkerType>& markerData, string file, int entries) {
	ofstream fout;
	fout.open(file.c_str() );
	
	for (int i=0; i<=entries; ++i) {
		fout<<markerData[i].length<<"	"<<markerData[i].marker<<endl;
	}
	fout.close();
}

void MPrintData(vector<MarkerType>& markerData, int entries) {
	cout<<"Length	Marker"<<endl;
	cout<<"---------------------------"<<endl;
	for (int i=0; i<=entries; ++i) {
		cout<<fixed<<setprecision(2)<<markerData[i].length<<"	"<<markerData[i].marker<<endl;
	}
	cout<<"---------------------------"<<endl<<endl;
}
