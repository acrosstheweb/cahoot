#include "struct.h"

char* getPacketPath(char* packetName);

FILE* createPacketFile(char* packetName);

void addQuestionToFile(char* packetName, char* question, char* answer1, char* answer2, char* answer3, char* answer4);

char* deletePacket(char* packetName);

void readPacket(char* packetName, QuestionData*** questionData, int** questionsNb);

char* modifyPacket(char* packetName, QuestionData* questionData, int questionsNb);

void deleteQuestion(QuestionData** questionData, int* questionsNb, int questionToDelete);

char** listPackets(int* packetNb);