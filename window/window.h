#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    SDL_Window* sdl_window;
    SDL_Renderer* renderer;
} Window;

typedef struct {
    int isHovered;
    SDL_Rect rect;
    SDL_Texture* texture;
    SDL_Texture* textureHovered;
} Button;

typedef struct Node Node;
struct Node{
    Button button;
    Node* next;
};

typedef struct {
    Node *first;
    int size;
} List;

void menu(Window* window);

void checkHover(Node* first, int mouseX, int mouseY);

void addButtonToList(Node** head, SDL_Rect rect, SDL_Texture* texture, SDL_Texture* textureHovered);

void print(Node* first);