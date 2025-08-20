#include <ncurses.h>
#include <stdbool.h>

#define COLOR_RESET   "\x1b[0m"
#define COLOR_OFFSET  "\033[92m"

#define WIDTH 10
#define HEIGHT 20

int playground[HEIGHT][WIDTH] = {0};

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

//initialize ncurses
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
            mvprintw(y, x * 2, " . ");
        }
    }

    for(int y=0;y<HEIGHT;y++){
        for(int x=0;x<WIDTH;++x){
            if(playground[y][x]){
                mvprintw(y, x * 2, "[]");
            }
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
void update_tetromino(tetro *t){

    bool collision = false;

    for(int y=0;y<4;++y){
        for(int x=0;x<4;++x){
            if(t->shape[y][x]){
                int nexty = t->y + y + 1;
                int nextx = t->x + x;
                if(nexty >= HEIGHT || playground[nexty][nextx]) collision = true;
            }
        }
    }

    if(collision){
        ground_lock(t);
        clear_full_rows();
        spawn_another(t);
    } else {
        t->y += 1;
    }
    
}

// new tetromino after the last one landed
void spawn_another(tetro *t){
    *t = (tetro){
        .shape = {
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    .x = 3,
    .y = 0
    }; 
}

// keyboard controls
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

// locks the tetromino once it collides
void ground_lock(tetro *t){
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if(t->shape[y][x]){
                int locky = t->y + y;
                int lockx = t->x + x;

                if(locky >= 0 && locky < HEIGHT && lockx >=0 && lockx < WIDTH)
                    playground[locky][lockx] = 1;
            }
        }
    }
}

// clears the grid rows if they're full
void clear_full_rows(){
    for(int y=HEIGHT-1;y>=0;--y){
        bool full = true;

        for(int x=0;x<WIDTH;++x){
            if(playground[y][x] == 0){
                full = false;
                break;
            }
        }

        if(full){
            for(int row = y; row > 0; --row){
                for(int col = 0; col < WIDTH; ++col){
                    playground[row][col] = playground[row -1][col]; 
                }
            }
            
            for(int col = 0; col < WIDTH; ++col){
                playground[0][col] = 0;
            }
            
            y++;
        }

        
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