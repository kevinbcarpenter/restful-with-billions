#include <chrono>
#include <thread>
#include <mutex>

using namespace std;

#include "AppState.h"

mutex js_mutex;
AppState* AppState::appState{nullptr};

const ushort HEARTBEAT_TIME{5};

//---------------------------------------------------------------------------
AppState::AppState( const json& config, zmq::context_t& context, const string& appName ) : state( State::idle ), context( context ), socket( context, ZMQ_DEALER ) {
	socket.connect( "tcp://" + config.at( "control" ).at( "host" ).get< string >( ) + ":" + to_string( config.at( "control" ).at( "port" ).get< short >( ) ) );
	js[ "ip" ] = config.at( "server" ).at( "ip" ).get< string >( );
	js[ "port" ] = config.at( "server" ).at( "zmqPort" ).get< short >( );
	js[ "appName" ] = appName;
	js[ "state" ] = this->state;

	thread( &AppState::heartbeat, this ).detach( );
	send( Action::refresh );
}

//---------------------------------------------------------------------------
void AppState::createInstance( const json& config, zmq::context_t& context, const string& appName ) {
	if ( !appState ) {
		appState = new AppState( config, context, appName );
	}
}

//---------------------------------------------------------------------------
AppState* AppState::getInstance( ) {
	return appState;
}

//---------------------------------------------------------------------------
void AppState::send( Action action, const json& jMessage ) {
	lock_guard< mutex > lck( js_mutex );
	json jTemp = merge( js, jMessage );

	jTemp[ "action" ] = action;
	string temp{jTemp.dump( )};

	zmq::message_t zMsg( temp.data( ), temp.length( ) );
	socket.send( zMsg );
}

//---------------------------------------------------------------------------
void AppState::send( Action action, const string& message ) {
	lock_guard< mutex > lck( js_mutex );
	js[ "state" ] = state;
	js[ "action" ] = action;
	js[ "message" ] = message;
	string temp{js.dump( )};
	js.erase( "message" );

	zmq::message_t zMsg( temp.data( ), temp.length( ) );
	socket.send( zMsg );
}

//---------------------------------------------------------------------------
void AppState::heartbeat( ) {
	while ( true ) {
		send( Action::heartbeat );
		this_thread::sleep_for( chrono::seconds( HEARTBEAT_TIME ) );
	}
}

//---------------------------------------------------------------------------
void AppState::setState( State state ) {
	if ( this->state != state ) {
		this->state = state;
		send( Action::state );
	}
}

//---------------------------------------------------------------------------
State AppState::getState( ) const {
	return state;
}

//---------------------------------------------------------------------------
bool AppState::isRunning( ) const {
	return state == State::idle || state == State::working;
}
