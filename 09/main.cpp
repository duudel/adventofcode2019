// 776996-20191205-0a4a7f2b

/*

--- Day 9: Sensor Boost ---

You've just said goodbye to the rebooted rover and left Mars when you receive a
faint distress signal coming from the asteroid belt. It must be the Ceres
monitoring station!

In order to lock on to the signal, you'll need to boost your sensors. The Elves
send up the latest BOOST program - Basic Operation Of System Test.

While BOOST (your puzzle input) is capable of boosting your sensors, for
tenuous safety reasons, it refuses to do so until the computer it runs on
passes some checks to demonstrate it is a complete Intcode computer.

Your existing Intcode computer is missing one key feature: it needs support for
parameters in relative mode.

Parameters in mode 2, relative mode, behave very similarly to parameters in
position mode: the parameter is interpreted as a position. Like position mode,
parameters in relative mode can be read from or written to.

The important difference is that relative mode parameters don't count from
address 0. Instead, they count from a value called the relative base. The
relative base starts at 0.

The address a relative mode parameter refers to is itself plus the current
relative base. When the relative base is 0, relative mode parameters and
position mode parameters with the same value refer to the same address.

For example, given a relative base of 50, a relative mode parameter of -7
refers to memory address 50 + -7 = 43.

The relative base is modified with the relative base offset instruction:

    Opcode 9 adjusts the relative base by the value of its only parameter. The
    relative base increases (or decreases, if the value is negative) by the
    value of the parameter.

For example, if the relative base is 2000, then after the instruction 109,19,
the relative base would be 2019. If the next instruction were 204,-34, then the
value at address 1985 would be output.

Your Intcode computer will also need a few other capabilities:

    The computer's available memory should be much larger than the initial
    program. Memory beyond the initial program starts with the value 0 and can
    be read or written like any other memory. (It is invalid to try to access
    memory at a negative address, though.)

    The computer should have support for large numbers. Some instructions near
    the beginning of the BOOST program will verify this capability.

Here are some example programs that use these features:

    109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99 takes no input and produces a copy of itself as output.
    1102,34915192,34915192,7,4,7,99,0 should output a 16-digit number.
    104,1125899906842624,99 should output the large number in the middle.

The BOOST program will ask for a single input; run it in test mode by providing
it the value 1. It will perform a series of checks on each opcode, output any
opcodes (and the associated parameter modes) that seem to be functioning
incorrectly, and finally output a BOOST keycode.

Once your Intcode computer is fully functional, the BOOST program should report
no malfunctioning opcodes when run in test mode; it should only output a single
value, the BOOST keycode. What BOOST keycode does it produce?

Your puzzle answer was 2932210790.
--- Part Two ---

You now have a complete Intcode computer.

Finally, you can lock on to the Ceres distress signal! You just need to boost
your sensors using the BOOST program.

The program runs in sensor boost mode by providing the input instruction the
value 2. Once run, it will boost the sensors automatically, but it might take a
few seconds to complete the operation on slower hardware. In sensor boost mode,
the program will output a single value: the coordinates of the distress signal.

Run the BOOST program in sensor boost mode. What are the coordinates of the
distress signal?

Your puzzle answer was 73144.

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

#define MAX_IO_BUFFER 100
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
    IntWord *data = (IntWord*)malloc(sizeof(IntWord) * (N + extra_memory));
    memcpy(data, c, sizeof(IntWord) * N);
    memset(data + N, 0, sizeof(IntWord) * extra_memory);
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
    switch (mode)
    {
        case 0: return code[a];
        case 1: return a;
        case 2: return code[s->relative_base + a];
        default:
            assert("invalid address mode" && 0);
    }
    return 0;
}

IntWord read_pos(State *s, Code code, int pos, int mode)
{
    IntWord a = code[pos];
    switch (mode)
    {
        case 0: return a;
        case 2: return s->relative_base + a;
        default:
            assert("invalid address mode" && 0);
    }
    return 0;
}

int step(State *s, Code code)
{
    int pos = s->position;
    IntWord instr = code[pos];
    int m1, m2, m3;
    Opcode op = decode_instr(instr, &m1, &m2, &m3);
    //printf("%d: %s[%d, %d, %d]\n", pos, Opcode_names[op], m1, m2, m3);
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
            printf("out=%lld\n", out);
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
    //default:
        printf("BUG!\n");
        return -1;
    }
    s->position = pos;
    return 1;
}

void execute(State *state, Code code)
{
    while (step(state, code) == 1);
}

namespace sample01
{
    IntWord code_data[] = { 109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99 };

    void test()
    {
        Code code = to_code(code_data, 200);
        Buffer output = {};
        State state = {};
        state.output = &output;
        execute(&state, code);

        IntWord *program = code.data;
        IntWord *copy = output.data + output.read;
        int result = memcmp(program, copy, code.code_size*sizeof(IntWord));
        fflush(stdout);
        assert(result == 0);
        printf("PASS\n");
    }
}

namespace sample02
{
    IntWord code_data[] = { 1102,34915192,34915192,7,4,7,99,0 };

    void test()
    {
        Code code = to_code(code_data, 20000);
        Buffer output = {};
        State state = {};
        state.output = &output;
        execute(&state, code);

        IntWord result;
        read(&output, &result);
        fflush(stdout);
        assert(result == 1219070632396864);
        printf("PASS %lld\n", result);
    }
}


IntWord actual_code[] = {
    1102,34463338,34463338,63,1007,63,34463338,63,1005,63,53,1101,3,0,1000,109,988,209,12,9,1000,209,6,209,3,203,0,1008,1000,1,63,1005,63,65,1008,1000,2,63,1005,63,904,1008,1000,0,63,1005,63,58,4,25,104,0,99,4,0,104,0,99,4,17,104,0,99,0,0,1101,35,0,1007,1102,30,1,1013,1102,37,1,1017,1101,23,0,1006,1101,0,32,1008,1102,1,29,1000,1101,0,38,1010,1101,0,24,1002,1101,33,0,1003,1101,1,0,1021,1102,31,1,1019,1101,27,0,1014,1102,20,1,1005,1101,0,0,1020,1102,1,892,1027,1101,895,0,1026,1102,39,1,1015,1102,1,370,1029,1102,1,28,1001,1102,34,1,1012,1101,25,0,1016,1101,0,375,1028,1101,36,0,1018,1101,0,21,1004,1102,1,26,1009,1101,0,249,1022,1101,0,660,1025,1101,0,665,1024,1102,1,22,1011,1102,242,1,1023,109,5,2102,1,3,63,1008,63,31,63,1005,63,205,1001,64,1,64,1105,1,207,4,187,1002,64,2,64,109,8,21102,40,1,5,1008,1018,37,63,1005,63,227,1105,1,233,4,213,1001,64,1,64,1002,64,2,64,109,7,2105,1,3,1001,64,1,64,1106,0,251,4,239,1002,64,2,64,109,-7,1201,-7,0,63,1008,63,20,63,1005,63,271,1106,0,277,4,257,1001,64,1,64,1002,64,2,64,109,-10,1208,0,33,63,1005,63,295,4,283,1106,0,299,1001,64,1,64,1002,64,2,64,109,-6,1207,4,27,63,1005,63,319,1001,64,1,64,1105,1,321,4,305,1002,64,2,64,109,12,1207,-1,33,63,1005,63,339,4,327,1105,1,343,1001,64,1,64,1002,64,2,64,109,6,1206,6,355,1106,0,361,4,349,1001,64,1,64,1002,64,2,64,109,21,2106,0,-8,4,367,1106,0,379,1001,64,1,64,1002,64,2,64,109,-29,1202,0,1,63,1008,63,36,63,1005,63,403,1001,64,1,64,1105,1,405,4,385,1002,64,2,64,109,11,21107,41,40,-6,1005,1012,421,1105,1,427,4,411,1001,64,1,64,1002,64,2,64,109,-11,2101,0,-4,63,1008,63,33,63,1005,63,453,4,433,1001,64,1,64,1106,0,453,1002,64,2,64,109,-7,21108,42,40,10,1005,1010,469,1105,1,475,4,459,1001,64,1,64,1002,64,2,64,109,1,1201,4,0,63,1008,63,20,63,1005,63,497,4,481,1105,1,501,1001,64,1,64,1002,64,2,64,109,5,21107,43,44,5,1005,1011,523,4,507,1001,64,1,64,1106,0,523,1002,64,2,64,109,20,21108,44,44,-7,1005,1019,541,4,529,1106,0,545,1001,64,1,64,1002,64,2,64,109,2,1205,-8,561,1001,64,1,64,1106,0,563,4,551,1002,64,2,64,109,-23,2108,22,0,63,1005,63,583,1001,64,1,64,1105,1,585,4,569,1002,64,2,64,109,-6,2107,30,1,63,1005,63,605,1001,64,1,64,1105,1,607,4,591,1002,64,2,64,109,23,1205,-1,621,4,613,1105,1,625,1001,64,1,64,1002,64,2,64,109,-19,2102,1,-3,63,1008,63,29,63,1005,63,647,4,631,1106,0,651,1001,64,1,64,1002,64,2,64,109,28,2105,1,-7,4,657,1106,0,669,1001,64,1,64,1002,64,2,64,109,-17,1206,6,687,4,675,1001,64,1,64,1105,1,687,1002,64,2,64,109,2,21101,45,0,1,1008,1017,42,63,1005,63,707,1106,0,713,4,693,1001,64,1,64,1002,64,2,64,109,-6,2101,0,-3,63,1008,63,34,63,1005,63,733,1105,1,739,4,719,1001,64,1,64,1002,64,2,64,109,3,21101,46,0,1,1008,1014,46,63,1005,63,761,4,745,1106,0,765,1001,64,1,64,1002,64,2,64,109,5,21102,47,1,-7,1008,1011,47,63,1005,63,787,4,771,1105,1,791,1001,64,1,64,1002,64,2,64,109,-24,2108,24,8,63,1005,63,813,4,797,1001,64,1,64,1106,0,813,1002,64,2,64,109,5,1208,10,29,63,1005,63,829,1105,1,835,4,819,1001,64,1,64,1002,64,2,64,109,7,2107,23,-4,63,1005,63,853,4,841,1105,1,857,1001,64,1,64,1002,64,2,64,109,-2,1202,0,1,63,1008,63,21,63,1005,63,879,4,863,1105,1,883,1001,64,1,64,1002,64,2,64,109,15,2106,0,8,1106,0,901,4,889,1001,64,1,64,4,64,99,21102,1,27,1,21102,915,1,0,1105,1,922,21201,1,51839,1,204,1,99,109,3,1207,-2,3,63,1005,63,964,21201,-2,-1,1,21101,942,0,0,1106,0,922,21201,1,0,-1,21201,-2,-3,1,21101,957,0,0,1105,1,922,22201,1,-1,-2,1105,1,968,21201,-2,0,-2,109,-3,2106,0,0
};

void part_one()
{
    Code code = to_code(actual_code, 200);
    Buffer input = {};
    Buffer output = {};
    State state = {};
    state.input = &input;
    state.output = &output;

    write(&input, 1);
    execute(&state, code);
}

void part_two()
{
    Code code = to_code(actual_code, 200);
    Buffer input = {};
    Buffer output = {};
    State state = {};
    state.input = &input;
    state.output = &output;

    write(&input, 2);
    execute(&state, code);
}

int main(int argc, const char **argv)
{
    //sample01::test();
    //sample02::test();
    //part_one();
    part_two();
    return 0;
}

