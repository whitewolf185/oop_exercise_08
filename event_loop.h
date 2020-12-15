#ifndef OOP_EXERCISE_08_EVENT_LOOP_H
#define OOP_EXERCISE_08_EVENT_LOOP_H

#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
#include <unordered_map>
#include <memory>
#include <thread>
#include <mutex>
#include "event.h"

template<tempFigTYPE>
class Event_Manager{
private:
    std::unordered_multimap<eventType, std::shared_ptr<Handler<FIGURE_TYPE>>& hendlers;

public:
    void subscribe(eventType _type, std::shared_ptr<Handler<FIGURE_TYPE>& _hendler){
        hendlers.emplace(_type,_hendler);
    }

    void notify(eventType type, Event& event){
        auto keys = hendlers.equal_range(type);
        auto doEvent = [&](auto pointer){
            pointer.second->do_event(event);
        };
        std::for_each(keys.first, keys.second,doEvent);
    }
};


template <tempFigTYPE>
class Event_loop{
private:
    bool exit = false;
    std::queue<Event> events;
    Event_Manager<FIGURE_TYPE> eventManager;
    std::mutex wait;

public:
    void addHandler(eventType _type, std::shared_ptr<Handler<FIGURE_TYPE>>& handler){
        eventManager.subscribe(_type,handler);
    }

    void addEvent(Event& _event){
        std::lock_guard<std::mutex> guard(wait);
        events.push(_event);
    }

};
#endif //OOP_EXERCISE_08_EVENT_LOOP_H
