#ifndef _NETWORK
#define _NETWORK

#include "struct.h"

void* client_handler(void* arg);

void* startServer(void* arg);

char* getIp();

char* serializeQuestionData(QuestionData* questionData);

QuestionData* deserializeQuestionData(char* serializedData);

#endif