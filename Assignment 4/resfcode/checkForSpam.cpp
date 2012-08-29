#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "stdint.h"
#include <cstring>
#include <limits>
#include <iomanip>
#include "hash_32a.c"

using namespace std;

const string _DELIM_STR_ = " ";
const string _NULL_STR_ = "";

vector<double> nSPHash, sPHash;
int NGRAM;

int createVector( vector<string>&, string );
void initializeHash( string, string );

int main( int argc, char* argv[] )
{
	if( argc != 4 ) {
		cout << "Invalid no of arguments" << endl;
		cout << "Usage: ./checkForSpam <not-spam-prob-file> <spam-prob-file> <file-to-be-checked>" << endl;
		return 1;
	}

	string notSpamProbFile = argv[1];
	string spamProbFile = argv[2];
	string toCheckFile = argv[3];
	nSPHash.reserve( FNV_32_PRIME );
	sPHash.reserve( FNV_32_PRIME );
	NGRAM = 1;

	vector<string> grams;
	bool flag = false;

	initializeHash( notSpamProbFile, spamProbFile );
	int n = createVector( grams, toCheckFile );

	//First calculate the not spam probability
	double nSP = 1.0;
	for( int i = 0 ; i < n ; i++ )
	{
		int x = fnv_32a_str( (char*)grams[i].c_str(), 0 );
		int hashVal = x % FNV_32_PRIME;

		if( 0.0f != nSPHash[hashVal] )
			nSP *= nSPHash[hashVal];
	}

	if( 1.0 == nSP )
		nSP = 0.0;

	cout << "Probability for Not Spam :: " << nSP << endl;

	//Next calculate the spam probability
	double sP = 1.0;
	for( int i = 0 ; i < n ; i++ )
	{
		int x = fnv_32a_str( (char*)grams[i].c_str(), 0 );
		int hashVal = x % FNV_32_PRIME;

		if( 0.0f != sPHash[hashVal] )
			sP *= sPHash[hashVal];
	}

	if( 1.0 == sP )
		sP = 0.0;

	cout << "Probability for Spam :: " << sP << endl;

	nSPHash.erase( nSPHash.begin(), nSPHash.end() );
	nSPHash.swap( nSPHash );

	sPHash.erase( sPHash.begin(), sPHash.end() );
	sPHash.swap( sPHash );

	if( nSP > sP )
		cout << "It is a not a spam" << endl;
	else if( nSP < sP )
		cout << "It is a spam" << endl;
	else
		cout << "OOPS !! I just cant figure it out. May be you didnt train me well ;)" << endl;

	return 0;
}

void initializeHash( string nSPFile, string sPFile )
{
	for( int i = 0 ; i < FNV_32_PRIME ; i++ ) {
		nSPHash[i] = 0.0;
		sPHash[i] = 0.0;
	}

	// First read the not spam file
	ifstream file ( nSPFile.c_str() );

	if(file.is_open())
	{
		while ( file.good() ) 
		{
			string curLine;
			getline( file, curLine );

			if ( curLine == _NULL_STR_ )
				break;
			else {
				int curPos = curLine.find_last_of ( _DELIM_STR_ );
				string curString = curLine.substr( 0, curPos );

				int x = fnv_32a_str( (char*)curString.c_str(), 0 );
				int hashVal = x % FNV_32_PRIME;
				double prob = atof( (curLine.substr( curPos + 1, curLine.length() - curPos )).c_str() );

				if( 0.0f == nSPHash[hashVal] )
					nSPHash[hashVal] = prob;
				else
					nSPHash[hashVal] = ( nSPHash[hashVal] + prob )/2;
			}
		}
	}

	file.close();

	// Next read the spam file
	ifstream file2 ( sPFile.c_str() );

	if(file2.is_open())
	{
		while ( file2.good() ) 
		{
			string curLine;
			getline( file2, curLine );

			if ( curLine == _NULL_STR_ )
				break;
			else {
				int curPos = curLine.find_last_of ( _DELIM_STR_ );
				string curString = curLine.substr( 0, curPos );

				int x = fnv_32a_str( (char*)curString.c_str(), 0 );
				int hashVal = x % FNV_32_PRIME;
				double prob = atof( (curLine.substr( curPos + 1, curLine.length() - curPos )).c_str() );

				if( 0.0f == sPHash[hashVal] )
					sPHash[hashVal] = prob;
				else
					sPHash[hashVal] = ( sPHash[hashVal] + prob )/2;
			}
		}
	}

	file2.close();
}

int createVector( vector <string> &grams, string File )
{
	ifstream file (File.c_str());
	int i = 0;

	if(file.is_open())
	{
		while ( file.good() ) 
		{
			string curLine;
			getline(file, curLine);

			if ( curLine == _NULL_STR_ )
				break;
			else {
				grams.push_back( curLine );
				i++;
			}
		}
	}

	file.close();
	return i;
}
