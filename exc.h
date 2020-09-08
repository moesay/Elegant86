#ifndef EXC_C
#define EXC_H
#include <exception>

class InvalidPointer : public std::exception {
    public:
        using std::exception::what;
        const char* what() const throw();
};
class InvalidSegmentOverridePrefix : public std::exception {
    public:
        using std::exception::what;
        const char* what() const throw();
};
#endif
