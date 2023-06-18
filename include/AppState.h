#ifndef APPSTATE_class
#define APPSTATE_class

#include <string>

#include "AppResources.h"
#include "thirdparty/zmq.hpp"

/***********************************************************************
** Class Declaration
***********************************************************************/
class AppState {
private:
	json js;
	State state;
	static AppState* appState;

	zmq::context_t& context;
	zmq::socket_t socket;

	AppState( const json& config, zmq::context_t& context, const std::string& appName );
	void heartbeat( );

public:
	AppState( ) = delete;
	AppState( AppState&& ) = delete;
	AppState( const AppState& ) = delete;
	void operator=( AppState&& ) = delete;
	void operator=( const AppState& ) = delete;

	// Create  & get singleton
	static void createInstance( const json& config, zmq::context_t& context, const std::string& appName );
	static AppState* getInstance( );

	// Sends message to master automation director
	void send( Action action, const json& jMessage );
	void send( Action action, const std::string& message = "" );

	// State related functions
	void setState( State state );
	State getState( ) const;
	bool isRunning( ) const;
};

#endif
