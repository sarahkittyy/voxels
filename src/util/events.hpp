#pragma once

#include <functional>
#include <set>
#include <unordered_map>
#include <vector>

#include "util/data.hpp"

namespace util {

typedef std::function<void(util::data)> event_callback_t;
typedef int evt_t;

class evtmgr;

// inherited by classes to subscribe to events
class subscriber {
public:
	subscriber();
	virtual ~subscriber();

protected:
	// call to initialize the event manger to bind to
	void init(evtmgr* mgr);

	// hook into an event
	void hook(evt_t id, event_callback_t callback);
	// unhook from an event
	void unhook(evt_t id);

private:
	friend class evtmgr;

	// internal event manager
	evtmgr* m_mgr;

	// initalized?
	bool initd() const;

	// all hooked events
	std::set<evt_t> m_events;
	void rmevt(evt_t);
};

// inherited by classes that emit events
class evtmgr {
public:
	evtmgr();
	virtual ~evtmgr();

	// call all callbacks of a given event
	void emit(int evt_id, util::data d);

private:
	friend class subscriber;

	// all hooks
	std::unordered_map<evt_t, std::unordered_map<subscriber*, event_callback_t>> m_hooks;
	// attach a new event
	void hook(subscriber* sub, evt_t id, event_callback_t cb);
	// unhook from an event
	void unhook(subscriber* sub, evt_t id);
};

}
