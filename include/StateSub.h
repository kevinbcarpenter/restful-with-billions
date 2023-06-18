#ifndef STATESUB_class
#define STATESUB_class

#include <shared_mutex>
#include <string>
#include <unordered_map>

#include "AppResources.h"
#include "zmq.hpp"

typedef std::unordered_map< AppIdentity, AppResource > AppMap;

class StateSub {
private:
	zmq::context_t& context;
	static StateSub* stateSub;
	AppMap appMap;

	StateSub( zmq::context_t& context, const json& config, ushort level );
	void subscribe( const json& config, ushort level );

	static std::string logInfo( const std::string& function ) {
		return "StateSub:" + function;
	}

public:
	std::shared_timed_mutex stateMutex;

	StateSub( ) = delete;
	StateSub( StateSub&& ) = delete;
	StateSub( const StateSub& ) = delete;
	void operator=( const StateSub& ) = delete;

	/***********************************************************************
	** Singlton functions
	**
	** Return:	N/A
	***********************************************************************/
	static void createInstance( zmq::context_t& context, const json& config, ushort level );
	static StateSub* getInstance( );

	/***********************************************************************
	** Retreive the application mapping list
	**
	** Return:	appMap
	***********************************************************************/
	const AppMap& getApps( );

	/***********************************************************************
	** Gets best connection from StateSub for the provided application
	**
	** Return:	N/A
	***********************************************************************/
	static std::string getConnection( AppType appType );
};

#endif
