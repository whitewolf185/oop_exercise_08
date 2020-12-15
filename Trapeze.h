#ifndef OOP_EXERSICE_03_TRAPEZE_H
#define OOP_EXERSICE_03_TRAPEZE_H
#include "Figures.h"

const unsigned int TRAPEZE_TYPE_ID = 3;

template<class Tpoint, class Tside>
class Trapeze : public Figures<Tpoint>{
private:
    Tside A, B, C;
public:
    Trapeze(const std::pair<Tpoint,Tside> &_point, const Tside &a, const Tside &b, const Tside &c): A(a), B(b), C(c){
        this->point.first = _point.first;
        this->point.second = _point.second;

        if (A < 0 or B < 0 or C < 0) {
            throw std::invalid_argument("Invalid trapeze parameters!");
        }
        if (B > A) {
            std::swap(A, B);
        }
    }

    void Print(std::ostream& out) override{
        out << *this << std::endl;
    }

    void Write(std::ofstream& file) override{
        file.write((const char*) &(TRAPEZE_TYPE_ID), sizeof(unsigned));
        file.write((const char*) &(this->point.first), sizeof (Tpoint));
        file.write((const char*) &(this->point.second), sizeof(Tpoint));
        file.write((const char*) &(this->A), sizeof(Tside));
        file.write((const char*) &(this->B), sizeof (Tside));
        file.write((const char*) &(this->C), sizeof (Tside));
    }

    template<class U>
    friend typename std::enable_if<is_double<U,U>::value, std::ostream &>::type
    operator<<(std::ostream &out, const Trapeze<U,U> &trapeze) {
        Tside diff = (trapeze.A - trapeze.B) / 2.0;
        double height = std::sqrt(fabs(trapeze.C * trapeze.C - diff * diff));
        out << "Trapeze vertices: [";
        out << std::pair<U,U> (trapeze.point.first, trapeze.point.second) << ", ";
        out << std::pair<U,U> (trapeze.point.first + diff, trapeze.point.second + height) << ", ";
        out << std::pair<U,U> (trapeze.point.first + trapeze.A - diff, trapeze.point.second + height) << ", ";
        out << std::pair<U,U> (trapeze.point.first + trapeze.A, trapeze.point.second);
        out << "]";
        return out;
    }

    template<class U>
    friend typename std::enable_if<is_int<U,U>::value, std::ostream &>::type
    operator<<(std::ostream &out, const Trapeze<U,U> &trapeze) {
        out << "Trapeze: [ I cant calculate coordinates, because the type is int ]" << std::endl;
        return out;
    }
};
#endif //OOP_EXERSICE_03_TRAPEZE_H
