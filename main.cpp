/*
 * Волков Матвей Андреевич
Создать приложение, которое будет считывать из стандартного ввода данные фигур, согласно варианту задания, выводить
 их характеристики на экран и записывать в файл. Фигуры могут задаваться как своими вершинами, так и другими
 характеристиками (например, координата центра, количество точек и радиус).
Программа должна:
1.	Осуществлять ввод из стандартного ввода данных фигур, согласно варианту задания;
2.	Программа должна создавать классы, соответствующие введенным данных фигур;
3.	Программа должна содержать внутренний буфер, в который помещаются фигуры. Для создания буфера допускается
 использовать стандартные контейнеры STL. Размер буфера задается параметром командной строки. Например, для буфера
 размером 10 фигур: oop_exercise_08 10
4.	При накоплении буфера они должны запускаться на асинхронную обработку, после чего буфер должен очищаться;
5.	Обработка должна производиться в отдельном потоке;
6.	Реализовать два обработчика, которые должны обрабатывать данные буфера:
    a.	Вывод информации о фигурах в буфере на экран;
    b.	Вывод информации о фигурах в буфере в файл. Для каждого буфера должен создаваться файл с уникальным именем.
7.	Оба обработчика должны обрабатывать каждый введенный буфер. Т.е. после каждого заполнения буфера его содержимое
 должно выводиться как на экран, так и в файл.
8.	Обработчики должны быть реализованы в виде лямбда-функций и должны хранится в специальном массиве обработчиков.
 Откуда и должны последовательно вызываться в потоке – обработчике.
9.	В программе должно быть ровно два потока (thread). Один основной (main) и второй для обработчиков;
10.	В программе должен явно прослеживаться шаблон Publish-Subscribe. Каждый обработчик должен быть реализован как
 отдельный подписчик.
11.	Реализовать в основном потоке (main) ожидание обработки буфера в потоке-обработчике. Т.е. после отправки буфера на
 обработку основной поток должен ждать, пока поток обработчик выведет данные на экран и запишет в файл.

Вариант: Прямоугольник, трапеция, ромб
*/

#include "event_loop.h"
#include <exception>
#include <functional>
//нужно, чтобы было 2 одинаковых типа
#define yourTYPE double , double
#define figTYPE <double>



int main(int argc, char* argv[]) {
    try{
        if(argc != 2){
            throw std::invalid_argument("Too many arguments");
        }
    }
    catch (std::invalid_argument& er){
        std::terminate();
    }

    int buffer_size = std::stoi(argv[1]);
    try{
        if(buffer_size <= 0){
            throw std::invalid_argument("buffer_size is less or equal than zero");
        }
    }
    catch (std::invalid_argument& er){
        std::terminate();
    }

    TDocument<yourTYPE> doc;
    Event_loop figTYPE eventLoop;
    std::shared_ptr<Handler figTYPE> handler_printer(new Handler_Printer figTYPE);
    std::shared_ptr<Handler figTYPE> handler_saver(new Handler_Saver figTYPE);
    eventLoop.addHandler(eventType::print, handler_printer);
    eventLoop.addHandler(eventType::save, handler_saver);

    std::thread threadHandler(std::ref(eventLoop));
    std::string s;
    while ((std::cout << "> ") && (std::cin >> s)) {
        if (s == "+") {
            size_t pos;
            unsigned short type;
            std::cin >> pos >> type;
            doc.Add(pos, type);
        }
        else if (s == "-") {
            size_t pos;
            std::cin >> pos;
            doc.Delete(pos);
        }
        else if (s == "p") {
            std::cout << "Printing document:" << std::endl;
            std::cout << doc;
        }
        else if (s == "u") {
            doc.Undo();
        }
        else if (s == "h") {
            std::cout << "> \'+\' - add a figure\n"
                         "when you add figure:\nfirst - enter your position you want insert your figure\n"
                         "second - enter figure ID:\n"
                         "  1 - rhombus\n  2 - rectangle\n  3 - trapeze" << std::endl;
            std::cout << "> \'-\' - remove a figure\n"
                         "when you remove figure, enter your position you want to delete" << std::endl;
            std::cout << "> \'p\' - print document" << std::endl;
            std::cout << "> \'u\' - undo changes" << std::endl;
            std::cout << "> \'h\' - show this message" << std::endl;
            std::cout << "> \'e\' - exit\n" << std::endl;
        }

        else if(s == "e"){
            Event figTYPE ev (eventType::exit,
                              std::make_shared<Event_data>(),
                              std::make_shared<Event_Response>(),
                              [](auto){});
            eventLoop.addEvent(ev);
            break;
        }
        else {
            std::cout << "Unknown command. Type \'h\' to show help" << std::endl;
        }

        bool printer_done = false;
        bool save_done = false;
        if(doc.getFigs().size() >= buffer_size){
            auto do_lam_ev1 = [](const std::shared_ptr<Event_Response>& response){
                if(auto ptr = std::static_pointer_cast<Event_Response_Printer>(response)){
                    ptr->done = true;
                }
            };
            auto do_lam_ev2 = [](const std::shared_ptr<Event_Response>& response){
                if(auto ptr = std::static_pointer_cast<Event_Response_Saver>(response)){
                    ptr->done = true;
                }
            };


            Event figTYPE ev1(eventType::print,
                              std::make_shared<Event_data_Printer figTYPE>(doc.getFigs()),
                              std::make_shared<Event_Response_Printer>(printer_done),
                              do_lam_ev1);
            Event figTYPE ev2(eventType::save,
                              std::make_shared<Event_data_Saver figTYPE>(doc.getFigs()),
                              std::make_shared<Event_Response_Saver>(save_done),
                              do_lam_ev2);

            eventLoop.addEvent(ev1);
            eventLoop.addEvent(ev2);
            while(!printer_done && !save_done){
                std::cout << '.' << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }
            doc.Clear_list();
        }
    }
    return 0;
}