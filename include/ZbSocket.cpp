#include <thread>
using namespace std;

#include "Logger.h"
#include "ZbSocket.h"

//---------------------------------------------------------------------------
ZbSocket::ZbSocket( zmq::context_t& context, int pattern, int timeout, int linger ) : context( context ), socket( context, pattern ) {
	socket.setsockopt( ZMQ_SNDTIMEO, timeout );
	socket.setsockopt( ZMQ_RCVTIMEO, timeout );
	socket.setsockopt( ZMQ_LINGER, linger );
	logger = Logger::getLogger( );
}

//---------------------------------------------------------------------------
ZbSocket::ZbSocket( zmq::context_t& context, int pattern, AppType appType, int timeout, int linger ) : ZbSocket( context, pattern, timeout, linger ) {
	this->appType = appType;
	this->connect( );
}

//---------------------------------------------------------------------------
void ZbSocket::connect( ) {
	if ( appType == AppType::unknown ) {
		throw runtime_error( "Invalid AppType" );
	}
	string connection{StateSub::getConnection( appType )};
	socket.connect( "tcp://" + connection );
	logger->save( logInfo( __func__ ), "Connected to " + toString( appType ) + "@" + connection, IS_MAIN );
}

//---------------------------------------------------------------------------
void ZbSocket::connect( const AppType& appType ) {
	this->appType = appType;
	connect( );
}

//---------------------------------------------------------------------------
void ZbSocket::send( const string& sMsg ) {
	zmq::message_t msg( sMsg.data( ), sMsg.length( ) );
	this->send( msg );
}

//---------------------------------------------------------------------------
void ZbSocket::send( zmq::message_t& msg ) {
	// If this is a new message, save it
	if ( message.size( ) == 0 ) {
		message.copy( &msg );
	}

	// Attempt to send to our connection
	if ( !socket.send( msg ) ) {
		// If send fails, try another connection and send again
		logger->warn( logInfo( __func__ ), "Failed to send message to " + toString( appType ) + ". Trying another connection.", IS_MAIN );
		connect( );
		msg.copy( &message );
		send( msg );
	}
}

//---------------------------------------------------------------------------
void ZbSocket::recv( string& response ) {
	zmq::message_t resp;
	while ( !socket.recv( &resp ) ) {
		if ( retry >= 2 ) {
			throw runtime_error( "Retry attempts exceeded" );
		}

		// If send fails, try another connection
		logger->warn( logInfo( __func__ ), "Failed to receive response from " + toString( appType ) + ". Trying another connection.", IS_MAIN );
		++retry;
		connect( );

		// Try to send and receive again
		zmq::message_t msg;
		msg.copy( &message );
		send( msg );
	}
	response = string( static_cast< char* >( resp.data( ) ), resp.size( ) );
	resp.move( &message );	// Clear saved message
}
