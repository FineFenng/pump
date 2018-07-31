//
// Created by finefenng on 2018/7/5.
//

#ifndef QPSTEST_HANDLE_H
#define QPSTEST_HANDLE_H


class Handle
{
public:
    virtual void handleCallbackFunction(struct kevent* event) = 0;

    virtual void updateCallbackFunction() = 0;

    virtual void disableCallbackFunction() = 0;

    virtual void deleteCallbackFunction() =0;

    virtual int getIndex() = 0;

    virtual int setIndex(int index) = 0;

    virtual int getFd() = 0;

    virtual short getFilter() = 0;


};


#endif //QPSTEST_HANDLE_H
