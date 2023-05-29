#ifndef APPRESOURCE_class
#define APPRESOURCE_class

#include <iosfwd>
#include <string>
#include <vector>

#include "EpxUtils.h"

/***********************************************************************
** Enumerations
***********************************************************************/
enum class AppType { mcp, workHorse, unknown };
enum class State { disconnected, error, idle, paused, unknown, working };
enum class Action { dump, error, heartbeat, job, pause, progress, refresh, result, resume, state, terminate, task, warn };

/***********************************************************************
** StateLevel of each app determines what updates it receives
** from MasterControlPrg regarding other application's state changes.
** MCP = Only receive state changes of MasterControlPrgs
** RESOURCE = Receive MCP and Resource-Level application state changes
** MAX_LEVEL = Receive state changes of all applications
***********************************************************************/
enum StateLevel { MCP, RESOURCE, MAX_LEVEL };

/***********************************************************************
** Class Definitions
***********************************************************************/
struct AppIdentity {
	AppType name;
	std::string ip;
	ushort port;

	AppIdentity( ){};
	AppIdentity( AppType name, std::string ip, ushort port ) : name( name ), ip( ip ), port( port ){};
	friend bool operator<( const AppIdentity& lhs, const AppIdentity& rhs );
	friend bool operator==( const AppIdentity& lhs, const AppIdentity& rhs );
	std::string getAppId( ) const;
	std::string getConnectionInfo( ) const;
};

struct AppResource {
	State state;
	tm lastChange;
	std::string log;

	bool isAlive( ) const;
	bool isDisconnected( ) const;
};

/***********************************************************************
** Resource Functions
***********************************************************************/
std::string toString( AppType appType );
bool operator<( const tm& lhs, const tm& rhs );
std::ostream& operator<<( std::ostream& os, const AppType& appType );
std::ostream& operator<<( std::ostream& os, const State& state );
std::ostream& operator<<( std::ostream& os, const Action& actn );
std::ostream& operator<<( std::ostream& os, const tm& t );
// AppType jobToApp( const json& js );

// Merges json b into json a
json merge( const json& a, const json& b );

// Json stuff
void to_json( json& j, const AppType& appType );
void from_json( const json& j, AppType& appType );
void to_json( json& j, const State& state );
void from_json( const json& j, State& state );
void to_json( json& j, const Action& action );
void from_json( const json& j, Action& action );
void to_json( json& j, const AppIdentity& id );
void from_json( const json& j, AppIdentity& id );
void to_json( json& j, const AppResource& ar );
void from_json( const json& j, AppResource& ar );

namespace std {
template <>
struct hash< AppIdentity > {
	size_t operator( )( const AppIdentity& id ) const {
		return ( hash< string >( )( toString( id.name ) ) ^ hash< std::string >( )( id.ip ) ) ^ hash< ushort >( )( id.port );
	}
};
}	// namespace std

#endif
