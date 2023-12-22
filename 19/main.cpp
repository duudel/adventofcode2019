/*

--- Day 19: Tractor Beam ---

Unsure of the state of Santa's ship, you borrowed the tractor beam technology
from Triton. Time to test it out.

When you're safely away from anything else, you activate the tractor beam, but
nothing happens. It's hard to tell whether it's working if there's nothing to
use it on. Fortunately, your ship's drone system can be configured to deploy a
drone to specific coordinates and then check whether it's being pulled. There's
even an Intcode program (your puzzle input) that gives you access to the drone
system.

The program uses two input instructions to request the X and Y position to which
the drone should be deployed. Negative numbers are invalid and will confuse the
drone; all numbers should be zero or positive.

Then, the program will output whether the drone is stationary (0) or being
pulled by something (1). For example, the coordinate X=0, Y=0 is directly in
front of the tractor beam emitter, so the drone control program will always
report 1 at that location.

To better understand the tractor beam, it is important to get a good picture of
the beam itself. For example, suppose you scan the 10x10 grid of points closest
to the emitter:

       X
  0->      9
 0#.........
 |.#........
 v..##......
  ...###....
  ....###...
Y .....####.
  ......####
  ......####
  .......###
 9........##

In this example, the number of points affected by the tractor beam in the 10x10
area closest to the emitter is 27.

However, you'll need to scan a larger area to understand the shape of the beam.
How many points are affected by the tractor beam in the 50x50 area closest to
the emitter? (For each of X and Y, this will be 0 through 49.)

Your puzzle answer was 116.

--- Part Two ---

You aren't sure how large Santa's ship is. You aren't even sure if you'll need
to use this thing on Santa's ship, but it doesn't hurt to be prepared. You
figure Santa's ship might fit in a 100x100 square.

The beam gets wider as it travels away from the emitter; you'll need to be a
minimum distance away to fit a square of that size into the beam fully. (Don't
rotate the square; it should be aligned to the same axes as the drone grid.)

For example, suppose you have the following tractor beam readings:

#.......................................
.#......................................
..##....................................
...###..................................
....###.................................
.....####...............................
......#####.............................
......######............................
.......#######..........................
........########........................
.........#########......................
..........#########.....................
...........##########...................
...........############.................
............############................
.............#############..............
..............##############............
...............###############..........
................###############.........
................#################.......
.................########OOOOOOOOOO.....
..................#######OOOOOOOOOO#....
...................######OOOOOOOOOO###..
....................#####OOOOOOOOOO#####
.....................####OOOOOOOOOO#####
.....................####OOOOOOOOOO#####
......................###OOOOOOOOOO#####
.......................##OOOOOOOOOO#####
........................#OOOOOOOOOO#####
.........................OOOOOOOOOO#####
..........................##############
..........................##############
...........................#############
............................############
.............................###########

In this example, the 10x10 square closest to the emitter that fits entirely
within the tractor beam has been marked O. Within it, the point closest to the
emitter (the only highlighted O) is at X=25, Y=20.

Find the 100x100 square closest to the emitter that fits entirely within the
tractor beam; within that square, find the point closest to the emitter. What
value do you get if you take that point's X coordinate, multiply it by 10000,
then add the point's Y coordinate? (In the example above, this would be 250020.)

Your puzzle answer was 10311666.

*/

#include <cstdio>
#include <cstdint>
#include <cinttypes>
#include <cstdlib>
#include <cstring>
#include <cassert>

typedef int64_t IntWord;

#define OPCODES \
    OPCODE(Add, 1)\
    OPCODE(Mul, 2)\
    OPCODE(Inp, 3)\
    OPCODE(Out, 4)\
    OPCODE(Jnz, 5)\
    OPCODE(Jz, 6)\
    OPCODE(Lt, 7)\
    OPCODE(Equ, 8)\
    OPCODE(ModRel, 9)\
    OPCODE(Halt, 99)

#define OPCODE(op, x) OP_ ## op = x,
enum Opcode
{
    OPCODES

    Opcode_Max
};
#undef OPCODE

static const char* Opcode_names[Opcode_Max] = { };
#define OPCODE(op, x) Opcode_names[x] = #op;
bool init_opcode_names()
{
    OPCODES
    return true;
}
#undef OPCODE
static bool init = init_opcode_names();

#define MAX_IO_BUFFER 10000
//#define MAX_IO_BUFFER 1000
struct Buffer
{
    int write;
    int read;
    IntWord data[MAX_IO_BUFFER];
};

void write(Buffer *buf, IntWord x)
{
    buf->data[buf->write] = x;
    buf->write++;
    if (buf->write == MAX_IO_BUFFER)
    {
        buf->write = 0;
    }
}

bool read(Buffer *buf, IntWord *x)
{
    if (buf->read != buf->write)
    {
        *x = buf->data[buf->read];
        buf->read++;
        if (buf->read == MAX_IO_BUFFER)
        {
            buf->read = 0;
        }
        return true;
    }
    return false;
}

int readable_num(Buffer buf)
{
    if (buf.write >= buf.read)
    {
        return buf.write - buf.read;
    }
    return buf.write + MAX_IO_BUFFER - buf.read;
}

struct State
{
    int position;
    int relative_base;
    bool halted;

    Buffer *input;
    Buffer *output;
};

struct Code
{
    IntWord *data;
    int code_size;
    int memory_size; // code_size + extra memory

    IntWord operator[](int pos) const
    {
        return data[pos];
    }
    IntWord& operator[](int pos)
    {
        return data[pos];
    }
};

template <int N>
Code to_code(IntWord (&c)[N], int extra_memory)
{
    IntWord *data = (IntWord*)calloc((N + extra_memory), sizeof(IntWord));
    //IntWord *data = (IntWord*)malloc(sizeof(IntWord) * (N + extra_memory));
    memcpy(data, c, sizeof(IntWord) * N);
    //memset(data + N, 0, sizeof(IntWord) * extra_memory);
    return { .data = data, .code_size = N, .memory_size = N + extra_memory };
};

void reset_extra_memory(Code &c) {
    int extra_memory = c.memory_size - c.code_size;
    memset(c.data + c.code_size, 0, extra_memory * sizeof(IntWord));
}

Code copy_code(Code &c) {
    IntWord *data = (IntWord*)calloc(c.memory_size, sizeof(IntWord));
    memcpy(data, c.data, sizeof(IntWord) * c.code_size);
    return { .data = data, .code_size = c.code_size, .memory_size = c.memory_size };
}

void free_code(Code &c)
{
    free(c.data);
    c = { };
}

void print_code(Code code)
{
    if (code.code_size > 0)
    {
        printf("%" PRId64, code[0]);
        for (int i = 1; i < code.code_size; i++) printf(",%" PRId64, code[i]);
        printf("\n");
    }
}

Opcode decode_instr(IntWord instr, int *m1, int *m2, int *m3)
{
    int op = instr % 100;
    *m1 = (instr / 100) % 10;
    *m2 = (instr / 1000) % 10;
    *m3 = (instr / 10000) % 10;
    return (Opcode)op;
}

IntWord read(State *s, Code code, int pos, int mode)
{
    IntWord a = code[pos];
    IntWord result;
    switch (mode)
    {
        case 0: result = code[a]; break;
        case 1: result = a; break;
        case 2: result = code[s->relative_base + a]; break;
        default:
            assert("invalid address mode" && 0);
    }
    return result;
}

IntWord read_pos(State *s, Code code, int pos, int mode)
{
    IntWord a = code[pos];
    IntWord result;
    switch (mode)
    {
        case 0: result = a; break;
        case 2: result = s->relative_base + a; break;
        default:
            assert("invalid address mode" && 0);
    }
    return result;
}

int step(State *s, Code code)
{
    int pos = s->position;
    IntWord instr = code[pos];
    int m1, m2, m3;
    Opcode op = decode_instr(instr, &m1, &m2, &m3);
    //printf("%d: (%d) %s[%d, %d, %d]\n", pos, op, Opcode_names[op], m1, m2, m3);
    switch (op)
    {
    case OP_Add:
        {
            IntWord oper1 = read(s, code, pos+1, m1);
            IntWord oper2 = read(s, code, pos+2, m2);
            IntWord res_pos = read_pos(s, code, pos+3, m3);
            //printf(" [%lld] <- %lld + %lld\n", res_pos, oper1, oper2);
            code[res_pos] = oper1 + oper2;
            pos += 4;
        } break;
    case OP_Mul:
        {
            IntWord oper1 = read(s, code, pos+1, m1);
            IntWord oper2 = read(s, code, pos+2, m2);
            IntWord res_pos = read_pos(s, code, pos+3, m3);
            //printf(" [%lld] <- %lld * %lld\n", res_pos, oper1, oper2);
            code[res_pos] = oper1 * oper2;
            pos += 4;
        } break;
    case OP_Inp:
        {
            IntWord res;
            if (!read(s->input, &res))
            {
                printf("Waiting input...\n");
                getchar();
                return 0;
            }
            IntWord res_pos = read_pos(s, code, pos+1, m1);
            //printf(" [%lld] <- inp=%lld\n", res_pos, res);
            code[res_pos] = res;
            pos += 2;
        } break;
    case OP_Out:
        {
            IntWord out = read(s, code, pos+1, m1);
            write(s->output, out);
            //printf("out=%lld\n", out);
            pos += 2;
        } break;
    case OP_Jnz:
        {
            IntWord oper1 = read(s, code, pos+1, m1);
            IntWord oper2 = read(s, code, pos+2, m2);
            //printf(" %lld != 0 => pos=%lld\n", oper1, oper2);
            if (oper1 != 0)
            {
                pos = oper2;
            }
            else
            {
                pos += 3;
            }
        } break;
    case OP_Jz:
        {
            IntWord oper1 = read(s, code, pos+1, m1);
            IntWord oper2 = read(s, code, pos+2, m2);
            //printf(" %lld == 0 => pos=%lld\n", oper1, oper2);
            if (oper1 == 0)
            {
                pos = oper2;
            }
            else
            {
                pos += 3;
            }
        } break;
    case OP_Lt:
        {
            IntWord oper1 = read(s, code, pos+1, m1);
            IntWord oper2 = read(s, code, pos+2, m2);
            IntWord res_pos = read_pos(s, code, pos+3, m3);
            //printf(" [%lld] <- %lld < %lld\n", res_pos, oper1, oper2);
            code[res_pos] = (oper1 < oper2) ? 1 : 0;
            pos += 4;
        } break;
    case OP_Equ:
        {
            IntWord oper1 = read(s, code, pos+1, m1);
            IntWord oper2 = read(s, code, pos+2, m2);
            IntWord res_pos = read_pos(s, code, pos+3, m3);
            //printf(" [%lld] <- %lld == %lld\n", res_pos, oper1, oper2);
            code[res_pos] = (oper1 == oper2) ? 1 : 0;
            pos += 4;
        } break;
    case OP_ModRel:
        {
            IntWord oper1 = read(s, code, pos+1, m1);
            //printf(" %d => ", s->relative_base);
            s->relative_base += oper1;
            //printf(" %d\n", s->relative_base);
            pos += 2;
        } break;
    case OP_Halt:
        {
            //printf("Halt!\n");
            s->halted = true;
            return -1;
        } break;

    case Opcode_Max:
    default:
        printf("BUG!\n");
        pos += 1;
        //return -1;
    }
    s->position = pos;
    return 1;
}

void execute(State *state, Code code)
{
    while (step(state, code) == 1);
}

void read_operand(Code code, int pos, int mode)
{
    IntWord a = code[pos];
    switch (mode)
    {
        case 0: printf("[%" PRId64 "]=%" PRId64, a, code[a]); break;
        case 1: printf("%" PRId64, a); break;
        case 2: printf("[B+%" PRId64 "]=?", a); break;
        default:
            assert("invalid address mode" && 0);
    }
}

void write_operand(Code code, int pos, int mode)
{
    IntWord a = code[pos];
    switch (mode)
    {
        case 0: printf("[%" PRId64 "]", a); break;
        case 2: printf("[B+%" PRId64 "]", a); break;
        default:
            assert("invalid address mode" && 0);
    }
}

void print_instr(Code code, IntWord *position)
{
    IntWord pos = *position;
    IntWord instr = code[pos];
    int m1, m2, m3;
    Opcode op = decode_instr(instr, &m1, &m2, &m3);
    printf("%" PRId64 ": (%d) %s[%d, %d, %d]    ", pos, op, Opcode_names[op], m1, m2, m3);
    switch (op)
    {
    case OP_Add:
        {
            write_operand(code, pos+2, m3);
            printf(" <- ");
            read_operand(code, pos+1, m1);
            printf(" + ");
            read_operand(code, pos+2, m2);
            pos += 4;
        } break;
    case OP_Mul:
        {
            write_operand(code, pos+2, m3);
            printf(" <- ");
            read_operand(code, pos+1, m1);
            printf(" * ");
            read_operand(code, pos+2, m2);
            pos += 4;
        } break;
    case OP_Inp:
        {
            write_operand(code, pos+1, m1);
            printf("<- inp");
            pos += 2;
        } break;
    case OP_Out:
        {
            printf("out <- ");
            read_operand(code, pos+1, m1);
            pos += 2;
        } break;
    case OP_Jnz:
        {
            read_operand(code, pos+1, m1);
            printf(" != 0 => pos <- ");
            read_operand(code, pos+2, m2);
            pos += 3;
        } break;
    case OP_Jz:
        {
            read_operand(code, pos+1, m1);
            printf(" == 0 => pos <- ");
            read_operand(code, pos+2, m2);
            pos += 3;
        } break;
    case OP_Lt:
        {
            write_operand(code, pos+3, m3);
            printf(" <- ");
            read_operand(code, pos+1, m1);
            printf(" < ");
            read_operand(code, pos+2, m2);
            pos += 4;
        } break;
    case OP_Equ:
        {
            write_operand(code, pos+3, m3);
            printf(" <- ");
            read_operand(code, pos+1, m1);
            printf(" == ");
            read_operand(code, pos+2, m2);
            pos += 4;
        } break;
    case OP_ModRel:
        {
            printf("B <- ");
            read_operand(code, pos+1, m1);
            pos += 2;
        } break;
    case OP_Halt:
        {
            pos += 1;
        } break;

    case Opcode_Max:
    default:
        printf("BUG!\n");
        pos += 1;
    }
    *position = pos;
}

void print_code(Code code, int beg, int end)
{
    IntWord position = beg;
    while (position < end)
    {
        print_instr(code, &position);
        putchar('\n');
    }
}



int min(int a, int b)
{
    return (a < b) ? a : b;
}
int max(int a, int b)
{
    return (a > b) ? a : b;
}

struct Pos
{
    int x, y;
};

bool operator == (Pos a, Pos b) { return a.x == b.x && a.y == b.y; }
bool operator != (Pos a, Pos b) { return a.x != b.x || a.y != b.y; }

int manhattan_distance(Pos a, Pos b)
{
    return abs(a.x - b.x) + abs(a.y - b.y);
}

bool is_adjacent(Pos a, Pos b) { return manhattan_distance(a, b) == 1; }

struct Bounds
{
    int min_x;
    int max_x;
    int min_y;
    int max_y;
};

bool bounds_contains(Bounds b, int x, int y)
{
    return (b.min_x <= x && x <= b.max_x)
        && (b.min_y <= y && y <= b.max_y);
}

struct Grid;
void resize_grid(Grid *grid, int x, int y);

struct Grid
{
    int *data;
    Bounds bounds;

    int operator()(int x, int y) const
    {
        if (!bounds_contains(bounds, x, y))
        {
            return 0;
        }
        int width = bounds.max_x - bounds.min_x + 1;
        int nx = x - bounds.min_x;
        int ny = y - bounds.min_y;
        return data[ny*width + nx];
    }

    int& operator()(int x, int y)
    {
        if (!bounds_contains(bounds, x, y))
        {
            resize_grid(this, x, y);
        }
        int width = bounds.max_x - bounds.min_x + 1;
        int nx = x - bounds.min_x;
        int ny = y - bounds.min_y;
        return data[ny*width + nx];
    }
    int operator()(Pos p) const
    {
        if (!bounds_contains(bounds, p.x, p.y))
        {
            return 0;
        }
        int width = bounds.max_x - bounds.min_x + 1;
        int nx = p.x - bounds.min_x;
        int ny = p.y - bounds.min_y;
        return data[ny*width + nx];
    }

    int& operator()(Pos p)
    {
        if (!bounds_contains(bounds, p.x, p.y))
        {
            resize_grid(this, p.x, p.y);
        }
        int width = bounds.max_x - bounds.min_x + 1;
        int nx = p.x - bounds.min_x;
        int ny = p.y - bounds.min_y;
        return data[ny*width + nx];
    }
};

Grid alloc_grid(Bounds bounds)
{
    int width = bounds.max_x - bounds.min_x + 1;
    int height = bounds.max_y - bounds.min_y + 1;

    Grid result = {};
    result.data = (int*)calloc(width*height, sizeof(int));
    result.bounds = bounds;
    return result;
}

void free_grid(Grid *grid)
{
    free(grid->data);
}

void print_grid(Grid grid);
void resize_grid(Grid *grid, int nx, int ny)
{
    Bounds new_bounds = {};
    new_bounds.min_x = min(grid->bounds.min_x, nx);
    new_bounds.max_x = max(grid->bounds.max_x, nx);
    new_bounds.min_y = min(grid->bounds.min_y, ny);
    new_bounds.max_y = max(grid->bounds.max_y, ny);

    Grid new_grid = alloc_grid(new_bounds);

    for (int y = grid->bounds.min_y; y <= grid->bounds.max_y; y++)
    {
        for (int x = grid->bounds.min_x; x <= grid->bounds.max_x; x++)
        {
            new_grid(x, y) = (*grid)(x, y);
        }
    }

    free_grid(grid);
    *grid = new_grid;
}

void print_grid(Grid grid)
{
    const char *clear = "\e[m";
    //const char *oxy = "\e[1m\e[38;5;12m";
    const char *droid = "\e[1m\e[36m";
    const char *scaffolding = "\e[38;2;148;148;100m\e[48;5;8m";
    const char *traversed = "\e[38;2;128;128;148m\e[48;5;8m";
    const char *space = "\e[38;5;8m";
    for (int y = grid.bounds.min_y; y <= grid.bounds.max_y; y++)
    {
        for (int x = grid.bounds.min_x; x <= grid.bounds.max_x; x++)
        {
            int ch = grid(x, y);
            switch (ch)
            {
                case '.': fputs(space, stdout); break;
                case '*': fputs(traversed, stdout); break;
                case '#': fputs(scaffolding, stdout); break;
                case 'X':
                case '>':
                case '<':
                case 'v':
                case '^': fputs(droid, stdout); break;
            }
            putchar(ch);
            fputs(clear, stdout);
        }
        putchar('\n');
    }
    puts(clear);
}

enum Direction
{
    North = 1,
    South = 2,
    West = 3,
    East = 4,
};

Direction to_left(Direction dir)
{
    switch (dir)
    {
        case North: return West;
        case South: return East;
        case West: return South;
        case East: return North;
    }
    return South;
}

Direction to_right(Direction dir)
{
    switch (dir)
    {
        case North: return East;
        case South: return West;
        case West: return North;
        case East: return South;
    }
    return South;
}

Direction select_direction_towards(Pos from, Pos to)
{
    if (abs(from.x - to.x) > abs(from.y - to.y))
    {
        return (from.x < to.x) ?  East : West;
    }
    else
    {
        return (from.y < to.y) ?  South : North;
    }
}

Direction random_dir()
{
    return (Direction)(rand() % 4 + 1);
}

Direction opposite_dir(Direction dir)
{
    switch (dir)
    {
        case North: return South;
        case South: return North;
        case West: return East;
        case East: return West;
    }
    return South;
}

Direction select_other_direction(Direction current)
{
    while (true)
    {
        Direction result = random_dir();
        if (result != current) return result;
    }
}

struct Robot
{
    Pos pos;
    Direction dir;

    Pos target;
};

Pos move(Pos p, Direction dir)
{
    switch (dir)
    {
        case 1:
            p.y -= 1;
            break;
        case 2:
            p.y += 1;
            break;
        case 3:
            p.x -= 1;
            break;
        case 4:
            p.x += 1;
            break;
    }
    return p;
}

template <class T>
struct Stack
{
    T *data;
    int len, cap;

    T operator[](int i) const { return data[i]; }
    T& operator[](int i) { return data[i]; }
};

template <class T>
void free_stack(Stack<T> s)
{
    free(s.data);
}

template <class T>
void push(Stack<T> *s, T x)
{
    if (s->len >= s->cap)
    {
        int new_cap = (s->cap + 8) * 2;
        s->data = (T*)realloc(s->data, new_cap * sizeof(T));
        s->cap = new_cap;
    }
    s->data[s->len++] = x;
}

template <class T>
T pop(Stack<T> *s)
{
    if (s->len == 0) assert("Stack is empty" && 0);
    return s->data[--s->len];
}

template <class T>
Stack<T> copy_n(Stack<T> source, int start, int n)
{
    Stack<T> result = {};
    result.len = n;
    result.cap = n;
    result.data = (T*)malloc(n * sizeof(T));
    memcpy(result.data, source.data + start, n * sizeof(T));
    return result;
}

template <class T>
void print(T x);

template <class T>
void print_stack(Stack<T> s, void (*print_elem)(T x) = print<T>)
{
    printf("[");
    if (s.len > 0)
    {
        print(s.data[0]);
        for (int i = 1; i < s.len; i++)
        {
            putchar(','); print(s.data[i]);
        }
    }
    printf("]");
}

void find_points_affected(Code original_code)
{
    Buffer input = {};
    Buffer output = {};
    State state = {};
    state.input = &input;
    state.output = &output;

    // Write coordinates to input for the int code to consume
    for (int y = 0; y < 50; y++) {
        for (int x = 0; x < 50; x++) {
            write(&input, x);
            write(&input, y);
        }
    }

    Code code = copy_code(original_code);

    int output_n = 0;
    int points_affected = 0;
    while (!state.halted)
    {
        int res;
        for (int i = 0; i < 20; i++)
        {
            res = step(&state, code);
            if (res != 1) break;
        }
        while (readable_num(output) > 0)
        {
            IntWord out;
            read(&output, &out);

            printf("Got output %" PRId64 " (%d)\n", out, output_n);

            output_n++;
            if (out == 1) points_affected++;
        }
        if (state.halted) {
            if (output_n < 50*50) {
                state = {};
                state.input = &input;
                state.output = &output;
                free_code(code);
                code = copy_code(original_code);
            }
        }
    }

    printf("Part 1: %d\n", points_affected);
}

IntWord actual_code[] = {
    109,   424,   203,   1,     21101, 0,     11,    0,     1106,  0,     282,
    21102, 1,     18,    0,     1106,  0,     259,   2101,  0,     1,     221,
    203,   1,     21101, 0,     31,    0,     1106,  0,     282,   21101, 0,
    38,    0,     1106,  0,     259,   21001, 23,    0,     2,     21202, 1,
    1,     3,     21102, 1,     1,     1,     21102, 1,     57,    0,     1106,
    0,     303,   2102,  1,     1,     222,   20102, 1,     221,   3,     21001,
    221,   0,     2,     21102, 1,     259,   1,     21102, 80,    1,     0,
    1106,  0,     225,   21102, 106,   1,     2,     21102, 91,    1,     0,
    1105,  1,     303,   1201,  1,     0,     223,   21001, 222,   0,     4,
    21101, 259,   0,     3,     21102, 1,     225,   2,     21101, 225,   0,
    1,     21101, 0,     118,   0,     1106,  0,     225,   20101, 0,     222,
    3,     21102, 42,    1,     2,     21101, 133,   0,     0,     1105,  1,
    303,   21202, 1,     -1,    1,     22001, 223,   1,     1,     21101, 0,
    148,   0,     1106,  0,     259,   1201,  1,     0,     223,   21001, 221,
    0,     4,     20101, 0,     222,   3,     21101, 10,    0,     2,     1001,
    132,   -2,    224,   1002,  224,   2,     224,   1001,  224,   3,     224,
    1002,  132,   -1,    132,   1,     224,   132,   224,   21001, 224,   1,
    1,     21101, 195,   0,     0,     106,   0,     108,   20207, 1,     223,
    2,     20102, 1,     23,    1,     21101, -1,    0,     3,     21101, 214,
    0,     0,     1105,  1,     303,   22101, 1,     1,     1,     204,   1,
    99,    0,     0,     0,     0,     109,   5,     1202,  -4,    1,     249,
    22102, 1,     -3,    1,     22101, 0,     -2,    2,     21202, -1,    1,
    3,     21101, 250,   0,     0,     1105,  1,     225,   21202, 1,     1,
    -4,    109,   -5,    2106,  0,     0,     109,   3,     22107, 0,     -2,
    -1,    21202, -1,    2,     -1,    21201, -1,    -1,    -1,    22202, -1,
    -2,    -2,    109,   -3,    2105,  1,     0,     109,   3,     21207, -2,
    0,     -1,    1206,  -1,    294,   104,   0,     99,    22102, 1,     -2,
    -2,    109,   -3,    2106,  0,     0,     109,   5,     22207, -3,    -4,
    -1,    1206,  -1,    346,   22201, -4,    -3,    -4,    21202, -3,    -1,
    -1,    22201, -4,    -1,    2,     21202, 2,     -1,    -1,    22201, -4,
    -1,    1,     21202, -2,    1,     3,     21101, 343,   0,     0,     1106,
    0,     303,   1105,  1,     415,   22207, -2,    -3,    -1,    1206,  -1,
    387,   22201, -3,    -2,    -3,    21202, -2,    -1,    -1,    22201, -3,
    -1,    3,     21202, 3,     -1,    -1,    22201, -3,    -1,    2,     22101,
    0,     -4,    1,     21102, 384,   1,     0,     1106,  0,     303,   1105,
    1,     415,   21202, -4,    -1,    -4,    22201, -4,    -3,    -4,    22202,
    -3,    -2,    -2,    22202, -2,    -4,    -4,    22202, -3,    -2,    -3,
    21202, -4,    -1,    -2,    22201, -3,    -2,    1,     22102, 1,     1,
    -4,    109,   -5,    2105,  1,     0
};

void part_one()
{
    Code code = to_code(actual_code, 200000);
    find_points_affected(code);
    free_code(code);
}

bool scan_point(int x, int y)
{
    Buffer input = {};
    Buffer output = {};
    State state = {};
    state.input = &input;
    state.output = &output;

    // Write coordinates to input for the int code to consume
    write(&input, x);
    write(&input, y);

    Code code = to_code(actual_code, 200000);

    bool result = false;
    while (!state.halted)
    {
        int res;
        for (int i = 0; i < 20; i++)
        {
            res = step(&state, code);
            if (res != 1) break;
        }
        while (readable_num(output) > 0)
        {
            IntWord out;
            read(&output, &out);
            printf("out %d,%d = %" PRId64 "\n", x, y, out);
            result = out == 1;
        }
    }
    free_code(code);

    return result;
}

void scan() {
    int x = 5;
    int b_start_y = -1;
    int b_end_y = -1;
    while (x < 100'000) {
        int start_y = -1;
        int end_y = -1;
        for (int y = b_start_y; y < 10000; y++) {
            bool affected = scan_point(x, y);
            if (affected && start_y == -1) start_y = y;
            if (!affected && start_y != -1) end_y = y;
            if (start_y != -1 && end_y != -1) break;
        }

        b_start_y = start_y;
        b_end_y = end_y;

        if (end_y - start_y >= 100) break;
        x++;
    }
    
    printf("scanned x=%d, from y=%d to %d inside tractor beam (%d)\n",
           x, b_start_y, b_end_y, b_end_y - b_start_y);

    int y = b_start_y;
    while (true) {
        int start_x = x;
        int end_x = -1;
        for (int x = start_x + 1; x < 100'000; x++) {
            bool affected = scan_point(x, y);
            if (!affected) {
                end_x = x;
                break;
            }
        }

        int dx = end_x - start_x;
        printf("dx=%d\n", dx);

        if (end_x - start_x >= 100) {
            bool inside = scan_point(x, y + 100 - 1);
            if (inside) break;
        }

        y++;

        bool inside = scan_point(x, y + 100 - 1);
        if (!inside) {
            x++;
        }
    }

    printf("scanned x=%d,y=%d from y=%d to %d inside tractor beam (%d)\n",
           x, y, b_start_y, b_end_y, b_end_y - b_start_y);

    int result = x * 10'000 + y;
    printf("Part 2: %d", result);
}

void part_two()
{
    scan();
}

int main(int argc, const char **argv)
{
    //part_one();
    part_two();
    return 0;
}

