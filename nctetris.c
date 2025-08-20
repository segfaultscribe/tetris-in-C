#include <ncurses.h>

#define WIDTH 10
#define HEIGHT 20


typedef struct {
    int shape[4][4];
    int x, y;
} tetro;

tetro current_piece = {
    .shape = {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    .x = 3,
    .y = 0
};


void init() {
    initscr();  
    noecho();
    cbreak();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);           
}

// The environment 
void draw_playground() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            mvprintw(y, x * 2, ".");
        }
    }
    refresh();
}

//draws the current tetromino into the playground
void draw_tetromino(tetro *t){
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
                if(t->shape[y][x]){
                int ycord = t->y + y;
                int xcord = (t->x + x) * 2;
                mvprintw(ycord, xcord, "[]");
            }
        }
    }
}

// falling update for tetromino
void update_tetromino(tetro *current){

    if(current->y > HEIGHT - 3){
        return;
    }
    current->y += 1;
}

void handle_input(tetro *t, int ch){
    if(t->y >= HEIGHT-3){
        return;
    }
    
    switch(ch){
        case KEY_LEFT:
            if(t->x > 0) t->x -= 1;
        break;

        case KEY_RIGHT:
            if(t->x < WIDTH) t->x += 1;
        break;

        case KEY_DOWN:
            if(t->y < HEIGHT-3) t->y += 1;
        break;
    }
}


int main(){

    init();
    
    while(1){
        clear();

        draw_playground();
        draw_tetromino(&current_piece);
        refresh();


        update_tetromino(&current_piece);

        int ch = getch();
        if(ch == 'q') break;

        handle_input(&current_piece, ch);

         usleep(300000);   
    }

    endwin();
    return 0;
}