#include <ncurses.h>
#include <stdbool.h>
#include <ncursesw/ncurses.h>
#include <locale.h>


// control right side collisions
// something wrong with the board


#define WIDTH 10
#define HEIGHT 20

#define GRID_WIDTH  (WIDTH * 2 + 1)     // Each cell is "..", plus newline
#define GRID_SIZE   (HEIGHT * GRID_WIDTH)

char static_grid[GRID_SIZE + 1];

int playground[HEIGHT][WIDTH] = {0};

void init_static_grid() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            static_grid[i * GRID_WIDTH + j * 2]     = '..';
            static_grid[i * GRID_WIDTH + j * 2 + 1] = '..';
        }
        static_grid[i * GRID_WIDTH + WIDTH * 2] = '\n';
    }
    static_grid[GRID_SIZE] = '\0';
}


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
    setlocale(LC_ALL, "");
    initscr();  
    noecho();
    cbreak();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);           
}

// The environment 
void draw_playground() {

    mvaddstr(0, 0, static_grid);

    for(int y=0;y<HEIGHT;y++){
        for(int x=0;x<WIDTH;++x){
            if(playground[y][x]){
                mvaddwstr(y, x * 2, L"██");
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
                mvaddwstr(ycord, xcord, L"██");
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
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    .x = 3,
    .y = 0
    }; 
}

bool can_move_right(tetro *t) {
    for (int y = 0; y < 4; y++) {
        for (int x = 3; x >= 0; x--) {
            if (t->shape[y][x]) {
                int new_x = t->x + x + 1;
                if (new_x >= WIDTH || playground[t->y + y][new_x]) {
                    return false;
                }
            }
        }
    }
    return true;
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
            if(can_move_right(t)) t->x += 1;
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
    init_static_grid();

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