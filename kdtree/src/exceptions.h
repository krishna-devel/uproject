//
// Created by Krishna Kamath on 11/22/16.
//

#ifndef KDTREE_EXCEPTIONS_H
#define KDTREE_EXCEPTIONS_H

#include <exception>
#include <string>

using namespace std;

class EmptyVectorException : public exception {
public:
    EmptyVectorException(const string &message) : message(message) {}
    virtual const char* what() const throw() { return message.c_str(); }
private:
    string message;
};

#endif //KDTREE_EXCEPTIONS_H
