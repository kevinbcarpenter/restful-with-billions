#include <thread>

using namespace std;

#include "AppState.h"
#include "Logger.h"
#include "StateSub.h"

StateSub* StateSub::stateSub{nullptr};

//---------------------------------------------------------------------------
StateSub::StateSub( zmq::context_t& context, const json& config, ushort level ) : context( context ) {
	thread( &StateSub::subscribe, this, config, level ).detach( );
}

//---------------------------------------------------------------------------
void StateSub::subscribe( const json& config, ushort level ) {
	zmq::socket_t socket( context, ZMQ_SUB );
	if ( level > StateLevel::MAX_LEVEL ) {
		level = StateLevel::MAX_LEVEL;
	}
	string sLevel{to_string( level )};
	socket.setsockopt( ZMQ_SUBSCRIBE, sLevel.data( ), sLevel.size( ) );
	socket.connect( "tcp://" + config.at( "control" ).at( "host" ).get< string >( ) + ":" + to_string( config.at( "control" ).at( "subPort" ).get< ushort >( ) ) );

	while ( true ) {
		try {
			// Get broadcast message
			zmq::message_t address, message;
			socket.recv( &address );
			socket.recv( &message );
			string sMessage( static_cast< char* >( message.data( ) ), message.size( ) );

			// Parse message
			json js = json::parse( sMessage );
			AppIdentity id = js;
			AppResource res = js;

			// Update map
			unique_lock< shared_timed_mutex > lck( stateMutex );
			if ( res.state == State::disconnected ) {
				appMap.erase( id );
			} else {
				appMap[ id ] = res;
			}

		} catch ( zmq::error_t& ex ) {
			Logger::getLogger( )->error( logInfo( __func__ ), ex.what( ), IS_THREAD );
			if ( ex.num( ) == ETERM ) {	// Context was terminated, exit program
				throw string( ex.what( ) );
			}
		} catch ( exception& ex ) {
			Logger::getLogger( )->warn( logInfo( __func__ ), ex.what( ), IS_THREAD );
		}
	}
}

//---------------------------------------------------------------------------
void StateSub::createInstance( zmq::context_t& context, const json& config, ushort level ) {
	if ( !stateSub ) {
		stateSub = new StateSub( context, config, level );
	}
}

//---------------------------------------------------------------------------
StateSub* StateSub::getInstance( ) {
	return stateSub;
}

//---------------------------------------------------------------------------
const AppMap& StateSub::getApps( ) {
	return appMap;
}

//---------------------------------------------------------------------------
string StateSub::getConnection( AppType appType ) {
	const AppMap& appMap{stateSub->getApps( )};

	while ( appMap.empty( ) ) {
		this_thread::sleep_for( chrono::seconds( 1 ) );
	}

	AppMap::const_iterator appRes;

	do {
		shared_lock< shared_timed_mutex > lck( stateSub->stateMutex );
		appRes = appMap.end( );

		for ( auto it{appMap.begin( )}; it != appMap.end( ); ++it ) {
			if ( it->first.name == appType ) {
				if ( it->second.state == State::idle ) {
					return it->first.getConnectionInfo( );
				} else {
					if ( appRes != appMap.end( ) ) {
						appRes = it->second.lastChange < appRes->second.lastChange ? it : appRes;
					} else {
						appRes = it;
					}
				}
			}
		}

		if ( appRes == appMap.end( ) ) {
			lck.unlock( );
			Logger::getLogger( )->warn( logInfo( __func__ ), "Failed to find " + toString( appType ) + " - Verify app and Automation Director are running", IS_THREAD );
			AppState::getInstance( )->send( Action::refresh );
			this_thread::sleep_for( chrono::seconds( 15 ) );
		} else {
			break;
		}
	} while ( true );

	return appRes->first.getConnectionInfo( );
}
