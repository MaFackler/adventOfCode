#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define MAX_NUM_CARDS 256
#define BOARD_DIM 5
#define NUM_BOARDS 256


typedef struct {
    int data[BOARD_DIM * BOARD_DIM];
    bool checked[BOARD_DIM * BOARD_DIM];
} Board;

typedef struct {
    size_t draw_index;
    int cards[MAX_NUM_CARDS];
    size_t num_cards;
    size_t num_boads; 
    Board boards[NUM_BOARDS];
} Bingo;


void bingo_parse_cards(Bingo *bingo, FILE *fp) {
    bingo->num_cards = 0;
    int card = -1;
    while (fscanf(fp, "%d", &card) != 0) {
        bingo->cards[bingo->num_cards++] = card;
        char c = fgetc(fp);
        assert(c == ',' || c == '\n');
        if (c == '\n')
            break;
    }
}

void bingo_parse_boards(Bingo *bingo, FILE *fp) {
    char c = 0;
    bingo->num_boads = 0;
    while ((c = fgetc(fp)) == '\n') {
        Board *board = &bingo->boards[bingo->num_boads++];
        for(size_t i = 0; i < 5; ++i) {
            fscanf(fp, "%d %d %d %d %d",
                   &board->data[i * 5 + 0],
                   &board->data[i * 5 + 1],
                   &board->data[i * 5 + 2],
                   &board->data[i * 5 + 3],
                   &board->data[i * 5 + 4]);
        }
    }
    // NOTE: little bit hacky
    bingo->num_boads--;
}

void board_draw(Board *board, int card) {
    for (size_t y = 0; y < BOARD_DIM; ++y) {
        for (size_t x = 0; x < BOARD_DIM; ++x) {
            size_t index = y * BOARD_DIM + x;
            int board_value = board->data[index];
            if (board_value == card) {
                board->checked[index] = true;
            }
        }
    }
}

int board_check_winner(Board *board) {
    for (size_t i = 0; i < BOARD_DIM; ++i) {
        bool winner = true;
        for (size_t x = 0; x < BOARD_DIM; ++x) {
            winner &= board->checked[i * BOARD_DIM + x];
        }
        if (winner)
            return winner;

        winner = true;
        for (size_t y = 0; y < BOARD_DIM; ++y) {
            winner &= board->checked[y * BOARD_DIM + i];
        }
        if (winner)
            return winner;
    }
    return false;
}

int board_calculate_score(Board *board) {
    int score = 0;
    for (size_t i = 0; i < BOARD_DIM * BOARD_DIM; ++i) {
        bool checked = board->checked[i];
        if (!checked) {
            score += board->data[i];
        }
    }
    return score;
}

int bingo_draw(Bingo *bingo) {
    int card = bingo->cards[bingo->draw_index++];
    for (size_t i = 0; i < bingo->num_boads; ++i) {
        Board *board = &bingo->boards[i];
        board_draw(board, card);
        bool winner = board_check_winner(board);
        if (winner) {
            int card = bingo->cards[bingo->draw_index - 1];
            return board_calculate_score(board) * card;
        }
    }
    return -1;
}

int bingo_draw_till_winner(Bingo *bingo) {
    for (size_t i = bingo->draw_index; i < bingo->num_cards; ++i) {
        int score = bingo_draw(bingo);
        if (score >= 0) {
            return score;
        }
    }
    return -1;
}

void board_print(Board *board) {
    char out[16];
    printf("-----------------------------\n");
    for (size_t y = 0; y < BOARD_DIM; ++y) {
        for (size_t x = 0; x < BOARD_DIM; ++x) {
            size_t index = y * BOARD_DIM + x;
            int board_value = board->data[index];
            if (board->checked[index]) {
                sprintf(out, " *%d* ", board_value);
            } else {
                sprintf(out, "  %d  ", board_value);
            }
            printf(out);
            if (strlen(out) == 5) {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("-----------------------------\n");
}

void bingo_print(Bingo *bingo) {
    for (size_t i = 0; i < bingo->num_boads; ++i) {
        Board *board = &bingo->boards[i];
        board_print(board);
    }
}

int main() {
    FILE *fp = fopen("data/input_day04.txt", "rb");  

    if (!fp)
        perror("Cannot open file\n");

    Bingo bingo = {0};

    bingo_parse_cards(&bingo, fp);
    bingo_parse_boards(&bingo, fp);

    fclose(fp);


    //int card = bingo_draw_till_winner(&bingo);
    int score = bingo_draw_till_winner(&bingo);
    bingo_print(&bingo);
    printf("score is %d\n", score);
    
}
