/*
*/

#include <cstdint>
#include <cstdio>
#include <cinttypes>
#include <cstdlib>

char* read_file(const char *filename, int *file_size) {
    FILE *file = fopen(filename, "rb");
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *data = (char*)malloc(size);
    int read_n = fread(data, 1, size, file);
    fclose(file);

    *file_size = size;
    return data;
}

struct Technique {
    enum Kind { Deal, Cut, DealN } kind;
    int n;
};

struct Techniques {
    Technique *ts;
    int num;
};

const char* skip_after(const char *p, const char * const p_end, char c) {
    while (p != p_end && p[0] != c) {
        p++;
    }
    return p < p_end ? p + 1 : p_end;
}

const char* read_technique(const char *p, const char * const p_end, Technique *result) {
    switch (p[0]) {
        case 'd': {
            p = skip_after(p, p_end, ' ');
            switch (p[0]) {
                case 'i':
                    *result = {
                        .kind = Technique::Deal,
                    };
                    break;
                case 'w': {
                    p = skip_after(p, p_end, ' ');
                    p = skip_after(p, p_end, ' ');
                    int n = atoi(p);
                    *result = {
                        .kind = Technique::DealN,
                        .n = n,
                    };
                    break;
                }
            }
        } break;
        case 'c': {
            p = skip_after(p, p_end, ' ');
            int n = atoi(p);
            *result = {
                .kind = Technique::Cut,
                .n = n,
            };
        } break;
        default:
            printf("Something else %d %c%c%c %p %p %d\n", p[0], p[0], p[1], p[2], p, p_end,
                   (int)(p_end - p));
            ;
    }
    return skip_after(p, p_end, '\n');
}

Techniques read_techniques() {
    int file_size;
    char *data = read_file("input.txt", &file_size);
    const char *data_end = data + file_size;
    const char *p = data;

    int cap = 100;
    int n = 0;
    Technique *result = (Technique*)malloc(cap * sizeof(Technique));

    while (p < data_end) {
        Technique t;
        p = read_technique(p, data_end, &t);
        if (n == cap) {
            cap *=2;
            result = (Technique*)realloc(result, cap * sizeof(Technique));
        }
        result[n] = t;
        n++;
    }
    free(data);

    return {
        .ts = result,
        .num = n,
    };
}

int64_t apply_technique(Technique t, int64_t cards_n, int64_t pos) {
    switch (t.kind) {
        case Technique::Deal:
            return cards_n - 1 - pos;
        case Technique::DealN:
            return (pos * t.n) % cards_n;
        case Technique::Cut:
            return (pos + cards_n - t.n) % cards_n;
    }
    return 0;
}

int64_t track_card_position(Techniques techniques, int64_t cards_n, int64_t card_i) {
    int64_t pos = card_i;
    for (int i = 0; i < techniques.num; i++) {
        Technique t = techniques.ts[i];
        pos = apply_technique(t, cards_n, pos);
    }
    return pos;
}


void part_one() {
    Techniques techniques = read_techniques();

    int result = track_card_position(techniques, 10007, 2019);

    printf("Part 1 - %d\n", result);
}

// Should not need to use 128 bit integers for card indices,
// only as intermediate results before modulo, as the modulo is
// less than max 64 bit integer.
typedef __int128 CardInt;

CardInt modulo(CardInt a, CardInt m);

template <CardInt N>
struct Affine {
    // f(x) = scale * x + shift
    CardInt scale;
    CardInt shift;
    CardInt eval(CardInt x) { return modulo(scale * 2020 + shift, N); }
};

CardInt modulo(CardInt a, CardInt m) {
    CardInt result = a % m;
    if (result < 0) result += m;
    return result;
}

template <CardInt N>
Affine<N> combine(Affine<N> A, Affine<N> B) {
    // A = <a,b>
    // B = <c,d>
    // A(x) = a x + b
    // B(x) = c x + d
    // (A . B) (x) = c(ax+b) + d
    //             = cax + cb + d
    // e = ca
    // f = cb + d
    // (A . B) = <e,f>
    return {
        .scale = modulo(A.scale * B.scale, N),
        .shift = modulo(B.scale * A.shift + B.shift, N),
    };
}

CardInt power_int(CardInt x, CardInt P, CardInt N) {
    CardInt result = x;
    CardInt n = 1;
    while (n*2 <= P) {
        result = modulo(result * result, N);
        n *= 2;
    }
    CardInt k = P - n;
    if (k > 0) {
        result = modulo(result * power_int(x, k, N), N);
    }
    return result;
}

template <CardInt N>
Affine<N> invert(Affine<N> affine) {
    // affine = A = <a,b>
    // inverted = I = <c,d>
    // (A . I) = <1,0>
    // => a*c = 1,
    //    c*b + d = 0
    //
    // If N is prime, scaling by a is rotating the position by a * x positions.
    // Cycling through all positions will be N rotations.
    // a^N = a         same as one rotation
    // a^(N-1) = 1     no rotation
    // a^(N-2)*a = 1   no rotation
    // a * c = a * a^(N-2) = 1
    // c = a^(N-2)
    
    CardInt c = power_int(affine.scale, N-2, N);
    CardInt d = modulo(-c * affine.shift, N);
    return {
        .scale = c,
        .shift = d,
    };
}

template <CardInt N>
Affine<N> power(Affine<N> A, CardInt P) {
    Affine<N> result = A;

    CardInt n = 1;
    while (n*2 <= P) {
        result = combine(result, result);
        n *= 2;
    }
    CardInt k = P - n;
    if (k > 0) {
        result = combine(result, power(A, k));
    }
    return result;
}

template <CardInt N> Affine<N> technique_to_affine(Technique t) {
    switch (t.kind) {
        case Technique::Deal:
            return {
                .scale = (N-1)%N,
                .shift = N - 1,
            };
        case Technique::DealN:
            return {
                .scale = t.n,
                .shift = 0,
            };
        case Technique::Cut:
            return {
                .scale = 1,
                .shift = N - t.n,
            };
    }
    return {};
}

template <CardInt N>
Affine<N> reduce_techniques(Techniques ts) {
    Affine<N> init = technique_to_affine<N>(ts.ts[0]);
    for (int i = 1; i < ts.num; i++) {
        Affine<N> x = technique_to_affine<N>(ts.ts[i]);
        init = combine(init, x);
    }
    return init;
}

int print_cardint(CardInt i) {
    int n = 0;
    if (i < 0) {
        n += putchar('-');
        i = -i;
    }
    if (i > UINT64_MAX) {
        uint64_t P1E19 = 10'000'000'000'000'000'000ull;
        CardInt upper = i / P1E19;
        uint64_t lower = i % P1E19;
        n += print_cardint(upper);
        return n + printf("%.19" PRIu64, lower);
    } else {
        return n + printf("%" PRIu64, (uint64_t)i);
    }
}

void part_two() {
    #if 0
    // This is too slow, will take ages to evaluate all techniques
    Techniques techniques = read_techniques();

    CardInt cards_n = 119315717514047LL;
    CardInt result = 2020LL;
    for (CardInt i = 0; i < 101741582076661LL; i++) {
        CardInt prev = result;
        result = track_card_position(techniques, cards_n, result);
        CardInt diff = result - prev;
        if (diff < 0) diff += cards_n;
        if ((i & 0xffff) == 0) printf("%20" PRId64 ": %20" PRId64 " - diff=%20" PRId64 "\n", i, result, diff);
    }
    #endif

    #define N 119315717514047LL
    Techniques techniques = read_techniques();

    Affine<N> one_round = reduce_techniques<N>(techniques);
    printf("One round: <%" PRId64 ", %" PRId64 ">\n", (int64_t)one_round.scale, (int64_t)one_round.shift);

    Affine<N> all_rounds = power(one_round, 101741582076661LL);
    printf("All rounds: <%" PRId64 ", %" PRId64 ">\n", (int64_t)all_rounds.scale, (int64_t)all_rounds.shift);

    Affine<N> inv = invert(all_rounds);
    printf("Inverted: <%" PRId64 ", %" PRId64 ">\n", (int64_t)inv.scale, (int64_t)inv.shift);

    CardInt result = inv.eval(2020);
    printf("Part 2 - %" PRId64 "\n", (int64_t)result);
}

int main(int argc, char **argv) {
    part_one();
    part_two();
    return 0;
}

