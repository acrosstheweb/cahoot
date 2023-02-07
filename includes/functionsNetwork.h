typedef struct {
    int client_id;
    int sock_fd;
} Client_Args;

typedef struct {
	int max_clients;
	char ip[16]; // INET_ADDRSTRLEN
} Server_Args;

void* client_handler(void* arg);

void* startServer(void* arg);

char* getIp();