#ifndef Logger_class
#define Logger_class

#include <algorithm>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <unistd.h>

bool const IS_MAIN = true;
bool const IS_THREAD = false;

//---------------------------------------------------------------------------
class Logger {
private:
	static bool _dump;

	std::string logFileName;
	std::string dumpLocation;

public:
	// Return our shared logger
	static Logger* getLogger( );
	static void quickDump( const std::string& filename, const std::string& message, bool inDumps = false );
	static void appError( const std::string& error );

	void setLogLocation( const std::string& appName, const std::string& location );
	void setDumpLocation( const std::string& location );

	// Log write routines
	void save( const std::string& methodName, const std::string& s, bool isMain, unsigned long alternateId = 0 );
	void save( const std::string& methodName, std::string s, int i, bool isMain, unsigned long alternateId = 0 );
	void save( const std::string& methodName, std::string s, int i, std::string s2, bool isMain, unsigned long alternateId = 0 );
	void save( const std::string& methodName, std::string s, std::string s2, bool isMain, unsigned long alternateId = 0 );

	void warn( const std::string& methodName, const std::string& s, bool isMain, unsigned long alternateId = 0 );
	void error( const std::string& methodName, const std::string& s, bool isMain, unsigned long alternateId = 0 );

	void dump( const std::string& fileName, const std::string& s, unsigned long alternateId = 0 );
	template < typename InputIter >
	void dump( const std::string& filename, InputIter begin, InputIter end, unsigned long alternateId = 0 );
	static void setDump( bool dump );
	static void setDump( const std::string& dump );
	static bool dump( ) {
		return _dump;
	}

	static std::string getTime( );
};

template < typename InputIter >
void Logger::dump( const std::string& fileName, InputIter begin, InputIter end, unsigned long alternateId ) {
	if ( !Logger::_dump ) {
		return;
	}

	std::ostringstream dumpFileName;
	dumpFileName << dumpLocation << "/" << fileName;

	std::ofstream dumpFile( dumpFileName.str( ), std::ofstream::out | std::ofstream::app );

	// Try to open file
	if ( !dumpFile ) {
		std::ostringstream errorMessage;
		errorMessage << "Error opening " << dumpFileName.str( );
	}

	dumpFile << "=== packet ===\n"
							"-- at: "
					 << getTime( ) << "\n";

	if ( alternateId == 0 ) {
		dumpFile << "-- pid: " << getpid( ) << "\n";
	} else {
		dumpFile << "-- id: " << alternateId << "\n";
	}

	std::copy( begin, end, std::ostream_iterator< typename std::iterator_traits< InputIter >::value_type >( dumpFile ) );
	dumpFile << std::endl;
	dumpFile << "==============\n\n";
}

#endif
