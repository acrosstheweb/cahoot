char* getPacketPath(char* packetName);

FILE* createPacketFile(char* packetName);

void addQuestionToFile(char* packetName, char* question, char* answer1, char* answer2, char* answer3, char* answer4);

char* deletePacket(char* packetName);

void readPacket(QuestionData* questionData);

char* modifyPacket(char* packetName, QuestionData* questionData);

char** listPackets(int* packetNb);