// 776996-20191205-0a4a7f2b

/*

--- Day 15: Oxygen System ---

Out here in deep space, many things can go wrong. Fortunately, many of those
things have indicator lights. Unfortunately, one of those lights is lit: the
oxygen system for part of the ship has failed!

According to the readouts, the oxygen system must have failed days ago after a
rupture in oxygen tank two; that section of the ship was automatically sealed
once oxygen levels went dangerously low. A single remotely-operated repair
droid is your only option for fixing the oxygen system.

The Elves' care package included an Intcode program (your puzzle input) that
you can use to remotely control the repair droid. By running that program, you
can direct the repair droid to the oxygen system and fix the problem.

The remote control program executes the following steps in a loop forever:

    Accept a movement command via an input instruction.
    Send the movement command to the repair droid.
    Wait for the repair droid to finish the movement operation.
    Report on the status of the repair droid via an output instruction.

Only four movement commands are understood: north (1), south (2), west (3), and
east (4). Any other command is invalid. The movements differ in direction, but
not in distance: in a long enough east-west hallway, a series of commands like
4,4,4,4,3,3,3,3 would leave the repair droid back where it started.

The repair droid can reply with any of the following status codes:

    0: The repair droid hit a wall. Its position has not changed.
    1: The repair droid has moved one step in the requested direction.
    2: The repair droid has moved one step in the requested direction; its new
    position is the location of the oxygen system.

You don't know anything about the area around the repair droid, but you can
figure it out by watching the status codes.

For example, we can draw the area using D for the droid, # for walls, . for
locations the droid can traverse, and empty space for unexplored locations.
Then, the initial state looks like this:



   D



To make the droid go north, send it 1. If it replies with 0, you know that
location is a wall and that the droid didn't move:


   #
   D



To move east, send 4; a reply of 1 means the movement was successful:


   #
   .D



Then, perhaps attempts to move north (1), south (2), and east (4) are all met
with replies of 0:


   ##
   .D#
    #


Now, you know the repair droid is in a dead end. Backtrack with 3 (which you
already know will get a reply of 1 because you already know that location is open):


   ##
   D.#
    #


Then, perhaps west (3) gets a reply of 0, south (2) gets a reply of 1, south
again (2) gets a reply of 0, and then west (3) gets a reply of 2:


   ##
  #..#
  D.#
   #

Now, because of the reply of 2, you know you've found the oxygen system! In
this example, it was only 2 moves away from the repair droid's starting
position.

What is the fewest number of movement commands required to move the repair
droid from its starting position to the location of the oxygen system?

Your puzzle answer was 254.
--- Part Two ---

You quickly repair the oxygen system; oxygen gradually fills the area.

Oxygen starts in the location containing the repaired oxygen system. It takes
one minute for oxygen to spread to all open locations that are adjacent to a
location that already contains oxygen. Diagonal locations are not adjacent.

In the example above, suppose you've used the droid to explore the area fully
and have the following map (where locations that currently contain oxygen are
        marked O):

 ##
#..##
#.#..#
#.O.#
 ###

Initially, the only location which contains oxygen is the location of the
repaired oxygen system. However, after one minute, the oxygen spreads to all
open (.) locations that are adjacent to a location containing oxygen:

 ##
#..##
#.#..#
#OOO#
 ###

After a total of two minutes, the map looks like this:

 ##
#..##
#O#O.#
#OOO#
 ###

After a total of three minutes:

 ##
#O.##
#O#OO#
#OOO#
 ###

And finally, the whole region is full of oxygen after a total of four minutes:

 ##
#OO##
#O#OO#
#OOO#
 ###

So, in this example, all locations contain oxygen after 4 minutes.

Use the repair droid to get a complete map of the area. How many minutes will
it take to fill with oxygen?

Your puzzle answer was 268.

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

enum Tile
{
    T_Empty = 0,
    T_Walkable = 1,
    T_Wall = 2,
    T_Droid = 3,
    T_OxygenSys = 4,
};

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

    //printf("New bounds: %d < x %d; %d < y < %d\n",
    //        new_bounds.min_x, new_bounds.max_x,
    //        new_bounds.min_y, new_bounds.max_y);
    //fflush(stdout);

    Grid new_grid = alloc_grid(new_bounds);

    for (int y = grid->bounds.min_y; y <= grid->bounds.max_y; y++)
    {
        for (int x = grid->bounds.min_x; x <= grid->bounds.max_x; x++)
        {
            //printf("Copy %d,%d\n", x, y);
            new_grid(x, y) = (*grid)(x, y);
        }
    }
    //printf("Copied!\n");
    //fflush(stdout);

    //free_grid(grid);
    *grid = new_grid;

    //print_grid(new_grid);
    //fflush(stdout);
}

void print_grid(Grid grid)
{
    const char *clear = "\e[m";
    const char *oxy = "\e[1m\e[38;5;12m";
    const char *droid = "\e[1m\e[36m";
    const char *wall = "\e[38;5;8m\e[48;5;8m";
    const char *walkable = "\e[38;5;8m";
    for (int y = grid.bounds.min_y; y <= grid.bounds.max_y; y++)
    {
        for (int x = grid.bounds.min_x; x <= grid.bounds.max_x; x++)
        {
            char c = ' ';
            int tile_bits = grid(x, y);
            bool has_oxygen_sys = ((tile_bits & T_OxygenSys) != 0);
            Tile tile = (Tile)(tile_bits & 0x3);
            switch (tile)
            {
                case T_Empty:     c = ' '; break;
                case T_Walkable:  c = '.'; break;
                case T_Wall:      c = '#'; break;
                case T_Droid:     c = 'D'; break;
                case T_OxygenSys: c = 'X'; break;
            }
            if (has_oxygen_sys)
            {
                fputs(oxy, stdout);
                c = 'X';
            }
            else
            {
                switch (tile)
                {
                    case T_Empty:     fputs(clear, stdout); break;
                    case T_Walkable:  fputs(walkable, stdout); break;
                    case T_Wall:      fputs(wall, stdout); break;
                    case T_Droid:     fputs(droid, stdout); break;
                    case T_OxygenSys: fputs(oxy, stdout); break;
                }
            }
            putchar(c);
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

int manhattan_distance(Pos a, Pos b)
{
    return abs(a.x - b.x) + abs(a.y - b.y);
}

bool is_adjacent(Pos a, Pos b) { return manhattan_distance(a, b) == 1; }

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

enum DroidState
{
    S_NoTarget,
    S_GotoTarget,
};

struct Droid
{
    Pos pos;
    Direction moving_dir;

    Pos target;
    DroidState state;
};

Pos move(Pos p, int dir)
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

void move_droid(Droid *d, Grid *grid)
{
    Pos p = d->pos;
    (*grid)(p.x, p.y) &= ~(T_OxygenSys-1);
    (*grid)(p.x, p.y) |= T_Walkable;

    p = move(p, d->moving_dir);
    (*grid)(p.x, p.y) = T_Droid;

    d->pos = p;
}

void bump_to_wall(Droid *d, Grid *grid)
{
    Pos p = move(d->pos, d->moving_dir);
    (*grid)(p.x, p.y) = T_Wall;
}

void found_oxygen_system(Droid *d, Grid *grid)
{
    Pos p = d->pos;
    (*grid)(p.x, p.y) &= ~(T_OxygenSys-1);

    p = move(p, d->moving_dir);
    (*grid)(p.x, p.y) = T_Droid | T_OxygenSys;

    d->pos = p;
}

template <class T>
struct Stack
{
    T *data;
    int len, cap;
};

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
void print_stack(Stack<T> *s)
{
    printf("[");
    if (s->len > 0)
    {
        printf("(%d,%d)", s->data[0].x, s->data[0].y);
        for (int i = 1; i < s->len; i++)
        {
            printf(",(%d,%d)", s->data[i].x, s->data[i].y);
        }
    }
    printf("]");
}

struct DroidControlState
{
    Stack<Pos> ps;
    Stack<int> ps_move_index; // the position in moves stack to unwind to get to the adjacent pos
    Stack<Direction> moves;
};

void push_next_positions(DroidControlState *ds, Pos p, Grid *grid)
{
    Grid g = *grid;
    Pos left = {p.x-1, p.y};
    Pos right = {p.x+1, p.y};
    Pos up = {p.x, p.y-1};
    Pos down = {p.x, p.y+1};
    if (g(left) == T_Empty)
    {
        push(&ds->ps, left);
        push(&ds->ps_move_index, ds->moves.len-1);
    }
    if (g(right) == T_Empty)
    {
        push(&ds->ps, right);
        push(&ds->ps_move_index, ds->moves.len-1);
    }
    if (g(up) == T_Empty)
    {
        push(&ds->ps, up);
        push(&ds->ps_move_index, ds->moves.len-1);
    }
    if (g(down) == T_Empty)
    {
        push(&ds->ps, down);
        push(&ds->ps_move_index, ds->moves.len-1);
    }
}

void pop_target(DroidControlState *ds, Droid *droid, Grid grid)
{
    while (ds->ps.len > 0)
    {
        Pos p = pop(&ds->ps);
        if (grid(p) != T_Empty) continue;

        droid->target = p;
        break;
    }
}

void handle_node(Grid scores, Grid grid, Stack<Pos> *st, Pos n, int cur_score)
{
    if (grid(n) == T_Walkable || grid(n) == T_Droid || (grid(n) & T_OxygenSys))
    {
        int cs = scores(n);
        if (cs == 0 || cur_score + 1 < cs)
        {
            scores(n) = cur_score + 1;
        }
        if (cs == 0)
        {
            push(st, n);
        }
    }
}

int find_shortest_path_len(Grid grid, Pos from, Pos to)
{
    Grid scores = alloc_grid(grid.bounds);
    Stack<Pos> st = {};
    Pos p = from;
    int cur_score = 1;
    scores(p) = cur_score;
    push(&st, p);
    while (st.len > 0)
    {
        //print_stack(&st);
        //printf("\n");
        Pos p = pop(&st);
        cur_score = scores(p);
        if (p == to)
        {
            break;
        }

        Pos nn = move(p, North);
        Pos sn = move(p, South);
        Pos wn = move(p, West);
        Pos en = move(p, East);
        handle_node(scores, grid, &st, nn, cur_score);
        handle_node(scores, grid, &st, sn, cur_score);
        handle_node(scores, grid, &st, wn, cur_score);
        handle_node(scores, grid, &st, en, cur_score);
    }
    free_grid(&scores);

    // Score is 1+steps
    int dist = cur_score - 1;
    return dist;
}

Pos execute_droid_control(Code code, Grid *grid, Droid *droid, DroidControlState *ds)
{
    Buffer input = {};
    Buffer output = {};
    State state = {};
    state.input = &input;
    state.output = &output;

    Pos oxygen_sys_pos = {};

    int round = 0;
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
            IntWord status;
            read(&output, &status);

            //printf("Status received: %lld\n", status);
            //fflush(stdout);

            if (status == 0)
            {
                bump_to_wall(droid, grid);
                Pos wall = move(droid->pos, droid->moving_dir);
                if (droid->target == wall)
                {
                    pop_target(ds, droid, *grid);
                }
                else
                {
                    droid->moving_dir = select_other_direction(droid->moving_dir);
                }
            }
            else if (status == 1)
            {
                if (is_adjacent(droid->pos, droid->target))
                {
                    push(&ds->moves, droid->moving_dir);
                }
                move_droid(droid, grid);
                if (droid->pos == droid->target)
                {
                    push_next_positions(ds, droid->pos, grid);
                }
            }
            else if (status == 2)
            {
                found_oxygen_system(droid, grid);
                push(&ds->moves, droid->moving_dir);
                if (droid->pos == droid->target)
                {
                    push_next_positions(ds, droid->pos, grid);
                }

                oxygen_sys_pos = droid->pos;
            }
        }

        if (res == 0) // waiting input
        {
            if (round % 50 == 0)
            {
                print_grid(*grid);
                fflush(stdout);

                //print_stack(s);
                printf("\n");
                printf("Droid (%d,%d) -> (%d,%d)\n",
                        droid->pos.x, droid->pos.y,
                        droid->target.x, droid->target.y);
                printf("> ");
                fflush(stdout);
                int c;
                while ((c = getchar()) != '\n')
                {
                    if (c == 'q') state.halted = true;
                }
                /*
                int c;
                while ((c = getchar()) != '\n')
                {
                    //int c = getchar();
                    if (c == 'w') {
                        droid->moving_dir = 1;
                    }
                    else if (c == 'a') {
                        droid->moving_dir = 3;
                    }
                    else if (c == 's') {
                        droid->moving_dir = 2;
                    }
                    else if (c == 'd') {
                        droid->moving_dir = 4;
                    }
                    else
                    {
                        printf("%d was not recognized char :(\n", c);
                    }
                    fflush(stdout);
                }
                */
            }

            if (droid->pos == droid->target)
            {
                push_next_positions(ds, droid->pos, grid);
                pop_target(ds, droid, *grid);
                if (is_adjacent(droid->pos, droid->target))
                {
                    droid->moving_dir = select_direction_towards(droid->pos, droid->target);
                }
                else
                {
                    //droid->moving_dir = random_dir();
                    droid->moving_dir = opposite_dir(pop(&ds->moves));
                }
            }
            else if (is_adjacent(droid->pos, droid->target))
            {
                droid->moving_dir = select_direction_towards(droid->pos, droid->target);
            }
            else
            {
                if (ds->moves.len > 0)
                    droid->moving_dir = opposite_dir(pop(&ds->moves));
                // ...
            }
            write(&input, droid->moving_dir);
        }
        round++;
    }

    return oxygen_sys_pos;
}

IntWord actual_code[] = {
    3,1033,1008,1033,1,1032,1005,1032,31,1008,1033,2,1032,1005,1032,58,1008,1033,3,1032,1005,1032,81,1008,1033,4,1032,1005,1032,104,99,1002,1034,1,1039,1002,1036,1,1041,1001,1035,-1,1040,1008,1038,0,1043,102,-1,1043,1032,1,1037,1032,1042,1105,1,124,1001,1034,0,1039,101,0,1036,1041,1001,1035,1,1040,1008,1038,0,1043,1,1037,1038,1042,1105,1,124,1001,1034,-1,1039,1008,1036,0,1041,101,0,1035,1040,1001,1038,0,1043,101,0,1037,1042,1105,1,124,1001,1034,1,1039,1008,1036,0,1041,1001,1035,0,1040,1002,1038,1,1043,1001,1037,0,1042,1006,1039,217,1006,1040,217,1008,1039,40,1032,1005,1032,217,1008,1040,40,1032,1005,1032,217,1008,1039,37,1032,1006,1032,165,1008,1040,39,1032,1006,1032,165,1102,2,1,1044,1105,1,224,2,1041,1043,1032,1006,1032,179,1101,0,1,1044,1106,0,224,1,1041,1043,1032,1006,1032,217,1,1042,1043,1032,1001,1032,-1,1032,1002,1032,39,1032,1,1032,1039,1032,101,-1,1032,1032,101,252,1032,211,1007,0,37,1044,1106,0,224,1102,0,1,1044,1105,1,224,1006,1044,247,1002,1039,1,1034,1001,1040,0,1035,1002,1041,1,1036,102,1,1043,1038,1002,1042,1,1037,4,1044,1105,1,0,2,32,78,22,32,29,53,14,61,46,21,16,34,19,73,25,76,17,97,20,4,63,23,46,15,13,75,30,58,28,29,82,23,32,11,22,16,82,2,57,24,31,48,51,4,52,25,92,15,78,78,55,32,46,5,31,88,21,74,29,47,89,34,80,58,14,33,4,69,74,33,70,60,7,39,29,68,12,1,11,64,17,75,4,52,11,47,24,71,23,99,83,28,17,56,94,33,8,90,9,83,7,62,15,77,45,49,5,53,36,67,18,82,93,22,53,9,20,20,60,90,22,25,48,15,27,68,12,27,13,50,25,92,73,35,81,15,1,48,22,12,35,38,1,36,44,12,82,30,92,22,71,31,39,20,43,34,46,36,24,67,72,13,85,45,18,68,64,20,40,2,67,25,15,33,40,53,48,32,59,13,57,28,61,26,15,88,21,42,15,95,34,74,32,7,82,63,22,95,22,83,22,20,25,11,81,88,94,31,9,50,26,76,78,34,88,19,68,72,7,85,14,54,80,5,5,45,24,24,91,22,34,39,32,22,11,15,87,57,35,83,86,51,23,71,29,13,23,59,51,36,46,33,27,99,4,13,59,14,55,88,89,29,22,97,46,40,2,17,48,93,9,40,35,94,6,71,34,14,2,39,29,36,5,55,72,31,22,87,4,50,27,92,36,88,20,82,79,21,35,67,57,23,48,6,15,65,10,69,12,29,3,8,51,56,90,29,88,59,28,40,89,18,93,83,2,66,46,22,50,30,86,3,49,55,22,33,97,27,51,15,7,26,57,36,98,3,64,35,84,90,16,88,3,7,98,94,13,1,13,71,88,36,17,84,29,5,57,50,84,14,47,25,85,64,31,95,8,43,10,81,36,58,3,40,24,40,20,13,5,14,50,42,23,9,74,40,92,4,10,3,60,1,91,39,27,77,9,20,42,47,35,15,90,43,21,46,30,63,85,28,93,6,82,8,86,86,88,30,33,26,8,92,58,32,20,1,40,72,79,49,68,14,73,6,2,99,9,5,12,47,43,14,29,66,8,31,12,97,8,69,32,63,31,96,23,32,24,60,69,74,15,24,6,76,39,14,33,89,36,6,63,21,10,95,95,32,45,41,8,76,82,14,78,15,79,72,71,34,39,27,56,27,48,28,94,21,30,25,27,53,1,81,26,24,80,55,27,51,2,93,15,80,12,28,36,56,3,7,77,34,90,49,44,24,35,99,63,11,88,93,28,75,21,62,57,8,44,10,57,9,61,4,43,3,21,20,41,95,13,6,98,16,93,70,98,64,27,35,49,12,18,23,17,68,5,11,13,61,79,30,87,53,11,11,26,80,23,55,92,46,31,70,13,76,87,29,6,91,19,90,88,36,39,25,99,12,87,90,1,93,12,98,28,27,44,51,18,32,80,86,1,26,1,19,99,83,18,2,58,29,68,3,77,82,6,55,63,56,2,61,4,90,21,22,71,30,36,51,64,32,44,52,9,51,80,93,9,71,20,41,98,21,12,61,80,10,80,33,92,80,78,8,29,9,70,4,76,24,13,92,5,26,80,88,72,3,3,49,73,27,98,15,46,30,73,17,94,30,78,5,75,16,2,57,3,96,15,47,36,31,53,39,34,44,26,96,41,68,9,81,20,40,25,76,55,9,67,3,28,18,63,1,31,31,87,22,20,67,10,2,77,20,74,28,79,34,52,91,51,24,47,13,58,9,61,10,77,25,72,17,45,8,51,16,72,3,69,80,79,6,53,48,83,34,63,86,42,19,42,0,0,21,21,1,10,1,0,0,0,0,0,0
};

void part_one()
{
    Bounds initial_bounds = {
        .min_x = -10, .max_x = 10,
        .min_y = -10, .max_y = 10,
    };
    Grid grid = alloc_grid(initial_bounds);

    Droid droid = {};
    droid.moving_dir = North;
    grid(0, 0) = T_Droid;

    DroidControlState control_state = {};

    Code code = to_code(actual_code, 200000);
    Pos oxygen_sys_pos = execute_droid_control(code, &grid, &droid, &control_state);
    free_code(code);

    int dist = find_shortest_path_len(grid, Pos{}, oxygen_sys_pos);
    printf("Distance to oxygen system: %d\n", dist);
    fflush(stdout);

    free_grid(&grid);
}

int fill_with_oxygen(Grid grid, Pos start)
{
    Grid scores = alloc_grid(grid.bounds);
    Stack<Pos> st = {};
    Pos p = start;
    int cur_score = 1;
    int max_score = 1;
    scores(p) = cur_score;
    push(&st, p);
    while (st.len > 0)
    {
        //print_stack(&st);
        //printf("\n");
        Pos p = pop(&st);
        cur_score = scores(p);

        max_score = max(max_score, cur_score);

        Pos nn = move(p, North);
        Pos sn = move(p, South);
        Pos wn = move(p, West);
        Pos en = move(p, East);
        handle_node(scores, grid, &st, nn, cur_score);
        handle_node(scores, grid, &st, sn, cur_score);
        handle_node(scores, grid, &st, wn, cur_score);
        handle_node(scores, grid, &st, en, cur_score);
    }
    free_grid(&scores);

    int minutes = max_score - 1;
    return minutes;
}

void part_two()
{
    Bounds initial_bounds = {
        .min_x = -25, .max_x = 25,
        .min_y = -25, .max_y = 25,
    };
    Grid grid = alloc_grid(initial_bounds);

    Droid droid = {};
    droid.moving_dir = North;
    grid(0, 0) = T_Droid;

    DroidControlState control_state = {};

    Code code = to_code(actual_code, 200000);
    Pos oxygen_sys_pos = execute_droid_control(code, &grid, &droid, &control_state);
    free_code(code);

    int minutes = fill_with_oxygen(grid, oxygen_sys_pos);
    printf("Filling with oxygen takes %d minutes.\n", minutes);
    fflush(stdout);

    free_grid(&grid);
}

int main(int argc, const char **argv)
{
    //part_one();
    part_two();
    return 0;
}

