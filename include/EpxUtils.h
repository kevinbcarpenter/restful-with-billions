#ifndef EPXUTILS_h
#define EPXUTILS_h

#include <string>
#include <map>
#include <vector>

#include "json.hpp"
using json = nlohmann::json;

namespace epx {
	/***********************************************************************
	** Template function for finding a value in a vector
	**
	** Params:	vec - refrence to a vector of type T
	**			v -	type T value to search for
	**
	** Return:	True if v is found in vec
	***********************************************************************/
	template < typename T >
	bool vecFind( const std::vector< T >& vec, const T& v ) {
		return find( vec.begin( ), vec.end( ), v ) != vec.end( );
	}

	/***********************************************************************
	** Attempts to acquire map data at position key. Returns a default
	** constructed object T2 if the key does not exist.
	**
	** Params:	map - reference to the map that might contain data
	**			key - the map key you are looking for
	**
	** Return:	Data at key or default construction of type T2
	***********************************************************************/
	template < typename T, typename T2 >
	T2 mapGet( const std::map< T, T2 >& mp, const T& key ) {
		try {
			return mp.at( key );
		} catch ( ... ) {
			return T2( );
		}
	}

	/***********************************************************************
	** Attempts to acquire json data at position key. Returns a default
	** constructed object T if the key does not exist.
	**
	** Params:	js - reference to the inner-most json that might contain
	**				data to be returned
	**			key - the json key you are looking for
	**
	** Return:	Data at key or default construction of type T
	***********************************************************************/
	template < typename T >
	T jsGet( const json& js, std::string key ) {
		try {
			return js.at( key ).get< T >( );
		} catch ( ... ) {
			return T( );
		}
	}

	/***********************************************************************
	** This version is for use with arrays
	***********************************************************************/
	template < typename T >
	T jsGet( const json& js, size_t pos ) {
		try {
			return js.at( pos ).get< T >( );
		} catch ( ... ) {
			return T( );
		}
	}

	/***********************************************************************
	** Same as previous function, however allows the user to pass in a
	** default value to return, should the get<T> call throw.
	**
	** Params:	js - reference to the inner-most json that might contain
	**				data to be returned
	**			key - the json key you are looking for
	**			retDefault - default return value should key not exist
	**
	** Return:	Data at key or default construction of type T
	***********************************************************************/
	template < typename T >
	T jsGet( const json& js, std::string key, T retDefault ) {
		try {
			return js.at( key ).get< T >( );
		} catch ( ... ) {
			return retDefault;
		}
	}

	/***********************************************************************
	** This version is for use with arrays
	***********************************************************************/
	template < typename T >
	T jsGet( const json& js, size_t pos, T retDefault ) {
		try {
			return js.at( pos ).get< T >( );
		} catch ( ... ) {
			return retDefault;
		}
	}
}	// namespace epx

#endif
