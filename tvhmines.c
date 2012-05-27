#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MINE 88
#define ZERO 48
#define SIZE 10

void interpret_input(char*);
void print_board();
void place_mines(int);
void init(void);
void game_loop(void);
void clr_area(int);
void expose_mines(void);
void restart_game(void);
int check_win(void);

int board[SIZE][SIZE];
int game_state;
int draw_board = 1;
char input[10];
time_t start_time;

int main(int *argc, char *argv[]){
	init();
	game_loop();
}

void game_loop(){
	while(1){
		if (draw_board == 1){
			print_board();
		}
		gets(input);
		interpret_input(input);
		if (check_win() == 0){
			game_state = 2;
		}
	}
}

void init(){
	srand(time(NULL));
	restart_game();
}

void restart_game(){
	int i,j;

	game_state = 0;
	for (i=0; i<SIZE; i++){
		for (j=0; j<SIZE; j++){
			board[i][j] = 0;
		}
	}
	place_mines(10);
	start_time = time(NULL);
}

void place_mines(int number){
	int r, j, i, n, m;
	int count = number;

	while (count>0){
		r = rand() % (SIZE*SIZE);
		i = r/SIZE;
		j = r%SIZE;
		if (board[i][j] != -1){
			for (n=-1; n<2; n++){
				for (m=-1; m<2; m++){
					if (i+n >= 0 && i+n < SIZE && j+m >= 0 && j+m < SIZE){
						if (board[i+n][j+m] != -1){
							board[i+n][j+m]++;
						}
					}
				}
			}
			board[i][j] = -1;
			count--;
		}		
	}
}

int check_win(){
	int i, j;
	for (i=0; i<SIZE; i++){
		for (j=0; j<SIZE; j++){
			if (board[i][j] < 9 && board[i][j] != -1){
				return -1;
			}
		}
	}
	return 0;
}


void interpret_input(char* in){
	char delims[] = " ";
	char *command = NULL;
	char *target = NULL;
	int cmd = 0;
	int row, col;

	if (strlen(in) > 2){
		//there is a command, find it
		command = strtok(in, delims);
		if (strcmp(command, "hit") == 0){
			cmd = 0;
		}else if (strcmp(command, "flag") == 0){
			cmd = 1;
		}else if (strcmp(command, "exit") == 0){
			printf("\nUser exits the program.\n");
			exit(1);
		}else if (strcmp(command, "newgame") == 0){
			printf("\nStarts a new game.\n");
			restart_game();
			return;
		}else{
			printf("\nI did not understand that, please type it again..\n");
			draw_board = 0;
			return;
		}
		target = command + strlen(command) + 1;
	}else{
		target = in;
	}
	col = (int)(*target);
	if (col > 'J'){
		col -= 97;
	}else{
		col -= 65;
	}
	row = atoi(target+1);
	if (cmd == 0 & game_state == 0){
		if (board[row][col] >= 19){
			board[row][col] -= 10;
		}else{
			board[row][col] += 10;
		}
		if (board[row][col] == 10){
			clr_area(row*10 + col);
		}else if (board[row][col] == 9){
			expose_mines();
			game_state = 1;
		}
	}else if (cmd == 1){
		board[row][col] += 20;
	}
	draw_board = 1;
}

void expose_mines(){
	int i, j;

	for (i=0; i<SIZE; i++){
		for (j=0; j<SIZE; j++){
			if (board[i][j] == -1){
				board[i][j] += 10;
			}
		}
	}
}

void clr_area(int index){
	int i, j, n, m;

	i = index / SIZE;
	j = index % SIZE;
	if (board[i][j] == 10){
		for (n=-1; n<2; n++){
			for (m=-1; m<2; m++){
				if (i+n >= 0 && i+n < SIZE && j+m >= 0 && j+m < SIZE){
					if (board[i+n][j+m] < 10){
						board[i+n][j+m] += 10;
						clr_area((i+n)*SIZE + (j+m));
					}
				}
			}
		}
	}
}

void print_board(){
	int i,j;
	char c = 'A';

	system("clear");
	printf("   ");
	for (i=0; i<SIZE; i++){
		printf("%c ", c);
		c++;
	}
	printf("\n");
	for (i=0; i<SIZE; i++){
		printf("\n%d  ", i);
		for (j=0; j<SIZE; j++){
			if (board[i][j] == 9){
				printf("X ");
			}else if (board[i][j] >= 10 && board[i][j] < 19){
				if (board[i][j] == 10){
					printf("# ");
				}else{
					printf("%c ",  ZERO + board[i][j] - 10);
				}
			}else if (board[i][j] >= 19){
				printf("\033\[33mF \033[0m");
			}else{
				printf("  ");
			}
		}
	}
	printf("\n\n");
	if (game_state == 1){
		printf("GAME OVER.\n\n");
	}else if (game_state == 2){
		printf("YOU WIN!\nYour winning time is %d seconds.\n\n", (int)difftime(time(NULL), start_time));
	}
}
