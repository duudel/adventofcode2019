/*

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

int survey_hull_damage(Code code, const char *script)
{
    Buffer input = {};
    Buffer output = {};
    State state = {};
    state.input = &input;
    state.output = &output;

    int script_len = strlen(script);
    for (int i = 0; i < script_len; i++) {
        write(&input, script[i]);
    }

    int total_damage = 0;
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

            if (out < 128) { // ASCII
                putchar(out);
            } else {
                total_damage += out;
            }
        }
    }

    return total_damage;
}

IntWord actual_code[] = {
    109,   2050,  21102, 1,     966,   1,     21102, 1,     13,    0,     1106,
    0,     1378,  21102, 1,     20,    0,     1106,  0,     1337,  21101, 0,
    27,    0,     1106,  0,     1279,  1208,  1,     65,    748,   1005,  748,
    73,    1208,  1,     79,    748,   1005,  748,   110,   1208,  1,     78,
    748,   1005,  748,   132,   1208,  1,     87,    748,   1005,  748,   169,
    1208,  1,     82,    748,   1005,  748,   239,   21101, 0,     1041,  1,
    21102, 73,    1,     0,     1105,  1,     1421,  21102, 78,    1,     1,
    21101, 1041,  0,     2,     21102, 1,     88,    0,     1105,  1,     1301,
    21102, 68,    1,     1,     21102, 1,     1041,  2,     21102, 103,   1,
    0,     1106,  0,     1301,  1102,  1,     1,     750,   1106,  0,     298,
    21102, 1,     82,    1,     21102, 1041,  1,     2,     21101, 125,   0,
    0,     1105,  1,     1301,  1101,  2,     0,     750,   1105,  1,     298,
    21101, 0,     79,    1,     21101, 1041,  0,     2,     21101, 147,   0,
    0,     1106,  0,     1301,  21101, 0,     84,    1,     21102, 1,     1041,
    2,     21102, 1,     162,   0,     1106,  0,     1301,  1102,  3,     1,
    750,   1106,  0,     298,   21102, 1,     65,    1,     21102, 1041,  1,
    2,     21102, 1,     184,   0,     1106,  0,     1301,  21101, 0,     76,
    1,     21102, 1041,  1,     2,     21101, 0,     199,   0,     1106,  0,
    1301,  21101, 0,     75,    1,     21102, 1041,  1,     2,     21101, 0,
    214,   0,     1106,  0,     1301,  21102, 221,   1,     0,     1106,  0,
    1337,  21102, 10,    1,     1,     21101, 1041,  0,     2,     21102, 1,
    236,   0,     1106,  0,     1301,  1105,  1,     553,   21102, 1,     85,
    1,     21101, 0,     1041,  2,     21102, 1,     254,   0,     1106,  0,
    1301,  21101, 78,    0,     1,     21101, 0,     1041,  2,     21102, 269,
    1,     0,     1105,  1,     1301,  21101, 0,     276,   0,     1105,  1,
    1337,  21101, 0,     10,    1,     21101, 1041,  0,     2,     21101, 291,
    0,     0,     1106,  0,     1301,  1101,  1,     0,     755,   1105,  1,
    553,   21102, 32,    1,     1,     21102, 1041,  1,     2,     21101, 0,
    313,   0,     1106,  0,     1301,  21102, 1,     320,   0,     1106,  0,
    1337,  21101, 327,   0,     0,     1106,  0,     1279,  2101,  0,     1,
    749,   21101, 65,    0,     2,     21101, 0,     73,    3,     21102, 1,
    346,   0,     1106,  0,     1889,  1206,  1,     367,   1007,  749,   69,
    748,   1005,  748,   360,   1101,  0,     1,     756,   1001,  749,   -64,
    751,   1106,  0,     406,   1008,  749,   74,    748,   1006,  748,   381,
    1101,  -1,    0,     751,   1105,  1,     406,   1008,  749,   84,    748,
    1006,  748,   395,   1101,  0,     -2,    751,   1105,  1,     406,   21101,
    1100,  0,     1,     21102, 1,     406,   0,     1106,  0,     1421,  21101,
    32,    0,     1,     21102, 1100,  1,     2,     21101, 421,   0,     0,
    1106,  0,     1301,  21102, 428,   1,     0,     1106,  0,     1337,  21101,
    0,     435,   0,     1106,  0,     1279,  2101,  0,     1,     749,   1008,
    749,   74,    748,   1006,  748,   453,   1101,  -1,    0,     752,   1106,
    0,     478,   1008,  749,   84,    748,   1006,  748,   467,   1102,  1,
    -2,    752,   1106,  0,     478,   21101, 1168,  0,     1,     21102, 1,
    478,   0,     1105,  1,     1421,  21101, 0,     485,   0,     1105,  1,
    1337,  21102, 10,    1,     1,     21102, 1,     1168,  2,     21102, 500,
    1,     0,     1106,  0,     1301,  1007,  920,   15,    748,   1005,  748,
    518,   21102, 1209,  1,     1,     21102, 518,   1,     0,     1106,  0,
    1421,  1002,  920,   3,     529,   1001,  529,   921,   529,   1002,  750,
    1,     0,     1001,  529,   1,     537,   1002,  751,   1,     0,     1001,
    537,   1,     545,   1002,  752,   1,     0,     1001,  920,   1,     920,
    1106,  0,     13,    1005,  755,   577,   1006,  756,   570,   21101, 1100,
    0,     1,     21101, 570,   0,     0,     1105,  1,     1421,  21102, 1,
    987,   1,     1106,  0,     581,   21101, 0,     1001,  1,     21101, 588,
    0,     0,     1106,  0,     1378,  1102,  1,     758,   594,   102,   1,
    0,     753,   1006,  753,   654,   20102, 1,     753,   1,     21102, 1,
    610,   0,     1106,  0,     667,   21101, 0,     0,     1,     21102, 621,
    1,     0,     1105,  1,     1463,  1205,  1,     647,   21101, 1015,  0,
    1,     21101, 635,   0,     0,     1106,  0,     1378,  21102, 1,     1,
    1,     21101, 646,   0,     0,     1106,  0,     1463,  99,    1001,  594,
    1,     594,   1105,  1,     592,   1006,  755,   664,   1102,  0,     1,
    755,   1106,  0,     647,   4,     754,   99,    109,   2,     1102,  726,
    1,     757,   21201, -1,    0,     1,     21102, 9,     1,     2,     21101,
    0,     697,   3,     21102, 692,   1,     0,     1106,  0,     1913,  109,
    -2,    2106,  0,     0,     109,   2,     101,   0,     757,   706,   1201,
    -1,    0,     0,     1001,  757,   1,     757,   109,   -2,    2105,  1,
    0,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     1,     1,
    1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     255,
    63,    159,   223,   191,   127,   95,    0,     217,   116,   190,   245,
    34,    136,   235,   118,   153,   70,    227,   139,   39,    43,    222,
    56,    213,   143,   137,   115,   110,   78,    49,    94,    196,   182,
    229,   38,    220,   51,    101,   203,   154,   188,   169,   242,   58,
    244,   93,    173,   175,   214,   254,   243,   54,    251,   174,   198,
    200,   177,   234,   61,    155,   219,   181,   98,    215,   247,   241,
    206,   201,   156,   207,   202,   167,   189,   250,   114,   252,   158,
    166,   253,   218,   46,    125,   120,   199,   77,    109,   140,   152,
    239,   186,   170,   86,    197,   121,   178,   84,    100,   62,    55,
    102,   35,    123,   237,   171,   85,    163,   162,   42,    231,   111,
    99,    124,   107,   142,   187,   79,    47,    233,   205,   168,   221,
    103,   232,   92,    179,   226,   249,   246,   184,   172,   69,    50,
    204,   113,   122,   248,   138,   119,   185,   183,   236,   108,   71,
    126,   60,    53,    212,   68,    157,   228,   141,   87,    117,   106,
    238,   59,    230,   216,   76,    57,    0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    0,     0,     0,     0,     0,     0,     0,     0,     0,     20,    73,
    110,   112,   117,   116,   32,    105,   110,   115,   116,   114,   117,
    99,    116,   105,   111,   110,   115,   58,    10,    13,    10,    87,
    97,    108,   107,   105,   110,   103,   46,    46,    46,    10,    10,
    13,    10,    82,    117,   110,   110,   105,   110,   103,   46,    46,
    46,    10,    10,    25,    10,    68,    105,   100,   110,   39,    116,
    32,    109,   97,    107,   101,   32,    105,   116,   32,    97,    99,
    114,   111,   115,   115,   58,    10,    10,    58,    73,    110,   118,
    97,    108,   105,   100,   32,    111,   112,   101,   114,   97,    116,
    105,   111,   110,   59,    32,    101,   120,   112,   101,   99,    116,
    101,   100,   32,    115,   111,   109,   101,   116,   104,   105,   110,
    103,   32,    108,   105,   107,   101,   32,    65,    78,    68,    44,
    32,    79,    82,    44,    32,    111,   114,   32,    78,    79,    84,
    67,    73,    110,   118,   97,    108,   105,   100,   32,    102,   105,
    114,   115,   116,   32,    97,    114,   103,   117,   109,   101,   110,
    116,   59,    32,    101,   120,   112,   101,   99,    116,   101,   100,
    32,    115,   111,   109,   101,   116,   104,   105,   110,   103,   32,
    108,   105,   107,   101,   32,    65,    44,    32,    66,    44,    32,
    67,    44,    32,    68,    44,    32,    74,    44,    32,    111,   114,
    32,    84,    40,    73,    110,   118,   97,    108,   105,   100,   32,
    115,   101,   99,    111,   110,   100,   32,    97,    114,   103,   117,
    109,   101,   110,   116,   59,    32,    101,   120,   112,   101,   99,
    116,   101,   100,   32,    74,    32,    111,   114,   32,    84,    52,
    79,    117,   116,   32,    111,   102,   32,    109,   101,   109,   111,
    114,   121,   59,    32,    97,    116,   32,    109,   111,   115,   116,
    32,    49,    53,    32,    105,   110,   115,   116,   114,   117,   99,
    116,   105,   111,   110,   115,   32,    99,    97,    110,   32,    98,
    101,   32,    115,   116,   111,   114,   101,   100,   0,     109,   1,
    1005,  1262,  1270,  3,     1262,  20102, 1,     1262,  0,     109,   -1,
    2105,  1,     0,     109,   1,     21101, 0,     1288,  0,     1105,  1,
    1263,  20101, 0,     1262,  0,     1101,  0,     0,     1262,  109,   -1,
    2105,  1,     0,     109,   5,     21102, 1310,  1,     0,     1106,  0,
    1279,  22102, 1,     1,     -2,    22208, -2,    -4,    -1,    1205,  -1,
    1332,  22102, 1,     -3,    1,     21102, 1332,  1,     0,     1106,  0,
    1421,  109,   -5,    2105,  1,     0,     109,   2,     21102, 1,     1346,
    0,     1106,  0,     1263,  21208, 1,     32,    -1,    1205,  -1,    1363,
    21208, 1,     9,     -1,    1205,  -1,    1363,  1105,  1,     1373,  21101,
    1370,  0,     0,     1106,  0,     1279,  1106,  0,     1339,  109,   -2,
    2106,  0,     0,     109,   5,     2102,  1,     -4,    1386,  20101, 0,
    0,     -2,    22101, 1,     -4,    -4,    21101, 0,     0,     -3,    22208,
    -3,    -2,    -1,    1205,  -1,    1416,  2201,  -4,    -3,    1408,  4,
    0,     21201, -3,    1,     -3,    1106,  0,     1396,  109,   -5,    2106,
    0,     0,     109,   2,     104,   10,    21202, -1,    1,     1,     21101,
    1436,  0,     0,     1106,  0,     1378,  104,   10,    99,    109,   -2,
    2106,  0,     0,     109,   3,     20002, 594,   753,   -1,    22202, -1,
    -2,    -1,    201,   -1,    754,   754,   109,   -3,    2105,  1,     0,
    109,   10,    21101, 0,     5,     -5,    21101, 0,     1,     -4,    21102,
    0,     1,     -3,    1206,  -9,    1555,  21102, 3,     1,     -6,    21102,
    1,     5,     -7,    22208, -7,    -5,    -8,    1206,  -8,    1507,  22208,
    -6,    -4,    -8,    1206,  -8,    1507,  104,   64,    1106,  0,     1529,
    1205,  -6,    1527,  1201,  -7,    716,   1515,  21002, 0,     -11,   -8,
    21201, -8,    46,    -8,    204,   -8,    1106,  0,     1529,  104,   46,
    21201, -7,    1,     -7,    21207, -7,    22,    -8,    1205,  -8,    1488,
    104,   10,    21201, -6,    -1,    -6,    21207, -6,    0,     -8,    1206,
    -8,    1484,  104,   10,    21207, -4,    1,     -8,    1206,  -8,    1569,
    21101, 0,     0,     -9,    1105,  1,     1689,  21208, -5,    21,    -8,
    1206,  -8,    1583,  21101, 1,     0,     -9,    1106,  0,     1689,  1201,
    -5,    716,   1588,  21001, 0,     0,     -2,    21208, -4,    1,     -1,
    22202, -2,    -1,    -1,    1205,  -2,    1613,  22102, 1,     -5,    1,
    21102, 1,     1613,  0,     1105,  1,     1444,  1206,  -1,    1634,  21201,
    -5,    0,     1,     21102, 1,     1627,  0,     1105,  1,     1694,  1206,
    1,     1634,  21101, 2,     0,     -3,    22107, 1,     -4,    -8,    22201,
    -1,    -8,    -8,    1206,  -8,    1649,  21201, -5,    1,     -5,    1206,
    -3,    1663,  21201, -3,    -1,    -3,    21201, -4,    1,     -4,    1105,
    1,     1667,  21201, -4,    -1,    -4,    21208, -4,    0,     -1,    1201,
    -5,    716,   1676,  22002, 0,     -1,    -1,    1206,  -1,    1686,  21102,
    1,     1,     -4,    1106,  0,     1477,  109,   -10,   2105,  1,     0,
    109,   11,    21102, 1,     0,     -6,    21101, 0,     0,     -8,    21102,
    0,     1,     -7,    20208, -6,    920,   -9,    1205,  -9,    1880,  21202,
    -6,    3,     -9,    1201,  -9,    921,   1725,  20101, 0,     0,     -5,
    1001,  1725,  1,     1732,  21002, 0,     1,     -4,    21202, -4,    1,
    1,     21102, 1,     1,     2,     21101, 0,     9,     3,     21102, 1754,
    1,     0,     1105,  1,     1889,  1206,  1,     1772,  2201,  -10,   -4,
    1766,  1001,  1766,  716,   1766,  21002, 0,     1,     -3,    1105,  1,
    1790,  21208, -4,    -1,    -9,    1206,  -9,    1786,  22102, 1,     -8,
    -3,    1106,  0,     1790,  22102, 1,     -7,    -3,    1001,  1732,  1,
    1796,  20102, 1,     0,     -2,    21208, -2,    -1,    -9,    1206,  -9,
    1812,  22101, 0,     -8,    -1,    1105,  1,     1816,  22102, 1,     -7,
    -1,    21208, -5,    1,     -9,    1205,  -9,    1837,  21208, -5,    2,
    -9,    1205,  -9,    1844,  21208, -3,    0,     -1,    1106,  0,     1855,
    22202, -3,    -1,    -1,    1105,  1,     1855,  22201, -3,    -1,    -1,
    22107, 0,     -1,    -1,    1105,  1,     1855,  21208, -2,    -1,    -9,
    1206,  -9,    1869,  21202, -1,    1,     -8,    1106,  0,     1873,  21201,
    -1,    0,     -7,    21201, -6,    1,     -6,    1105,  1,     1708,  21202,
    -8,    1,     -10,   109,   -11,   2106,  0,     0,     109,   7,     22207,
    -6,    -5,    -3,    22207, -4,    -6,    -2,    22201, -3,    -2,    -1,
    21208, -1,    0,     -6,    109,   -7,    2106,  0,     0,     0,     109,
    5,     1201,  -2,    0,     1912,  21207, -4,    0,     -1,    1206,  -1,
    1930,  21101, 0,     0,     -4,    22101, 0,     -4,    1,     21201, -3,
    0,     2,     21102, 1,     1,     3,     21101, 0,     1949,  0,     1106,
    0,     1954,  109,   -5,    2106,  0,     0,     109,   6,     21207, -4,
    1,     -1,    1206,  -1,    1977,  22207, -5,    -3,    -1,    1206,  -1,
    1977,  21202, -5,    1,     -5,    1106,  0,     2045,  22102, 1,     -5,
    1,     21201, -4,    -1,    2,     21202, -3,    2,     3,     21102, 1,
    1996,  0,     1105,  1,     1954,  21201, 1,     0,     -5,    21102, 1,
    1,     -2,    22207, -5,    -3,    -1,    1206,  -1,    2015,  21101, 0,
    0,     -2,    22202, -3,    -2,    -3,    22107, 0,     -4,    -1,    1206,
    -1,    2037,  21202, -2,    1,     1,     21101, 2037,  0,     0,     106,
    0,     1912,  21202, -3,    -1,    -3,    22201, -5,    -3,    -5,    109,
    -6,    2105,  1,     0
};

void part_one()
{
    Code code = to_code(actual_code, 200000);

    const char *script = ""
        "NOT C T\n"
        "AND D T\n"
        "NOT T T\n"
        "NOT T J\n"
        "AND A T\n"
        "NOT T T\n"
        "OR T J\n"
        "WALK\n"
    ;

    int result = survey_hull_damage(code, script);
    free_code(code);
    
    printf("Part 1: %d\n", result);
}

void part_two()
{
    Code code = to_code(actual_code, 200000);

    // If at least one hole in A, B, C,
    // AND landing tile D is ground,
    // AND (
    //  we can move to E (one after landing) and jump to I, (E and I are ground)
    //  OR we can jump from D to H. (H is ground)
    // )
    // OR A is a hole. Overrides other logic, as otherwise we would fall next step.
    //
    // => ((!A OR !B OR !C) AND D AND ((E AND I) OR H)) OR !A
    //
    const char *script =
        "NOT B T\n"
        "NOT C J\n"
        "OR J T\n"
        "NOT A J\n"
        "OR T J\n"
        "AND E T\n"
        "AND I T\n"
        "OR H T\n"
        "AND D T\n"
        "AND T J\n"
        "NOT A T\n"
        "OR T J\n"
        "RUN\n"
    ;

    int result = survey_hull_damage(code, script);
    free_code(code);
    
    printf("Part 2: %d\n", result);
}

int main(int argc, const char **argv)
{
    part_one();
    part_two();
    return 0;
}

