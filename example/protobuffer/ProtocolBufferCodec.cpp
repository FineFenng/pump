//
// Created by fenng on 18-8-6.
//

#include "ProtocolBufferCodec.h"

#include "../include/pump/Common.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <string>


#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

void ProtocolBufferCodec::send(Buffer *buffer)
{

}


void ProtocolBufferCodec::fillEmptyBuffer(const google::protobuf::Message &message, Buffer *buffer)
{
    const std::string& type_name = message.GetTypeName();
    int32_t name_len = static_cast<int32_t>(type_name.size()) + 1;

    int byte_size = message.ByteSize();
    buffer->ensure_capacity(byte_size);

    uint8_t* start = reinterpret_cast<uint8_t*>(buffer->get_writable_address());
    uint8_t* end = message.SerializeWithCachedSizesToArray(start);

    int32_t len =  name_len + static_cast<int32_t >(type_name.size()) + 1 + byte_size;


    buffer->appendString(reinterpret_cast<char *>(len), sizeof(int32_t));
    buffer->appendString(type_name.c_str(), name_len);
    buffer->appendString(type_name);
}

void ProtocolBufferCodec::on_message(Buffer* buffer)
{
    while (buffer->get_readable_bytes() >= kMinMessageLen_ + kHeaderLen_)
    {
        int32_t message_len = 0;
        memcpy(&message_len, buffer->get_readable_address(), kHeaderLen_);

        if (message_len > kMaxMessageLen_ || message_len < kMinMessageLen_)
        {

        }
        else if(buffer->get_readable_bytes() >= message_len + kHeaderLen_)
        {
            Message_Ptr message_ptr = parse_message(&*buffer->get_readable_begin() + kHeaderLen_, message_len);
            buffer->retrieve(kHeaderLen_ + message_len);
        }

    }
}




google::protobuf::Message* ProtocolBufferCodec::create_message(const std::string &type_name)
{
    google::protobuf::Message* message = nullptr;
    const google::protobuf::Descriptor* descriptor =
            google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);

    if (descriptor)
    {
        const google::protobuf::Message* proto_type =
                google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);

        if (proto_type)
        {
            message = proto_type->New();
        }

    }
    return message;
}

 ProtocolBufferCodec::Message_Ptr ProtocolBufferCodec::parse_message(const char* data, int len)
{
    int8_t name_len = 0;
    memcpy(&len, data + 4, 1);

    std::string type_name(data + 4 + 1, data + 4 + 1 + name_len - 1);
    Message_Ptr message_ptr(create_message(type_name));

    if (message_ptr)
    {
        const char* message_data =  data + 4 + 1 + name_len;
        int32_t message_len = static_cast<int32_t >(len - name_len - 4 + 1);
        message_ptr->ParseFromArray(message_data, message_len);

    }

}


