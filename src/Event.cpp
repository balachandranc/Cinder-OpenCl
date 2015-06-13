//
//  Event.cpp
//  ImplProject
//
//  Created by Ryan Bartley on 3/18/14.
//
//

#include "Event.h"
#include "Context.h"

namespace cl {
		
Event::~Event()
{
	if( mId != nullptr )
		clReleaseEvent( mId );
}
	
void Event::setCompletedCallback( EventCallback pFunc, void *userData )
{
	cl_int errNum;
	errNum = clSetEventCallback( mId, CL_COMPLETE, pFunc, userData );
}
	
void Event::setUserEventStatus( cl_int status )
{
	if( mType == SYS_EVENT ) {
		clSetUserEventStatus( mId, status);
	}
}
	
Event::Event()
: mId( nullptr ), mType( SYS_EVENT )
{
}
	
Event::Event( cl_event event, EventType type )
: mId( event ), mType( type )
{
	mId = event;
	clRetainEvent( mId );
}
	
Event::Event( const Event &rhs )
: mId( rhs.mId ), mType( rhs.mType )
{
	std::cout << "Using Event::Event Copy Constructor" << std::endl;
	clRetainEvent( mId );
}
	
Event::Event( const ContextRef &context )
: mType( USER_EVENT )
{
	cl_int errNum = CL_SUCCESS;
	mId = clCreateUserEvent( context->getId(), &errNum);
	
	//! TODO: Throw event error
	if ( errNum ) {
		std::cout << "ERROR: creating user event" << std::endl;
	}
}
	
EventRef Event::create( const Event &event )
{
	return EventRef( new Event( event ) );
}
	
Event& Event::operator=( const Event &rhs )
{
	if (this != &rhs) {
		mId = rhs.mId;
		if( mId )
			clRetainEvent( mId );
		mType = rhs.mType;
	}
	return *this;
}
	
Event& Event::operator=( Event &&rhs )
{
	if (this != &rhs) {
		mId = rhs.mId;
		mType = rhs.mType;
		rhs.mId = nullptr;
		rhs.mType = (EventType)-1;
	}
	return *this;
}

Event::Event( Event &&rhs )
: mId( rhs.mId ), mType( rhs.mType )
{
	rhs.mId = nullptr;
	rhs.mType = (EventType)-1;
}
	
} // namespace cl