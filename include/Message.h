#ifndef MESSAGE_H
#define MESSAGE_H

#include <functional>
#include <memory>

class Message {

public:
    int what;
    int arg1{0}, arg2{0};
    std::string content {};
public:

    Message(int _what) : what(_what) {}
    Message(int _what, int _arg1) : what(_what), arg1(_arg1)  {}
    Message(int _what, int _arg1, int _arg2) : what(_what), arg1(_arg1), arg2(_arg2)  {}
};

#endif // MESSAGE_H
