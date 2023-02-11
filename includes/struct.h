//#warning struct
#ifndef _STRUCT
#define _STRUCT

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <netinet/in.h> // sockaddr_in

#define MAX_LEN 15

typedef struct {
    SDL_Window* sdl_window;
    SDL_Renderer* renderer;
    TTF_Font* font;
} Window;

typedef struct {
    SDL_Texture* normal;
    SDL_Texture* hover;
} States;

typedef struct {
  SDL_Rect refRect;
  SDL_Rect inputRect;
  char *text;
} TextInput;

typedef struct {
    int isHovered;
    int isOption;
    int returnValue;
    SDL_Rect iconRect;
    States* icon;
    SDL_Rect textRect;
    States* text;
} Button;

typedef struct Node Node;
struct Node{
    Button button;
    Node* next;
};

typedef struct {
    char* question;
    char** answers;
} QuestionData;

typedef struct {
    char* maxConnections;
    char* packetPath;
    char* ip_base;
} Conf;

typedef struct {
    char thread_ip_addr[INET_ADDRSTRLEN];
    int thread_sock_fd;
} Ip_Args;

typedef struct {
    int client_id;
    int sock_fd;
    int nb_questions;
    int good_answers;
    QuestionData** game_packet; // packet
} Client_Args;

typedef struct {
	int max_clients;
	char ip[16]; // INET_ADDRSTRLEN
    int nb_questions;
    QuestionData** game_packet; // packet
} Server_Args;

#endif
