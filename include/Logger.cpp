#include <errno.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#include "Logger.h"
#include "StringUtils.h"

// Static vars
bool Logger::_dump = false;

//---------------------------------------------------------------------------
void Logger::quickDump( const string& filename, const string& message, bool inDumps ) {
	try {
		ofstream logFile;

		// See if we want to save it in the dumps directory
		if ( inDumps ) {
			ostringstream dumpFileName;
			Logger* logger = Logger::getLogger( );

			// See if we have a dumps directory set
			if ( logger && logger->dumpLocation.size( ) > 0 ) {
				dumpFileName << logger->dumpLocation << "/" << filename;
			} else {
				dumpFileName << filename;
			}

			logFile.open( dumpFileName.str( ).c_str( ), ios::out | ios::app );
		} else {
			logFile.open( filename.c_str( ), ios::out | ios::app );
		}

		// Save the time and the message
		logFile << "-- at: " << getTime( ) << endl;
		logFile << message << endl;
		logFile.close( );
	} catch ( ... ) {
	}
}

//---------------------------------------------------------------------------
void Logger::appError( const string& error ) {
	quickDump( "appError.txt", error );
}

//---------------------------------------------------------------------------
Logger* Logger::getLogger( ) {
	static Logger logger;

	return &logger;
}

//---------------------------------------------------------------------------
void Logger::setLogLocation( const string& appName, const string& location ) {
	ostringstream logFileName;
	logFileName << location << "/" << appName << ".log";

	this->logFileName = logFileName.str( );
}

//---------------------------------------------------------------------------
void Logger::setDumpLocation( const string& location ) {
	this->dumpLocation = location;
}

//---------------------------------------------------------------------------
string Logger::getTime( ) {
	char dateTime[ 128 ];
	struct tm* newtime;
	long ltime;

	time( &ltime );
	newtime = gmtime( &ltime );
	strftime( dateTime, 128, "%a, %d %b %Y %H:%M:%S GMT", newtime );
	return dateTime;
}

//---------------------------------------------------------------------------
void Logger::save( const string& methodName, const string& s, bool isMain, unsigned long alternateId ) {
	ofstream logFile;

	// Try to open file
	logFile.open( logFileName.c_str( ), ios::out | ios::app );

	if ( !logFile.is_open( ) ) {
		ostringstream errorMessage;
		errorMessage << "Error opening: " << logFileName << " errno: " << errno;
		cout << errorMessage.str( ) << endl;
		// TODO - Maybe put in a mutex to help with two threads writing at the same time
		// I just don't want the logging to slow down the app
		return;
	}

	if ( isMain ) {
		logFile << getTime( ) << " PID:" << getpid( ) << " [" << methodName << "] " << s << endl;
	} else {
		logFile << getTime( );

		if ( alternateId == 0 ) {
			logFile << " TID:" << pthread_self( );
		} else {
			logFile << " ID:" << alternateId;
		}

		logFile << " [" << methodName << "] " << s << endl;
	}

	logFile.close( );
}

//---------------------------------------------------------------------------
void Logger::save( const string& methodName, string s, int i, bool isMain, unsigned long alternateId ) {
	ostringstream logString;
	logString << s << i;
	save( methodName, logString.str( ), isMain, alternateId );
}

//---------------------------------------------------------------------------
void Logger::save( const string& methodName, string s, int i, string s2, bool isMain, unsigned long alternateId ) {
	ostringstream logString;
	logString << s << i << s2;
	save( methodName, logString.str( ), isMain, alternateId );
}

//---------------------------------------------------------------------------
void Logger::save( const string& methodName, string s, string s2, bool isMain, unsigned long alternateId ) {
	ostringstream logString;
	logString << s << s2;
	save( methodName, logString.str( ), isMain, alternateId );
}

//---------------------------------------------------------------------------
void Logger::warn( const string& methodName, const string& s, bool isMain, unsigned long alternateId ) {
	ostringstream logString;
	logString << "[WARN] " << s;
	save( methodName, logString.str( ), isMain, alternateId );
}

//---------------------------------------------------------------------------
void Logger::error( const string& methodName, const string& s, bool isMain, unsigned long alternateId ) {
	ostringstream logString;
	logString << "[ERROR] " << s;
	save( methodName, logString.str( ), isMain, alternateId );
}

//---------------------------------------------------------------------------
void Logger::dump( const string& fileName, const string& s, unsigned long alternateId ) {
	dump( fileName, s.begin( ), s.end( ), alternateId );
}

// ---------------------------------------------------------------------------
void Logger::setDump( bool dump ) {
	Logger::_dump = dump;
}

// ---------------------------------------------------------------------------
void Logger::setDump( const string& dump ) {
	setDump( StringUtils::stringToBool( dump ) );
}
