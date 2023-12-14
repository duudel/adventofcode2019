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
//#include <unordered_map>

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

    //struct KeyHash {
    //    std::size_t operator()(const Key &key) const {
    //        return hash(key.key_bits) ^ hash(key.pos.x) ^ hash(key.pos.y);
    //    }
    //};

    //struct KeyEq {
    //    bool operator()(Key a, Key b) const {
    //        return a.key_bits == b.key_bits && a.pos == b.pos;
    //    }
    //};

    //std::unordered_map<Key, int, KeyHash, KeyEq> map;
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

struct Key
{
    char key;
    Pos position;
};

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

/*
void memo_put(Memo *memo, Memo::Key key, int distance) {
    if (memo->values_n + 1 >= memo->values_cap) {
        int new_cap = memo->values_cap * 2 + 4;
        memo->values = (Memo::Value*)realloc(memo->values, new_cap * sizeof(Memo::Value));
        memo->values_cap = new_cap;
    }
    int index = memo->values_n;
    memo->values_n++;
    memo->values[index].key = key;
    memo->values[index].dist = distance;
}

bool memo_get(Memo *memo, Memo::Key key, int *distance) {
    for (int i = 0; i < memo->values_n; i++) {
        Memo::Value v = memo->values[i];
        if (v.key.pos == key.pos &&
            v.key.key_bits == key.key_bits) {

            *distance = v.dist;
            return true;
        }
    }
    return false;
}
*/

void print_memo(Memo *memo) {
    //for (int i = 0; i < memo->values_n; i++) {
    //    Memo::Value v = memo->values[i];
    //    if (v.key.key_bits) {
    //        printf("(%d,%d), keys=%d => %d\n",
    //               v.key.pos.x, v.key.pos.y, v.key.key_bits, v.dist);
    //    }
    //}
    printf("Memo: %d items, %d capacity; hit/miss %.3f, hit%% %.3f\n",
           memo->values_n, memo->values_cap,
           memo_hit_miss_ratio(memo), 100*memo_hit_ratio(memo));
}

struct Door
{
    char door;
    char key;
    Pos position;
};

struct State
{
    Memo *memo;
    Key keys_in_map[26];
    int keys_in_map_n;
    
    Door doors_in_map[26];
    int doors_in_map_n;

    Pos start_position;

    Key keys_reachable[26];
    int keys_reachable_n;
    unsigned int keys_reachable_bits;

    bool ignore_doors;
};

void st_add_reachable_key(State &st, char key_char) {
    unsigned int key_bit = 1u << (key_char - 'a');
    if (st.keys_reachable_bits & key_bit) return;
    for (int i = 0; i < st.keys_in_map_n; i++) {
        if (st.keys_in_map[i].key == key_char) {
            int index = st.keys_reachable_n;
            st.keys_reachable_n++;
            st.keys_reachable[index] = st.keys_in_map[i];
            st.keys_reachable_bits |= key_bit;
            break;
        }
    }
}

void st_clear_reachable_keys(State &st) {
    st.keys_reachable_n = 0;
    st.keys_reachable_bits = 0;
}


bool is_key(char c) { return 'a' <= c && c <= 'z'; }
bool is_door(char c) { return 'A' <= c && c <= 'Z'; }
bool is_walkable(char c) {
    return (c == '.') || (c == '@') || is_key(c);
}

char key_to_door(char key) { return key + 'A' - 'a'; }
char door_to_key(char key) { return key + 'a' - 'A'; }

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

void find_objects(State *state, Map map)
{
    for (int y = 0; y < map.height; y++)
    {
        for (int x = 0; x < map.width; x++)
        {
            char c = map(x, y);
            if (is_key(c)) {
                int index = state->keys_in_map_n;
                state->keys_in_map[index] = Key{
                    .key = c,
                    .position = {x, y},
                };
                state->keys_in_map_n++;
            } else if (is_door(c)) {
                int index = state->doors_in_map_n;
                state->doors_in_map[index] = Door{
                    .door = c,
                    .key = door_to_key(c),
                    .position = {x, y},
                };
                state->doors_in_map_n++;
            } else if (c == '@') {
                state->start_position = {x, y};
            }
        }
    }
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

bool astar_fill_dead_ends_(State &st, AStar &astar, Map map, Pos from, Pos pos, int dist, int &filled);

bool astar_fill_dead_ends_to_(State &st, AStar &astar, Map map, Pos pos, Pos new_pos, int dist, int &filled) {
    if (is_valid_pos(map, new_pos)) {
        char c = map[new_pos];
        if (is_walkable(c)) {
            return astar_fill_dead_ends_(st, astar, map, pos, new_pos, dist + 1, filled);
            //return true;
        } else if (is_door(c)) {
            astar_fill_dead_ends_(st, astar, map, pos, new_pos, dist + 1, filled);
            return true;
        }
    }
    return false;
}

bool astar_fill_dead_ends_(State &st, AStar &astar, Map map, Pos from, Pos pos, int dist, int &filled)
{
    if (!astar_update_value(astar, pos, dist)) return true;
    if (is_key(map[pos])) {
        st_add_reachable_key(st, map[pos]);
    }
    Pos new_pos[4] = {
        pos.move(-1,  0),
        pos.move( 1,  0),
        pos.move( 0, -1),
        pos.move( 0,  1),
    };
    bool valid_path = false;
    for (int i = 0; i < 4; i++) {
        if (new_pos[i] != from) {
            valid_path |= astar_fill_dead_ends_to_(st, astar, map, pos, new_pos[i], dist, filled);
        }
    }
    if (!valid_path) {
        char c = map[pos];
        if (is_key(c) || is_door(c)) return true;
        //map[pos] = '=';
        //print_map(map, st.keys_reachable_bits, {-1,-1});
        //getchar();
        map[pos] = '$';
        filled++;
        return false;
    }
    return true;
}

void astar_fill_dead_ends_in_map(State &st, AStar &astar, Map map, Pos pos)
{
    st_clear_reachable_keys(st);
    while (true) {
        astar_init(&astar);
        int filled = 0;
        astar_fill_dead_ends_(st, astar, map, pos, pos, 0, filled);
        if (filled == 0) break;
        else printf("filled = %d\n", filled);
    }

    //print_map(map, st.keys_reachable_bits, {-1,-1});
    //getchar();
}


void astar_calculate_distances_(State &st, AStar &astar, Map map, Pos from, Pos pos, int dist);

inline void astar_calculate_distances_to_(State &st, AStar &astar, Map map, Pos pos, Pos new_pos, int dist) {
    if (is_valid_pos(map, new_pos)) {
        char c = map[new_pos];
        if (is_walkable(c)) { // || (st.ignore_doors && is_door(c))) {
            astar_calculate_distances_(st, astar, map, pos, new_pos, dist + 1);
        }
    }
}

void astar_calculate_distances_(State &st, AStar &astar, Map map, Pos from, Pos pos, int dist)
{
    if (!astar_update_value(astar, pos, dist)) return;
    if (is_key(map[pos])) {
        st_add_reachable_key(st, map[pos]);
    }
    if (from.x < pos.x) {
        Pos npos1 = pos.move(1, 0);
        Pos npos2 = pos.move(0, -1);
        Pos npos3 = pos.move(0, 1);
        if (is_walkable(map[npos1])) {
            astar_calculate_distances_(st, astar, map, pos, npos1, dist + 1);
        }
        if (is_walkable(map[npos2])) {
            astar_calculate_distances_(st, astar, map, pos, npos2, dist + 1);
        }
        if (is_walkable(map[npos3])) {
            astar_calculate_distances_(st, astar, map, pos, npos3, dist + 1);
        }
    } else if (from.x > pos.x) {
        Pos npos1 = pos.move(-1, 0);
        Pos npos2 = pos.move(0, -1);
        Pos npos3 = pos.move(0, 1);
        if (is_walkable(map[npos1])) {
            astar_calculate_distances_(st, astar, map, pos, npos1, dist + 1);
        }
        if (is_walkable(map[npos2])) {
            astar_calculate_distances_(st, astar, map, pos, npos2, dist + 1);
        }
        if (is_walkable(map[npos3])) {
            astar_calculate_distances_(st, astar, map, pos, npos3, dist + 1);
        }
    } else if (from.y < pos.y) {
        Pos npos1 = pos.move(-1, 0);
        Pos npos2 = pos.move(1, 0);
        Pos npos3 = pos.move(0, 1);
        if (is_walkable(map[npos1])) {
            astar_calculate_distances_(st, astar, map, pos, npos1, dist + 1);
        }
        if (is_walkable(map[npos2])) {
            astar_calculate_distances_(st, astar, map, pos, npos2, dist + 1);
        }
        if (is_walkable(map[npos3])) {
            astar_calculate_distances_(st, astar, map, pos, npos3, dist + 1);
        }
    } else if (from.y > pos.y) {
        Pos npos1 = pos.move(-1, 0);
        Pos npos2 = pos.move(1, 0);
        Pos npos3 = pos.move(0, -1);
        if (is_walkable(map[npos1])) {
            astar_calculate_distances_(st, astar, map, pos, npos1, dist + 1);
        }
        if (is_walkable(map[npos2])) {
            astar_calculate_distances_(st, astar, map, pos, npos2, dist + 1);
        }
        if (is_walkable(map[npos3])) {
            astar_calculate_distances_(st, astar, map, pos, npos3, dist + 1);
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
                //astar_calculate_distances_to_(st, astar, map, pos, new_pos[i], dist);
                //if (is_valid_pos(map, npos) && is_walkable(map[npos])) {
                if (is_walkable(map[npos])) {
                    astar_calculate_distances_(st, astar, map, pos, npos, dist + 1);
                }
            }
        }
    }
}

uint64_t a_star_time_nanos;

void astar_calculate_distances_from_map(State &st, AStar &astar, Map map, Pos pos)
{
    uint64_t start_time = perf_time_nanos();
    st_clear_reachable_keys(st);
    astar_init(&astar);
    astar_calculate_distances_(st, astar, map, pos, pos, 0);
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

void print_state(State &st) {
    printf("Keys reachable: ");
    for (int i = 0; i < st.keys_reachable_n; i++) {
        printf("%c,", st.keys_reachable[i].key);
    }
    printf("\n");
}

int open_door(State &st, Map map, char key) {
    for (int i = 0; i < st.doors_in_map_n; i++) {
        if (st.doors_in_map[i].key == key) {
            map[st.doors_in_map[i].position] = '.';
            return i;
        }
    }
    return 0;
}

void close_door(State &st, Map map, int door_index) {
    Door door = st.doors_in_map[door_index];
    map[door.position] = door.door;
    //for (int i = 0; i < st.doors_in_map_n; i++) {
    //    Door door = st.doors_in_map[i];
    //    if (door.key == key) {
    //        map[door.position] = door.door;
    //        return;
    //    }
    //}
}

int take_step(State &initial_st, AStar &astar, Map map, Pos pos, int depth);

int take_key(State st, AStar &prev_astar, AStar &astar, Map map, int key_index, int depth)
{
    Key key = st.keys_reachable[key_index];
    //printf("Take key %d %c\n", key_index, key.key);

    int door_index = open_door(st, map, key.key);

    int dist = prev_astar[key.position];
    //printf("Dist to key (%d,%d) = %d\n", key.position.x, key.position.y, dist);
    int result;
    if (true) {
        result = take_step(st, astar, map, key.position, depth) + dist;
    } else {
        Map new_map = map_copy(map);
        astar_fill_dead_ends_in_map(st, astar, new_map, key.position);
        result = take_step(st, astar, new_map, key.position, depth) + dist;
        map_free(new_map);
    }

    close_door(st, map, door_index);

    return result;
}

unsigned steps;

int take_step(State &initial_st, AStar &astar, Map map, Pos pos, int depth)
{
    steps++;
    char save_c = map[pos];
    map[pos] = '@';
    State st = initial_st;

    astar_calculate_distances_from_map(st, astar, map, pos);

    //print_astar(astar);
    //print_map(map, st.keys_reachable_bits, pos);
    //print_state(st);

    if (st.keys_reachable_n > 0) {
        int memo_result;
        Memo::Key mk = memo_key(pos, st.keys_reachable, st.keys_reachable_n);
        if (memo_get(st.memo, mk, &memo_result)) {
            //printf("memo: %d\n", memo_result);
            map[pos] = save_c;
            return memo_result;
        }

        AStar new_astar = astar_alloc(map.width, map.height);
        int min_dist = take_key(st, astar, new_astar, map, 0, depth + 1);
        for (int i = 1; i < st.keys_reachable_n; i++) {
            if (depth < 2) printf("%d: %d/%d keys tested\n", depth, i, st.keys_reachable_n);
            int dist = take_key(st, astar, new_astar, map, i, depth + 1);
            if (dist < min_dist) min_dist = dist;
        }
        astar_free(new_astar);
        //printf("Min dist = %d\n", min_dist);

        memo_put(st.memo, mk, min_dist);

        map[pos] = save_c;
        return min_dist;
    } else {
        //memo_put(st.memo, mk, 0);
        map[pos] = save_c;
        return 0;
    }
}

struct PrecompAstar {
    AStar astar_for_keys[26];
};

PrecompAstar precomp_astar(State st, Map map) {
    st.ignore_doors = true;
    PrecompAstar result = {};
    for (int i = 0; i < st.keys_in_map_n; i++) {
        Key key = st.keys_in_map[i];
        int key_index = key.key - 'a';
        result.astar_for_keys[key_index] = astar_alloc(map.width, map.height);
        astar_calculate_distances_from_map(
            st,
            result.astar_for_keys[key_index],
            map,
            key.position);
    }
    return result;
}

void part_one()
{
    uint64_t start_time = perf_time_nanos();

    //Map map = map_from_string(sample3::map_data);
    Map map = map_from_file("input.txt");
    Memo memo = {
        //.map = std::unordered_map<Memo::Key, int, Memo::KeyHash, Memo::KeyEq>()
    };
    State state = { .memo = &memo };
    find_objects(&state, map);

    printf("map size [%d,%d]\n", map.width, map.height);
    printf("%d keys\n", state.keys_in_map_n);
    printf("%d doors\n", state.doors_in_map_n);
    printf("start at %d,%d\n", state.start_position.x, state.start_position.y);

    AStar astar = astar_alloc(map.width, map.height);
    astar_fill_dead_ends_in_map(state, astar, map, state.start_position);

    int result = take_step(state, astar, map, state.start_position, 0);
    astar_free(astar);

    print_memo(&memo);
    memo_free(&memo);

    uint64_t total_time_nanos = perf_time_elapsed_nanos(start_time);

    printf("Astar %u ms, %u ms total\n",
           (uint32_t)(a_star_time_nanos/1000000), (uint32_t)(total_time_nanos/1000000));
    printf("Part 1 - result: %d - %u steps\n", result, steps);
}

int main(int argc, char **argv)
{
    part_one();
    return 0;
}

