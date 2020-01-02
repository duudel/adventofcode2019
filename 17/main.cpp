// 776996-20191205-0a4a7f2b

/*

 --- Day 17: Set and Forget ---

An early warning system detects an incoming solar flare and automatically
activates the ship's electromagnetic shield. Unfortunately, this has cut off
the Wi-Fi for many small robots that, unaware of the impending danger, are now
trapped on exterior scaffolding on the unsafe side of the shield. To rescue
them, you'll have to act quickly!

The only tools at your disposal are some wired cameras and a small vacuum robot
currently asleep at its charging station. The video quality is poor, but the
vacuum robot has a needlessly bright LED that makes it easy to spot no matter
where it is.

An Intcode program, the Aft Scaffolding Control and Information Interface
(ASCII, your puzzle input), provides access to the cameras and the vacuum
robot. Currently, because the vacuum robot is asleep, you can only access the
cameras.

Running the ASCII program on your Intcode computer will provide the current
view of the scaffolds. This is output, purely coincidentally, as ASCII code: 35
means #, 46 means ., 10 starts a new line of output below the current one, and
so on. (Within a line, characters are drawn left-to-right.)

In the camera output, # represents a scaffold and . represents open space. The
vacuum robot is visible as ^, v, <, or > depending on whether it is facing up,
down, left, or right respectively. When drawn like this, the vacuum robot is
always on a scaffold; if the vacuum robot ever walks off of a scaffold and
begins tumbling through space uncontrollably, it will instead be visible as X.

In general, the scaffold forms a path, but it sometimes loops back onto itself.
For example, suppose you can see the following view from the cameras:

..#..........
..#..........
#######...###
#.#...#...#.#
#############
..#...#...#..
..#####...^..

Here, the vacuum robot, ^ is facing up and sitting at one end of the scaffold
near the bottom-right of the image. The scaffold continues up, loops across
itself several times, and ends at the top-left of the image.

The first step is to calibrate the cameras by getting the alignment parameters
of some well-defined points. Locate all scaffold intersections; for each, its
alignment parameter is the distance between its left edge and the left edge of
the view multiplied by the distance between its top edge and the top edge of
the view. Here, the intersections from the above image are marked O:

..#..........
..#..........
##O####...###
#.#...#...#.#
##O###O###O##
..#...#...#..
..#####...^..

For these intersections:

    The top-left intersection is 2 units from the left of the image and 2 units
    from the top of the image, so its alignment parameter is 2 * 2 = 4.  The
    bottom-left intersection is 2 units from the left and 4 units from the top,
    so its alignment parameter is 2 * 4 = 8.
    The bottom-middle intersection is 6 from the left and 4 from the top, so
    its alignment parameter is 24.  The bottom-right intersection's alignment
    parameter is 40.

To calibrate the cameras, you need the sum of the alignment parameters. In the
above example, this is 76.

Run your ASCII program. What is the sum of the alignment parameters for the
scaffold intersections?

Your puzzle answer was 4600.
--- Part Two ---

Now for the tricky part: notifying all the other robots about the solar flare.
The vacuum robot can do this automatically if it gets into range of a robot.
However, you can't see the other robots on the camera, so you need to be
thorough instead: you need to make the vacuum robot visit every part of the
scaffold at least once.

The vacuum robot normally wanders randomly, but there isn't time for that
today. Instead, you can override its movement logic with new rules.

Force the vacuum robot to wake up by changing the value in your ASCII program
at address 0 from 1 to 2. When you do this, you will be automatically prompted
for the new movement rules that the vacuum robot should use. The ASCII program
will use input instructions to receive them, but they need to be provided as
ASCII code; end each line of logic with a single newline, ASCII code 10.

First, you will be prompted for the main movement routine. The main routine may
only call the movement functions: A, B, or C. Supply the movement functions to
use as ASCII text, separating them with commas (,, ASCII code 44), and ending
the list with a newline (ASCII code 10). For example, to call A twice, then
alternate between B and C three times, provide the string A,A,B,C,B,C,B,C and
then a newline.

Then, you will be prompted for each movement function. Movement functions may
use L to turn left, R to turn right, or a number to move forward that many
units. Movement functions may not call other movement functions. Again,
separate the actions with commas and end the list with a newline. For
example, to move forward 10 units, turn left, move forward 8 units, turn
right, and finally move forward 6 units, provide the string 10,L,8,R,6 and
then a newline.

Finally, you will be asked whether you want to see a continuous video feed;
provide either y or n and a newline. Enabling the continuous video feed can
help you see what's going on, but it also requires a significant amount of
processing power, and may even cause your Intcode computer to overheat.

Due to the limited amount of memory in the vacuum robot, the ASCII definitions
of the main routine and the movement functions may each contain at most 20
characters, not counting the newline.

For example, consider the following camera feed:

#######...#####
#.....#...#...#
#.....#...#...#
......#...#...#
......#...###.#
......#.....#.#
^########...#.#
......#.#...#.#
......#########
........#...#..
....#########..
....#...#......
....#...#......
....#...#......
....#####......

In order for the vacuum robot to visit every part of the scaffold at least
once, one path it could take is:

R,8,R,8,R,4,R,4,R,8,L,6,L,2,R,4,R,4,R,8,R,8,R,8,L,6,L,2

Without the memory limit, you could just supply this whole string to function A
and have the main routine call A once. However, you'll need to split it into
smaller parts.

One approach is:

    Main routine: A,B,C,B,A,C
    (ASCII input: 65, 44, 66, 44, 67, 44, 66, 44, 65, 44, 67, 10)
    Function A:   R,8,R,8
    (ASCII input: 82, 44, 56, 44, 82, 44, 56, 10)
    Function B:   R,4,R,4,R,8
    (ASCII input: 82, 44, 52, 44, 82, 44, 52, 44, 82, 44, 56, 10)
    Function C:   L,6,L,2
    (ASCII input: 76, 44, 54, 44, 76, 44, 50, 10)

Visually, this would break the desired path into the following parts:

A,        B,            C,        B,            A,        C
R,8,R,8,  R,4,R,4,R,8,  L,6,L,2,  R,4,R,4,R,8,  R,8,R,8,  L,6,L,2

CCCCCCA...BBBBB
C.....A...B...B
C.....A...B...B
......A...B...B
......A...CCC.B
......A.....C.B
^AAAAAAAA...C.B
......A.A...C.B
......AAAAAA#AB
........A...C..
....BBBB#BBBB..
....B...A......
....B...A......
....B...A......
....BBBBA......

Of course, the scaffolding outside your ship is much more complex.

As the vacuum robot finds other robots and notifies them of the impending solar
flare, it also can't help but leave them squeaky clean, collecting any space
dust it finds. Once it finishes the programmed set of movements, assuming it
hasn't drifted off into space, the cleaning robot will return to its docking
station and report the amount of space dust it collected as a large, non-ASCII
value in a single output instruction.

After visiting every part of the scaffold at least once, how much dust does the
vacuum robot report it has collected?

Your puzzle answer was 1113411.

*/

#include <cstdio>
#include <cstdint>
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

#define MAX_IO_BUFFER 1000
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

void free_code(Code &c)
{
    free(c.data);
    c = { };
}

void print_code(Code code)
{
    if (code.code_size > 0)
    {
        printf("%lld", code[0]);
        for (int i = 1; i < code.code_size; i++) printf(",%lld", code[i]);
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
                //printf("Waiting input...\n");
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
            printf("Halt!\n");
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
        case 0: printf("[%lld]=%lld", a, code[a]); break;
        case 1: printf("%lld", a); break;
        case 2: printf("[B+%lld]=?", a); break;
        default:
            assert("invalid address mode" && 0);
    }
}

void write_operand(Code code, int pos, int mode)
{
    IntWord a = code[pos];
    switch (mode)
    {
        case 0: printf("[%lld]", a); break;
        case 2: printf("[B+%lld]", a); break;
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
    printf("%lld: (%d) %s[%d, %d, %d]    ", pos, op, Opcode_names[op], m1, m2, m3);
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

void calculate_alignment_parameters(Code code)
{
    Buffer input = {};
    Buffer output = {};
    State state = {};
    state.input = &input;
    state.output = &output;

    Bounds initial_bounds = {
        .min_x = 0, .max_x = 1,
        .min_y = 0, .max_y = 1,
    };
    Grid grid = alloc_grid(initial_bounds);

    Pos cursor = {};
    while (!state.halted)
    {
        int res;
        for (int i = 0; i < 10; i++)
        {
            res = step(&state, code);
            if (res != 1) break;
        }
        while (readable_num(output) > 0)
        {
            IntWord ch;
            read(&output, &ch);

            //putchar(ch);
            //fflush(stdout);

            if (ch != 10)
            {
                grid(cursor) = ch;
                cursor.x++;
            }
            else
            {
                cursor.x = 0;
                cursor.y++;
            }
        }
    }

    int total_alignment = 0;
    for (int y = grid.bounds.min_y; y <= grid.bounds.max_y; y++)
    {
        for (int x = grid.bounds.min_x; x <= grid.bounds.max_x; x++)
        {
            if (grid(x, y) == '#')
            {
                if ((grid(x-1, y) == '#' && grid(x+1, y) == '#')
                    && (grid(x, y-1) == '#' && grid(x, y+1) == '#'))
                {
                    grid(x, y) = 'O';
                    total_alignment += x * y;
                }
            }
        }
    }

    print_grid(grid);
    printf("Total alignment: %d\n", total_alignment);
}

IntWord actual_code[] = {
    1,330,331,332,109,4286,1102,1,1182,16,1101,1491,0,24,102,1,0,570,1006,570,36,1002,571,1,0,1001,570,-1,570,1001,24,1,24,1106,0,18,1008,571,0,571,1001,16,1,16,1008,16,1491,570,1006,570,14,21102,58,1,0,1105,1,786,1006,332,62,99,21101,0,333,1,21101,0,73,0,1105,1,579,1102,0,1,572,1102,1,0,573,3,574,101,1,573,573,1007,574,65,570,1005,570,151,107,67,574,570,1005,570,151,1001,574,-64,574,1002,574,-1,574,1001,572,1,572,1007,572,11,570,1006,570,165,101,1182,572,127,1002,574,1,0,3,574,101,1,573,573,1008,574,10,570,1005,570,189,1008,574,44,570,1006,570,158,1106,0,81,21101,0,340,1,1106,0,177,21101,0,477,1,1106,0,177,21102,514,1,1,21102,176,1,0,1106,0,579,99,21102,1,184,0,1105,1,579,4,574,104,10,99,1007,573,22,570,1006,570,165,101,0,572,1182,21101,0,375,1,21101,0,211,0,1106,0,579,21101,1182,11,1,21101,0,222,0,1105,1,979,21102,388,1,1,21102,233,1,0,1106,0,579,21101,1182,22,1,21102,244,1,0,1105,1,979,21101,0,401,1,21102,255,1,0,1105,1,579,21101,1182,33,1,21101,266,0,0,1106,0,979,21101,414,0,1,21102,277,1,0,1106,0,579,3,575,1008,575,89,570,1008,575,121,575,1,575,570,575,3,574,1008,574,10,570,1006,570,291,104,10,21101,0,1182,1,21101,313,0,0,1105,1,622,1005,575,327,1101,0,1,575,21102,1,327,0,1105,1,786,4,438,99,0,1,1,6,77,97,105,110,58,10,33,10,69,120,112,101,99,116,101,100,32,102,117,110,99,116,105,111,110,32,110,97,109,101,32,98,117,116,32,103,111,116,58,32,0,12,70,117,110,99,116,105,111,110,32,65,58,10,12,70,117,110,99,116,105,111,110,32,66,58,10,12,70,117,110,99,116,105,111,110,32,67,58,10,23,67,111,110,116,105,110,117,111,117,115,32,118,105,100,101,111,32,102,101,101,100,63,10,0,37,10,69,120,112,101,99,116,101,100,32,82,44,32,76,44,32,111,114,32,100,105,115,116,97,110,99,101,32,98,117,116,32,103,111,116,58,32,36,10,69,120,112,101,99,116,101,100,32,99,111,109,109,97,32,111,114,32,110,101,119,108,105,110,101,32,98,117,116,32,103,111,116,58,32,43,10,68,101,102,105,110,105,116,105,111,110,115,32,109,97,121,32,98,101,32,97,116,32,109,111,115,116,32,50,48,32,99,104,97,114,97,99,116,101,114,115,33,10,94,62,118,60,0,1,0,-1,-1,0,1,0,0,0,0,0,0,1,42,16,0,109,4,1202,-3,1,586,21001,0,0,-1,22101,1,-3,-3,21102,1,0,-2,2208,-2,-1,570,1005,570,617,2201,-3,-2,609,4,0,21201,-2,1,-2,1106,0,597,109,-4,2106,0,0,109,5,1202,-4,1,629,21002,0,1,-2,22101,1,-4,-4,21102,1,0,-3,2208,-3,-2,570,1005,570,781,2201,-4,-3,653,20101,0,0,-1,1208,-1,-4,570,1005,570,709,1208,-1,-5,570,1005,570,734,1207,-1,0,570,1005,570,759,1206,-1,774,1001,578,562,684,1,0,576,576,1001,578,566,692,1,0,577,577,21101,0,702,0,1106,0,786,21201,-1,-1,-1,1106,0,676,1001,578,1,578,1008,578,4,570,1006,570,724,1001,578,-4,578,21102,1,731,0,1106,0,786,1106,0,774,1001,578,-1,578,1008,578,-1,570,1006,570,749,1001,578,4,578,21102,1,756,0,1106,0,786,1106,0,774,21202,-1,-11,1,22101,1182,1,1,21102,1,774,0,1105,1,622,21201,-3,1,-3,1105,1,640,109,-5,2106,0,0,109,7,1005,575,802,21002,576,1,-6,20101,0,577,-5,1106,0,814,21102,0,1,-1,21102,1,0,-5,21101,0,0,-6,20208,-6,576,-2,208,-5,577,570,22002,570,-2,-2,21202,-5,43,-3,22201,-6,-3,-3,22101,1491,-3,-3,2101,0,-3,843,1005,0,863,21202,-2,42,-4,22101,46,-4,-4,1206,-2,924,21102,1,1,-1,1106,0,924,1205,-2,873,21102,1,35,-4,1105,1,924,1202,-3,1,878,1008,0,1,570,1006,570,916,1001,374,1,374,1202,-3,1,895,1102,2,1,0,1201,-3,0,902,1001,438,0,438,2202,-6,-5,570,1,570,374,570,1,570,438,438,1001,578,558,921,21001,0,0,-4,1006,575,959,204,-4,22101,1,-6,-6,1208,-6,43,570,1006,570,814,104,10,22101,1,-5,-5,1208,-5,65,570,1006,570,810,104,10,1206,-1,974,99,1206,-1,974,1101,0,1,575,21101,973,0,0,1106,0,786,99,109,-7,2105,1,0,109,6,21101,0,0,-4,21101,0,0,-3,203,-2,22101,1,-3,-3,21208,-2,82,-1,1205,-1,1030,21208,-2,76,-1,1205,-1,1037,21207,-2,48,-1,1205,-1,1124,22107,57,-2,-1,1205,-1,1124,21201,-2,-48,-2,1106,0,1041,21102,-4,1,-2,1105,1,1041,21101,-5,0,-2,21201,-4,1,-4,21207,-4,11,-1,1206,-1,1138,2201,-5,-4,1059,2102,1,-2,0,203,-2,22101,1,-3,-3,21207,-2,48,-1,1205,-1,1107,22107,57,-2,-1,1205,-1,1107,21201,-2,-48,-2,2201,-5,-4,1090,20102,10,0,-1,22201,-2,-1,-2,2201,-5,-4,1103,2101,0,-2,0,1105,1,1060,21208,-2,10,-1,1205,-1,1162,21208,-2,44,-1,1206,-1,1131,1106,0,989,21102,1,439,1,1106,0,1150,21102,1,477,1,1106,0,1150,21102,514,1,1,21101,0,1149,0,1105,1,579,99,21101,0,1157,0,1105,1,579,204,-2,104,10,99,21207,-3,22,-1,1206,-1,1138,1201,-5,0,1176,2102,1,-4,0,109,-6,2105,1,0,4,11,32,1,9,1,32,1,9,1,32,1,9,1,32,1,9,1,32,1,9,1,32,1,9,1,32,1,9,1,32,13,40,1,1,1,40,1,1,1,40,1,1,1,40,11,34,1,7,1,34,1,7,1,34,1,7,1,34,11,3,13,24,1,1,1,3,1,36,1,1,1,3,1,36,1,1,1,3,1,36,1,1,1,3,1,36,1,1,1,3,1,36,1,1,1,3,1,36,1,1,1,3,1,28,9,1,1,3,1,28,1,9,1,3,1,16,9,3,1,5,9,16,1,7,1,3,1,5,1,3,1,20,1,7,1,1,13,20,1,7,1,1,1,1,1,5,1,24,1,7,1,1,1,1,1,5,1,24,1,7,1,1,1,1,1,5,1,24,1,7,1,1,1,1,1,5,1,24,1,7,1,1,1,1,1,5,1,24,13,5,1,32,1,1,1,7,1,20,13,1,1,7,1,20,1,13,1,7,1,20,1,13,9,20,1,42,1,1,9,32,1,1,1,40,1,1,1,40,1,1,1,40,1,1,1,40,1,1,1,40,1,1,1,40,1,1,1,40,11,34,1,7,1,34,1,7,1,34,1,7,1,34,11,40,1,1,1,40,1,1,1,40,1,1,1,40,13,32,1,9,1,32,1,9,1,32,1,9,1,32,1,9,1,32,1,9,1,32,1,9,1,32,1,9,1,32,11,20
};

void part_one()
{
    Code code = to_code(actual_code, 200000);
    calculate_alignment_parameters(code);
    free_code(code);
}

char robot_char(Robot r)
{
    switch (r.dir)
    {
        case South: return 'v';
        case North: return '^';
        case West: return '<';
        case East: return '>';
    }
    return 'o';
}

struct Move
{
    enum {
        TurnLeft,
        TurnRight,
        MoveForward,

        FuncA,
        FuncB,
        FuncC,
    } type;
    int steps;

    static Move left() { return {TurnLeft}; }
    static Move right() { return {TurnRight}; }
    static Move forward(int steps) { return {MoveForward, steps}; }

    bool is_turn() { return type <= TurnRight; }
    bool is_step() { return type == MoveForward; }
    bool is_func() { return type >= FuncA; }
    void next() { steps++; }
    Move next_func() {
        Move result = {};
        result.type = decltype(type)((int)type + 1);
        return result;
    }

    static Move A() { return {FuncA}; }
    static Move B() { return {FuncB}; }
    static Move C() { return {FuncC}; }
};

bool operator == (Move a, Move b)
{
    return a.type == b.type && a.steps == b.steps;
}

bool operator != (Move a, Move b)
{
    return !(a.type == b.type && a.steps == b.steps);
}

template <>
void print(Move move)
{
    switch (move.type)
    {
        case Move::TurnLeft:    printf("L"); break;
        case Move::TurnRight:   printf("R"); break;
        case Move::MoveForward: printf("%d", move.steps); break;
        case Move::FuncA:        printf("A%d", move.steps); break;
        case Move::FuncB:        printf("B%d", move.steps); break;
        case Move::FuncC:        printf("C%d", move.steps); break;
    }
}

void take_step(Stack<Move> *moves, Robot *r)
{
    push(moves, Move::forward(1));
    r->pos = move(r->pos, r->dir);
}

void turn_left(Stack<Move> *moves, Robot *r)
{
    push(moves, Move::left());
    push(moves, Move::forward(1));
    r->dir = to_left(r->dir);
    r->pos = move(r->pos, r->dir);
}

void turn_right(Stack<Move> *moves, Robot *r)
{
    push(moves, Move::right());
    push(moves, Move::forward(1));
    r->dir = to_right(r->dir);
    r->pos = move(r->pos, r->dir);
}

int reduced_len(Stack<Move> s)
{
    int result = 0;
    int moves = 0;
    int steps = 0;
    Move last_func = {};
    for (int i = 0; i < s.len; i++)
    {
        if (!s[i].is_step() && steps > 0)
        {
            if (steps >= 10) result += 2;
            moves++;
            steps = 0;
        }

        if (s[i].is_turn())
        {
            result++;
            moves++;
        }
        else if (s[i].is_step())
        {
            steps += s[i].steps;
        }
        else if (s[i].is_func() && s[i] != last_func)
        {
            result++;
            moves++;
            last_func = s[i];
        }
    }
    if (steps > 0)
    {
        if (steps >= 10) result += 2;
        moves++;
        steps = 0;
    }
    return result + moves - 1;
}

int reduced_len(Stack<Move> moves, int start, int len)
{
    Stack<Move> func = {};
    func.data = moves.data + start;
    func.len = len;
    return reduced_len(func);
}

bool fits_in_limit(Stack<Move> moves, int start, int len)
{
    return reduced_len(moves, start, len) <= 20;
}

void replace_reps_by_func(Stack<Move> *moves, Stack<Move> func, Move func_move)
{
    int find_start_index = 0;
    while (find_start_index + func.len <= moves->len)
    {
        int find_index = find_start_index;
        int index = 0;
        while (index < func.len && func[index] == (*moves)[find_index])
        {
            index++;
            find_index++;
        }
        if (index == func.len)
        {
            for (int i = find_start_index; i < find_index; i++)
                (*moves)[i] = func_move;
            func_move.next();

            find_start_index = find_index;
        }
        else
        {
            find_start_index = find_start_index + 1;
        }
    }
}

bool contains_only_funcs(Stack<Move> moves)
{
    for (int i = 0; i < moves.len; i++) if (!moves[i].is_func()) return false;
    return true;
}

struct Routines
{
    Stack<Move> main;
    Stack<Move> A;
    Stack<Move> B;
    Stack<Move> C;
};

bool find_routines(Stack<Move> moves, Move func_move, Routines *result)
{
    int cur_func_len = 1;
    int cur_func_start = 0;
    while (cur_func_start < moves.len && moves[cur_func_start].is_func())
    {
        cur_func_start++;
    }
    //printf("Cur func start %d\n", cur_func_start); fflush(stdout);
    while (cur_func_start + cur_func_len < moves.len
            && !moves[cur_func_start + cur_func_len - 1].is_func()
        && fits_in_limit(moves, cur_func_start, cur_func_len))
    {
        Stack<Move> new_moves = copy_n(moves, 0, moves.len);
        Stack<Move> func = moves;
        func.data += cur_func_start;
        func.len = cur_func_len;
        replace_reps_by_func(&new_moves, func, func_move);

        switch (func_move.type)
        {
        case Move::FuncA:
            {
                bool found = find_routines(new_moves, func_move.next_func(), result);
                if (found)
                {
                    result->A = copy_n(moves, cur_func_start, cur_func_len);
                    return found;
                }
            } break;
        case Move::FuncB:
            {
                bool found = find_routines(new_moves, func_move.next_func(), result);
                if (found)
                {
                    result->B = copy_n(moves, cur_func_start, cur_func_len);
                    return found;
                }
            } break;
        case Move::FuncC:
            {
                if (fits_in_limit(new_moves, 0, new_moves.len) && contains_only_funcs(new_moves))
                {
                    result->main = new_moves;
                    result->C = copy_n(moves, cur_func_start, cur_func_len);
                    return true;
                }
            } break;
        default: break;
        }

        free_stack(new_moves);

        cur_func_len++;
    }
    return false;
}

Stack<Move> reduce(Stack<Move> s)
{
    Stack<Move> result = {};
    int steps = 0;
    Move last_func = {};
    for (int i = 0; i < s.len; i++)
    {
        if (!s[i].is_step() && steps > 0)
        {
            push(&result, Move::forward(steps));
            steps = 0;
        }

        if (s[i].is_turn())
        {
            push(&result, s[i]);
        }
        else if (s[i].is_step())
        {
            steps += s[i].steps;
        }
        else if (s[i].is_func() && s[i] != last_func)
        {
            push(&result, s[i]);
            last_func = s[i];
        }
    }
    if (steps > 0)
    {
        push(&result, Move::forward(steps));
        steps = 0;
    }
    return result;
}

Routines find_solution()
{
    Code code = to_code(actual_code, 200000);

    Buffer input = {};
    Buffer output = {};
    State state = {};
    state.input = &input;
    state.output = &output;

    Bounds initial_bounds = {
        .min_x = 0, .max_x = 1,
        .min_y = 0, .max_y = 1,
    };
    Grid grid = alloc_grid(initial_bounds);

    Pos cursor = {};
    while (!state.halted)
    {
        int res;
        for (int i = 0; i < 10; i++)
        {
            res = step(&state, code);
            if (res != 1) break;
        }
        while (readable_num(output) > 0)
        {
            IntWord ch;
            read(&output, &ch);

            if (ch != 10)
            {
                grid(cursor) = ch;
                cursor.x++;
            }
            else
            {
                cursor.x = 0;
                cursor.y++;
            }
        }
    }
    free_code(code);

    int scaffolding = 0;
    Robot robot = {};
    for (int y = grid.bounds.min_y; y <= grid.bounds.max_y; y++)
    {
        for (int x = grid.bounds.min_x; x <= grid.bounds.max_x; x++)
        {
            if (grid(x, y) == '^')
            {
                robot.pos = {x, y};
                robot.dir = North;
            }
            else if (grid(x, y) == '#')
            {
                scaffolding++;
            }
        }
    }

    Stack<Move> moves = {};
    while (scaffolding > 0)
    {
        Pos fwd = move(robot.pos, robot.dir);
        if (grid(fwd) == '#')
        {
            grid(robot.pos) = '*';
            take_step(&moves, &robot);
            grid(robot.pos) = robot_char(robot);
            scaffolding--;
        }
        else if (grid(fwd) == '*' && grid(move(fwd, robot.dir)) == '#')
        {
            grid(robot.pos) = '*';
            take_step(&moves, &robot);
            take_step(&moves, &robot);
            grid(robot.pos) = robot_char(robot);
            scaffolding--;
        }
        else if (grid(move(robot.pos, to_left(robot.dir))) == '#')
        {
            grid(robot.pos) = '*';
            turn_left(&moves, &robot);
            grid(robot.pos) = robot_char(robot);
            scaffolding--;
        }
        else if (grid(move(robot.pos, to_right(robot.dir))) == '#')
        {
            grid(robot.pos) = '*';
            turn_right(&moves, &robot);
            grid(robot.pos) = robot_char(robot);
            scaffolding--;
        }
        else
        {
            printf("Deadend, scaffolding left: %d\n", scaffolding);
            break;
        }
    }

    print_grid(grid);

    Routines routines = {};
    if (find_routines(moves, Move::A(), &routines))
    {
        Routines result;
        result.main = reduce(routines.main);
        result.A = reduce(routines.A);
        result.B = reduce(routines.B);
        result.C = reduce(routines.C);

        free_stack(routines.main);
        free_stack(routines.A);
        free_stack(routines.B);
        free_stack(routines.C);

        printf("Main: "); print_stack(result.main); printf("\n");
        printf("A: "); print_stack(result.A); printf("\n");
        printf("B: "); print_stack(result.B); printf("\n");
        printf("C: "); print_stack(result.C); printf("\n");
        return result;
    }
    else
    {
        printf("NO SOLUTION FOUND!!\n");
        fflush(stdout);
        abort();
    }
}

int convert_to_string(char *buf, Stack<Move> m)
{
    char *s = buf;
    for (int i = 0; i < m.len; i++)
    {
        switch (m[i].type)
        {
            case Move::TurnLeft:    *s++ = 'L'; break;
            case Move::TurnRight:   *s++ = 'R'; break;
            case Move::MoveForward:
                {
                    int n = sprintf(s, "%d", m[i].steps);
                    s += n;
                } break;
            case Move::FuncA:       *s++ = 'A'; break;
            case Move::FuncB:       *s++ = 'B'; break;
            case Move::FuncC:       *s++ = 'C'; break;
        }
        if (i + 1 < m.len) *s++ = ',';
    }
    *s++ = '\n';
    return s - buf;
}

void traverse_scaffolding(Code code)
{
    Routines routines = find_solution();

    // To wake up the vacuum robot, set to 2
    code[0] = 2;

    Buffer input = {};
    Buffer output = {};
    State state = {};
    state.input = &input;
    state.output = &output;

    char A_buf[80], B_buf[80], C_buf[80], main_buf[80];
    int A_len = convert_to_string(A_buf, routines.A);
    int B_len = convert_to_string(B_buf, routines.B);
    int C_len = convert_to_string(C_buf, routines.C);
    int main_len = convert_to_string(main_buf, routines.main);

    for (int i = 0; i < main_len; i++) write(&input, main_buf[i]);
    for (int i = 0; i < A_len; i++) write(&input, A_buf[i]);
    for (int i = 0; i < B_len; i++) write(&input, B_buf[i]);
    for (int i = 0; i < C_len; i++) write(&input, C_buf[i]);

    // Continuous video feed y/n?
    write(&input, 'n');
    write(&input, '\n');

    IntWord dust;
    while (!state.halted)
    {
        int res;
        for (int i = 0; i < 10; i++)
        {
            res = step(&state, code);
            if (res != 1) break;
        }
        if (res == 0)
        {
            printf("Waiting for input..\n");
            break;
        }
        if (readable_num(output) > 0)
        {
            if (state.halted)
            {
                read(&output, &dust);
            }
            else
            {
                while (readable_num(output) > 0)
                {
                    IntWord ch;
                    read(&output, &ch);

                    putchar(ch);
                    fflush(stdout);
                }
            }
        }
    }

    printf("Dust collected %lld\n", dust);
}

void part_two()
{
    Code code = to_code(actual_code, 200000);
    traverse_scaffolding(code);
    free_code(code);
}

int main(int argc, const char **argv)
{
    //part_one();
    part_two();
    return 0;
}

