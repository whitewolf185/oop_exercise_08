#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include "Figures.h"

const unsigned int RECTANGLE_TYPE_ID = 2;

template<class T1, class T2>
class Rectangle : public Figures<T1> {
private:
    T2 A, B;
public:
    Rectangle(std::pair<T1,T2> _point, const T2 &a, const T2 &b): A(a), B(b){// x and y are left-bottom point of figure
        this->point.first = _point.first;
        this->point.second = _point.second;

        if (A < 0 or B < 0) {
            throw std::invalid_argument("Invalid rectangle parameters!");
        }
    }


    void Print(std::ostream& out) override {
        out << *this << std::endl;
    }

    void Write(std::ofstream& file) override{
        file.write((const char*) &(RECTANGLE_TYPE_ID), sizeof(unsigned));
        file.write((const char*) &(this->point.first), sizeof (T1));
        file.write((const char*) &(this->point.second), sizeof(T1));
        file.write((const char*) &(this->A), sizeof(T2));
        file.write((const char*) &(this->B), sizeof (T2));
    }

    template<class U>
    friend std::ostream & operator << (std::ostream & out, const Rectangle<U,U> & rectangle){
        out << "Rectangle vertices: [";
        out << std::pair<U,U> (rectangle.point.first, rectangle.point.second) << ", ";
        out << std::pair<U,U> (rectangle.point.first, rectangle.point.second + rectangle.B) << ", ";
        out << std::pair<U,U> (rectangle.point.first + rectangle.A, rectangle.point.second + rectangle.B) << ", ";
        out << std::pair<U,U> (rectangle.point.first + rectangle.A, rectangle.point.second);
        out << "]";
        return out;
    }
};

#endif /* RECTANGLE_HPP */
