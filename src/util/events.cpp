#include "util/events.hpp"

#include <iostream>
#include <stdexcept>

namespace util {

subscriber::subscriber()
	: m_mgr(nullptr) {
}

subscriber::~subscriber() {
	for (auto& evt : m_events) {
		m_mgr->unhook(this, evt);
	}
}

void subscriber::init(evtmgr* mgr) {
	m_mgr = mgr;
}

void subscriber::hook(evt_t id, event_callback_t callback) {
	if (!initd()) throw std::runtime_error("Attempted to hook without calling init()!");
	if (m_events.find(id) != m_events.end()) {
		throw std::runtime_error("Event already hooked in!");
	}

	m_mgr->hook(this, id, callback);
	m_events.insert(id);
}

void subscriber::unhook(evt_t id) {
	if (m_events.find(id) == m_events.end()) {
		return;
	}

	m_mgr->unhook(this, id);
	rmevt(id);
}

void subscriber::rmevt(evt_t id) {
	m_events.erase(id);
}

bool subscriber::initd() const {
	return m_mgr != nullptr;
}

///////////////////////////////////////////////////////////

evtmgr::evtmgr() {
}

evtmgr::~evtmgr() {
	for (auto& [id, cbs] : m_hooks) {
		for (auto& [sub, cb] : cbs) {
			sub->rmevt(id);
		}
	}
}

void evtmgr::emit(evt_t evt_id, util::data d) {
	for (auto& [_, cb] : m_hooks[evt_id]) {
		cb(d);
	}
}

void evtmgr::hook(subscriber* sub, evt_t id, event_callback_t cb) {
	if (m_hooks.find(id) == m_hooks.end()) {
		m_hooks[id] = {};
	}
	m_hooks[id][sub] = cb;
}

void evtmgr::unhook(subscriber* sub, evt_t id) {
	if (m_hooks.find(id) == m_hooks.end()) {
		return;
	}
	m_hooks[id].erase(sub);
}

}
