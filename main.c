/* 
 * File:   main.c
 * Author: zhongyu
 * ID: 14216042
 *pawprint:zlqtd
 * Created on 2015年6月19日, 下午5:13
 */

#include <stdio.h>
#include <stdlib.h> //for malloc and rand
#include <stdbool.h>
#include <time.h>// to seed rand() function



#define BOARD_SIZE_ROW 10
#define BOARD_SIZE_COLUMN 10

const char WATER = '~';
const char HIT = '*';
const char MISS = 'x';
const char MYSTERY = '?';

const char CARRIER = 'c';
const char BATTLESHIP = 'b';
const char CRUISER = 'r';
const char SUBMARINE = 's';
const char DESTROYER = 'd';

typedef struct {
    int row;
    int column;
} Coordinates;

typedef enum {
    USER, COMPUTER, OPPONENT
} Player;

typedef enum {
    NORTH, SOUTH, EAST, WEST
} Direction;

typedef enum {
    CARRIER_LENGTH = 5,
    BATTLESHIP_LENGTH = 4,
    CRUISER_LENGTH = 3,
    SUBMARINE_LENGTH = 3,
    DESTROYER_LENGTH = 2
} ShipLength;


void init_game_board(char gameBoard[][BOARD_SIZE_COLUMN], const char initValue);
void place_ships_randomly(char gameBoard[][BOARD_SIZE_COLUMN]);
void place_ships_manually(char gameBoard[][BOARD_SIZE_COLUMN]);
bool check_ship_placement(char gameBoard[][BOARD_SIZE_COLUMN], Coordinates startingPoint, Direction dir, ShipLength len);
void place_ship(char gameBoard[][BOARD_SIZE_COLUMN], Coordinates startingPoint, Direction dir, const char shipType, ShipLength len);
void print_game_board(char gameBoard[][BOARD_SIZE_COLUMN], Player player);
void acquire_strike_coordinates(char gameBoard[][BOARD_SIZE_COLUMN], Coordinates* strikeAttempt);
bool check_strike_coords(int row, int col);
bool fire_missile(char gameBoard[][BOARD_SIZE_COLUMN], Coordinates coord);
void update_game_board(char gameBoard[][BOARD_SIZE_COLUMN], Coordinates coord, const char value);

int main(int argc, char** argv) {
    char userGameBoard[BOARD_SIZE_ROW][BOARD_SIZE_COLUMN];
    char computerGameBoard[BOARD_SIZE_ROW][BOARD_SIZE_COLUMN];
    char userGameState[BOARD_SIZE_ROW][BOARD_SIZE_COLUMN];

    int userHealth = 17;
    int computerHealth = 17;

    Coordinates strikeAttempt;
    bool didHit;
    srand(time(NULL));

    init_game_board(userGameBoard, WATER);
    init_game_board(computerGameBoard, WATER);
    init_game_board(userGameState, MYSTERY);

    //print_game_board(userGameBoard, USER);
    //print_game_board(computerGameBoard, COMPUTER);
    //print_game_board(userGameState, OPPONENT);

    place_ships_randomly(computerGameBoard);
    //print_game_board(computerGameBoard, COMPUTER);
    place_ships_manually(userGameBoard);
    print_game_board(userGameBoard, USER);

    print_game_board(userGameState, OPPONENT);
    do {
        print_game_board(userGameBoard, USER);
        print_game_board(userGameState, OPPONENT);

        acquire_strike_coordinates(userGameBoard, &strikeAttempt);


        printf("Strike coordinates are: %d, %d\n", strikeAttempt.row, strikeAttempt.column);

        didHit = fire_missile(computerGameBoard, strikeAttempt);
        
        if (didHit == true) {
            computerHealth -= 1;
            update_game_board(computerGameBoard, strikeAttempt, HIT);
           // update_game_board(userGameBoard, strikeAttempt, HIT);
            update_game_board(userGameState, strikeAttempt, HIT);
        } else {
            update_game_board(computerGameBoard, strikeAttempt, MISS);
            //update_game_board(userGameBoard, strikeAttempt, MISS);
            update_game_board(userGameState, strikeAttempt, MISS);
        }

        strikeAttempt.row = rand() % 10;
        strikeAttempt.column = rand() % 10;
        didHit = fire_missile(userGameBoard, strikeAttempt);
        if (didHit == true) {
            userHealth -= 1;
           // update_game_board(computerGameBoard, strikeAttempt, HIT);
            update_game_board(userGameBoard, strikeAttempt, HIT);
            //update_game_board(userGameState, strikeAttempt, HIT);
        } else {
           // update_game_board(computerGameBoard, strikeAttempt, MISS);
            update_game_board(userGameBoard, strikeAttempt, MISS);
            //update_game_board(userGameState, strikeAttempt, MISS);
        }
        
        printf("User health:%d\nComputer health:%d\n", userHealth, computerHealth);
    } while (userHealth > 0 && computerHealth > 0);



    if (computerHealth == 0) {
        printf("\n~~~Congratulations! You won!~~~\n");
        print_game_board(userGameBoard, USER);
        print_game_board(computerGameBoard, COMPUTER);
    } else {
        printf("\n~~~Sorry! You lost!~~~\n");
        print_game_board(userGameBoard, USER);
        print_game_board(computerGameBoard, COMPUTER);
    }

    return (EXIT_SUCCESS);
}

void init_game_board(char gameBoard[][BOARD_SIZE_COLUMN], const char initValue) {
    int i, j;
    for (i = 0; i < BOARD_SIZE_ROW; i++) {
        for (j = 0; j < BOARD_SIZE_COLUMN; j++) {
            gameBoard[i][j] = initValue;
        }

    }
}

void update_game_board(char gameBoard[][BOARD_SIZE_COLUMN], Coordinates coord, const char value) {
    gameBoard[coord.row][coord.column] = value;

}

bool fire_missile(char gameBoard[][BOARD_SIZE_COLUMN], Coordinates coord) {

    char strikeZone = gameBoard[coord.row][coord.column];
    if (strikeZone != WATER && strikeZone != MISS && strikeZone != HIT) {
        return true;
    }
    return false;
}

void print_game_board(char gameBoard[][BOARD_SIZE_COLUMN], Player player) {
    int i, j;

    char* whichPlayer;
    if (player == USER) {
        whichPlayer = "Player";
    } else if (player == COMPUTER) {
        whichPlayer = "Computer";
    } else {
        whichPlayer = "Opponent";
    }
    printf("~~~ %s\'s game board ~~~\n\n", whichPlayer);
    printf("%2c", ' ');
    for (i = 0; i < BOARD_SIZE_ROW; i++) {
        printf("%2d", i);
    }
    printf("\n");

    for (i = 0; i < BOARD_SIZE_ROW; i++) {
        printf("%2d", i);
        for (j = 0; j < BOARD_SIZE_COLUMN; j++) {

            printf("%2c", gameBoard[i][j]);
        }
        printf("\n");
    }
}

void acquire_strike_coordinates(char gameBoard[][BOARD_SIZE_COLUMN], Coordinates* strikeAttempt) {
    bool coordsAreValid = false;

    do {
        printf("Enter row you want to strike(0-9):");
        scanf("%d", &strikeAttempt->row);
        printf("Enter col you want to strike(0-9):");
        scanf("%d", &strikeAttempt->column);

        coordsAreValid = check_strike_coords(strikeAttempt->row, strikeAttempt->column);

        if (coordsAreValid == false) {
            printf("Sorry, those coordinates are invalid. Please try again.\n\n");
            print_game_board(gameBoard, USER);
        }
    } while (coordsAreValid == false);

}

bool check_strike_coords(int row, int col) {

    if (row > BOARD_SIZE_ROW || row < 0) {
        return false;
        if (col > BOARD_SIZE_COLUMN || col < 0) {
            return false;
        }
    }
    return true;
}

void place_ships_randomly(char gameBoard[][BOARD_SIZE_COLUMN]) {

    Coordinates startingPoint;
    Direction dir;
    bool check;

    do {
        dir = (Direction) rand() % 4;
        startingPoint.row = rand() % 10;
        startingPoint.column = rand() % 10;
        check = check_ship_placement(gameBoard, startingPoint, dir, BATTLESHIP_LENGTH);
    } while (check == false);

    place_ship(gameBoard, startingPoint, dir, BATTLESHIP, BATTLESHIP_LENGTH);

    do {
        dir = (Direction) rand() % 4;
        startingPoint.row = rand() % 10;
        startingPoint.column = rand() % 10;
        check = check_ship_placement(gameBoard, startingPoint, dir, CARRIER_LENGTH);
    } while (check == false);

    place_ship(gameBoard, startingPoint, dir, CARRIER, CARRIER_LENGTH);


    do {
        dir = (Direction) rand() % 4;
        startingPoint.row = rand() % 10;
        startingPoint.column = rand() % 10;
        check = check_ship_placement(gameBoard, startingPoint, dir, CRUISER_LENGTH);
    } while (check == false);

    place_ship(gameBoard, startingPoint, dir, CRUISER, CRUISER_LENGTH);

    do {
        dir = (Direction) rand() % 4;
        startingPoint.row = rand() % 10;
        startingPoint.column = rand() % 10;
        check = check_ship_placement(gameBoard, startingPoint, dir, SUBMARINE_LENGTH);
    } while (check == false);

    place_ship(gameBoard, startingPoint, dir, SUBMARINE, SUBMARINE_LENGTH);

    do {
        dir = (Direction) rand() % 4;
        startingPoint.row = rand() % 10;
        startingPoint.column = rand() % 10;
        check = check_ship_placement(gameBoard, startingPoint, dir, DESTROYER_LENGTH);
    } while (check == false);

    place_ship(gameBoard, startingPoint, dir, DESTROYER, DESTROYER_LENGTH);

}

bool check_ship_placement(char gameBoard[][BOARD_SIZE_COLUMN], Coordinates startingPoint, Direction dir, ShipLength len) {
    int i;
    int length = len;

    if (dir == 3) {
        if (gameBoard[startingPoint.row][startingPoint.column] != WATER) {
            return false;
        }
        for (i = 0; i < length; i++) {
            if (startingPoint.column - i < 0 || gameBoard[startingPoint.row][startingPoint.column - i] != WATER) {
                return false;
            }

        }
        return true;

    } else if (dir == 2) {

        if (gameBoard[startingPoint.row][startingPoint.column] != WATER) {
            return false;
        }
        for (i = 0; i < length; i++) {
            if (startingPoint.column + i > 9 || gameBoard[startingPoint.row][startingPoint.column + i] != WATER) {
                return false;
            }

        }
        return true;

    } else if (dir == 0) {

        if (gameBoard[startingPoint.row][startingPoint.column] != WATER) {
            return false;
        }


        for (i = 0; i < length; i++) {

            if (startingPoint.row - i < 0 || gameBoard[startingPoint.row - i][startingPoint.column] != WATER) {
                return false;
            }

        }
        return true;


    } else if (dir == 1) {

        if (gameBoard[startingPoint.row][startingPoint.column] != WATER
                ) {
            return false;
        }


        for (i = 0; i < length; i++) {

            if (startingPoint.row + i > 9 || gameBoard[startingPoint.row + i][startingPoint.column] != WATER) {
                return false;
            }

        }
        return true;

    }

}

void place_ship(char gameBoard[][BOARD_SIZE_COLUMN], Coordinates startingPoint, Direction dir, const char shipType, ShipLength len) {
    int i;
    if (dir == 0) {
        for (i = 0; i < len; i++) {
            update_game_board(gameBoard, startingPoint, shipType);
            startingPoint.row -= 1;
        }
    } else if (dir == 1) {
        for (i = 0; i < len; i++) {
            update_game_board(gameBoard, startingPoint, shipType);
            startingPoint.row += 1;
        }
    } else if (dir == 2) {
        for (i = 0; i < len; i++) {
            update_game_board(gameBoard, startingPoint, shipType);
            startingPoint.column += 1;
        }
    } else if (dir == 3) {
        for (i = 0; i < len; i++) {
            update_game_board(gameBoard, startingPoint, shipType);
            startingPoint.column -= 1;
        }
    }
}

void place_ships_manually(char gameBoard[][BOARD_SIZE_COLUMN]) {
    int tempDir;
    Direction dir;
    Coordinates startingPoint;

    printf("\n\n~~~Placing ships!~~~\n\n");


    print_game_board(gameBoard, USER);
    printf("Placing your battleship: \n");
    printf("Please enter starting point row (0 - %d): ", BOARD_SIZE_ROW - 1);
    scanf("%d", &startingPoint.row);
    printf("Please enter starting point column (0 - %d): ", BOARD_SIZE_COLUMN - 1);
    scanf("%d", &startingPoint.column);
    printf("Directions: North(0), South(1), East(2), West(3)\n");
    printf("Please enter a direction for the ship to \"face\": ");
    scanf("%d", &tempDir);
    dir = (Direction) tempDir;

    while (check_ship_placement(gameBoard, startingPoint, dir, BATTLESHIP_LENGTH) == false) {
        printf("Something was wrong with the starting coordinates and direction you entered. Please try again.\n");

        print_game_board(gameBoard, USER);
        printf("Placing your battleship: \n");

        printf("Please enter starting point row (0 - %d): ", BOARD_SIZE_ROW - 1);
        scanf("%d", &startingPoint.row);
        printf("Please enter starting point column (0 - %d): ", BOARD_SIZE_COLUMN - 1);
        scanf("%d", &startingPoint.column);

        printf("Directions: North(0), South(1), East(2), West(3)\n");
        printf("Please enter a direction for the ship to \"face\": ");
        scanf("%d", &tempDir);
        dir = (Direction) tempDir;
    }
    place_ship(gameBoard, startingPoint, dir, BATTLESHIP, BATTLESHIP_LENGTH);
    print_game_board(gameBoard, USER);


    printf("Placing your carrier: \n");
    printf("Please enter starting point row (0 - %d): ", BOARD_SIZE_ROW - 1);
    scanf("%d", &startingPoint.row);
    printf("Please enter starting point column (0 - %d): ", BOARD_SIZE_COLUMN - 1);
    scanf("%d", &startingPoint.column);
    printf("Directions: North(0), South(1), East(2), West(3)\n");
    printf("Please enter a direction for the ship to \"face\": ");
    scanf("%d", &tempDir);
    dir = (Direction) tempDir;

    while (check_ship_placement(gameBoard, startingPoint, dir, CARRIER_LENGTH) == false) {
        printf("Something was wrong with the starting coordinates and direction you entered. Please try again.\n");
        print_game_board(gameBoard, USER);
        printf("Placing your carrier: \n");

        printf("Please enter starting point row (0 - %d): ", BOARD_SIZE_ROW - 1);
        scanf("%d", &startingPoint.row);
        printf("Please enter starting point column (0 - %d): ", BOARD_SIZE_COLUMN - 1);
        scanf("%d", &startingPoint.column);

        printf("Directions: North(0), South(1), East(2), West(3)\n");
        printf("Please enter a direction for the ship to \"face\": ");
        scanf("%d", &tempDir);
        dir = (Direction) tempDir;
    }
    place_ship(gameBoard, startingPoint, dir, CARRIER, CARRIER_LENGTH);
    print_game_board(gameBoard, USER);




    printf("Placing your cruiser: \n");
    printf("Please enter starting point row (0 - %d): ", BOARD_SIZE_ROW - 1);
    scanf("%d", &startingPoint.row);
    printf("Please enter starting point column (0 - %d): ", BOARD_SIZE_COLUMN - 1);
    scanf("%d", &startingPoint.column);
    printf("Directions: North(0), South(1), East(2), West(3)\n");
    printf("Please enter a direction for the ship to \"face\": ");
    scanf("%d", &tempDir);
    dir = (Direction) tempDir;
    while (check_ship_placement(gameBoard, startingPoint, dir, CRUISER_LENGTH) == false) {
        printf("Something was wrong with the starting coordinates and direction you entered. Please try again.\n");

        print_game_board(gameBoard, USER);
        printf("Placing your cruiser: \n");

        printf("Please enter starting point row (0 - %d): ", BOARD_SIZE_ROW - 1);
        scanf("%d", &startingPoint.row);
        printf("Please enter starting point column (0 - %d): ", BOARD_SIZE_COLUMN - 1);
        scanf("%d", &startingPoint.column);

        printf("Directions: North(0), South(1), East(2), West(3)\n");
        printf("Please enter a direction for the ship to \"face\": ");
        scanf("%d", &tempDir);
        dir = (Direction) tempDir;
    }
    place_ship(gameBoard, startingPoint, dir, CRUISER, CRUISER_LENGTH);
    print_game_board(gameBoard, USER);


    printf("Placing your submarine: \n");
    printf("Please enter starting point row (0 - %d): ", BOARD_SIZE_ROW - 1);
    scanf("%d", &startingPoint.row);
    printf("Please enter starting point column (0 - %d): ", BOARD_SIZE_COLUMN - 1);
    scanf("%d", &startingPoint.column);
    printf("Directions: North(0), South(1), East(2), West(3)\n");
    printf("Please enter a direction for the ship to \"face\": ");
    scanf("%d", &tempDir);
    dir = (Direction) tempDir;


    while (check_ship_placement(gameBoard, startingPoint, dir, SUBMARINE_LENGTH) == false) {
        printf("Something was wrong with the starting coordinates and direction you entered. Please try again.\n");

        print_game_board(gameBoard, USER);
        printf("Placing your submarine: \n");

        printf("Please enter starting point row (0 - %d): ", BOARD_SIZE_ROW - 1);
        scanf("%d", &startingPoint.row);
        printf("Please enter starting point column (0 - %d): ", BOARD_SIZE_COLUMN - 1);
        scanf("%d", &startingPoint.column);

        printf("Directions: North(0), South(1), East(2), West(3)\n");
        printf("Please enter a direction for the ship to \"face\": ");
        scanf("%d", &tempDir);
        dir = (Direction) tempDir;
    }
    place_ship(gameBoard, startingPoint, dir, SUBMARINE, SUBMARINE_LENGTH);
    print_game_board(gameBoard, USER);



    printf("Placing your DESTROYER : \n");
    printf("Please enter starting point row (0 - %d): ", BOARD_SIZE_ROW - 1);
    scanf("%d", &startingPoint.row);
    printf("Please enter starting point column (0 - %d): ", BOARD_SIZE_COLUMN - 1);
    scanf("%d", &startingPoint.column);
    printf("Directions: North(0), South(1), East(2), West(3)\n");
    printf("Please enter a direction for the ship to \"face\": ");
    scanf("%d", &tempDir);
    dir = (Direction) tempDir;
    while (check_ship_placement(gameBoard, startingPoint, dir, DESTROYER_LENGTH) == false) {
        printf("Something was wrong with the starting coordinates and direction you entered. Please try again.\n");
        print_game_board(gameBoard, USER);
        printf("Placing your cruiser: \n");
        printf("Please enter starting point row (0 - %d): ", BOARD_SIZE_ROW - 1);
        scanf("%d", &startingPoint.row);
        printf("Please enter starting point column (0 - %d): ", BOARD_SIZE_COLUMN - 1);
        scanf("%d", &startingPoint.column);
        printf("Directions: North(0), South(1), East(2), West(3)\n");
        printf("Please enter a direction for the ship to \"face\": ");
        scanf("%d", &tempDir);
        dir = (Direction) tempDir;
    }
    place_ship(gameBoard, startingPoint, dir, DESTROYER, DESTROYER_LENGTH);
    print_game_board(gameBoard, USER);
}
