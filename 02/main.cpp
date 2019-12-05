// 776996-20191205-0a4a7f2b

#include <cstdio>
#include <cstdint>

enum Opcode
{
    OP_Add = 1,
    OP_Mul = 2,
    OP_Halt = 99
};

struct State
{
    int position;
};

struct Code
{
    int *data;
    int size;

    int operator[](int pos) const
    {
        return data[pos];
    }
    int& operator[](int pos)
    {
        return data[pos];
    }
};

template <int N>
Code to_code(int (&c)[N])
{
    return { .data = c, .size = N };
};

void print_code(Code code)
{
    if (code.size > 0)
    {
        printf("%d", code[0]);
        for (int i = 1; i < code.size; i++) printf(",%d", code[i]);
        printf("\n");
    }
}

int step(State *s, Code code)
{
    int pos = s->position;
    int op = code[pos];
    switch (op)
    {
    case OP_Add:
        {
            int oper1_pos = code[pos+1];
            int oper2_pos = code[pos+2];
            int res_pos = code[pos+3];
            int oper1 = code[oper1_pos];
            int oper2 = code[oper2_pos];
            code[res_pos] = oper1 + oper2;
            pos += 4;
        } break;
    case OP_Mul:
        {
            int oper1_pos = code[pos+1];
            int oper2_pos = code[pos+2];
            int res_pos = code[pos+3];
            int oper1 = code[oper1_pos];
            int oper2 = code[oper2_pos];
            code[res_pos] = oper1 * oper2;
            pos += 4;
        } break;
    case OP_Halt:
        return -1;
        break;
    default:
        printf("BUG!\n");
        return -1;
    }
    s->position = pos;
    return 1;
}

int code_1[] = { 1,0,0,0,99 };
int code_2[] = { 2,3,0,3,99 };
int code_3[] = { 2,4,4,5,99,0 };
int code_4[] = { 1,1,1,4,99,5,6,0,99 };

int actual_code[] = {
    1,0,0,3,1,1,2,3,1,3,4,3,1,5,0,3,2,9,1,19,1,19,5,23,1,9,23,27,2,27,6,31,1,5,31,35,2,9,35,39,2,6,39,43,2,43,13,47,2,13,47,51,1,10,51,55,1,9,55,59,1,6,59,63,2,63,9,67,1,67,6,71,1,71,13,75,1,6,75,79,1,9,79,83,2,9,83,87,1,87,6,91,1,91,13,95,2,6,95,99,1,10,99,103,2,103,9,107,1,6,107,111,1,10,111,115,2,6,115,119,1,5,119,123,1,123,13,127,1,127,5,131,1,6,131,135,2,135,13,139,1,139,2,143,1,143,10,0,99,2,0,14,0
};

int main(int argc, const char **argv)
{
    actual_code[1] = 12;
    actual_code[2] = 2;
    Code code = to_code(actual_code);

    State state = {};
    while (step(&state, code) != -1);
    print_code(code);
    return 0;
}

