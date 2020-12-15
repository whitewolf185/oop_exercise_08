#ifndef OOP_EXERSICE_03_FIGURES_H
#define OOP_EXERSICE_03_FIGURES_H
#include <fstream>
#include <iostream>
#include <cmath>

template<class T1,class T2>
struct is_int : std::false_type {};
template<>
struct is_int<int,int>:std::true_type {};

template<class T1,class T2>
struct is_double : std::false_type {};
template<>
struct is_double<double,double>:std::true_type {};

template<class T>
class Figures {
protected:
    using coord = std::pair<T,T>;
    coord point;

public:
    virtual void Print(std::ostream& out) = 0;
    virtual void Write(std::ofstream& file){};
    virtual ~Figures() = default;
};

template<class T1, class T2>
inline std::ostream & operator<<(std::ostream& out, const std::pair<T1, T2>& pair){
    out << "(" << pair.first << ", " << pair.second << ")";
    return out;
}

#endif //OOP_EXERSICE_03_FIGURES_H