/**
--- Day 20: Donut Maze ---

You notice a strange pattern on the surface of Pluto and land nearby to get a
closer look. Upon closer inspection, you realize you've come across one of the
famous space-warping mazes of the long-lost Pluto civilization!

Because there isn't much space on Pluto, the civilization that used to live here
thrived by inventing a method for folding spacetime. Although the technology is
no longer understood, mazes like this one provide a small glimpse into the daily
life of an ancient Pluto citizen.

This maze is shaped like a donut. Portals along the inner and outer edge of the
donut can instantly teleport you from one side to the other. For example:

         A
         A
  #######.#########
  #######.........#
  #######.#######.#
  #######.#######.#
  #######.#######.#
  #####  B    ###.#
BC...##  C    ###.#
  ##.##       ###.#
  ##...DE  F  ###.#
  #####    G  ###.#
  #########.#####.#
DE..#######...###.#
  #.#########.###.#
FG..#########.....#
  ###########.#####
             Z
             Z

This map of the maze shows solid walls (#) and open passages (.). Every maze on
Pluto has a start (the open tile next to AA) and an end (the open tile next to
ZZ). Mazes on Pluto also have portals; this maze has three pairs of portals: BC,
DE, and FG. When on an open tile next to one of these labels, a single step can
take you to the other tile with the same label. (You can only walk on . tiles;
labels and empty space are not traversable.)

One path through the maze doesn't require any portals. Starting at AA, you could
go down 1, right 8, down 12, left 4, and down 1 to reach ZZ, a total of 26
steps.

However, there is a shorter path: You could walk from AA to the inner BC portal
(4 steps), warp to the outer BC portal (1 step), walk to the inner DE (6 steps),
warp to the outer DE (1 step), walk to the outer FG (4 steps), warp to the inner
FG (1 step), and finally walk to ZZ (6 steps). In total, this is only 23 steps.

Here is a larger example:

                   A
                   A
  #################.#############
  #.#...#...................#.#.#
  #.#.#.###.###.###.#########.#.#
  #.#.#.......#...#.....#.#.#...#
  #.#########.###.#####.#.#.###.#
  #.............#.#.....#.......#
  ###.###########.###.#####.#.#.#
  #.....#        A   C    #.#.#.#
  #######        S   P    #####.#
  #.#...#                 #......VT
  #.#.#.#                 #.#####
  #...#.#               YN....#.#
  #.###.#                 #####.#
DI....#.#                 #.....#
  #####.#                 #.###.#
ZZ......#               QG....#..AS
  ###.###                 #######
JO..#.#.#                 #.....#
  #.#.#.#                 ###.#.#
  #...#..DI             BU....#..LF
  #####.#                 #.#####
YN......#               VT..#....QG
  #.###.#                 #.###.#
  #.#...#                 #.....#
  ###.###    J L     J    #.#.###
  #.....#    O F     P    #.#...#
  #.###.#####.#.#####.#####.###.#
  #...#.#.#...#.....#.....#.#...#
  #.#####.###.###.#.#.#########.#
  #...#.#.....#...#.#.#.#.....#.#
  #.###.#####.###.###.#.#.#######
  #.#.........#...#.............#
  #########.###.###.#############
           B   J   C
           U   P   P

Here, AA has no direct path to ZZ, but it does connect to AS and CP. By passing
through AS, QG, BU, and JO, you can reach ZZ in 58 steps.

In your maze, how many steps does it take to get from the open tile marked AA to
the open tile marked ZZ?

Your puzzle answer was 498.

--- Part Two ---

Strangely, the exit isn't open when you reach it. Then, you remember: the
ancient Plutonians were famous for building recursive spaces.

The marked connections in the maze aren't portals: they physically connect to a
larger or smaller copy of the maze. Specifically, the labeled tiles around the
inside edge actually connect to a smaller copy of the same maze, and the smaller
copy's inner labeled tiles connect to yet a smaller copy, and so on.

When you enter the maze, you are at the outermost level; when at the outermost
level, only the outer labels AA and ZZ function (as the start and end,
respectively); all other outer labeled tiles are effectively walls. At any other
level, AA and ZZ count as walls, but the other outer labeled tiles bring you one
level outward.

Your goal is to find a path through the maze that brings you back to ZZ at the
outermost level of the maze.

In the first example above, the shortest path is now the loop around the right
side. If the starting level is 0, then taking the previously-shortest path would
pass through BC (to level 1), DE (to level 2), and FG (back to level 1). Because
this is not the outermost level, ZZ is a wall, and the only option is to go back
around to BC, which would only send you even deeper into the recursive maze.

In the second example above, there is no path that brings you to ZZ at the
outermost level.

Here is a more interesting example:

             Z L X W       C
             Z P Q B       K
  ###########.#.#.#.#######.###############
  #...#.......#.#.......#.#.......#.#.#...#
  ###.#.#.#.#.#.#.#.###.#.#.#######.#.#.###
  #.#...#.#.#...#.#.#...#...#...#.#.......#
  #.###.#######.###.###.#.###.###.#.#######
  #...#.......#.#...#...#.............#...#
  #.#########.#######.#.#######.#######.###
  #...#.#    F       R I       Z    #.#.#.#
  #.###.#    D       E C       H    #.#.#.#
  #.#...#                           #...#.#
  #.###.#                           #.###.#
  #.#....OA                       WB..#.#..ZH
  #.###.#                           #.#.#.#
CJ......#                           #.....#
  #######                           #######
  #.#....CK                         #......IC
  #.###.#                           #.###.#
  #.....#                           #...#.#
  ###.###                           #.#.#.#
XF....#.#                         RF..#.#.#
  #####.#                           #######
  #......CJ                       NM..#...#
  ###.#.#                           #.###.#
RE....#.#                           #......RF
  ###.###        X   X       L      #.#.#.#
  #.....#        F   Q       P      #.#.#.#
  ###.###########.###.#######.#########.###
  #.....#...#.....#.......#...#.....#.#...#
  #####.#.###.#######.#######.###.###.#.#.#
  #.......#.......#.#.#.#.#...#...#...#.#.#
  #####.###.#####.#.#.#.#.###.###.#.###.###
  #.......#.....#.#...#...............#...#
  #############.#.#.###.###################
               A O F   N
               A A D   M

One shortest path through the maze is the following:

    Walk from AA to XF (16 steps)
    Recurse into level 1 through XF (1 step)
    Walk from XF to CK (10 steps)
    Recurse into level 2 through CK (1 step)
    Walk from CK to ZH (14 steps)
    Recurse into level 3 through ZH (1 step)
    Walk from ZH to WB (10 steps)
    Recurse into level 4 through WB (1 step)
    Walk from WB to IC (10 steps)
    Recurse into level 5 through IC (1 step)
    Walk from IC to RF (10 steps)
    Recurse into level 6 through RF (1 step)
    Walk from RF to NM (8 steps)
    Recurse into level 7 through NM (1 step)
    Walk from NM to LP (12 steps)
    Recurse into level 8 through LP (1 step)
    Walk from LP to FD (24 steps)
    Recurse into level 9 through FD (1 step)
    Walk from FD to XQ (8 steps)
    Recurse into level 10 through XQ (1 step)
    Walk from XQ to WB (4 steps)
    Return to level 9 through WB (1 step)
    Walk from WB to ZH (10 steps)
    Return to level 8 through ZH (1 step)
    Walk from ZH to CK (14 steps)
    Return to level 7 through CK (1 step)
    Walk from CK to XF (10 steps)
    Return to level 6 through XF (1 step)
    Walk from XF to OA (14 steps)
    Return to level 5 through OA (1 step)
    Walk from OA to CJ (8 steps)
    Return to level 4 through CJ (1 step)
    Walk from CJ to RE (8 steps)
    Return to level 3 through RE (1 step)
    Walk from RE to IC (4 steps)
    Recurse into level 4 through IC (1 step)
    Walk from IC to RF (10 steps)
    Recurse into level 5 through RF (1 step)
    Walk from RF to NM (8 steps)
    Recurse into level 6 through NM (1 step)
    Walk from NM to LP (12 steps)
    Recurse into level 7 through LP (1 step)
    Walk from LP to FD (24 steps)
    Recurse into level 8 through FD (1 step)
    Walk from FD to XQ (8 steps)
    Recurse into level 9 through XQ (1 step)
    Walk from XQ to WB (4 steps)
    Return to level 8 through WB (1 step)
    Walk from WB to ZH (10 steps)
    Return to level 7 through ZH (1 step)
    Walk from ZH to CK (14 steps)
    Return to level 6 through CK (1 step)
    Walk from CK to XF (10 steps)
    Return to level 5 through XF (1 step)
    Walk from XF to OA (14 steps)
    Return to level 4 through OA (1 step)
    Walk from OA to CJ (8 steps)
    Return to level 3 through CJ (1 step)
    Walk from CJ to RE (8 steps)
    Return to level 2 through RE (1 step)
    Walk from RE to XQ (14 steps)
    Return to level 1 through XQ (1 step)
    Walk from XQ to FD (8 steps)
    Return to level 0 through FD (1 step)
    Walk from FD to ZZ (18 steps)

This path takes a total of 396 steps to move from AA at the outermost layer to
ZZ at the outermost layer.

In your maze, when accounting for recursion, how many steps does it take to get
from the open tile marked AA to the open tile marked ZZ, both at the outermost
layer?

Your puzzle answer was 5564.
*/

#include <cstdint>
#include <climits>
#include <ctime>

unsigned int min(unsigned int a, unsigned int b) {
    return a < b ? a : b;
}

unsigned int max(unsigned int a, unsigned int b) {
    return a > b ? a : b;
}

uint64_t perf_time_nanos() {
    timespec ts;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
    return ts.tv_sec * 1000'000'000ULL + ts.tv_nsec;
}


uint64_t perf_time_elapsed_nanos(uint64_t start_time_nanos) {
    return perf_time_nanos() - start_time_nanos;
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

bool is_teleport(char c) { return 'A' <= c && c <= 'Z'; }

void print_map(Map map, unsigned int keys_reachable_bits, Pos pos) {
    for (int y = 0; y < map.height; y++) {
        for (int x = 0; x < map.width; x++) {
            char c = map[{x, y}];
            if (pos == Pos{x, y}) {
                printf("\e[31;1m");
                putchar('@');
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

enum Dir {
    DIR_north,
    DIR_east,
    DIR_south,
    DIR_west,
};

struct Teleports {
    typedef uint16_t Label;
    struct Teleport {
        Label label;
        Pos pos;
        Dir teleport_dir; // in which direction teleport is from pos
        bool outer;
    };
    Teleport teleports[100];
    int teleports_n;
    
    Pos start_pos;
    Pos goal_pos;
};

void map_find_teleports(Teleports *tp, Map map)
{
    // horizontal labels
    for (int y = 0; y < map.height; y++)
    {
        for (int x = 0; x < map.width - 1; x++)
        {
            char c1 = map(x, y);
            char c2 = map(x + 1, y);
            bool t1 = is_teleport(c1);
            bool t2 = is_teleport(c2);
            if (!t1 && !t2) x++;
            else if (t1 && t2) {
                Pos pos;
                Dir dir;
                bool outer;
                if (x > 0 && map(x - 1, y) == '.') {
                    pos = Pos{x - 1, y};
                    dir = DIR_east;
                    outer = (x == map.width - 2);
                } else {
                    pos = Pos{x + 2, y};
                    dir = DIR_west;
                    outer = (x == 0);
                }
                Teleports::Label label = ((uint16_t)c1 << 8) | c2;
                int index = tp->teleports_n;
                tp->teleports[index] = Teleports::Teleport{
                    .label = label,
                    .pos = pos,
                    .teleport_dir = dir,
                    .outer = outer,
                };
                tp->teleports_n++;
            }
        }
    }

    // vertical labels
    for (int x = 0; x < map.width; x++)
    {
        for (int y = 0; y < map.height - 1; y++)
        {
            char c1 = map(x, y);
            char c2 = map(x, y + 1);
            bool t1 = is_teleport(c1);
            bool t2 = is_teleport(c2);
            if (!t1 && !t2) y++;
            else if (t1 && t2) {
                Pos pos;
                Dir dir;
                bool outer;
                if (y > 0 && map(x, y - 1) == '.') {
                    pos = Pos{x, y - 1};
                    dir = DIR_south;
                    outer = (y == map.height - 2);
                } else {
                    pos = Pos{x, y + 2};
                    dir = DIR_north;
                    outer = (y == 0);
                }
                Teleports::Label label = ((uint16_t)c1 << 8) | c2;
                int index = tp->teleports_n;
                tp->teleports[index] = Teleports::Teleport{
                    .label = label,
                    .pos = pos,
                    .teleport_dir = dir,
                    .outer = outer,
                };
                tp->teleports_n++;
            }
        }
    }

    // Sort teleports by label -> AA will be first and ZZ last
    for (int i = 1; i < tp->teleports_n; i++) {
        Teleports::Teleport t = tp->teleports[i];
        int j = i - 1;
        for (; j >= 0; j--) {
            if (tp->teleports[j].label <= t.label) break;
            tp->teleports[j + 1] = tp->teleports[j];
        }
        tp->teleports[j + 1] = t;
    }

    tp->start_pos = tp->teleports[0].pos;
    tp->goal_pos = tp->teleports[tp->teleports_n - 1].pos;

    //printf("%d teleports found!\n", tp->teleports_n);
    //for (int i = 0; i < tp->teleports_n; i++) {
    //    auto teleport = tp->teleports[i];
    //    uint16_t label = teleport.label;
    //    char label_chars[3] = { (char)(label >> 8), (char)(label & 0xff) };
    //    printf("%s at (%d,%d) %s, ",
    //           label_chars, teleport.pos.x, teleport.pos.y,
    //           (teleport.outer ? "outer" : "inner"));
    //}
    //printf("\n");
}

typedef uint16_t ASint;

struct AStar {
    int width, height;
    ASint *map;

    ASint& operator[](Pos pos) {
        //if (!(0 <= pos.x && pos.x < width)) printf("INVALID A* ACCESS [%d,%d]: pos=%d,%d\n", width, height, pos.x, pos.y);
        //if (!(0 <= pos.y && pos.y < height)) printf("INVALID A* ACCESS [%d,%d]: pos=%d,%d\n", width, height, pos.x, pos.y);
        return map[pos.y * width + pos.x];
    }
};

AStar astar_alloc(int width, int height) {
    ASint *data = (ASint*)calloc(width * height, sizeof(ASint));
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
    for (int i = 0; i < N; i++) astar->map[i] = 9999;
}

bool astar_update_value(AStar &astar, Pos pos, int new_value)
{
    int i = astar.width * pos.y + pos.x;
    int v = astar.map[i];
    //int v = astar[pos];
    if (new_value < v) {
        astar.map[i] = new_value;
        //astar[pos] = new_value;
        return true;
    }
    return false;
}

bool is_valid_pos(Map map, Pos pos) {
    return (pos.x >= 0 && pos.x < map.width) && (pos.y >= 0 && pos.y < map.height);
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

// Part 1

struct AStarContext {
    Teleports *tp;
};

bool find_teleport_destination(Teleports &tp, Pos from, Teleports::Teleport *result, int *index = nullptr) {
    for (int i = 0; i < tp.teleports_n; i++) {
        Teleports::Teleport ti = tp.teleports[i];
        if (ti.pos == from) {
            char label[3] = { (char)(ti.label >> 8), (char)(ti.label & 0xff) };
            //printf("to teleport %s\n", label);
            if (i - 1 >= 0 && tp.teleports[i - 1].label == ti.label) {
                *result = tp.teleports[i - 1];
                if (index) *index = i - 1;
                return true;
            } else if (i + 1 < tp.teleports_n - 1 && tp.teleports[i + 1].label == ti.label) {
                *result = tp.teleports[i + 1];
                if (index) *index = i + 1;
                return true;
            } else {
                *result = ti;
                if (index) *index = i;
                return true;
            }
        }
    }
    return false;
}

void astar_to_west(AStarContext &ctx, AStar &astar, Map map, Pos from, int dist);
void astar_to_east(AStarContext &ctx, AStar &astar, Map map, Pos from, int dist);
void astar_to_north(AStarContext &ctx, AStar &astar, Map map, Pos from, int dist);
void astar_to_south(AStarContext &ctx, AStar &astar, Map map, Pos from, int dist);

void astar_from_teleport(AStarContext &ctx, AStar &astar, Map map, Teleports::Teleport from, int dist) {
    char label[3] = { (char)(from.label >> 8), (char)(from.label & 0xff) };
    //printf("from teleport %s: dist = %d\n", label, dist);
    if (!astar_update_value(astar, from.pos, dist)) return;
    switch (from.teleport_dir) {
        case DIR_north:
            astar_to_south(ctx, astar, map, from.pos, dist + 1);
            astar_to_west(ctx, astar, map, from.pos, dist + 1);
            astar_to_east(ctx, astar, map, from.pos, dist + 1);
            break;
        case DIR_east:
            astar_to_west(ctx, astar, map, from.pos, dist + 1);
            astar_to_north(ctx, astar, map, from.pos, dist + 1);
            astar_to_south(ctx, astar, map, from.pos, dist + 1);
            break;
        case DIR_south:
            astar_to_north(ctx, astar, map, from.pos, dist + 1);
            astar_to_west(ctx, astar, map, from.pos, dist + 1);
            astar_to_east(ctx, astar, map, from.pos, dist + 1);
            break;
        case DIR_west:
            astar_to_east(ctx, astar, map, from.pos, dist + 1);
            astar_to_north(ctx, astar, map, from.pos, dist + 1);
            astar_to_south(ctx, astar, map, from.pos, dist + 1);
            break;
    }
}

void astar_to_west(AStarContext &ctx, AStar &astar, Map map, Pos from, int dist) {
    // ASSUMPTION: map is bordered with walls => No bounds check needed
    Pos pos = from.move(-1, 0);
    char c = map[pos];
    if (c == '#') return;
    if (is_teleport(c)) {
        if (c == 'Z') return; // Found goal
        Teleports::Teleport to;
        find_teleport_destination(*ctx.tp, from, &to);
        astar_from_teleport(ctx, astar, map, to, dist);
        return;
    }

    if (!astar_update_value(astar, pos, dist)) return;

    astar_to_west(ctx, astar, map, pos, dist + 1);
    astar_to_north(ctx, astar, map, pos, dist + 1);
    astar_to_south(ctx, astar, map, pos, dist + 1);
}

void astar_to_east(AStarContext &ctx, AStar &astar, Map map, Pos from, int dist) {
    // ASSUMPTION: map is bordered with walls => No bounds check needed
    Pos pos = from.move(1, 0);
    char c = map[pos];
    if (c == '#') return;
    if (is_teleport(c)) {
        if (c == 'Z') return; // Found goal
        Teleports::Teleport to;
        find_teleport_destination(*ctx.tp, from, &to);
        astar_from_teleport(ctx, astar, map, to, dist);
        return;
    }

    if (!astar_update_value(astar, pos, dist)) return;

    astar_to_east(ctx, astar, map, pos, dist + 1);
    astar_to_north(ctx, astar, map, pos,dist + 1);
    astar_to_south(ctx, astar, map, pos, dist + 1);
}

void astar_to_north(AStarContext &ctx, AStar &astar, Map map, Pos from, int dist) {
    // ASSUMPTION: map is bordered with walls => No bounds check needed
    Pos pos = from.move(0, -1);
    char c = map[pos];
    if (c == '#') return;
    if (is_teleport(c)) {
        if (c == 'Z') return; // Found goal
        Teleports::Teleport to;
        find_teleport_destination(*ctx.tp, from, &to);
        astar_from_teleport(ctx, astar, map, to, dist);
        return;
    }

    if (!astar_update_value(astar, pos, dist)) return;

    astar_to_north(ctx, astar, map, pos, dist + 1);
    astar_to_east(ctx, astar, map, pos, dist + 1);
    astar_to_west(ctx, astar, map, pos, dist + 1);
}

void astar_to_south(AStarContext &ctx, AStar &astar, Map map, Pos from, int dist) {
    // ASSUMPTION: map is bordered with walls => No bounds check needed
    Pos pos = from.move(0, 1);
    char c = map[pos];
    if (c == '#') return;
    if (is_teleport(c)) {
        if (c == 'Z') return; // Found goal
        Teleports::Teleport to;
        find_teleport_destination(*ctx.tp, from, &to);
        astar_from_teleport(ctx, astar, map, to, dist);
        return;
    }

    if (!astar_update_value(astar, pos, dist)) return;

    astar_to_south(ctx, astar, map, pos, dist + 1);
    astar_to_east(ctx, astar, map, pos, dist + 1);
    astar_to_west(ctx, astar, map, pos, dist + 1);
}

void astar_calculate_distance_to_goal(AStarContext &ctx, AStar &astar, Map map, Teleports::Teleport from) {
    astar_from_teleport(ctx, astar, map, from, 0);
}

void part_one()
{
    uint64_t start_time = perf_time_nanos();

    //Map map = map_from_file("input-example.txt");
    Map map = map_from_file("input.txt");

    Teleports teleports = { };
    
    map_find_teleports(&teleports, map);

    printf("map size [%d,%d]\n", map.width, map.height);
    printf("start at (%d,%d)\n", teleports.start_pos.x, teleports.start_pos.y);
    printf("goal at (%d,%d)\n", teleports.goal_pos.x, teleports.goal_pos.y);

    AStarContext ctx = {};
    ctx.tp = &teleports;
    AStar astar = astar_alloc(map.width, map.height);
    astar_init(&astar);
    astar_calculate_distance_to_goal(ctx, astar, map, teleports.teleports[0]);
    int distance = astar[teleports.goal_pos];
    astar_free(astar);

    int result = distance;

    uint64_t total_time_nanos = perf_time_elapsed_nanos(start_time);
    printf("Time: %u us total\n", (uint32_t)(total_time_nanos/1000));
    printf("Part 1 - result: %d\n", result);
}

// Part 2

struct Edge {
    Teleports::Teleport dest;
    int teleport_index;
    int dist;
};

struct Edges {
    Edge *edges;
    int edges_n;
};

void edges_add_edge(Edges *edges, Teleports::Teleport teleport, int teleport_index, int dist) {
    Edge edge {
        .dest = teleport,
        .teleport_index = teleport_index,
        .dist = dist,
    };
    int n = edges->edges_n;
    edges->edges_n++;
    if ((edges->edges_n & (edges->edges_n - 1)) == 0) {
        // is power of 2, lets resize
        int next_cap = edges->edges_n << 1;
        edges->edges = (Edge*)realloc(edges->edges, next_cap * sizeof(Edge));
    }
    edges->edges[n] = edge;
}

void print_edges(Teleports::Teleport t, Edges edges) {
    char label[3] = { (char)(t.label >> 8), (char)(t.label & 0xff) }; 
    printf("From '%s' %s (%d,%d) (%d edges)\n",
           label, (t.outer ? "outer" : "inner"), t.pos.x, t.pos.y, edges.edges_n);
    for (int i = 0; i < edges.edges_n; i++) {
        Edge e = edges.edges[i];

        char label[3] = { (char)(e.dest.label >> 8), (char)(e.dest.label & 0xff) }; 
        printf(" to '%s' %s (%d,%d), distance=%d\n",
               label, (e.dest.outer ? "outer" : "inner"), e.dest.pos.x, e.dest.pos.y, e.dist);
    }
}

struct AStarContext2 {
    Edges *edges;
    Teleports *tp;
};

bool find_teleport_destination(Teleports &tp, Pos from, Teleports::Teleport *via, Teleports::Teleport *to, int *to_index) {
    for (int i = 0; i < tp.teleports_n; i++) {
        Teleports::Teleport ti = tp.teleports[i];
        if (ti.pos == from) {
            char label[3] = { (char)(ti.label >> 8), (char)(ti.label & 0xff) };
            //printf("to teleport %s\n", label);
            if (i - 1 >= 0 && tp.teleports[i - 1].label == ti.label) {
                *via = ti;
                *to = tp.teleports[i - 1];
                *to_index = i - 1;
                return true;
            } else if (i + 1 < tp.teleports_n - 1 && tp.teleports[i + 1].label == ti.label) {
                *via = ti;
                *to = tp.teleports[i + 1];
                *to_index = i + 1;
                return true;
            } else {
                *via = ti,
                *to = ti;
                *to_index = i;
                return true;
            }
        }
    }
    return false;
}

void astar_to_west2(AStarContext2 &ctx, AStar &astar, Map map, Pos from, int dist);
void astar_to_east2(AStarContext2 &ctx, AStar &astar, Map map, Pos from, int dist);
void astar_to_north2(AStarContext2 &ctx, AStar &astar, Map map, Pos from, int dist);
void astar_to_south2(AStarContext2 &ctx, AStar &astar, Map map, Pos from, int dist);

Teleports::Label start_label = ((uint16_t)'A' << 8) | (uint16_t)'A';
Teleports::Label goal_label = ((uint16_t)'Z' << 8) | (uint16_t)'Z';

void astar_to_west2(AStarContext2 &ctx, AStar &astar, Map map, Pos from, int dist) {
    // ASSUMPTION: map is bordered with walls => No bounds check needed
    Pos pos = from.move(-1, 0);
    char c = map[pos];
    if (c == '#') return;
    if (is_teleport(c)) {
        Teleports::Teleport to, via;
        int teleport_index;
        if (find_teleport_destination(*ctx.tp, from, &via, &to, &teleport_index)) {
            if (via.label == goal_label) {
                edges_add_edge(ctx.edges, to, teleport_index, dist - 1);
            } else {
                to.outer = via.outer;
                edges_add_edge(ctx.edges, to, teleport_index, dist);
            }
        }
    } else {
        if (!astar_update_value(astar, pos, dist)) return;

        astar_to_west2(ctx, astar, map, pos, dist + 1);
        astar_to_north2(ctx, astar, map, pos, dist + 1);
        astar_to_south2(ctx, astar, map, pos, dist + 1);
    }
}

void astar_to_east2(AStarContext2 &ctx, AStar &astar, Map map, Pos from, int dist) {
    // ASSUMPTION: map is bordered with walls => No bounds check needed
    Pos pos = from.move(1, 0);
    char c = map[pos];
    if (c == '#') return;
    if (is_teleport(c)) {
        Teleports::Teleport to, via;
        int teleport_index;
        if (find_teleport_destination(*ctx.tp, from, &via, &to, &teleport_index)) {
            if (via.label == goal_label) {
                edges_add_edge(ctx.edges, to, teleport_index, dist - 1);
            } else {
                to.outer = via.outer;
                edges_add_edge(ctx.edges, to, teleport_index, dist);
            }
        }
    } else {
        if (!astar_update_value(astar, pos, dist)) return;

        astar_to_east2(ctx, astar, map, pos, dist + 1);
        astar_to_north2(ctx, astar, map, pos,dist + 1);
        astar_to_south2(ctx, astar, map, pos, dist + 1);
    }
}

void astar_to_north2(AStarContext2 &ctx, AStar &astar, Map map, Pos from, int dist) {
    // ASSUMPTION: map is bordered with walls => No bounds check needed
    Pos pos = from.move(0, -1);
    char c = map[pos];
    if (c == '#') return;
    if (is_teleport(c)) {
        Teleports::Teleport to, via;
        int teleport_index;
        if (find_teleport_destination(*ctx.tp, from, &via, &to, &teleport_index)) {
            if (via.label == goal_label) {
                edges_add_edge(ctx.edges, to, teleport_index, dist - 1);
            } else {
                to.outer = via.outer;
                edges_add_edge(ctx.edges, to, teleport_index, dist);
            }
        }
    } else {
        if (!astar_update_value(astar, pos, dist)) return;

        astar_to_north2(ctx, astar, map, pos, dist + 1);
        astar_to_east2(ctx, astar, map, pos, dist + 1);
        astar_to_west2(ctx, astar, map, pos, dist + 1);
    }
}

void astar_to_south2(AStarContext2 &ctx, AStar &astar, Map map, Pos from, int dist) {
    // ASSUMPTION: map is bordered with walls => No bounds check needed
    Pos pos = from.move(0, 1);
    char c = map[pos];
    if (c == '#') return;
    if (is_teleport(c)) {
        Teleports::Teleport to, via;
        int teleport_index;
        if (find_teleport_destination(*ctx.tp, from, &via, &to, &teleport_index)) {
            if (via.label == goal_label) {
                edges_add_edge(ctx.edges, to, teleport_index, dist - 1);
            } else {
                to.outer = via.outer;
                edges_add_edge(ctx.edges, to, teleport_index, dist);
            }
        }
    } else {
        if (!astar_update_value(astar, pos, dist)) return;

        astar_to_south2(ctx, astar, map, pos, dist + 1);
        astar_to_east2(ctx, astar, map, pos, dist + 1);
        astar_to_west2(ctx, astar, map, pos, dist + 1);
    }
}

void astar_from_teleport2(AStarContext2 &ctx, AStar &astar, Map map, Teleports::Teleport from, int dist) {
    //char label[3] = { (char)(from.label >> 8), (char)(from.label & 0xff) };
    //printf("from teleport %s: dist = %d\n", label, dist);
    if (!astar_update_value(astar, from.pos, dist)) return;
    switch (from.teleport_dir) {
        case DIR_north:
            astar_to_south2(ctx, astar, map, from.pos, dist + 1);
            astar_to_west2(ctx, astar, map, from.pos, dist + 1);
            astar_to_east2(ctx, astar, map, from.pos, dist + 1);
            break;
        case DIR_east:
            astar_to_west2(ctx, astar, map, from.pos, dist + 1);
            astar_to_north2(ctx, astar, map, from.pos, dist + 1);
            astar_to_south2(ctx, astar, map, from.pos, dist + 1);
            break;
        case DIR_south:
            astar_to_north2(ctx, astar, map, from.pos, dist + 1);
            astar_to_west2(ctx, astar, map, from.pos, dist + 1);
            astar_to_east2(ctx, astar, map, from.pos, dist + 1);
            break;
        case DIR_west:
            astar_to_east2(ctx, astar, map, from.pos, dist + 1);
            astar_to_north2(ctx, astar, map, from.pos, dist + 1);
            astar_to_south2(ctx, astar, map, from.pos, dist + 1);
            break;
    }
}

void astar_calculate_edges(AStarContext2 ctx, AStar &astar, Map map, Teleports::Teleport from) {
    astar_init(&astar);
    astar_from_teleport2(ctx, astar, map, from, 0);

    // Sort the edges with descending key and distance
    Edge *es = ctx.edges->edges;
    int i = 1;
    while (i < ctx.edges->edges_n) {
        Edge x = es[i];
        unsigned ik = es[i].dist;
        int j = i - 1;
        while (j >= 0) {
            unsigned jk = es[j].dist;
            if (jk <= ik) break;
            es[j + 1] = es[j];
            j--;
        }
        es[j + 1] = x;
        i++;
    }
}

struct Precomp {
    // by teleport index
    Teleports *tp;
    Edges for_teleports[100];
};

Precomp precompute(Map map, Teleports *tp) {
    uint64_t start_time = perf_time_nanos();
    Precomp result = {
        .tp = tp,
    };

    AStar astar = astar_alloc(map.width, map.height);
    for (int i = 0; i < tp->teleports_n - 1; i++) {
        uint64_t a_start_time = perf_time_nanos();

        AStarContext2 ctx = {
            .edges = &result.for_teleports[i],
            .tp = tp,
        };
        astar_calculate_edges(ctx, astar, map, tp->teleports[i]);

        uint64_t a_elapsed = perf_time_elapsed_nanos(a_start_time);
        //printf("'%c' took %d us\n", key.key, (int)(a_elapsed / 1000));
    }
    astar_free(astar);

    uint64_t elapsed = perf_time_elapsed_nanos(start_time);
    printf("Precompute took %d us\n", (int)(elapsed / 1000));
    return result;
}

void print_precomp(Precomp &precomp) {
    for (int i = 0; i < precomp.tp->teleports_n; i++) {
        print_edges(precomp.tp->teleports[i], precomp.for_teleports[i]);
    }
}

void print_teleport(Teleports::Teleport tp) {
    char label[3] = { (char)(tp.label >> 8), (char)(tp.label & 0xff) };
    printf("'%s' %s (%d,%d)", label, (tp.outer ? "outer" : "inner"), tp.pos.x, tp.pos.y);
}

#define MAX_DEPTH 100

struct Traversed {
    uint64_t inner[MAX_DEPTH];
    uint64_t outer[MAX_DEPTH];

    void add_inner(int depth, int teleport_index) {
        inner[depth] |= (1ull << teleport_index);
    }
    void add_outer(int depth, int teleport_index) {
        outer[depth] |= (1ull << teleport_index);
    }

    bool contains_inner(int depth, int teleport_index) {
        for (int i = 0; i <= depth; i++) {
            if ((inner[i] & (1ull << teleport_index)) != 0) return true;
        }
        return false;
        //return (inner[depth] & (1ull << teleport_index)) != 0;
    }
    bool contains_outer(int depth, int teleport_index) {
        return (outer[depth] & (1ull << teleport_index)) != 0;
    }
};

struct State {
    int8_t teleport_index;
    int8_t depth;
    int16_t dist;
};

struct SavedSet {
    State *states;
    int cap;
    int num;
};

struct TraverseContext {
    SavedSet *states;
    int max_dist;
};

void push_state(SavedSet *set, int teleport_index, int depth, int dist) {
    assert(teleport_index < 256);
    assert(depth < 256);
    assert(dist < 256*256);
    if (set->num + 1 > set->cap) {
        int new_cap = max(128, (set->cap + 1) * 2);
        set->states = (State*)realloc(set->states, new_cap * sizeof(State));
        set->cap = new_cap;
    }
    int index = set->num;
    set->num++;
    set->states[index] = {
        .teleport_index = (int8_t)teleport_index,
        .depth = (int8_t)depth,
        .dist = (int16_t)dist,
    };
}

State pop_state(SavedSet *set) {
    int min_index = 0;
    int min_depth = set->states[0].depth;
    for (int i = 1; i < set->num; i++) {
        if (set->states[i].depth < min_depth) {
            min_index = i;
            min_depth = set->states[i].depth;
        }
    }
    State result = set->states[min_index];
    set->states[min_index] = set->states[set->num-1];
    set->num--;
    return result;
}

int traverse_edges(Precomp &pc, TraverseContext ctx, Traversed traversed, int teleport_index, int depth, int dist, int best_result) {
    if (depth >= MAX_DEPTH) {
        printf("\e[31mMAX DEPTH REACHED: depth=%d, dist=%d\e[0m\n", depth, dist);
        return INT_MAX;
    } else if (dist >= best_result) {
        //printf("\e[32mEARLY OUT: best dist=%d, dist=%d, depth=%d\e[0m\n", best_result, dist, depth);
        return INT_MAX;
    } else if (dist > ctx.max_dist) {
        //printf("\e[32mSAVING STATE depth=%d, dist=%d - best dist=%d\e[0m\n", depth, dist, best_result);
        push_state(ctx.states, teleport_index, depth, dist);
        return INT_MAX;
    }

    Teleports::Teleport current = pc.tp->teleports[teleport_index];

    int min_dist = best_result;
    Edges &es = pc.for_teleports[teleport_index];
    for (int i = 0; i < es.edges_n; i++) {
        Edge e = es.edges[i];

        if (e.dest.label == goal_label) {
            if (depth == 0) {
                if (dist + e.dist < min_dist) min_dist = dist + e.dist;
            } else continue;
        } else if (e.dest.label == start_label) {
            continue;
        } else if (e.dest.outer && depth > 0) {
            if (traversed.contains_outer(depth, e.teleport_index)) {
                continue;
            }
            traversed.add_outer(depth, e.teleport_index);

            int d = traverse_edges(pc, ctx, traversed, e.teleport_index, depth - 1, dist + e.dist, min_dist);
            if (d < min_dist) min_dist = d;
        } else if (!e.dest.outer) {
            if (traversed.contains_inner(depth, e.teleport_index)) {
                continue;
            }
            traversed.add_inner(depth, e.teleport_index);

            int d = traverse_edges(pc, ctx, traversed, e.teleport_index, depth + 1, dist + e.dist, min_dist);
            if (d < min_dist) min_dist = d;
        }
    }
    return min_dist;
}

int traverse_edges_loop(Precomp &pc) {
    SavedSet states = {};
    TraverseContext ctx = {
        .states = &states,
        .max_dist = 1000,
    };
    push_state(&states, 0, 0, 0);

    Traversed traversed = {};
    int min_dist = INT_MAX;
    while (states.num > 0) {
        State st = pop_state(&states);
        int d = traverse_edges(pc, ctx, traversed, st.teleport_index, st.depth, st.dist, min_dist);
        if (d < min_dist) min_dist = d;

        //printf("Terminated traversal at max distance %d; %d saved states\n", ctx.max_dist, states.num);
        //printf("Current min distance = %d\n", min_dist);
        if (min_dist == INT_MAX) ctx.max_dist += 100;
        else {
            ctx.max_dist = min_dist;
            //getchar();
        }
    }
    return min_dist;
}

void part_two() 
{
    uint64_t start_time = perf_time_nanos();
    
    //Map map = map_from_file("input-example3.txt");
    Map map = map_from_file("input.txt");

    Teleports teleports = { };
    
    map_find_teleports(&teleports, map);

    printf("map size [%d,%d]\n", map.width, map.height);
    printf("start at (%d,%d)\n", teleports.start_pos.x, teleports.start_pos.y);
    printf("goal at (%d,%d)\n", teleports.goal_pos.x, teleports.goal_pos.y);

    Precomp precomp = precompute(map, &teleports);
    //print_precomp(precomp);

    printf("\n----\n");

    //Traversed traversed = {};
    //int result = traverse_edges(precomp, traversed, 0, 0, 0, INT_MAX);
    int result = traverse_edges_loop(precomp);

    uint64_t total_time_nanos = perf_time_elapsed_nanos(start_time);
    printf("Time: %u us total\n", (uint32_t)(total_time_nanos/1000));
    printf("Part 2 - result: %d\n", result);
}

int main(int argc, char **argv)
{
    part_one();
    part_two();
    return 0;
}

