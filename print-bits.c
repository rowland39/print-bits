#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <math.h>
#include <arpa/inet.h>

#define MAXBIN (sizeof(unsigned int) * 8)

void
reset_array(int array[], int size)
{
    int t;

    for (t = 0; t < size; t++) {
        array[t] = 0;
    }
}

int
main(int argc, char **argv)
{
    unsigned int num, old_num;
    int binary[MAXBIN];
    int times;
    int n, x;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <number>\n", argv[0]);
        return 1;
    }

    if (strtol(argv[1], NULL, 0) < 0) {
        fprintf(stderr, "number must be positive\n");
        return 1;
    }

    reset_array(binary, MAXBIN);
    /* Base 8, 10, or 16 depending on if "0" or "0x" is in the string. */
    num = (unsigned int)strtol(argv[1], NULL, 0);
    printf("\nHost Order\n");
    printf("----------\n");
    printf("Decimal:     %d\n", num);
    printf("Hexidecimal: %#010x\n", num);
    old_num = num;
    printf("Binary:      ");
    times = 0;

    /*
     * There might be a way to do this without using a buffer, but
     * this is easiest to understand. Remember that ~ has the highest
     * precedence below. What happens is we start with all 0 bits,
     * negate that to all 1 bits, shift that to the left once place
     * leaving a 0 at the start, then we negate that again. This
     * results in all zeros with a 1 in the first bit position. We
     * then and that with the number to determine if the first bit
     * is a 0 or a 1. We keep shifting the number to the right to
     * evaluate each bit in turn. Basically we figure out the bits,
     * but they are in reverse - hence the use of a buffer to print
     * them out in the opposite order.
     *
     */
    do {
        binary[times++] = (~(~0 << 1) & num) ? 1 : 0;
        num >>= 1;
    } while (num > 0);

    for (times = MAXBIN - 1; times >= 0; times--) {
        printf("%d", binary[times]);

        if (times % 8 == 0)
            printf(" ");
    }

    printf("\n\n");

    // This is meant to show each byte as hexidecimal in the order the bytes
    // are stored in RAM. This is mostly interesting on a little endian system.
    printf("RAM Bytes in Order: ");
    num = old_num;

    for (n = 0; n < 4; n++) {
        x = (num >> (8 * n)) & 0xff;
        printf("%#04x ", x);
    }

    printf("\n\n");

    reset_array(binary, MAXBIN);
    /* Repeat the algorithm for network byte order (big-endian). */
    num = htonl((unsigned int)strtol(argv[1], NULL, 0));
    printf("\nNetwork Order (on this host)\n");
    printf("----------------------------\n");
    printf("Decimal:     %d\n", num);
    printf("Hexidecimal: %#010x\n", num);
    printf("Binary:      ");
    times = 0;

    do {
        binary[times++] = (~(~0 << 1) & num) ? 1 : 0;
        num >>= 1;
    } while (num > 0);

    for (times = MAXBIN - 1; times >= 0; times--) {
        printf("%d", binary[times]);

        if (times % 8 == 0)
            printf(" ");
    }

    printf("\n\n");
    exit(EXIT_SUCCESS);
}
