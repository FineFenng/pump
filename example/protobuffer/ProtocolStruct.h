//
// Created by fenng on 18-8-6.
//

#ifndef ECHOSERVER_PROTOCOL_H
#define ECHOSERVER_PROTOCOL_H

#include <cstdint>

typedef unsigned short DWORD;

typedef struct MSGHEAD
{
    DWORD msg_type;
    DWORD msg_len;
} MSGHEAD;

typedef struct PROTOMSG
{
    int32_t len;
    int8_t  type_name;
    char    name;

} PROTOMSG;




#endif //ECHOSERVER_PROTOCOL_H
