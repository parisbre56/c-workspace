#include "DataObject.hpp"

using namespace std;

DataObject::DataObject(ConnectionData* returnLocation, string request):retLoc(returnLocation),req(request) {
	retLoc->linksIncrement();
}

/* Will also delete returnLocation if this is the last remaining pointer to it.
 */
DataObject::~DataObject() {
	retLoc->linksDecrement();
}