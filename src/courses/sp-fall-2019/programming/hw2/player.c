#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

const char *const usage_msg = "usage: ./player [player_id]";

const int ROUND_NUM = 10;

void ERR_EXIT(char *msg) { perror(msg); exit(128); }

void flush_fsync(FILE *stream) { fflush(stream); fsync(fileno(stream)); }

void read_winner_id() {
    int winner_id;
    scanf("%d", &winner_id);
}

void tell_host_money(int player_id, int money) {
    printf("%d %d\n", player_id, money);
    flush_fsync(stdout);
}

int main(int argc, char *argv[])
{
    if ( argc != 2 ) {
        // the number of arguments should be exactly 1
        fprintf(stderr, usage_msg);
        exit(2);
    }

    const int player_id = atoi(argv[1]);
    const int money = player_id * 100;

    for ( int round = 1; round <= ROUND_NUM; ++round) {
        if ( round > 1 )
            read_winner_id();

        tell_host_money(player_id, money);
    }
    
    return EXIT_SUCCESS;
}