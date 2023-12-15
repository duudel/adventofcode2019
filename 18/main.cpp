/**
--- Day 18: Many-Worlds Interpretation ---

As you approach Neptune, a planetary security system detects you and activates a
giant tractor beam on Triton! You have no choice but to land.

A scan of the local area reveals only one interesting feature: a massive
underground vault. You generate a map of the tunnels (your puzzle input). The
tunnels are too narrow to move diagonally.

Only one entrance (marked @) is present among the open passages (marked .) and
stone walls (#), but you also detect an assortment of keys (shown as lowercase
letters) and doors (shown as uppercase letters). Keys of a given letter open the
door of the same letter: a opens A, b opens B, and so on. You aren't sure which
key you need to disable the tractor beam, so you'll need to collect all of them.

For example, suppose you have the following map:

#########
#b.A.@.a#
#########

Starting from the entrance (@), you can only access a large door (A) and a key
(a). Moving toward the door doesn't help you, but you can move 2 steps to
collect the key, unlocking A in the process:

#########
#b.....@#
#########

Then, you can move 6 steps to collect the only other key, b:

#########
#@......#
#########

So, collecting every key took a total of 8 steps.

Here is a larger example:

########################
#f.D.E.e.C.b.A.@.a.B.c.#
######################.#
#d.....................#
########################

The only reasonable move is to take key a and unlock door A:

########################
#f.D.E.e.C.b.....@.B.c.#
######################.#
#d.....................#
########################

Then, do the same with key b:

########################
#f.D.E.e.C.@.........c.#
######################.#
#d.....................#
########################

...and the same with key c:

########################
#f.D.E.e.............@.#
######################.#
#d.....................#
########################

Now, you have a choice between keys d and e. While key e is closer, collecting
it now would be slower in the long run than collecting key d first, so that's
the best choice:

########################
#f...E.e...............#
######################.#
#@.....................#
########################

Finally, collect key e to unlock door E, then collect key f, taking a grand
total of 86 steps.

Here are a few more examples:

    ########################
    #...............b.C.D.f#
    #.######################
    #.....@.a.B.c.d.A.e.F.g#
    ########################

    Shortest path is 132 steps: b, a, c, d, f, e, g

    #################
    #i.G..c...e..H.p#
    ########.########
    #j.A..b...f..D.o#
    ########@########
    #k.E..a...g..B.n#
    ########.########
    #l.F..d...h..C.m#
    #################

    Shortest paths are 136 steps;
    one is: a, f, b, j, g, n, h, d, l, o, e, p, c, i, k, m

    ########################
    #@..............ac.GI.b#
    ###d#e#f################
    ###A#B#C################
    ###g#h#i################
    ########################

    Shortest paths are 81 steps; one is: a, c, f, i, d, g, b, e, h

How many steps is the shortest path that collects all of the keys?
*/

#include <cstdint>
#include <ctime>

uint64_t perf_time_nanos() {
    timespec ts;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
    return ts.tv_sec * 1000'000'000ULL + ts.tv_nsec;
}


uint64_t perf_time_elapsed_nanos(uint64_t start_time_nanos) {
    return perf_time_nanos() - start_time_nanos;
}

namespace sample0
{
    constexpr char map_data[] =
        "#########\n"
        "#b.A.@.a#\n"
        "#########\n";
}

namespace sample1
{
    constexpr char map_data[] =
        "########################\n"
        "#f.D.E.e.C.b.A.@.a.B.c.#\n"
        "######################.#\n"
        "#d.....................#\n"
        "########################\n";
}

namespace sample2
{
    constexpr char map_data[] =
        "########################\n"
        "#...............b.C.D.f#\n"
        "#.######################\n"
        "#.....@.a.B.c.d.A.e.F.g#\n"
        "########################\n";
}

namespace sample3
{
    constexpr char map_data[] =
        "#################\n"
        "#i.G..c...e..H.p#\n"
        "########.########\n"
        "#j.A..b...f..D.o#\n"
        "########@########\n"
        "#k.E..a...g..B.n#\n"
        "########.########\n"
        "#l.F..d...h..C.m#\n"
        "#################\n";
}

namespace sample4
{
    constexpr char map_data[] =
        "########################\n"
        "#@..............ac.GI.b#\n"
        "###d#e#f################\n"
        "###A#B#C################\n"
        "###g#h#i################\n"
        "########################\n";
}

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cassert>

struct Pos
{
    int x, y;

    Pos move(int dx, int dy) {
        return { x + dx, y + dy };
    }
};

bool operator==(Pos a, Pos b) { return a.x == b.x && a.y == b.y; }
bool operator!=(Pos a, Pos b) { return !(a.x == b.x && a.y == b.y); }

struct Map
{
    char *data;
    int width, height;

    char operator()(int x, int y) {
        //assert(0 <= x && x < width);
        //assert(0 <= y && y < height);
        return data[y * width + x];
    }
    char& operator[](Pos pos) {
        //assert(0 <= pos.x && pos.x < width);
        //assert(0 <= pos.y && pos.y < height);
        return data[pos.y * width + pos.x];
    }
};

void map_free(Map map) {
    free(map.data);
}

Map map_copy(Map map)
{
    char *data = (char*)malloc(map.width * map.height * sizeof(char));
    memcpy(data, map.data, map.width * map.height * sizeof(char));
    return {
        .data = data,
        .width = map.width,
        .height = map.height,
    };
}

bool is_key(char c) { return 'a' <= c && c <= 'z'; }
bool is_door(char c) { return 'A' <= c && c <= 'Z'; }
bool is_walkable(char c) {
    return (c == '.') || (c == '@') || is_key(c);
}

void print_map(Map map, unsigned int keys_reachable_bits, Pos pos) {
    for (int y = 0; y < map.height; y++) {
        for (int x = 0; x < map.width; x++) {
            char c = map[{x, y}];
            if (pos == Pos{x, y}) {
                printf("\e[31;1m");
                putchar('@');
            } else if (is_key(c)) {
                if (keys_reachable_bits & (1u << (c - 'a')))
                    printf("\e[35;1m");
                else printf("\e[32;1m");
                putchar(c);
            } else if (is_door(c)) {
                printf("\e[93;1m");
                putchar(c);
            } else if (c == '=') {
                printf("\e[96m");
                putchar(c);
            } else if (c == '.') {
                printf("\e[90m");
                putchar(c);
            } else {
                printf("\e[90m");
                putchar(c);
            }
        }
        printf("\n");
    }
}

char key_to_door(char key) { return key + 'A' - 'a'; }
char door_to_key(char key) { return key + 'a' - 'A'; }

struct Key
{
    char key;
    Pos position;
};

struct Door
{
    char door;
    char key;
    Pos position;
};

struct Objects {
    Key keys_in_map[26];
    int keys_in_map_n;
    
    Door doors_in_map[26];
    int doors_in_map_n;

    Pos start_position;
};

void map_find_objects(Objects *objects, Map map)
{
    for (int y = 0; y < map.height; y++)
    {
        for (int x = 0; x < map.width; x++)
        {
            char c = map(x, y);
            if (is_key(c)) {
                int index = objects->keys_in_map_n;
                objects->keys_in_map[index] = Key{
                    .key = c,
                    .position = {x, y},
                };
                objects->keys_in_map_n++;
            } else if (is_door(c)) {
                int index = objects->doors_in_map_n;
                objects->doors_in_map[index] = Door{
                    .door = c,
                    .key = door_to_key(c),
                    .position = {x, y},
                };
                objects->doors_in_map_n++;
            } else if (c == '@') {
                objects->start_position = {x, y};
            }
        }
    }
}


void get_map_dims(const char *map, int *width, int *height)
{
    bool w_ready = false;
    int w = 0, h = 0;
    while (map[0])
    {
        if (map[0] == '\n')
        {
            h++;
            w_ready = true;
        }
        else if (!w_ready)
        {
            w++;
        }
    }
    *width = w;
    *height = h;
}

constexpr int calc_map_height(const char *map)
{
    int h = 0;
    while (map[0])
    {
        if (map[0] == '\n')
        {
            h++;
        }
        map++;
    }
    return h;
}

constexpr int calc_map_width(const char *map)
{
    int w = 0;
    while (map[0])
    {
        if (map[0] == '\n') break;
        w++;
        map++;
    }
    return w;
}

char *copy_map_from_string(char *dst, const char *src)
{
    size_t dst_i = 0;
    for (size_t i = 0; i < strlen(src); i++)
    {
        if (src[i] != '\n') {
            dst[dst_i] = src[i];
            dst_i++;
        }
    }
    return dst;
}

Map map_from_string(const char *str) {
    int width = calc_map_width(str);
    int height = calc_map_height(str);
    char *map_data = (char*)malloc(width * height * sizeof(char));
    return Map{
        .data = copy_map_from_string(map_data, str),
        .width = width,
        .height = height,
    };
}

Map map_from_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char*)malloc(file_size);
    fread(data, 1, file_size, file);

    fclose(file);
    Map map = map_from_string(data);
    free(data);
    return map;
}

unsigned int hash(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

struct Memo
{
    struct Key {
        Pos pos;
        unsigned int key_bits;
    };
    struct Value {
        Key key;
        int dist;
    };
    Value *values;
    int values_n;
    int values_cap;

    struct {
        int64_t hit;
        int64_t miss;
    } stats;
};

void memo_free(Memo *memo) {
    if (memo->values) free(memo->values);
}

double memo_hit_miss_ratio(Memo *memo) {
    return (double)memo->stats.hit / memo->stats.miss;
}

double memo_hit_ratio(Memo *memo) {
    return (double)memo->stats.hit / (memo->stats.hit + memo->stats.miss);
}

unsigned int keys_to_bits(Key *keys, int keys_n) {
    unsigned int result = 0;
    for (int i = 0; i < keys_n; i++) {
        Key key = keys[i];
        int key_bit_index = key.key - 'a';
        result |= 1 << key_bit_index;
    }
    return result;
}

Memo::Key memo_key(Pos pos, Key *keys, int keys_n) {
    return {
        .pos = pos,
        .key_bits = keys_to_bits(keys, keys_n),
    };
}

unsigned int memo_hash(Memo::Key key) {
    //return hash(key.key_bits) + hash(key.pos.x) + hash(key.pos.y);

    //int lo = key.key_bits & 0xffff;
    //int hi = key.key_bits >> 16;
    //return key.key_bits + lo * key.pos.x + key.pos.y * (hi + (1 << 14));
    //return lo ^ hi + key.pos.x + key.pos.y * 111;
    //return hi ^ (key.pos.y * 517741 + key.pos.x * 109 + lo);
    //return key.key_bits ^ (key.pos.y * 111 + key.pos.x * 83);
    
    //return key.key_bits ^ (key.pos.y * 107739 + key.pos.x * 2749);
    
    return key.key_bits ^ (key.pos.y * 93719 + key.pos.x * 2749);
    //return key.key_bits ^ (key.pos.y * 2749 + key.pos.x * 7879);
}

unsigned int min(unsigned int a, unsigned int b) {
    return a < b ? a : b;
}

unsigned int memo_hash2(Memo::Key key) {
    //return key.key_bits ^ (key.pos.y * 107739 + key.pos.x * 83);
    return key.key_bits ^ (key.pos.y * 2749 + key.pos.x * 7879);

    //int lo = key.key_bits & 0xfffff;
    //int hi = key.key_bits >> 20;
    //return lo * key.pos.x + key.pos.y * (hi + (1 << 14));
    //return lo ^ hi + key.pos.x + key.pos.y * 111;
    ////return hi ^ (key.pos.y * 517741 + key.pos.x * 109 + lo);

    //return key.key_bits ^ (key.pos.y * 107739 + key.pos.x * 83);
    //return key.key_bits ^ (key.pos.y * 111 + key.pos.x * 83);
}

#define MEMO_MAX_SEARCH 100
//#define MEMO_MAX_SEARCH 20

void memo_put(Memo *memo, Memo::Key key, int distance) {
    //memo->map[key] = distance;
    //return;
    if (memo->values_cap - memo->values_n <= memo->values_n / 2) {
        //int new_cap = memo->values_cap * 2 + 4;
        int new_cap = (memo->values_cap < 512) ? 512 : memo->values_cap * 4 + 103;
        Memo::Value *new_values = (Memo::Value*)calloc(new_cap, sizeof(Memo::Value));
        for (int i = 0; i < memo->values_cap; i++) {
            Memo::Value v = memo->values[i];
            if (v.key.key_bits == 0) continue;
            unsigned int index_p = memo_hash(v.key);
            for (unsigned int ni = 0; ni < memo->values_cap + 4; ni++) {
                unsigned int index = (index_p + ni) % (unsigned)new_cap;
                Memo::Value &nv = new_values[index];
                if (nv.key.key_bits == 0) {
                    nv = v;
                    break;
                }
            }
        }
        free(memo->values);
        memo->values = new_values;
        memo->values_cap = new_cap;
    }

    //int max_search = memo->values_cap;
    unsigned int max_search = min(MEMO_MAX_SEARCH, memo->values_cap);
            
    unsigned int index_p = memo_hash(key);
    int round = 0;
    while (round < 2) {
        for (unsigned int ni = 0; ni < max_search; ni++) {
            unsigned int index = (index_p + ni) % (unsigned)memo->values_cap;
            Memo::Value &nv = memo->values[index];
            if (nv.key.key_bits == 0) {
                nv = {
                    .key = key,
                    .dist = distance,
                };
                memo->values_n++;
                return;
            } else if (nv.key.pos == key.pos && nv.key.key_bits == key.key_bits && nv.dist > distance) {
                printf("Duplicate! %c\n", key.pos.x);
                nv.dist = distance;
                return;
            }

            //if ((index & 127) == 0) printf("Tried index %d\n", index);
        }
        round++;
        index_p = memo_hash2(key);
        max_search /= 2;
    }

    unsigned int index = (index_p + max_search) % (unsigned)memo->values_cap;
    Memo::Value &nv = memo->values[index];
    if (nv.key.key_bits == 0) memo->values_n++;
    nv = {
        .key = key,
        .dist = distance,
    };

    //printf("COULD NOT INSERT!\n");
    //fflush(stdout);
    //*((int*)100) = 1;
}

bool memo_get(Memo *memo, Memo::Key key, int *distance) {
    //if (auto result = memo->map.find(key); result != memo->map.end()) {
    //    *distance = result->second;
    //    return true;
    //} else return false;
    if (memo->values_n == 0) return false;
    
    //int max_search = memo->values_cap;
    unsigned int max_search = min(MEMO_MAX_SEARCH, memo->values_cap);

    unsigned int index_p = memo_hash(key);
    int round = 0;
    while (round < 2) {
        for (unsigned int ni = 0; ni < max_search; ni++) {
            unsigned index = (index_p + ni) % (unsigned)memo->values_cap;
            const Memo::Value &nv = memo->values[index];
            if (nv.key.pos == key.pos && nv.key.key_bits == key.key_bits) {
                *distance = nv.dist;
                memo->stats.hit++;
                return true;
            }
        }
        round++;
        index_p = memo_hash2(key);
        max_search /= 2;
    }
    memo->stats.miss++;
    return false;
}

void print_memo(Memo *memo) {
    for (int i = 0; i < memo->values_cap; i++) {
        Memo::Value v = memo->values[i];
        if (v.key.key_bits) {
            printf("(%d,%d), keys=%d => %d; keys: ",
                   v.key.pos.x, v.key.pos.y, v.key.key_bits, v.dist);
            uint32_t bits = v.key.key_bits;
            char key = 'a';
            while (bits != 0) {
                if (bits & 1) {
                    putchar(key);
                }
                bits >>= 1;
                key++;
            }
            printf("\n");
        }
    }
    printf("Memo: %d items, %d capacity; hit/miss %.3f, hit%% %.3f\n",
           memo->values_n, memo->values_cap,
           memo_hit_miss_ratio(memo), 100*memo_hit_ratio(memo));
}

struct Context
{
    Memo *memo;
    Objects objects;

    Key keys_reachable[26];
    int keys_reachable_n;
    unsigned int keys_reachable_bits;
};

void ctx_add_reachable_key(Context &ctx, char key_char) {
    unsigned int key_bit = 1u << (key_char - 'a');
    if (ctx.keys_reachable_bits & key_bit) return;
    for (int i = 0; i < ctx.objects.keys_in_map_n; i++) {
        if (ctx.objects.keys_in_map[i].key == key_char) {
            int index = ctx.keys_reachable_n;
            ctx.keys_reachable_n++;
            ctx.keys_reachable[index] = ctx.objects.keys_in_map[i];
            ctx.keys_reachable_bits |= key_bit;
            break;
        }
    }
}

void ctx_clear_reachable_keys(Context &ctx) {
    ctx.keys_reachable_n = 0;
    ctx.keys_reachable_bits = 0;
}


struct AStar {
    int width, height;
    int *map;

    int& operator[](Pos pos) {
        //if (!(0 <= pos.x && pos.x < width)) printf("INVALID A* ACCESS [%d,%d]: pos=%d,%d\n", width, height, pos.x, pos.y);
        //if (!(0 <= pos.y && pos.y < height)) printf("INVALID A* ACCESS [%d,%d]: pos=%d,%d\n", width, height, pos.x, pos.y);
        //fflush(stdout);
        return map[pos.y * width + pos.x];
    }
};

AStar astar_alloc(int width, int height) {
    int *data = (int*)calloc(width * height, sizeof(int));
    return {
        .width = width,
        .height = height,
        .map = data,
    };
}

void astar_free(AStar &astar) {
    free(astar.map);
}

void astar_init(AStar *astar) {
    const int N = astar->width * astar->height;
    for (int i = 0; i < N; i++) astar->map[i] = 999999;
}

bool astar_update_value(AStar &astar, Pos pos, int new_value)
{
    int v = astar[pos];
    if (new_value < v) {
        astar[pos] = new_value;
        return true;
    }
    return false;
}

bool is_valid_pos(Map map, Pos pos) {
    return (pos.x >= 0 && pos.x < map.width) && (pos.y >= 0 && pos.y < map.height);
}

bool astar_fill_dead_ends_(AStar &astar, Map map, Pos from, Pos pos, int dist, int &filled);

bool astar_fill_dead_ends_to_(AStar &astar, Map map, Pos pos, Pos new_pos, int dist, int &filled) {
    if (is_valid_pos(map, new_pos)) {
        char c = map[new_pos];
        if (is_walkable(c)) {
            return astar_fill_dead_ends_(astar, map, pos, new_pos, dist + 1, filled);
        } else if (is_door(c)) {
            astar_fill_dead_ends_(astar, map, pos, new_pos, dist + 1, filled);
            return true;
        }
    }
    return false;
}

bool astar_fill_dead_ends_(AStar &astar, Map map, Pos from, Pos pos, int dist, int &filled)
{
    if (!astar_update_value(astar, pos, dist)) return true;
    Pos new_pos[4] = {
        pos.move(-1,  0),
        pos.move( 1,  0),
        pos.move( 0, -1),
        pos.move( 0,  1),
    };
    bool valid_path = false;
    for (int i = 0; i < 4; i++) {
        if (new_pos[i] != from) {
            valid_path |= astar_fill_dead_ends_to_(astar, map, pos, new_pos[i], dist, filled);
        }
    }
    if (!valid_path) {
        char c = map[pos];
        if (is_key(c) || is_door(c)) return true;
        //map[pos] = '=';
        //print_map(map, st.keys_reachable_bits, {-1,-1});
        //getchar();
        //map[pos] = '$';
        map[pos] = '#';
        filled++;
        return false;
    }
    return true;
}

void astar_fill_dead_ends_in_map(AStar &astar, Map map, Pos pos)
{
    while (true) {
        astar_init(&astar);
        int filled = 0;
        astar_fill_dead_ends_(astar, map, pos, pos, 0, filled);
        if (filled == 0) break;
        else printf("filled = %d\n", filled);
    }

    //print_map(map, st.keys_reachable_bits, {-1,-1});
    //getchar();
}

void astar_calculate_distances_(Context &ctx, AStar &astar, Map map, Pos from, Pos pos, int dist)
{
    if (!astar_update_value(astar, pos, dist)) return;
    if (is_key(map[pos])) {
        ctx_add_reachable_key(ctx, map[pos]);
    }
    if (from.x < pos.x) {
        Pos npos1 = pos.move(1, 0);
        Pos npos2 = pos.move(0, -1);
        Pos npos3 = pos.move(0, 1);
        if (is_walkable(map[npos1])) {
            astar_calculate_distances_(ctx, astar, map, pos, npos1, dist + 1);
        }
        if (is_walkable(map[npos2])) {
            astar_calculate_distances_(ctx, astar, map, pos, npos2, dist + 1);
        }
        if (is_walkable(map[npos3])) {
            astar_calculate_distances_(ctx, astar, map, pos, npos3, dist + 1);
        }
    } else if (from.x > pos.x) {
        Pos npos1 = pos.move(-1, 0);
        Pos npos2 = pos.move(0, -1);
        Pos npos3 = pos.move(0, 1);
        if (is_walkable(map[npos1])) {
            astar_calculate_distances_(ctx, astar, map, pos, npos1, dist + 1);
        }
        if (is_walkable(map[npos2])) {
            astar_calculate_distances_(ctx, astar, map, pos, npos2, dist + 1);
        }
        if (is_walkable(map[npos3])) {
            astar_calculate_distances_(ctx, astar, map, pos, npos3, dist + 1);
        }
    } else if (from.y < pos.y) {
        Pos npos1 = pos.move(-1, 0);
        Pos npos2 = pos.move(1, 0);
        Pos npos3 = pos.move(0, 1);
        if (is_walkable(map[npos1])) {
            astar_calculate_distances_(ctx, astar, map, pos, npos1, dist + 1);
        }
        if (is_walkable(map[npos2])) {
            astar_calculate_distances_(ctx, astar, map, pos, npos2, dist + 1);
        }
        if (is_walkable(map[npos3])) {
            astar_calculate_distances_(ctx, astar, map, pos, npos3, dist + 1);
        }
    } else if (from.y > pos.y) {
        Pos npos1 = pos.move(-1, 0);
        Pos npos2 = pos.move(1, 0);
        Pos npos3 = pos.move(0, -1);
        if (is_walkable(map[npos1])) {
            astar_calculate_distances_(ctx, astar, map, pos, npos1, dist + 1);
        }
        if (is_walkable(map[npos2])) {
            astar_calculate_distances_(ctx, astar, map, pos, npos2, dist + 1);
        }
        if (is_walkable(map[npos3])) {
            astar_calculate_distances_(ctx, astar, map, pos, npos3, dist + 1);
        }
    } else {
        Pos new_pos[4] = {
            pos.move(-1,  0),
            pos.move( 1,  0),
            pos.move( 0, -1),
            pos.move( 0,  1),
        };
        for (int i = 0; i < 4; i++) {
            Pos npos = new_pos[i];
            if (new_pos[i] != from) {
                if (is_walkable(map[npos])) {
                    astar_calculate_distances_(ctx, astar, map, pos, npos, dist + 1);
                }
            }
        }
    }
}

uint64_t a_star_time_nanos;

void astar_calculate_distances_from_map(Context &ctx, AStar &astar, Map map, Pos pos)
{
    uint64_t start_time = perf_time_nanos();
    ctx_clear_reachable_keys(ctx);
    astar_init(&astar);
    astar_calculate_distances_(ctx, astar, map, pos, pos, 0);
    a_star_time_nanos += perf_time_elapsed_nanos(start_time);
}

void print_astar(AStar &astar)
{
    for (int y = 0; y < astar.height; y++) {
        for (int x = 0; x < astar.width; x++) {
            int v = astar[{x, y}];
            if (v > 99) {
                printf("##");
            } else {
                printf("%2d", v);
            }
        }
        printf("\n");
    }
}

void print_state(Context &ctx) {
    printf("Keys reachable: ");
    for (int i = 0; i < ctx.keys_reachable_n; i++) {
        printf("%c,", ctx.keys_reachable[i].key);
    }
    printf("\n");
}

int open_door(Context &ctx, Map map, char key) {
    for (int i = 0; i < ctx.objects.doors_in_map_n; i++) {
        if (ctx.objects.doors_in_map[i].key == key) {
            map[ctx.objects.doors_in_map[i].position] = '.';
            return i;
        }
    }
    return 0;
}

void close_door(Context &ctx, Map map, int door_index) {
    Door door = ctx.objects.doors_in_map[door_index];
    map[door.position] = door.door;
}

int take_step(Context &initial_st, AStar &astar, Map map, Pos pos, int depth);

int take_key(Context st, AStar &prev_astar, AStar &astar, Map map, int key_index, int depth)
{
    Key key = st.keys_reachable[key_index];
    //printf("Take key %d %c\n", key_index, key.key);

    int door_index = open_door(st, map, key.key);

    int dist = prev_astar[key.position];
    //printf("Dist to key (%d,%d) = %d\n", key.position.x, key.position.y, dist);
    int result = take_step(st, astar, map, key.position, depth) + dist;

    close_door(st, map, door_index);

    return result;
}

unsigned steps;

int take_step(Context &initial_ctx, AStar &astar, Map map, Pos pos, int depth)
{
    steps++;
    char save_c = map[pos];
    map[pos] = '@';
    Context ctx = initial_ctx;

    astar_calculate_distances_from_map(ctx, astar, map, pos);

    //print_astar(astar);
    //print_map(map, st.keys_reachable_bits, pos);
    //print_state(st);

    if (ctx.keys_reachable_n > 0) {
        int memo_result;
        Memo::Key mk = memo_key(pos, ctx.keys_reachable, ctx.keys_reachable_n);
        if (memo_get(ctx.memo, mk, &memo_result)) {
            //printf("memo: %d\n", memo_result);
            map[pos] = save_c;
            return memo_result;
        }

        AStar new_astar = astar_alloc(map.width, map.height);
        int min_dist = take_key(ctx, astar, new_astar, map, 0, depth + 1);
        for (int i = 1; i < ctx.keys_reachable_n; i++) {
            if (depth < 2) printf("%d: %d/%d keys tested\n", depth, i, ctx.keys_reachable_n);
            int dist = take_key(ctx, astar, new_astar, map, i, depth + 1);
            if (dist < min_dist) min_dist = dist;
        }
        astar_free(new_astar);
        //printf("Min dist = %d\n", min_dist);

        memo_put(ctx.memo, mk, min_dist);

        map[pos] = save_c;
        return min_dist;
    } else {
        //memo_put(st.memo, mk, 0);
        map[pos] = save_c;
        return 0;
    }
}

void part_one()
{
    uint64_t start_time = perf_time_nanos();

    //Map map = map_from_string(sample2::map_data);
    //Map map = map_from_string(sample3::map_data);
    Map map = map_from_file("input.txt");


    Objects objects = { };
    
    Memo memo = { };
    Context ctx = { .memo = &memo };
    map_find_objects(&ctx.objects, map);

    printf("map size [%d,%d]\n", map.width, map.height);
    printf("%d keys\n", ctx.objects.keys_in_map_n);
    printf("%d doors\n", ctx.objects.doors_in_map_n);
    printf("start at %d,%d\n", ctx.objects.start_position.x, ctx.objects.start_position.y);

    AStar astar = astar_alloc(map.width, map.height);
    astar_fill_dead_ends_in_map(astar, map, ctx.objects.start_position);

    int result = take_step(ctx, astar, map, ctx.objects.start_position, 0);
    astar_free(astar);

    print_memo(&memo);
    memo_free(&memo);

    uint64_t total_time_nanos = perf_time_elapsed_nanos(start_time);

    printf("Astar %u ms, %u ms total\n",
           (uint32_t)(a_star_time_nanos/1000000), (uint32_t)(total_time_nanos/1000000));
    printf("Part 1 - result: %d - %u steps\n", result, steps);
}

// Part 1 take 2

struct Edge {
    Key dest;
    uint32_t doors_mask;
    int dist;
};

struct Edges {
    Pos from;
    Edge *edges;
    int edges_n;
};

struct Precomp {
    Edges for_start;
    Edges for_keys[26];

    uint32_t all_keys_mask;
};

AStar astar_copy(AStar astar) {
    AStar result = astar_alloc(astar.width, astar.height);
    mempcpy(result.map, astar.map, astar.width * astar.height * sizeof(int));
    return result;
}

void edges_add_edge(Edges *edges, char key, Pos pos, uint32_t doors_mask, int dist) {
    Edge edge {
        .dest {
            .key = key,
            .position = pos,
        },
        .doors_mask = doors_mask,
        .dist = dist,
    };
    int n = edges->edges_n;
    edges->edges_n++;
    edges->edges = (Edge*)realloc(edges->edges, edges->edges_n * sizeof(Edge));
    edges->edges[n] = edge;
}

bool is_traversable(char c) {
    return (c == '.') || (c == '@') || is_key(c) || is_door(c);
}

//struct AStarStack {
//    AStar a[100];
//    int allocated;
//};
//
//AStar& astar_stack_next(AStarStack &stack) {
//
//}
//AStar &astar = astar_stack_current(st);

void astar_to_west(Edges *edges, AStar &astar, Map map, Pos from, uint32_t doors_mask, int dist);
void astar_to_east(Edges *edges, AStar &astar, Map map, Pos from, uint32_t doors_mask, int dist);
void astar_to_north(Edges *edges, AStar &astar, Map map, Pos from, uint32_t doors_mask, int dist);
void astar_to_south(Edges *edges, AStar &astar, Map map, Pos from, uint32_t doors_mask, int dist);

void astar_to_west(Edges *edges, AStar &astar, Map map, Pos from, uint32_t doors_mask, int dist) {
    // ASSUMPTION: map is bordered with walls => No bounds check needed
    Pos pos = from.move(-1, 0);
    char c = map[pos];
    if (c == '#') return;
    //if (!is_traversable(c)) return;

    if (!astar_update_value(astar, pos, dist)) return;

    if (is_key(c)) {
        edges_add_edge(edges, c, pos, doors_mask, dist);
    } else if (is_door(c)) {
        uint32_t door_bit = 1u << (c - 'A');
        doors_mask |= door_bit;
    }

    AStar branch = astar_copy(astar);
    astar_to_west(edges, branch, map, pos, doors_mask, dist + 1);
    astar_to_north(edges, branch, map, pos, doors_mask, dist + 1);
    astar_to_south(edges, branch, map, pos, doors_mask, dist + 1);
    astar_free(branch);
}

void astar_to_east(Edges *edges, AStar &astar, Map map, Pos from, uint32_t doors_mask, int dist) {
    Pos pos = from.move(1, 0);
    if (!is_traversable(map[pos])) return;

    if (!astar_update_value(astar, pos, dist)) return;

    char c = map[pos];
    if (is_key(c)) {
        edges_add_edge(edges, c, pos, doors_mask, dist);
    } else if (is_door(c)) {
        uint32_t door_bit = 1u << (c - 'A');
        doors_mask |= door_bit;
    }

    AStar branch = astar_copy(astar);
    astar_to_east(edges, branch, map, pos, doors_mask, dist + 1);
    astar_to_north(edges, branch, map, pos, doors_mask, dist + 1);
    astar_to_south(edges, branch, map, pos, doors_mask, dist + 1);
    astar_free(branch);
}

void astar_to_north(Edges *edges, AStar &astar, Map map, Pos from, uint32_t doors_mask, int dist) {
    Pos pos = from.move(0, -1);
    if (!is_traversable(map[pos])) return;

    if (!astar_update_value(astar, pos, dist)) return;

    char c = map[pos];
    if (is_key(c)) {
        edges_add_edge(edges, c, pos, doors_mask, dist);
    } else if (is_door(c)) {
        uint32_t door_bit = 1u << (c - 'A');
        doors_mask |= door_bit;
    }

    AStar branch = astar_copy(astar);
    astar_to_north(edges, branch, map, pos, doors_mask, dist + 1);
    astar_to_east(edges, branch, map, pos, doors_mask, dist + 1);
    astar_to_west(edges, branch, map, pos, doors_mask, dist + 1);
    astar_free(branch);
}

void astar_to_south(Edges *edges, AStar &astar, Map map, Pos from, uint32_t doors_mask, int dist) {
    Pos pos = from.move(0, 1);
    if (!is_traversable(map[pos])) return;

    if (!astar_update_value(astar, pos, dist)) return;

    char c = map[pos];
    if (is_key(c)) {
        edges_add_edge(edges, c, pos, doors_mask, dist);
    } else if (is_door(c)) {
        uint32_t door_bit = 1u << (c - 'A');
        doors_mask |= door_bit;
    }

    AStar branch = astar_copy(astar);
    astar_to_south(edges, branch, map, pos, doors_mask, dist + 1);
    astar_to_east(edges, branch, map, pos, doors_mask, dist + 1);
    astar_to_west(edges, branch, map, pos, doors_mask, dist + 1);
    astar_free(branch);
}

bool compare_edge(Edge a, Edge b) {
    if (a.dest.key < b.dest.key) return true;
    else if (a.dest.key == b.dest.key
            && a.dist < b.dist) return true;
    return false;
}
void print_edges(char c, Edges edges);

void astar_calculate_edges(Edges *edges, AStar &astar, Map map, Pos from) {
    astar_init(&astar);

    edges->from = from;
    astar_to_west(edges, astar, map, from, 0u, 1);
    astar_to_east(edges, astar, map, from, 0u, 1);
    astar_to_north(edges, astar, map, from, 0u, 1);
    astar_to_south(edges, astar, map, from, 0u, 1);

    // Sort the edges with descending key and distance
    Edge *es = edges->edges;
    int i = 1;
    while (i < edges->edges_n) {
        Edge x = es[i];
        unsigned ik = (es[i].dest.key*10000 + es[i].dist);
        int j = i - 1;
        while (j >= 0) {
            unsigned jk = (es[j].dest.key*10000 + es[j].dist);
            if (jk <= ik) break;
            es[j + 1] = es[j];
            j--;
        }
        es[j + 1] = x;
        i++;
    }

    // Compact and prune duplicates, keeping the edges for each key and door
    // mask combination with the lowest distance.
    int new_n = edges->edges_n;
    int j = 1;
    for (int i = 0; i < edges->edges_n && j < edges->edges_n; i++) {
        while (j < edges->edges_n) {
            if (es[i].dest.key != es[j].dest.key
                || es[i].doors_mask != es[j].doors_mask) break;

            j++;
            new_n--;
        }

        es[i+1] = es[j];
        j++;
    }
    edges->edges_n = new_n;
}

Precomp precompute(Map map, Objects objects) {
    uint64_t start_time = perf_time_nanos();
    Precomp result = {};

    uint32_t keys_mask = 0u;

    AStar astar = astar_alloc(map.width, map.height);
    uint64_t a_start_time = perf_time_nanos();
    astar_calculate_edges(&result.for_start, astar, map, objects.start_position);
    uint64_t a_elapsed = perf_time_elapsed_nanos(a_start_time);
    printf("'%c' took %d ms\n", '@', (int)(a_elapsed / 1000'000));
    for (int i = 0; i < objects.keys_in_map_n; i++) {
        uint64_t a_start_time = perf_time_nanos();

        Key key = objects.keys_in_map[i];
        int key_index = key.key - 'a';
        keys_mask |= (1u << key_index);
        astar_calculate_edges(&result.for_keys[key_index], astar, map, key.position);

        uint64_t a_elapsed = perf_time_elapsed_nanos(a_start_time);
        printf("'%c' took %d ms\n", key.key, (int)(a_elapsed / 1000'000));
    }
    astar_free(astar);
    result.all_keys_mask = keys_mask;

    uint64_t elapsed = perf_time_elapsed_nanos(start_time);
    printf("Precompute took %d ms\n", (int)(elapsed / 1000000));
    return result;
}

uint32_t door_to_bit(char door) {
    int index = door - 'A';
    return 1u << index;
}

int key_to_index(char key) {
    return key - 'a';
}

int traverse(Memo *memo, const Precomp &pc, char pos, uint32_t door_mask) {
    const Edges *edges;
    if (pos == '@') {
        edges = &pc.for_start;
    } else {
        assert(is_key(pos));
        door_mask |= door_to_bit(key_to_door(pos));
        edges = pc.for_keys + key_to_index(pos);
    
        if (door_mask == pc.all_keys_mask) {
            //char key = 'a';
            //while (door_mask != 0) {
            //    if (door_mask & 1) {
            //        putchar(key);
            //    }
            //    door_mask >>= 1;
            //    key++;
            //}
            //printf("; dist=%d\n", dist);

            //Memo::Key mk = Memo::Key{.pos={pos, 0}, .key_bits=0};
            ////Memo::Key mk = Memo::Key{.pos={pos, 0}, .key_bits=door_mask};
            //memo_put(memo, mk, dist);
            return 0;
        }
    }

    uint32_t reachable_mask = 0u;
    for (int i = 0; i < edges->edges_n; i++) {
        Edge edge = edges->edges[i];
        uint32_t bit = door_to_bit(key_to_door(edge.dest.key));
        bool already_traversed = (door_mask & bit) != 0;
        if (!already_traversed && (edge.doors_mask & door_mask) == edge.doors_mask) {
            reachable_mask |= bit;
        }
    }
    Memo::Key mk = Memo::Key{.pos={pos, 0}, .key_bits=reachable_mask};
    int memo_result;
    if (memo_get(memo, mk, &memo_result)) {
        //print_memo(memo);
        //printf("Memo: %c %x\n", pos, door_mask);
        return memo_result;
    }

    int min_dist = -1;
    for (int i = 0; i < edges->edges_n; i++) {
        Edge edge = edges->edges[i];
        bool already_traversed = (door_mask & door_to_bit(key_to_door(edge.dest.key))) != 0;
        if (!already_traversed && (edge.doors_mask & door_mask) == edge.doors_mask) {
            int d = traverse(memo, pc, edge.dest.key, door_mask) + edge.dist;
            min_dist = (min_dist == -1 || d < min_dist) ? d : min_dist;
        }
    }
    assert(min_dist != -1);

    //char key = 'a';
    //while (door_mask != 0) {
    //    if (door_mask & 1) {
    //        putchar(key);
    //    }
    //    door_mask >>= 1;
    //    key++;
    //}
    //printf("; at=%c dist=%d\n", pos, min_dist);

    memo_put(memo, mk, min_dist);
    return min_dist;
}

void print_edges(char c, Edges edges) {
    printf("From '%c' (%d,%d) (%d edges)\n", c, edges.from.x, edges.from.y, edges.edges_n);
    for (int i = 0; i < edges.edges_n; i++) {
        Edge e = edges.edges[i];

        printf(" to '%c' (%d,%d), distance=%d, doors=",
               e.dest.key, e.dest.position.x, e.dest.position.y, e.dist);

        uint32_t doors_mask = e.doors_mask;
        char door = 'A';
        while (doors_mask != 0) {
            if (doors_mask & 1) {
                putchar(door);
            }
            doors_mask >>= 1;
            door++;
        }
        printf("\n");
    }
}

void precomp_print(const Precomp &precomp) {
    print_edges('@', precomp.for_start);
}

void part_one_take2()
{
    uint64_t start_time = perf_time_nanos();

    //Map map = map_from_string(sample2::map_data);
    //Map map = map_from_string(sample3::map_data);
    Map map = map_from_file("input.txt");

    Objects objects = {};
    map_find_objects(&objects, map);

    if (1) {
        uint64_t start_time = perf_time_nanos();
        AStar astar = astar_alloc(map.width, map.height);
        astar_fill_dead_ends_in_map(astar, map, objects.start_position);
        uint64_t total_time_nanos = perf_time_elapsed_nanos(start_time);
        printf("Filling dead ends took %d ms\n", (int)(total_time_nanos / 1000'000));
    }

    Memo memo = { };
    Precomp precomp = precompute(map, objects);
    //precomp_print(precomp);

    uint64_t traverse_start_time = perf_time_nanos();
    int result = traverse(&memo, precomp, '@', 0u);
    //print_memo(&memo);
    uint64_t traverse_time_nanos = perf_time_elapsed_nanos(traverse_start_time);
    uint64_t total_time_nanos = perf_time_elapsed_nanos(start_time);

    printf("Traverse %u ms, %u ms total\n",
           (uint32_t)(traverse_time_nanos/1000'000), (uint32_t)(total_time_nanos/1000'000));
    printf("Part 1 - result: %d\n", result);
}

int main(int argc, char **argv)
{
    //part_one();
    part_one_take2();
    return 0;
}

