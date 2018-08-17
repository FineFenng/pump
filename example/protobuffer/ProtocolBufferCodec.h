//
// Created by fenng on 18-8-6.
//

#ifndef ECHOSERVER_PROTOCOLBUFFERCODEC_H
#define ECHOSERVER_PROTOCOLBUFFERCODEC_H


#include <cstddef>
#include <cstdint>
#include <string>

#include <google/protobuf/message.h>

#include "../include/pump/Buffer.h"


class ProtocolBufferCodec
{
public:
    typedef std::shared_ptr<google::protobuf::Message> Message_Ptr;
public:
    void on_message(Buffer* buffer);

    void send(Buffer* buffer);

    void fillEmptyBuffer(const google::protobuf::Message &message, Buffer *buffer);

private:

    /* must parse a complete message. */
    google::protobuf::Message* create_message(const std::string& type_name);

    Message_Ptr parse_message(const char* data, int len);

private:
    static const size_t kHeaderLen_     = sizeof(int32_t);
    static const size_t kMinMessageLen_ = kHeaderLen_;
    static const size_t kMaxMessageLen_ = 64 * 1024 * 1024;

};


#endif //ECHOSERVER_PROTOCOLBUFFERCODEC_H
