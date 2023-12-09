#include "advent.h"
#include <stdint.h>

typedef struct Hand {
    char *cards;
    int bid;
} Hand;

int cmp_i(int *a, int *b) {
    return *b - *a;
}

char lookup[] = {
    'A',
    'K',
    'Q',
    'T',
    '9',
    '8',
    '7',
    '6',
    '5',
    '4',
    '3',
    '2',
    'J',
};

int get_card_value(char c) {
    for (int i = 0; i < ArraySize(lookup); ++i) {
        if (c == lookup[i]) {
            return ArraySize(lookup) - i;
        }
    }
    INVALID;
    return 0;
}

int get_type_value(Hand *a) {
    int counts[256] = {0};
    int njokers = 0;
    for (int i = 0; i < strlen(a->cards); ++i) {
        char c = a->cards[i];
        if (c == 'J') {
            njokers++;
        } else {
            counts[(int)c]++;
        }
    }

    // print("Card %s:", a->cards);
    int *numbers = NULL;
    for (int i = 0; i < ArraySize(counts); ++i) {
        if (counts[i]) {
            append(numbers, counts[i]);
        }
    }
    sort(numbers, cmp_i);
    // print_ints(numbers);
    if (len(numbers) == 0) {
        assert (njokers == 5);
        append(numbers, 5);
    }

    while (njokers-- > 0) {
        numbers[0] += 1;
    }

    if (len(numbers) == 1) {
        return 9;
        assert(numbers[0] == 5);
    } else if (len(numbers) == 2 && numbers[0] == 4 && numbers[1] == 1) {
        return 8;
    } else if (len(numbers) == 2 && numbers[0] == 3 && numbers[1] == 2) {
        return 7;
    } else if (len(numbers) == 3 && numbers[0] == 3 && numbers[1] == 1 && numbers[2] == 1) {
        return 6;
    } else if (len(numbers) == 3 && numbers[0] == 2 && numbers[1] == 2 && numbers[2] == 1) {
        return 5;
    } else if (len(numbers) == 4 && numbers[0] == 2 && numbers[1] == 1 && numbers[2] == 1 && numbers[3] == 1) {
        return 4;
    } else if (len(numbers) == 5) {
        assert(numbers[0] == 1);
        assert(numbers[1] == 1);
        assert(numbers[2] == 1);
        assert(numbers[3] == 1);
        assert(numbers[4] == 1);
        return 3;
    } else {
        INVALID;
    }
    vec_clear(numbers);
    return 0;
}

int cmp(Hand *a, Hand *b) {
    int va = get_type_value(a);
    int vb = get_type_value(b);
    if (va != vb) {
        return va - vb;
    }

    assert (strlen(a->cards) == strlen(b->cards));
    for (int i = 0; i < strlen(a->cards); ++i) {
        char ac = a->cards[i];
        char bc = b->cards[i];
        int first = get_card_value(ac);
        int second = get_card_value(bc);
        if (first != second) {
            return first - second;
        }
    }
    return 0;
}

int main() {
    uint64_t res = 0;
    char *line = NULL;
    Hand *hands = NULL;
#if 0
    Hand foo = {
        .cards="K9KJ9",
        .bid=1337,
    };
    print("%d", get_type_value(&foo));
    return 0;
#endif

    //FILE *fp = fopen("data/day07.txt", "r");
    while (readline(&line)) {
        char *hand = buffer(1024);
        char *bid = buffer(1024);
        sscanf(line, "%s %s", hand, bid);
        *add(hands) = (Hand) {
            .cards=strdup(hand),
            .bid=atoi(bid)
        };
    }

    sort(hands, cmp);

    int rank = 1;
    for each(hand, hands) {
        res += rank++ * hand->bid;
        // print("%s %d", hand->cards, hand->bid);
    }
    print("%lu", res);
}
