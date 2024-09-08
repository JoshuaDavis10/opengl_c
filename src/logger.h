#pragma once

//honestly for now ima have 3 functions cuz lets not make it too complicated
//one function that is a catch all and then 3 macros to call specific versions
//WARN, INFO, ERROR

typedef enum message_type {
	LINFO = 0,  
	LWARN = 1,
	LERROR = 2
} message_type;

void log_message(message_type msgType, const char* msg); 

#define LOGINFO(msg)  log_message(LINFO, msg)
#define LOGWARN(msg)  log_message(LWARN, msg)
#define LOGERROR(msg) log_message(LERROR, msg)
