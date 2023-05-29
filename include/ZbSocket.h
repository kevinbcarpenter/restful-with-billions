#ifndef ZBSOCKET_class
#define ZBSOCKET_class

#include "AppResources.h"
#include "StateSub.h"
#include "zmq.hpp"

// In milliseconds
const int DEFAULT_TIMEOUT{180'000};
const int DEFAULT_LINGER{0};

/***********************************************************************
** Forward Declarations
***********************************************************************/
class Logger;

/***********************************************************************
** Class Declaration
***********************************************************************/
class ZbSocket {
private:
	ushort retry{0};
	zmq::context_t& context;
	zmq::socket_t socket;
	zmq::message_t message;

	AppType appType{AppType::unknown};

	Logger* logger{nullptr};

	ZbSocket( ) = delete;

	static string logInfo( string function ) {
		return "ZbSocket:" + function;
	}

public:
	ZbSocket( zmq::context_t& context, int type, int timeout = DEFAULT_TIMEOUT, int linger = DEFAULT_LINGER );
	ZbSocket( zmq::context_t& context, int type, AppType appType, int timeout = DEFAULT_TIMEOUT, int linger = DEFAULT_LINGER );

	/***********************************************************************
	** Gets best connection from StateSub for the provided application
	**
	** Return:	N/A
	***********************************************************************/
	string getConnection( const AppMap& appMap ) const;

	/***********************************************************************
	** Connects to the application. Must call manually if AppType is
	** not supplied to the constructor.
	**
	** Return:	N/A
	***********************************************************************/
	void connect( );
	void connect( const AppType& appType );

	/***********************************************************************
	** Sends message to the connected application. Upon timeout, forces
	** socket to connect to another valid application and attempts to
	** send the message once again.
	**
	** Return:	N/A
	***********************************************************************/
	void send( const string& sMsg );
	void send( zmq::message_t& msg );

	/***********************************************************************
	** Receive message sent to application. Upon timeout, forces socket
	** to connect to another valid application and attempts to send and
	** receive once again, up to 'retry' times, then throws an exception.
	**
	** Return:	N/A
	***********************************************************************/
	void recv( string& response );
};

#endif
