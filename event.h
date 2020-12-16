#ifndef OOP_EXERCISE_08_EVENT_H
#define OOP_EXERCISE_08_EVENT_H
#include <utility>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include "document.h"

unsigned FILE_COUNT = 1;

std::string str_gen(){
    std::string result = "./log";
    result += std::to_string(FILE_COUNT++);
    result += ".bin";
}

enum class eventType{
    save,print,exit
};

struct Event_Response{
    virtual ~Event_Response() = default;
};

struct Event_Response_Printer: public Event_Response{
    bool& done;
    explicit Event_Response_Printer(bool& _done):done(_done){}
};

struct Event_Response_Saver: public Event_Response{
    bool& done;
    explicit Event_Response_Saver(bool& _done):done(_done){}
};



struct Event_data{
    virtual ~Event_data() = default;
};

template<tempFigTYPE>
struct Event_data_Printer: public Event_data{
    const std::vector<std::shared_ptr<Figures FIGURE_TYPE>> figures;

    explicit Event_data_Printer(const std::vector<std::shared_ptr<Figures FIGURE_TYPE>>& _figures) : figures(_figures){}
};

template<tempFigTYPE>
struct Event_data_Saver: public Event_data{
    const std::vector<std::shared_ptr<Figures FIGURE_TYPE>> figures;

    explicit Event_data_Saver(const std::vector<std::shared_ptr<Figures FIGURE_TYPE>>& _figures) : figures(_figures){}
};


template<tempFigTYPE>
struct Event {
    eventType type;
    std::shared_ptr<Event_Response> response;
    std::shared_ptr<Event_data> data;
    std::function<void(std::shared_ptr<Event_Response>)> callback;

    Event(eventType _type,
          std::shared_ptr<Event_data> _data,
          std::shared_ptr<Event_Response> _response,
          std::function<void(std::shared_ptr<Event_Response>)> _callback): type(_type),
                                                                           data(std::move(_data)),
                                                                           response(std::move(_response)),
                                                                           callback(std::move(_callback)){}

};

template<tempFigTYPE>
struct Handler{
    virtual bool do_event(Event FIGURE_TYPE& event) = 0;
    virtual ~Handler() = default;
};

template<tempFigTYPE>
struct Handler_Printer: public Handler FIGURE_TYPE {
    bool do_event(Event FIGURE_TYPE& event) override{
        auto data = std::static_pointer_cast<Event_data_Printer FIGURE_TYPE>(event.data);
        //если данный нет, то мы ничего и не сделали
        if(!data){
            return false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        for (auto& fig : data->figures) {
            fig->Print(std::cout);
        }
        event.callback(event.response);
        return true;
    }
};

template<tempFigTYPE>
struct Handler_Saver: public Handler FIGURE_TYPE {
    bool do_event(Event FIGURE_TYPE& event) override{
        std::ofstream fout;
        fout.open(str_gen(), std::ios_base::out);
        auto data = std::static_pointer_cast<Event_data_Saver FIGURE_TYPE>(event.data);
        //если данный нет, то мы ничего и не сделали
        if(!data){
            fout.close();
            return false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        for (auto& fig : data->figures) {
            fig->Print(fout);
        }
        event.callback(event.response);
        fout.close();
        return true;
    }
};

#endif //OOP_EXERCISE_08_EVENT_H