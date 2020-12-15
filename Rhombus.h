#ifndef OOP_EXERSICE_03_RHOMBUS_H
#define OOP_EXERSICE_03_RHOMBUS_H
#include "Figures.h"

const unsigned int RHOMBUS_TYPE_ID = 1;

template<class T1, class T2>
class Rhombus : public Figures<T1> {
private:
    T2 D1, D2;
public:
    Rhombus(const std::pair<T1,T2> &_point, const T2 &d1, const T2 &d2): D1(d1), D2(d2){
        //point of left edge
        this->point.first = _point.first;
        this->point.second = _point.second;

        if (D1 < 0 || D2 < 0) {
            throw std::invalid_argument("Invalid Rhombus parameters!");
        }
    }

    void Print(std::ostream& out) override{
        out << *this << std::endl;
    }

    void Write(std::ofstream& file) override{
        file.write((const char*) &(RHOMBUS_TYPE_ID), sizeof(unsigned));
        file.write((const char*) &(this->point.first), sizeof (T1));
        file.write((const char*) &(this->point.second), sizeof(T1));
        file.write((const char*) &(this->D1), sizeof(T2));
        file.write((const char*) &(this->D2), sizeof (T2));
    }

    template<class U>
    friend std::ostream& operator<<(std::ostream& out, const Rhombus<U,U>& rhombus){
        out << "Rhombus vertices: [ "
            << std::pair<U,U> (rhombus.point.first, rhombus.point.second) << ", "
            << std::pair<U,U> (rhombus.point.first + rhombus.D1 / 2, rhombus.point.second + rhombus.D2 / 2) << ", "
            << std::pair<U,U> (rhombus.point.first + rhombus.D1, rhombus.point.second) << ", "
            << std::pair<U,U> (rhombus.point.first - rhombus.D1 / 2, rhombus.point.second - rhombus.D2 / 2) << " ]" << std::endl;

        return out;
    }
};

#endif //OOP_EXERSICE_03_RHOMBUS_H
