#ifndef OOP_EXERCISE_07_FACTORY_H
#define OOP_EXERCISE_07_FACTORY_H

#include <memory>

#include "Rectangle.h"
#include "Rhombus.h"
#include "Trapeze.h"

template<class Tpoint,class Tside, class FIGURE>
class TFactory;


template<class Tpoint, class Tside>
class TFactory< Tpoint, Tside,  Rectangle<Tpoint, Tside> > {
public:
    static std::shared_ptr<Figures<Tpoint>> CreateFigure() {
        std::pair<Tpoint, Tpoint> curCords;
        Tside curHeight, curWidth;
        std::cout << "Enter rectangle as follows: x y a b" << std::endl;
        std::cout << "x, y is a left bottom point cords" << std::endl;
        std::cout << "a and b are width and heigth" << std::endl;
        std::cin >> curCords.first >> curCords.second >> curHeight >> curWidth;
        auto rect = new Rectangle<Tpoint,Tside>(curCords, curHeight, curWidth);
        return std::shared_ptr<Figures<Tpoint>>(rect);
    }

    static std::shared_ptr<Figures<Tpoint>> Read(std::ifstream& file) {
        std::pair<Tpoint, Tpoint> curCords;
        Tside curHeight, curWidth;
        file.read((char*) &(curCords.first), sizeof(Tpoint));
        file.read((char*) &(curCords.second), sizeof (Tpoint));
        file.read((char*) &(curHeight), sizeof (Tside));
        file.read((char*) &(curWidth), sizeof (Tside));
        auto rect = new Rectangle<Tpoint,Tside>(curCords, curHeight, curWidth);
        return std::shared_ptr<Figures<Tpoint>>(rect);
    }
};

template<class Tpoint, class Tside>
class TFactory< Tpoint, Tside, Trapeze<Tpoint,Tside> > {
public:
    static std::shared_ptr<Figures<Tpoint>> CreateFigure() {
        std::pair<Tpoint, Tpoint> curCords;
        Tside curGreaterBase, curSmallerBase, curHeight;
        std::cout << "Enter trapeze as follows: x y a b c" << std::endl
                  << "x, y is a left bottom point cords" << std::endl
                  << "a, b and c are larger, smaller base and height" << std::endl;
        std::cin >> curCords.first >> curCords.second >> curGreaterBase >> curSmallerBase >> curHeight;
        auto trap = new Trapeze<Tpoint,Tside>(curCords, curGreaterBase, curSmallerBase, curHeight);
        return std::shared_ptr<Figures<Tpoint>>(trap);
    }

    static std::shared_ptr<Figures<Tpoint>> Read(std::ifstream& in) {
        std::pair<Tpoint, Tpoint> curCords;
        Tside curGreaterBase, curSmallerBase, curHeight;
        in.read((char*) &(curCords.first), sizeof (Tpoint));
        in.read((char*) &(curCords.second), sizeof (Tpoint));
        in.read((char*) &(curGreaterBase), sizeof (Tside));
        in.read((char*) &(curSmallerBase), sizeof (Tside));
        in.read((char*) &(curHeight), sizeof (Tside));
        auto trap = new Trapeze<Tpoint,Tside>(curCords, curGreaterBase, curSmallerBase, curHeight);
        return std::shared_ptr<Figures<Tpoint>>(trap);
    }
};


template<class Tpoint, class Tside>
class TFactory< Tpoint, Tside, Rhombus<Tpoint,Tside> > {
public:
    static std::shared_ptr<Figures<Tpoint>> CreateFigure() {
        std::pair<Tpoint, Tpoint> curCords;
        Tside curD1, curD2;
        std::cout << "Enter rhombus as follows: x y d1 d2" << std::endl
                  << "x, y is a left bottom point cords" << std::endl
                  << "d1 and d2 are diagonals of rhombus" << std::endl;
        std::cin >> curCords.first >> curCords.second >> curD1 >> curD2;
        auto pRhombus = new Rhombus<Tpoint,Tside>(curCords, curD1, curD2);
        return std::shared_ptr<Figures<Tpoint>>(pRhombus);
    }

    static std::shared_ptr<Figures<Tpoint>> Read(std::ifstream& in) {
        std::pair<Tpoint, Tpoint> curCords;
        Tside curD1, curD2;
        in.read((char*) &(curCords.first), sizeof (Tpoint));
        in.read((char*) &(curCords.second), sizeof (Tpoint));
        in.read((char*) &(curD1), sizeof (Tside));
        in.read((char*) &(curD2), sizeof (Tside));
        auto pRhombus = new Rhombus<Tpoint,Tside>(curCords, curD1, curD2);
        return std::shared_ptr<Figures<Tpoint>>(pRhombus);
    }
};

#endif //OOP_EXERCISE_07_FACTORY_H
