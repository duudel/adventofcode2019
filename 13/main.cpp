// 776996-20191205-0a4a7f2b

/*

--- Day 13: Care Package ---

As you ponder the solitude of space and the ever-increasing three-hour
roundtrip for messages between you and Earth, you notice that the Space Mail
Indicator Light is blinking. To help keep you sane, the Elves have sent you a
care package.

It's a new game for the ship's arcade cabinet! Unfortunately, the arcade is all
the way on the other end of the ship. Surely, it won't be hard to build your
own - the care package even comes with schematics.

The arcade cabinet runs Intcode software like the game the Elves sent (your
puzzle input). It has a primitive screen capable of drawing square tiles on a
grid. The software draws tiles to the screen with output instructions: every
three output instructions specify the x position (distance from the left), y
position (distance from the top), and tile id. The tile id is interpreted as
follows:

    0 is an empty tile. No game object appears in this tile.
    1 is a wall tile. Walls are indestructible barriers.
    2 is a block tile. Blocks can be broken by the ball.
    3 is a horizontal paddle tile. The paddle is indestructible.
    4 is a ball tile. The ball moves diagonally and bounces off objects.

For example, a sequence of output values like 1,2,3,6,5,4 would draw a
horizontal paddle tile (1 tile from the left and 2 tiles from the top) and a
ball tile (6 tiles from the left and 5 tiles from the top).

Start the game. How many block tiles are on the screen when the game exits?

Your puzzle answer was 255.
--- Part Two ---

The game didn't run because you didn't put in any quarters. Unfortunately, you
did not bring any quarters. Memory address 0 represents the number of quarters
that have been inserted; set it to 2 to play for free.

The arcade cabinet has a joystick that can move left and right. The software
reads the position of the joystick with input instructions:

    If the joystick is in the neutral position, provide 0.
    If the joystick is tilted to the left, provide -1.
    If the joystick is tilted to the right, provide 1.

The arcade cabinet also has a segment display capable of showing a single
number that represents the player's current score. When three output
instructions specify X=-1, Y=0, the third output instruction is not a tile; the
value instead specifies the new score to show in the segment display. For
example, a sequence of output values like -1,0,12345 would show 12345 as the
player's current score.

Beat the game by breaking all the blocks. What is your score after the last
block is broken?

Your puzzle answer was 12338.

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
    if (result == 549)
    {
        printf("!!!!! POS = %d\n", s->position);
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
    if (result == 549)
    {
        printf("!!!!! WRITE POS = %d\n", s->position);
        fflush(stdout);
        assert(0);
    }
    return result;
}

int step(State *s, Code code)
{
    int pos = s->position;
    IntWord instr = code[pos];
    int m1, m2, m3;
    Opcode op = decode_instr(instr, &m1, &m2, &m3);
    //if (op == 0) {
    //    s->halted = true;
    //    return -1;
    //}
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

int min(int a, int b)
{
    return (a < b) ? a : b;
}
int max(int a, int b)
{
    return (a > b) ? a : b;
}

struct Screen
{
    int *data;
    int width;
    int height;

    int operator()(int x, int y) const { return data[y*width + x]; }
    int& operator()(int x, int y) { return data[y*width + x]; }
};

Screen alloc_screen(int width, int height)
{
    Screen result = {};
    result.data = (int*)calloc(width*height, sizeof(int));
    result.width = width;
    result.height = height;
    return result;
}

void print_screen(Screen screen)
{
    for (int y = 0; y < screen.height; y++)
    {
        for (int x = 0; x < screen.width; x++)
        {
            char c = ' ';
            switch (screen(x, y))
            {
                case 0: c = ' '; break;
                case 1: c = '|'; break;
                case 2: c = '#'; break;
                case 3: c = '='; break;
                case 4: c = 'O'; break;
            }
            putchar(c);
        }
        putchar('\n');
    }
}

struct Bounds
{
    int min_x;
    int max_x;
    int min_y;
    int max_y;
};

Bounds determine_bounds(Code code)
{
    Bounds bounds = {};
    Buffer output = {};
    State state = {};
    state.output = &output;
    while (!state.halted)
    {
        for (int i = 0; i < 50; i++)
        {
            if (step(&state, code) != 1) break;
        }
        IntWord i0, i1, i2;
        while (read(&output, &i0))
        {
            read(&output, &i1);
            read(&output, &i2);
            bounds.max_x = max(bounds.max_x, i0);
            bounds.max_y = max(bounds.max_y, i1);
        }
    }
    return bounds;
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

int joystick_inputs[] = {
1,0,-1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,0,0,
0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,0,0,-1,-1,-1,-1,-1,1,1,1,1,1,1,1,1,1,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,-1,1,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,0,1,1,1,1,1,1,1,1,1,1,1,
1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,0,-1,1,1,1,-1,-1,-1,-1,-1,1,1,1,1,-1,-1,-1,1,1,1,1,1,1,1,1,0,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,1,1,1,0,-1,-1,-1,-1,1,
1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,-1,0,1,0,-1,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,-1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
1,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,-1,-1,-1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,1,1,1,1,1,1,1,-1,0,0,0,0,
-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,-1,0,-1,0,0,-1,-1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,
-1,0,0,0,-1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,-1,0,-1,-1,-1,0,
0,0,0,-1,-1,0,0,-1,0,-1,-1,-1,-1,-1,-1,0,0,0,0,0,-1,0,-1,-1,-1,-1,-1,-1,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,1,-1,0,0,0,0,0,
0,0,0,1,0,-1,-1,0,0,0,0,0,0,0,0,0,0,0,-1,0,-1,-1,-1,-1,0,0,0,-1,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,-1,
0,-1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,-1,0,0,-1,0,0,1,1,0,0,0,0,0,1,-1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,
0,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,-1,-1,0,0,-1,-1,
-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,1,-1,
-1,-1,-1,-1,0,-1,0,0,0,0,1,0,0,0,0,0,-1,-1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,
1,0,1,0,0,0,0,-1,-1,-1,0,-1,-1,0,-1,-1,-1,-1,0,-1,-1,-1,-1,-1,-1,0,-1,-1,0,0,0,0,0,0,0,1,0,0,0,1,-1,0,0,0,0,0,1,1,0,1,
0,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,-1,-1,0,-1,
-1,-1,-1,0,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,-1,0,0,-1,-1,-1,-1,-1,-1,-1,-1,0,0,-1,-1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,1,1,1,0,
1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,-1,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,-1,1,0,-1,1,0,1,1,1,1,1,1,0,
1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,0,0,0,0,0,-1,-1,-1,-1,0,-1,-1,-1,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,-1,-1,0,0,0,0,0,0,0,0,0,-1,0,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,0,-1,-1,-1,0,0,0,-1,-1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,0,1,-1,-1,0,-1,-1,-1,-1,-1,-1,-1,
-1,-1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,0,0,0,0,0,0,0,0,-1,0,-1,-1,0,-1,-1,0,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,1,0,0,0,0,
0,0,0,0,0,0,0,-1,0,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,-1,-1,0,-1,-1,-1,0,0,0,0,0,1,0,1,1,1,1,1,
0,1,1,1,1,1,0,1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,0,-1,-1,-1,-1,0,-1,0,-1,-1,-1,
0,-1,-1,-1,0,0,0,0,0,-1,0,-1,-1,0,0,0,0,0,0,0,1,1,0,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,0,0,0,0,0,0,-1,0,0,-1,1,0,
0,0,0,0,0,0,0,0,0,0,-1,0,-1,-1,-1,-1,-1,0,-1,-1,-1,-1,0,0,0,0,0,-1,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,
1,0,1,1,1,1,1,1,0,1,0,0,0,0,0,0,-1,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,-1,0,-1,-1,-1,-1,0,0,
0,-1,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
1,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,-1,-1,-1,0,-1,0,0,0,0,0,0,-1,-1,0,0,1,0,1,0,
1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,-1,0,0,0,0,0,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,1,0,0,1,0,1,1,1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,-1,0,-1,-1,-1,-1,0,0,-1,1,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,-1,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,
1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,-1,0,0,-1,-1,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,-1,-1,-1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,
1,0,1,1,1,0,1,1,1,1,1,1,1,1,-1,-1,-1,0,1,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,-1,0,
0,0,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,-1,0,-1,0,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,-1,0,0,0,0,0,0,0,0,-1,-1,-1,-1,0,-1,-1,
-1,-1,-1,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,1,1,0,1,0,
};

int auto_joystick_input_num = sizeof(joystick_inputs)/sizeof(int);
int auto_joystick_inputs_used = 0;

int *new_joystick_inputs;
int new_joystick_input_num = 0;

void add_input(int c)
{
    int num = new_joystick_input_num + 1;
    new_joystick_inputs = (int*)realloc(new_joystick_inputs, num * sizeof(int));
    new_joystick_input_num = num;
    new_joystick_inputs[num-1] = c;
}

void print_inputs()
{
    printf("INPUTS: \n");
    int i = 0;
    while (i < new_joystick_input_num)
    {
        for (int j = 0; i < new_joystick_input_num && j < 50; i++, j++)
        {
            printf("%d,", new_joystick_inputs[i]);
        }
        printf("\n");
    }
    printf("\n");
}

void execute_game(Code code, Screen screen, IntWord *score)
{
    Buffer input = {};
    Buffer output = {};
    State state = {};
    state.input = &input;
    state.output = &output;
    while (!state.halted)
    {
        int res;
        for (int i = 0; i < 10; i++)
        {
            res = step(&state, code);
            if (res != 1) break;
        }
        while (readable_num(output) >= 3)
        {
            IntWord i0, i1, i2;
            read(&output, &i0);
            read(&output, &i1);
            read(&output, &i2);
            if (i0 == -1 && i1 == 0)
            {
                *score = i2;
            }
            else
            {
                int x = i0;
                int y = i1;
                int tile = i2;
                screen(x, y) = tile;
            }
        }

        if (res == 0) // waiting input
        {
            print_screen(screen);
            fflush(stdout);

            int inp=0;
            if (auto_joystick_inputs_used < auto_joystick_input_num)
            {
                int i = auto_joystick_inputs_used++;
                inp = joystick_inputs[i];
            }
            else
            {
                int c;
                while ((c = getchar()) != '\n')
                {
                    //int c = getchar();
                    if (c == 'a') {
                        printf("A read!\n");
                        inp = -1;
                        //break;
                    }
                    else if (c == 's') {
                        printf("S read!\n");
                        inp = 0;
                        //break;
                    }
                    else if (c == 'd') {
                        printf("D read!\n");
                        inp = 1;
                        //break;
                    }
                    else
                    {
                        printf("%d was not recognized char :(\n", c);
                        inp = 0;
                        //write(&input, 0);
                    }
                    printf("%c was pressed\n", (char)c);
                    fflush(stdout);
                }
            }
            add_input(inp);
            write(&input, inp);
        }
    }

    print_inputs();
}

IntWord actual_code[] = {
    1,380,379,385,1008,2311,194223,381,1005,381,12,99,109,2312,1101,0,0,383,1101,0,0,382,20102,1,382,1,21002,383,1,2,21101,0,37,0,1106,0,578,4,382,4,383,204,1,1001,382,1,382,1007,382,38,381,1005,381,22,1001,383,1,383,1007,383,22,381,1005,381,18,1006,385,69,99,104,-1,104,0,4,386,3,384,1007,384,0,381,1005,381,94,107,0,384,381,1005,381,108,1105,1,161,107,1,392,381,1006,381,161,1101,-1,0,384,1106,0,119,1007,392,36,381,1006,381,161,1101,0,1,384,21002,392,1,1,21101,0,20,2,21101,0,0,3,21102,138,1,0,1106,0,549,1,392,384,392,21002,392,1,1,21102,20,1,2,21102,1,3,3,21102,1,161,0,1106,0,549,1101,0,0,384,20001,388,390,1,20102,1,389,2,21102,180,1,0,1106,0,578,1206,1,213,1208,1,2,381,1006,381,205,20001,388,390,1,21002,389,1,2,21101,205,0,0,1105,1,393,1002,390,-1,390,1101,0,1,384,20101,0,388,1,20001,389,391,2,21102,228,1,0,1105,1,578,1206,1,261,1208,1,2,381,1006,381,253,21002,388,1,1,20001,389,391,2,21101,0,253,0,1106,0,393,1002,391,-1,391,1102,1,1,384,1005,384,161,20001,388,390,1,20001,389,391,2,21101,0,279,0,1106,0,578,1206,1,316,1208,1,2,381,1006,381,304,20001,388,390,1,20001,389,391,2,21102,304,1,0,1105,1,393,1002,390,-1,390,1002,391,-1,391,1101,1,0,384,1005,384,161,20101,0,388,1,21002,389,1,2,21101,0,0,3,21102,1,338,0,1106,0,549,1,388,390,388,1,389,391,389,20102,1,388,1,21001,389,0,2,21101,0,4,3,21101,365,0,0,1106,0,549,1007,389,21,381,1005,381,75,104,-1,104,0,104,0,99,0,1,0,0,0,0,0,0,255,17,17,1,1,19,109,3,22101,0,-2,1,21201,-1,0,2,21102,1,0,3,21101,414,0,0,1105,1,549,21201,-2,0,1,21202,-1,1,2,21101,429,0,0,1105,1,601,2101,0,1,435,1,386,0,386,104,-1,104,0,4,386,1001,387,-1,387,1005,387,451,99,109,-3,2106,0,0,109,8,22202,-7,-6,-3,22201,-3,-5,-3,21202,-4,64,-2,2207,-3,-2,381,1005,381,492,21202,-2,-1,-1,22201,-3,-1,-3,2207,-3,-2,381,1006,381,481,21202,-4,8,-2,2207,-3,-2,381,1005,381,518,21202,-2,-1,-1,22201,-3,-1,-3,2207,-3,-2,381,1006,381,507,2207,-3,-4,381,1005,381,540,21202,-4,-1,-1,22201,-3,-1,-3,2207,-3,-4,381,1006,381,529,22102,1,-3,-7,109,-8,2105,1,0,109,4,1202,-2,38,566,201,-3,566,566,101,639,566,566,1202,-1,1,0,204,-3,204,-2,204,-1,109,-4,2105,1,0,109,3,1202,-1,38,593,201,-2,593,593,101,639,593,593,21001,0,0,-2,109,-3,2105,1,0,109,3,22102,22,-2,1,22201,1,-1,1,21101,0,421,2,21102,594,1,3,21101,0,836,4,21101,630,0,0,1106,0,456,21201,1,1475,-2,109,-3,2105,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,2,0,2,0,0,2,0,0,0,2,2,0,2,2,2,0,2,0,0,2,0,2,2,0,0,0,0,0,2,0,2,2,0,0,0,1,1,0,2,0,0,2,0,0,0,2,0,0,0,2,0,2,0,2,0,2,0,2,2,2,0,2,0,2,0,0,2,0,2,2,0,0,0,1,1,0,2,2,0,2,0,2,2,0,0,0,2,2,0,2,0,0,0,0,2,2,0,2,2,2,0,2,2,0,2,0,2,2,0,0,0,1,1,0,2,2,2,2,2,2,0,2,2,2,2,0,0,2,2,0,0,2,2,2,2,2,0,0,0,0,2,2,2,2,2,2,2,0,0,1,1,0,2,2,0,2,0,2,2,2,2,2,2,0,0,2,2,2,2,2,2,0,0,2,0,0,0,2,2,2,2,0,2,0,0,0,0,1,1,0,2,0,0,0,2,0,2,0,2,2,2,0,0,2,2,0,0,2,2,2,0,0,0,0,0,2,2,2,0,0,2,0,2,0,0,1,1,0,2,2,0,0,2,2,2,0,0,0,0,0,2,0,0,2,2,2,2,0,2,0,0,2,0,0,2,2,0,0,0,0,2,2,0,1,1,0,0,0,2,0,2,0,2,0,2,2,0,2,2,2,0,2,2,0,2,2,0,0,0,2,0,0,0,2,2,2,0,0,0,0,0,1,1,0,0,0,2,0,0,2,2,0,2,0,2,0,2,0,0,2,0,2,0,0,0,2,2,2,2,2,2,0,2,2,2,2,2,2,0,1,1,0,0,2,0,2,2,2,0,0,2,0,2,0,2,2,2,0,0,2,2,0,2,0,2,2,2,2,2,2,2,0,2,0,2,0,0,1,1,0,2,0,2,0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,2,0,2,2,0,2,0,2,2,2,2,2,2,0,1,1,0,2,2,0,0,0,2,2,2,0,0,2,2,0,2,0,2,2,0,2,2,2,0,0,2,2,0,2,2,0,0,2,2,2,2,0,1,1,0,0,2,0,0,2,0,0,0,0,0,0,2,2,0,2,2,2,0,0,0,0,0,0,0,2,0,0,2,0,0,2,2,0,2,0,1,1,0,2,0,0,2,2,2,0,2,2,2,2,0,0,2,2,0,2,0,2,2,2,2,2,2,2,0,2,2,2,2,2,2,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,64,86,97,12,60,6,31,61,53,79,45,82,7,4,9,5,60,42,15,39,54,90,43,57,68,25,79,48,37,54,2,55,31,6,48,97,78,53,17,72,15,58,88,20,60,98,39,43,49,7,32,59,11,32,67,3,96,42,4,6,34,58,24,37,15,87,72,83,76,30,89,70,86,55,42,15,62,21,65,88,35,52,79,16,20,94,18,98,68,58,83,25,72,39,92,66,41,50,92,25,90,17,86,53,89,64,12,54,98,97,49,31,43,59,33,55,58,11,27,62,30,23,20,4,24,13,37,16,82,66,57,58,28,97,35,84,89,13,1,57,1,55,90,98,17,22,43,70,33,6,77,9,17,6,70,72,15,22,72,75,35,84,93,74,10,69,8,6,85,39,7,98,55,39,66,39,52,60,63,3,49,59,28,37,8,84,75,6,98,37,8,19,91,74,54,42,70,94,18,10,1,34,67,10,5,75,98,59,35,77,54,59,88,14,28,10,72,11,30,85,35,88,94,44,3,71,2,91,63,71,4,50,23,38,75,95,17,20,28,88,34,30,93,79,63,61,75,40,75,47,72,25,15,49,62,64,91,72,5,36,90,45,52,80,48,19,70,45,7,72,44,5,39,11,27,32,97,98,73,51,33,56,9,54,33,36,4,10,84,82,20,28,9,41,26,78,96,5,61,20,44,70,59,69,59,48,24,91,88,46,2,67,14,89,44,82,40,25,53,91,11,61,55,88,95,55,92,9,81,76,59,76,94,2,34,3,57,61,11,87,18,23,77,94,72,88,1,95,77,64,3,77,2,6,42,52,79,27,69,59,33,36,20,44,6,45,36,9,10,51,12,64,11,62,83,36,50,61,85,20,16,81,36,94,54,17,72,28,26,53,47,42,38,72,87,59,17,63,8,12,48,22,77,45,42,33,6,29,87,53,66,35,32,32,24,72,31,96,17,83,62,1,66,54,96,1,37,74,53,26,55,9,22,69,66,46,40,97,9,85,10,51,38,70,44,5,59,59,87,25,90,73,11,74,63,33,33,25,65,69,80,20,30,32,10,86,29,18,67,77,76,89,9,55,89,70,95,49,38,89,58,45,52,44,35,66,19,48,82,67,60,92,66,38,21,54,6,6,86,29,45,2,24,13,35,51,20,5,61,1,47,88,50,45,78,80,46,81,17,26,7,34,28,41,14,15,79,23,8,57,69,58,92,66,92,70,59,40,74,28,21,33,77,27,95,93,67,7,14,68,29,44,98,14,51,25,64,10,60,67,9,6,69,25,41,78,81,32,35,96,89,29,69,35,93,61,25,35,71,61,97,40,67,36,29,77,42,34,31,59,47,63,22,19,39,6,42,33,79,4,76,38,75,5,1,29,31,38,3,64,35,33,19,90,43,47,30,43,86,33,76,4,85,66,26,98,91,33,59,93,6,78,27,31,22,89,78,86,70,49,83,81,15,20,8,2,13,30,18,22,73,53,37,48,66,93,46,27,62,72,55,65,9,83,20,32,41,12,63,20,16,55,98,31,20,46,27,17,93,84,59,15,90,29,72,13,83,88,21,49,29,67,47,7,7,12,38,36,25,16,20,80,63,55,46,27,51,72,79,94,68,75,34,41,24,91,72,64,90,81,82,93,96,47,1,57,75,81,56,14,57,58,54,24,40,40,71,46,16,3,34,79,46,28,42,9,55,87,85,23,14,11,98,15,31,28,44,81,96,94,10,51,44,57,11,55,31,15,9,93,76,92,69,12,25,27,82,43,80,54,18,58,6,82,59,81,65,96,38,69,2,28,86,70,22,66,10,5,88,56,79,31,77,48,61,34,87,7,17,21,37,16,26,68,64,72,30,3,6,88,26,24,3,77,50,34,67,79,31,3,77,26,72,51,23,25,194223
};

void part_one()
{
    Code code = to_code(actual_code, 20000);
    Bounds bounds = determine_bounds(code);
    free_code(code);

    Screen screen = alloc_screen(bounds.max_x+1, bounds.max_y+1);
    code = to_code(actual_code, 20000);
    IntWord score = 0;
    execute_game(code, screen, &score);
    free_code(code);

    print_screen(screen);

    int block_num = 0;
    for (int i = 0; i < screen.width*screen.height; i++)
    {
        if (screen.data[i] == 2) block_num++;
    }

    printf("Bounds %d, %d, num of blocks %d\n", bounds.max_x, bounds.max_y, block_num);
}

void part_two()
{
    int width = 38;
    int height = 22;

    Screen screen = alloc_screen(width+1, height+1);
    Code code = to_code(actual_code, 20000);

    // Insert quarters
    code[0] = 2;

    IntWord score = 0;
    execute_game(code, screen, &score);
    free_code(code);

    print_screen(screen);

    printf("Score: %lld", score);
}

int main(int argc, const char **argv)
{
    //part_one();
    part_two();
    return 0;
}

