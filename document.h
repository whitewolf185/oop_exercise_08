#ifndef OOP_EXERCISE_07_DOCUMENT_H
#define OOP_EXERCISE_07_DOCUMENT_H

//тип, используемый вами
#define tempTYPE class Tpoint, class Tside

//если нет типа у глобального класса фигуы, нужно закомментировать
//если имеется ваш собственный тип, обязательно нужно писать <>
#define FIGURE_TYPE <Tpoint>

//здесь находится применение ваших типов
#define TYPES Tpoint, Tside

#include <list>
#include <stack>

#include "factory.h"

template<tempTYPE>
class TDocument {
private:
    struct IAction;

    using figure_pointer = std::shared_ptr<Figures FIGURE_TYPE>;
    using action_pointer = std::shared_ptr<IAction>;
    using const_iterator = typename std::list< figure_pointer >::const_iterator;

    std::list< figure_pointer > FiguresList;
    std::stack< action_pointer > ActionStack;

    struct IAction {
        virtual void PerformAction(TDocument & fact) = 0;
        virtual ~IAction() {}
    };

    class TDeleteAction : public IAction {
    private:
        size_t DeletePos;
    public:
        TDeleteAction(const size_t & pos) : DeletePos(pos) {}
        void PerformAction(TDocument & fact) override {
            fact.Delete(DeletePos);
        }
    };

    class TAddAction : public IAction {
    private:
        size_t AddPos;
        figure_pointer AddFigure;
    public:
        TAddAction(const size_t & pos, const figure_pointer & fig) : AddPos(pos), AddFigure(fig) {}
        void PerformAction(TDocument & fact) override {
            fact.AddFigure(AddPos, AddFigure);
        }
    };


public:
    void CreateNew() {
        while (!ActionStack.empty()) {
            ActionStack.pop();
        }
        FiguresList.clear();
    }

    void LoadFromFile(std::ifstream& in) {
        CreateNew();
        size_t n;
        in.read((char*) &(n), sizeof (size_t));
        for (size_t i = 0; i < n; ++i) {
            unsigned int type;
            in.read((char*) &(type), sizeof(unsigned));
            if (type == RHOMBUS_TYPE_ID) {
                FiguresList.push_back(TFactory< TYPES, Rhombus<TYPES>>::Read(in));
            } else if (type == RECTANGLE_TYPE_ID) {
                FiguresList.push_back(TFactory< TYPES, Rectangle<TYPES>>::Read(in));
            } else if (type == TRAPEZE_TYPE_ID) {
                FiguresList.push_back(TFactory< TYPES, Trapeze<TYPES>>::Read(in));
            }
        }
    }

    void SaveToFile(std::ofstream& out) {
        size_t n = FiguresList.size();
        out.write((const char*) &n, sizeof (size_t));
        for (const_iterator it = FiguresList.begin(); it != FiguresList.end(); ++it) {
            (*it)->Write(out);
        }
    }

    void Add(const size_t& pos, const unsigned int& figureID) {
        if (figureID == RHOMBUS_TYPE_ID) {
            AddFigure(pos, TFactory<TYPES, Rhombus<TYPES> >::CreateFigure());
        } else if (figureID == RECTANGLE_TYPE_ID) {
            AddFigure(pos, TFactory<TYPES, Rectangle<TYPES> >::CreateFigure());
        } else if (figureID == TRAPEZE_TYPE_ID) {
            AddFigure(pos, TFactory<TYPES, Trapeze<TYPES> >::CreateFigure());
        }
    }

    void AddFigure(const size_t & pos, const figure_pointer & fig) {
        if (pos > FiguresList.size()) {
            FiguresList.push_back(fig);
            auto delAct = new TDeleteAction(FiguresList.size());
            ActionStack.push(action_pointer(delAct));
        } else {
            size_t cur = 0;
            const_iterator it = FiguresList.begin();
            while (cur < pos) {
                ++cur;
                ++it;
            }
            FiguresList.insert(it, fig);
            auto delAct = new TDeleteAction(pos + 1);
            ActionStack.push(action_pointer(delAct));
        }
    }

    void Delete(const size_t & pos) {
        if (FiguresList.empty()) {
            std::cout << "Nothing to delete!" << std::endl;
            return;
        }
        if (pos > FiguresList.size()) {
            auto addAct = new TAddAction(FiguresList.size() - 1, FiguresList.back());
            ActionStack.push(action_pointer(addAct));
            FiguresList.pop_back();
        } else {
            size_t cur = 1;
            const_iterator it = FiguresList.begin();
            while (cur < pos) {
                ++cur;
                ++it;
            }
            auto addAct = new TAddAction(cur - 1, *it);
            ActionStack.push(action_pointer(addAct));
            FiguresList.erase(it);
        }
    }

    void Undo() {
        if (ActionStack.empty()) {
            std::cout << "Nothing to undo!" << std::endl;
        } else {
            ActionStack.top()->PerformAction(*this);
            ActionStack.pop();
            ActionStack.pop();
        }
    }


    friend std::ostream & operator << (std::ostream & of, const TDocument<TYPES> & fact) {
        TDocument::const_iterator it = fact.FiguresList.begin();
        for (size_t i = 0; i < fact.FiguresList.size(); ++i) {
            of << "[" << i + 1 << "] ";
            (*it)->Print(of);
            of << std::endl;
            ++it;
        }
        return of;
    }
};

#endif //OOP_EXERCISE_07_DOCUMENT_H
