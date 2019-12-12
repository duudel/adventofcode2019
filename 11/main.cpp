// 776996-20191205-0a4a7f2b

/*

--- Day 11: Space Police ---

On the way to Jupiter, you're pulled over by the Space Police.

"Attention, unmarked spacecraft! You are in violation of Space Law! All
spacecraft must have a clearly visible registration identifier! You have 24
hours to comply or be sent to Space Jail!"

Not wanting to be sent to Space Jail, you radio back to the Elves on Earth for
help. Although it takes almost three hours for their reply signal to reach you,
they send instructions for how to power up the emergency hull painting robot
and even provide a small Intcode program (your puzzle input) that will cause it
to paint your ship appropriately.

There's just one problem: you don't have an emergency hull painting robot.

You'll need to build a new emergency hull painting robot. The robot needs to be
able to move around on the grid of square panels on the side of your ship,
detect the color of its current panel, and paint its current panel black or
white. (All of the panels are currently black.)

The Intcode program will serve as the brain of the robot. The program uses
input instructions to access the robot's camera: provide 0 if the robot is over
a black panel or 1 if the robot is over a white panel. Then, the program will
output two values:

    First, it will output a value indicating the color to paint the panel the
    robot is over: 0 means to paint the panel black, and 1 means to paint the
    panel white.

    Second, it will output a value indicating the direction the robot should
    turn: 0 means it should turn left 90 degrees, and 1 means it should turn right
    90 degrees.

After the robot turns, it should always move forward exactly one panel. The
robot starts facing up.

The robot will continue running for a while like this and halt when it is
finished drawing. Do not restart the Intcode computer inside the robot during
this process.

For example, suppose the robot is about to start running. Drawing black panels
as ., white panels as #, and the robot pointing the direction it is facing (< ^
> v), the initial state and region near the robot looks like this:

.....
.....
..^..
.....
.....

The panel under the robot (not visible here because a ^ is shown instead) is
also black, and so any input instructions at this point should be provided 0.
Suppose the robot eventually outputs 1 (paint white) and then 0 (turn left).
After taking these actions and moving forward one panel, the region now looks
like this:

.....
.....
.<#..
.....
.....

Input instructions should still be provided 0. Next, the robot might output 0
(paint black) and then 0 (turn left):

.....
.....
..#..
.v...
.....

After more outputs (1,0, 1,0):

.....
.....
..^..
.##..
.....

The robot is now back where it started, but because it is now on a white panel,
    input instructions should be provided 1. After several more outputs (0,1,
            1,0, 1,0), the area looks like this:

.....
..<#.
...#.
.##..
.....

Before you deploy the robot, you should probably have an estimate of the area
it will cover: specifically, you need to know the number of panels it paints at
least once, regardless of color. In the example above, the robot painted 6
panels at least once. (It painted its starting panel twice, but that panel is
        still only counted once; it also never painted the panel it ended on.)

Build a new emergency hull painting robot and run the Intcode program on it.
How many panels does it paint at least once?

Your puzzle answer was 2184.
--- Part Two ---

You're not sure what it's trying to paint, but it's definitely not a
registration identifier. The Space Police are getting impatient.

Checking your external ship cameras again, you notice a white panel marked
"emergency hull painting robot starting panel". The rest of the panels are
still black, but it looks like the robot was expecting to start on a white
panel, not a black one.

Based on the Space Law Space Brochure that the Space Police attached to one of
your windows, a valid registration identifier is always eight capital letters.
After starting the robot on a single white panel instead, what registration
identifier does it paint on your hull?

Your puzzle answer was AHCHZEPK.

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

struct Canvas
{
    int *data;
    int width;
    int height;

    int operator()(int x, int y) const { return data[y*width + x]; }
    int& operator()(int x, int y) { return data[y*width + x]; }
};

Canvas alloc_canvas(int width, int height)
{
    Canvas c = {};
    c.data = (int*)calloc(width*height, sizeof(int));
    c.width = width;
    c.height = height;
    return c;
}

struct Robot
{
    State cpu_state;

    enum Direction {
        Up, Right, Down, Left,
    } dir;

    int x;
    int y;

    int min_x;
    int max_x;
    int min_y;
    int max_y;
};

struct Node
{
    int color;
    int x; int y;
};

struct Nodes
{
    Node *data;
    int len;
    int cap;
};

void realloc(Nodes *nodes, int cap)
{
    nodes->data = (Node*)realloc(nodes->data, cap*sizeof(Node));
    nodes->cap = cap;
}

void put_node(Nodes *nodes, int x, int y, int color)
{
    if (nodes->len + 1 > nodes->cap)
    {
        realloc(nodes, (nodes->len + 1) * 2);
    }
    int i = nodes->len;
    nodes->data[i].x = x;
    nodes->data[i].y = y;
    nodes->data[i].color = color;
    nodes->len++;
}

int min(int a, int b)
{
    return (a < b) ? a : b;
}
int max(int a, int b)
{
    return (a > b) ? a : b;
}

void move(Robot *robot, int dir)
{
    Robot::Direction new_dir;
    switch (robot->dir)
    {
        case Robot::Up:
            if (dir == 0)
            {
                new_dir = Robot::Left;
            }
            else
            {
                new_dir = Robot::Right;
            }
            break;
        case Robot::Right:
            if (dir == 0)
            {
                new_dir = Robot::Up;
            }
            else
            {
                new_dir = Robot::Down;
            }
            break;
        case Robot::Down:
            if (dir == 0)
            {
                new_dir = Robot::Right;
            }
            else
            {
                new_dir = Robot::Left;
            }
            break;
        case Robot::Left:
            if (dir == 0)
            {
                new_dir = Robot::Down;
            }
            else
            {
                new_dir = Robot::Up;
            }
            break;
    }
    switch (new_dir)
    {
        case Robot::Up:     robot->y--; break;
        case Robot::Right:  robot->x++; break;
        case Robot::Down:   robot->y++; break;
        case Robot::Left:   robot->x--; break;
    }
    robot->min_x = min(robot->x, robot->min_x);
    robot->min_y = min(robot->y, robot->min_y);
    robot->max_x = max(robot->x, robot->max_x);
    robot->max_y = max(robot->y, robot->max_y);
    robot->dir = new_dir;
}

int determine_color(Nodes nodes, int x, int y)
{
    for (int i = 0; i < nodes.len; i++)
    {
        Node n = nodes.data[nodes.len-1-i];
        if (n.x == x && n.y == y)
        {
            return n.color;
        }
    }
    return 0;
}

#include <algorithm>

void paint(Code code)
{
    Nodes nodes= {};
    realloc(&nodes, 100);

    Buffer input = {};
    Buffer output = {};

    Robot robot = {};
    robot.cpu_state.input = &input;
    robot.cpu_state.output = &output;

    write(&input, 0);
    while (true)
    {
        execute(&robot.cpu_state, code);
        if (robot.cpu_state.halted) break;

        IntWord color;
        if (!read(&output, &color))
        {
            printf("ERROR: robot did not output paint color\n");
            break;
        }
        IntWord dir;
        if (!read(&output, &dir))
        {
            printf("ERROR: robot did not output direction\n");
            break;
        }

        put_node(&nodes, robot.x, robot.y, color);
        move(&robot, dir);

        color = determine_color(nodes, robot.x, robot.y);

        write(&input, color);
    }

    std::sort(nodes.data, nodes.data+nodes.len, [](Node a, Node b) {
        return (a.x < b.x) || (a.x == b.x && a.y < b.y);
    });

    int uniq = 1;
    int px = nodes.data[0].x, py = nodes.data[0].y;
    for (int i = 1; i < nodes.len; i++)
    {
        Node n = nodes.data[i];
        if (px == n.x && py == n.y) continue;
        px = n.x;
        py = n.y;
        uniq++;
    }

    printf("Total nodes %d, unique %d\n", nodes.len, uniq);
    printf("%d <= x <= %d\n", robot.min_x, robot.max_x);
    printf("%d <= y <= %d\n", robot.min_y, robot.max_y);
}

void paint2(Code code)
{
    Nodes nodes= {};
    realloc(&nodes, 400);

    Buffer input = {};
    Buffer output = {};

    Robot robot = {};
    robot.cpu_state.input = &input;
    robot.cpu_state.output = &output;

    write(&input, 1);
    while (true)
    {
        execute(&robot.cpu_state, code);
        if (robot.cpu_state.halted) break;

        IntWord color;
        if (!read(&output, &color))
        {
            printf("ERROR: robot did not output paint color\n");
            break;
        }
        IntWord dir;
        if (!read(&output, &dir))
        {
            printf("ERROR: robot did not output direction\n");
            break;
        }

        put_node(&nodes, robot.x, robot.y, color);
        move(&robot, dir);

        color = determine_color(nodes, robot.x, robot.y);

        write(&input, color);
    }

    printf("Total nodes %d\n", nodes.len);
    printf("%d <= x <= %d\n", robot.min_x, robot.max_x);
    printf("%d <= y <= %d\n", robot.min_y, robot.max_y);

    Canvas canvas = alloc_canvas(robot.max_x - robot.min_x + 20, robot.max_y - robot.min_y + 20);
    for (int i = 0; i < nodes.len; i++)
    {
        Node n = nodes.data[i];
        int x = n.x - robot.min_x + 10;
        int y = n.y - robot.min_y + 10;
        canvas(x, y) = n.color;
    }

    for (int y = 0; y < canvas.height; y++)
    {
        for (int x = 0; x < canvas.width; x++)
        {
            char c = canvas(x, y) ? '#' : ' ';
            putchar(c);
        }
        putchar('\n');
    }
}

IntWord actual_code[] = {
    3,8,1005,8,352,1106,0,11,0,0,0,104,1,104,0,3,8,102,-1,8,10,101,1,10,10,4,10,108,1,8,10,4,10,102,1,8,28,1,1003,20,10,2,106,11,10,2,1107,1,10,1,1001,14,10,3,8,1002,8,-1,10,1001,10,1,10,4,10,1008,8,0,10,4,10,1002,8,1,67,2,1009,7,10,3,8,1002,8,-1,10,1001,10,1,10,4,10,108,0,8,10,4,10,101,0,8,92,1,105,9,10,1006,0,89,1,108,9,10,3,8,1002,8,-1,10,1001,10,1,10,4,10,1008,8,1,10,4,10,1002,8,1,126,1,1101,14,10,1,1005,3,10,1006,0,29,1006,0,91,3,8,102,-1,8,10,101,1,10,10,4,10,108,1,8,10,4,10,1002,8,1,161,1,1,6,10,1006,0,65,2,106,13,10,1006,0,36,3,8,1002,8,-1,10,1001,10,1,10,4,10,1008,8,1,10,4,10,102,1,8,198,1,105,15,10,1,1004,0,10,3,8,1002,8,-1,10,1001,10,1,10,4,10,1008,8,0,10,4,10,101,0,8,228,2,1006,8,10,2,1001,16,10,3,8,102,-1,8,10,1001,10,1,10,4,10,108,0,8,10,4,10,1001,8,0,257,1006,0,19,2,6,10,10,2,4,13,10,2,1002,4,10,3,8,102,-1,8,10,1001,10,1,10,4,10,1008,8,1,10,4,10,1002,8,1,295,3,8,1002,8,-1,10,101,1,10,10,4,10,108,0,8,10,4,10,101,0,8,316,2,101,6,10,1006,0,84,2,1004,13,10,1,1109,3,10,101,1,9,9,1007,9,1046,10,1005,10,15,99,109,674,104,0,104,1,21101,387365315340,0,1,21102,369,1,0,1105,1,473,21101,666685514536,0,1,21102,380,1,0,1106,0,473,3,10,104,0,104,1,3,10,104,0,104,0,3,10,104,0,104,1,3,10,104,0,104,1,3,10,104,0,104,0,3,10,104,0,104,1,21102,1,46266346536,1,21102,427,1,0,1105,1,473,21101,235152829659,0,1,21101,438,0,0,1105,1,473,3,10,104,0,104,0,3,10,104,0,104,0,21102,838337188620,1,1,21101,461,0,0,1105,1,473,21102,988753429268,1,1,21102,1,472,0,1106,0,473,99,109,2,22101,0,-1,1,21101,40,0,2,21101,504,0,3,21102,494,1,0,1106,0,537,109,-2,2105,1,0,0,1,0,0,1,109,2,3,10,204,-1,1001,499,500,515,4,0,1001,499,1,499,108,4,499,10,1006,10,531,1101,0,0,499,109,-2,2106,0,0,0,109,4,2101,0,-1,536,1207,-3,0,10,1006,10,554,21102,1,0,-3,21202,-3,1,1,21201,-2,0,2,21102,1,1,3,21101,573,0,0,1105,1,578,109,-4,2105,1,0,109,5,1207,-3,1,10,1006,10,601,2207,-4,-2,10,1006,10,601,21201,-4,0,-4,1105,1,669,22101,0,-4,1,21201,-3,-1,2,21202,-2,2,3,21101,620,0,0,1106,0,578,22102,1,1,-4,21101,0,1,-1,2207,-4,-2,10,1006,10,639,21101,0,0,-1,22202,-2,-1,-2,2107,0,-3,10,1006,10,661,22101,0,-1,1,21102,661,1,0,106,0,536,21202,-2,-1,-2,22201,-4,-2,-4,109,-5,2106,0,0
};

void part_one()
{
    Code code = to_code(actual_code, 200);
    paint(code);
    free_code(code);
}

void part_two()
{
    Code code = to_code(actual_code, 10000);
    paint2(code);
    free_code(code);
}

int main(int argc, const char **argv)
{
    //part_one();
    part_two();
    return 0;
}

