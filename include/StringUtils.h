#ifndef STRINGUTILS_class
#define STRINGUTILS_class

#include <iomanip>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

enum CC_MASK_TYPE { CC_MASK_ALL, CC_MASK_LEAVE_LAST_FOUR };

const bool LEFT_JUSTIFY{true}, RIGHT_JUSTIFY{false};

// ---------------------------------------------------------------------------
class StringUtils {
private:
public:
	static void maskString( std::string& destString, CC_MASK_TYPE maskType );

	// XML
	static void maskXMLTag( std::string& xml, std::string tag, CC_MASK_TYPE maskType );
	static void maskMMLData( std::string& mml, std::string mmlField, CC_MASK_TYPE maskType );
	static void insertTagWithData( std::string tag, std::string beforeTag, std::string data, std::string& xml );
	static void insertTagWithData( std::string tag, std::string beforeTag, int data, std::string& xml );

	// Misc
	static std::vector< std::string > split( const std::string& s, char delim );
	static std::vector< std::string > split( const std::string& s, const std::string& delim );

	//
	static void stripLeadingNotOf( std::string& s, const std::string& notOf );
	static std::string stripLeadingNotOf( const std::string& s, const std::string& notOf );

	static bool stringToBool( const std::string& s );

	// Formatting
	static std::string formatWithCommas( unsigned int n );
	static std::string formatWithCommas( const std::string& s );

	static void trimEnd( std::string& s );
	static void trimStart( std::string& s );
	static void trim( std::string& s );

	static std::string trimLeading( const std::string& str, char c = ' ' );
	static std::string trimTrailing( const std::string& str, char c = ' ' );
	static std::string trim( const std::string& str, char c );

	static std::string ebcdicToAscii( const std::string& source );
	static std::string asciiToebcdic( const std::string& source );
	static std::string ebcdicToAscii( const char* source, size_t size );

	static std::map< std::string, std::string > parseKeyValueParams( const std::string& params );

	// static bool matchString( const std::string& source, const std::string& pattern );
	static void replaceStringWithString( std::string& source, const std::string& stringToFind, const std::string& replaceString );

	static std::pair< std::string, unsigned int > hostAndPort( const std::string& hostPort );

	static void toUpper( std::string& str );
	static void toLower( std::string& str );
	static std::string returnUpper( std::string str );
	static std::string returnLower( std::string str );

	static std::string safeSubstr( const std::string& str, size_t pos, size_t count = std::string::npos );
	static std::string formatString( const std::string& buffer, ushort size, char fillChar, bool leftJust );

	/***********************************************************************
	** Safely obtains a character from string str at position pos
	**
	** Params:	str - string to extract character from
	**			pos - position of character to obtain (0-based)
	**
	** Return:	Character at position pos or '\0' if not found
	***********************************************************************/
	static char getChar( const std::string& str, ushort pos );

	template < typename T >
	static std::string toString( const T& val, int precision = std::numeric_limits< T >::digits10 ) {
		std::ostringstream oss;
		oss.precision( precision );

		if ( precision != std::numeric_limits< T >::digits10 ) {
			oss << std::fixed;
		}

		oss << val;
		return oss.str( );
	}
};

#endif
