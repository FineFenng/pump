//
// Created by fenng on 18-8-10.
//

#ifndef PUMP_BACKENDABSTRACT_H
#define PUMP_BACKENDABSTRACT_H


#include <vector>


#include <pump/Common.h>
#include <pump/net/EventLoop.h>

namespace pump { namespace net
{

class WatchAbstract;

typedef std::vector<WatchAbstract> HandleList;

class BackendAbstract
{
public:

	virtual void poll() = 0;
	virtual void init_backend() = 0;

	virtual void add_interests(const WatchAbstract& handle) = 0;
	virtual void modify_interests(const WatchAbstract& handle) = 0;
	virtual void delete_interests(const WatchAbstract& handle) = 0;
};
}}

#endif //PUMP_BACKENDABSTRACT_H
