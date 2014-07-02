#ifndef __DATAOBJECT__INCLUDED__
#define __DATAOBJECT__INCLUDED__

#include <string>

#include "ConnectionData.hpp"

class DataObject {
public:
	/* Holds info about where the data should be transferred to (the connection)
	 * as well as a mutex to ensure that only one can access it at the same time.
	 */
	ConnectionData * const retLoc;
	/* The data we need to retrieve and send.
	 */
	const std::string req;
	
	DataObject(ConnectionData* returnLocation, std::string request);
	/* Will also delete returnLocation if this is the last remaining pointer to it.
	 */
	~DataObject();
};

#endif  //__DATAOBJECT__INCLUDED__