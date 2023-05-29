#include <algorithm>
#include <iostream>

using namespace std;

#include "AppResources.h"
#include "StringUtils.h"

//---------------------------------------------------------------------------
bool operator<( const AppIdentity& lhs, const AppIdentity& rhs ) {
	return lhs.name < rhs.name && lhs.ip < rhs.ip && lhs.port < rhs.port;
}

//---------------------------------------------------------------------------
bool operator==( const AppIdentity& lhs, const AppIdentity& rhs ) {
	return ( lhs.name == rhs.name && lhs.ip == rhs.ip && lhs.port == rhs.port );
}

//---------------------------------------------------------------------------
bool operator<( const tm& lhs, const tm& rhs ) {
	return difftime( mktime( const_cast< tm* >( &lhs ) ), mktime( const_cast< tm* >( &rhs ) ) ) < 0;
}

//---------------------------------------------------------------------------
string AppIdentity::getAppId( ) const {
	return toString( name ) + "@" + ip + ":" + to_string( port );
}

//---------------------------------------------------------------------------
string AppIdentity::getConnectionInfo( ) const {
	return ip + ":" + to_string( port );
}

//---------------------------------------------------------------------------
bool AppResource::isAlive( ) const {
	return state == State::idle || state == State::working || state == State::paused;
}

//---------------------------------------------------------------------------
bool AppResource::isDisconnected( ) const {
	return state == State::disconnected;
}

//---------------------------------------------------------------------------
string toString( AppType appType ) {
	ostringstream oss;
	oss << appType;
	return oss.str( );
}

//---------------------------------------------------------------------------
ostream& operator<<( ostream& os, const AppType& appType ) {
	switch ( appType ) {
		case AppType::mcp:
			os << "MasterControlPrg";
			break;
		case AppType::workHorse:
			os << "WorkHorse";
			break;
		case AppType::unknown:
			os << "Unknown";
			break;
	}
	return os;
}

//---------------------------------------------------------------------------
ostream& operator<<( ostream& os, const State& state ) {
	switch ( state ) {
		case State::idle:
			os << "Idle";
			break;
		case State::working:
			os << "Working";
			break;
		case State::paused:
			os << "Paused";
			break;
		case State::error:
			os << "Error";
			break;
		case State::disconnected:
			os << "Disconnected";
			break;
		case State::unknown:
			os << "Unknown";
			break;
	}
	return os;
}

//---------------------------------------------------------------------------
ostream& operator<<( ostream& os, const Action& actn ) {
	switch ( actn ) {
		case Action::job:
			os << "Job";
			break;
		case Action::progress:
			os << "Progress";
			break;
		case Action::heartbeat:
			os << "Heartbeat";
			break;
		case Action::pause:
			os << "Pause";
			break;
		case Action::refresh:
			os << "Refresh";
			break;
		case Action::result:
			os << "Result";
			break;
		case Action::resume:
			os << "Resume";
			break;
		case Action::dump:
			os << "Dump";
			break;
		case Action::state:
			os << "State";
			break;
		case Action::terminate:
			os << "Terminate";
			break;
		case Action::warn:
			os << "Warn";
			break;
		case Action::error:
			os << "Error";
			break;
		case Action::task:
			os << "Task";
			break;
	}
	return os;
}

//---------------------------------------------------------------------------
ostream& operator<<( ostream& os, const tm& t ) {
	char buffer[ 25 ];
	strftime( buffer, 25, "%c", &t );
	os << buffer;
	return os;
}

//---------------------------------------------------------------------------
void to_json( json& j, const AppType& appType ) {
	j = toString( appType );
}

//---------------------------------------------------------------------------
void from_json( const json& j, AppType& appType ) {
	try {
		string s = j;
		StringUtils::toLower( s );
		if ( s == "mcp" ) {
			appType = AppType::mcp;
		} else if ( s == "workhorse" ) {
			appType = AppType::workHorse;
		} else {
			appType = AppType::unknown;
		}
	} catch ( exception& ex ) {
		appType = AppType::unknown;
		throw;
	}
}

//---------------------------------------------------------------------------
void to_json( json& j, const State& state ) {
	ostringstream os;
	os << state;
	j = os.str( );
}

//---------------------------------------------------------------------------
void from_json( const json& j, State& state ) {
	try {
		string s = j.at( "state" );
		StringUtils::toLower( s );
		if ( s == "idle" )
			state = State::idle;
		else if ( s == "working" )
			state = State::working;
		else if ( s == "paused" )
			state = State::paused;
		else if ( s == "error" )
			state = State::error;
		else if ( s == "disconnected" )
			state = State::disconnected;
		else if ( s == "unknown" )
			state = State::unknown;
	} catch ( exception& ex ) {
		state = State::unknown;
		throw;
	}
}

//---------------------------------------------------------------------------
void to_json( json& j, const Action& action ) {
	ostringstream os;
	os << action;
	j = os.str( );
}

//---------------------------------------------------------------------------
void from_json( const json& j, Action& action ) {
	try {
		string a = j.at( "action" );
		StringUtils::toLower( a );
		if ( a == "job" )
			action = Action::job;
		else if ( a == "progress" )
			action = Action::progress;
		else if ( a == "heartbeat" )
			action = Action::heartbeat;
		else if ( a == "pause" )
			action = Action::pause;
		else if ( a == "refresh" )
			action = Action::refresh;
		else if ( a == "result" )
			action = Action::result;
		else if ( a == "resume" )
			action = Action::resume;
		else if ( a == "dump" )
			action = Action::dump;
		else if ( a == "state" )
			action = Action::state;
		else if ( a == "terminate" )
			action = Action::terminate;
		else if ( a == "task" )
			action = Action::task;
		else if ( a == "warn" )
			action = Action::warn;
		else if ( a == "error" )
			action = Action::error;
	} catch ( exception& ex ) {
		// TODO: handle exceptions?
		throw;
	}
}

//---------------------------------------------------------------------------
void to_json( json& j, const AppIdentity& id ) {
	j[ "appName" ] = toString( id.name );
	j[ "ip" ] = id.ip;
	j[ "port" ] = id.port;
}

//---------------------------------------------------------------------------
void from_json( const json& j, AppIdentity& id ) {
	id.name = j.at( "appName" ).get< AppType >( );
	id.ip = j.at( "ip" ).get< string >( );
	id.port = j.at( "port" ).get< ushort >( );
}

//---------------------------------------------------------------------------
void to_json( json& j, const AppResource& ar ) {
	ostringstream oss;
	oss << ar.lastChange;
	j[ "state" ] = ar.state;
	j[ "lastChange" ] = oss.str( );
}

//---------------------------------------------------------------------------
void from_json( const json& j, AppResource& ar ) {
	string lastChange = j.at( "lastChange" );
	ar.state = j;
	strptime( &lastChange[ 0 ], "%a %b %d %H:%M:%S %Y", &ar.lastChange );
}

//---------------------------------------------------------------------------
json merge( const json& a, const json& b ) {
	json result = a.flatten( );
	json tmp = b.flatten( );

	for ( auto it = tmp.begin( ); it != tmp.end( ); ++it ) {
		result[ it.key( ) ] = it.value( );
	}

	return result.unflatten( );
}
