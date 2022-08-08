#include <stdint.h>
#include <stdio.h>

#define N 624
#define M 397

struct rng_state {
    uint8_t val;
    uint32_t Y[N];
};

void test_xorshift8(struct rng_state *state);
void test_lcg(struct rng_state *state);
void test_mersenne_twister(struct rng_state *state);
uint8_t xorshift8(struct rng_state *state);
uint8_t lcg(struct rng_state *state);
void mersenne_twister_init(struct rng_state *state);
void mersenne_twister_update(struct rng_state *state);
uint8_t mersenne_twister(struct rng_state *state);

int main() {
    struct rng_state state;
    test_xorshift8(&state);
    test_lcg(&state);
    test_mersenne_twister(&state);

    return 0;
}

void test_xorshift8(struct rng_state *state)
{
    state->val = 240;
    FILE *fp;
    fp = fopen("/home/aci/randomness_output/output_xorshift8.txt", "w");
    uint32_t i, j;
    uint8_t res, buf;
    for(i = 0; i < 500000; i++) {
        res = xorshift8(state);
        for(j = 0; j < 8; j++) {
            buf = (res >> j) & 1;
            if(buf == 1) {
                fputc('1', fp);
            } else {
                fputc('0', fp);
            }
        }
    }
    fclose(fp);
    printf("Finished!");
}

void test_lcg(struct rng_state *state) 
{
    state->val = 240;
    FILE *fp;
    fp = fopen("/home/aci/randomness_output/output_lcg.txt", "w");
    uint32_t i, j;
    uint8_t res, buf;
    for(i = 0; i < 500000; i++) {
        res = lcg(state);
        for(j = 0; j < 8; j++) {
            buf = (res >> j) & 1;
            if(buf == 1) {
                fputc('1', fp);
            } else {
                fputc('0', fp);
            }
        }
    }
    fclose(fp);
    printf("Finished!"); 
}

void test_mersenne_twister(struct rng_state *state)
{
    FILE *fp;
    fp = fopen("/home/aci/randomness_output/output_mersenne.txt", "w");
    uint32_t i, j;
    uint8_t res, buf;
    for(i = 0; i < 500000; i++) {
        res = mersenne_twister(state);
        for(j = 0; j < 8; j++) {
            buf = (res >> j) & 1;
            if(buf == 1) {
                fputc('1', fp);
            } else {
                fputc('0', fp);
            }
        }
    }
    fclose(fp);
    printf("Finished!");
}

uint8_t xorshift8(struct rng_state *state)
{
    uint8_t x = state->val;
    x ^= x << 7;
    x ^= x >> 3;
    x ^= x << 2;
    return state->val = x;
}

uint8_t lcg(struct rng_state *state)
{
    uint8_t x = state->val;
    x = (uint8_t) (x * 157 + 205);
    return state->val = x;
}

void mersenne_twister_init(struct rng_state *state)
{
    const uint32_t mult = 1812433253ul;
    uint32_t seed = 3961ul;
    int i;
    for (i = 0; i < N; i++)
    {
        state->Y[i] = seed;
        seed = mult * (seed ^ (seed >> 30)) + (i+1);
    }
}

void mersenne_twister_update(struct rng_state *state)
{
    static const uint32_t  A[2] = { 0, 0x9908B0DF };
    int i = 0;
    for (; i < N - M; i++) {
        state->Y[i] = state->Y[i+(M)] ^ (((state->Y[i] & 0x80000000) | (state->Y[i+1] & 0x7FFFFFFF)) >> 1) ^ A[state->Y[i+1] & 1];
    }   
    for (; i < N-1; i++) {
        state->Y[i] = state->Y[i+(M-N)] ^ (((state->Y[i] & 0x80000000) | (state->Y[i+1] & 0x7FFFFFFF)) >> 1) ^ A[state->Y[i+1] & 1];
    }
    state->Y[N-1] = state->Y[M-1] ^ (((state->Y[N-1] & 0x80000000) | (state->Y[0] & 0x7FFFFFFF)) >> 1) ^ A[state->Y[0] & 1];
}

uint8_t mersenne_twister(struct rng_state *state)
{
    uint32_t res;
    static uint32_t index = N + 1;
    if(index >= N) {
        if(index > N) {
            mersenne_twister_init(state);
        }
        mersenne_twister_update(state);
        index = 0;
    }
    res = state->Y[index++];
    res ^= (res >> 11);
    res ^= (res <<  7) & 0x9D2C5680;
    res ^= (res << 15) & 0xEFC60000;
    res ^= (res >> 18);
    return (uint8_t) (res % 256);
}