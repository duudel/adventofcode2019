
/*

--- Day 16: Flawed Frequency Transmission ---

You're 3/4ths of the way through the gas giants. Not only do roundtrip signals
to Earth take five hours, but the signal quality is quite bad as well. You can
clean up the signal with the Flawed Frequency Transmission algorithm, or FFT.

As input, FFT takes a list of numbers. In the signal you received (your puzzle
input), each number is a single digit: data like 15243 represents the sequence
1, 5, 2, 4, 3.

FFT operates in repeated phases. In each phase, a new list is constructed with
the same length as the input list. This new list is also used as the input for
the next phase.

Each element in the new list is built by multiplying every value in the input
list by a value in a repeating pattern and then adding up the results. So, if
the input list were 9, 8, 7, 6, 5 and the pattern for a given element were 1,
2, 3, the result would be 9*1 + 8*2 + 7*3 + 6*1 + 5*2 (with each input element
on the left and each value in the repeating pattern on the right of each
multiplication). Then, only the ones digit is kept: 38 becomes 8, -17 becomes
7, and so on.

While each element in the output array uses all of the same input array
elements, the actual repeating pattern to use depends on which output element
is being calculated. The base pattern is 0, 1, 0, -1. Then, repeat each value
in the pattern a number of times equal to the position in the output list being
considered. Repeat once for the first element, twice for the second element,
three times for the third element, and so on. So, if the third element of the
output list is being calculated, repeating the values would produce: 0, 0, 0,
1, 1, 1, 0, 0, 0, -1, -1, -1.

When applying the pattern, skip the very first value exactly once. (In other
words, offset the whole pattern left by one.) So, for the second element of the
output list, the actual pattern used would be: 0, 1, 1, 0, 0, -1, -1, 0, 0, 1,
1, 0, 0, -1, -1, ....

After using this process to calculate each element of the output list, the
phase is complete, and the output list of this phase is used as the new input
list for the next phase, if any.

Given the input signal 12345678, below are four phases of FFT. Within each
phase, each output digit is calculated on a single line with the result at the
far right; each multiplication operation shows the input digit on the left and
the pattern value on the right:

Input signal: 12345678

1*1  + 2*0  + 3*-1 + 4*0  + 5*1  + 6*0  + 7*-1 + 8*0  = 4
1*0  + 2*1  + 3*1  + 4*0  + 5*0  + 6*-1 + 7*-1 + 8*0  = 8
1*0  + 2*0  + 3*1  + 4*1  + 5*1  + 6*0  + 7*0  + 8*0  = 2
1*0  + 2*0  + 3*0  + 4*1  + 5*1  + 6*1  + 7*1  + 8*0  = 2
1*0  + 2*0  + 3*0  + 4*0  + 5*1  + 6*1  + 7*1  + 8*1  = 6
1*0  + 2*0  + 3*0  + 4*0  + 5*0  + 6*1  + 7*1  + 8*1  = 1
1*0  + 2*0  + 3*0  + 4*0  + 5*0  + 6*0  + 7*1  + 8*1  = 5
1*0  + 2*0  + 3*0  + 4*0  + 5*0  + 6*0  + 7*0  + 8*1  = 8

After 1 phase: 48226158

4*1  + 8*0  + 2*-1 + 2*0  + 6*1  + 1*0  + 5*-1 + 8*0  = 3
4*0  + 8*1  + 2*1  + 2*0  + 6*0  + 1*-1 + 5*-1 + 8*0  = 4
4*0  + 8*0  + 2*1  + 2*1  + 6*1  + 1*0  + 5*0  + 8*0  = 0
4*0  + 8*0  + 2*0  + 2*1  + 6*1  + 1*1  + 5*1  + 8*0  = 4
4*0  + 8*0  + 2*0  + 2*0  + 6*1  + 1*1  + 5*1  + 8*1  = 0
4*0  + 8*0  + 2*0  + 2*0  + 6*0  + 1*1  + 5*1  + 8*1  = 4
4*0  + 8*0  + 2*0  + 2*0  + 6*0  + 1*0  + 5*1  + 8*1  = 3
4*0  + 8*0  + 2*0  + 2*0  + 6*0  + 1*0  + 5*0  + 8*1  = 8

After 2 phases: 34040438

3*1  + 4*0  + 0*-1 + 4*0  + 0*1  + 4*0  + 3*-1 + 8*0  = 0
3*0  + 4*1  + 0*1  + 4*0  + 0*0  + 4*-1 + 3*-1 + 8*0  = 3
3*0  + 4*0  + 0*1  + 4*1  + 0*1  + 4*0  + 3*0  + 8*0  = 4
3*0  + 4*0  + 0*0  + 4*1  + 0*1  + 4*1  + 3*1  + 8*0  = 1
3*0  + 4*0  + 0*0  + 4*0  + 0*1  + 4*1  + 3*1  + 8*1  = 5
3*0  + 4*0  + 0*0  + 4*0  + 0*0  + 4*1  + 3*1  + 8*1  = 5
3*0  + 4*0  + 0*0  + 4*0  + 0*0  + 4*0  + 3*1  + 8*1  = 1
3*0  + 4*0  + 0*0  + 4*0  + 0*0  + 4*0  + 3*0  + 8*1  = 8

After 3 phases: 03415518

0*1  + 3*0  + 4*-1 + 1*0  + 5*1  + 5*0  + 1*-1 + 8*0  = 0
0*0  + 3*1  + 4*1  + 1*0  + 5*0  + 5*-1 + 1*-1 + 8*0  = 1
0*0  + 3*0  + 4*1  + 1*1  + 5*1  + 5*0  + 1*0  + 8*0  = 0
0*0  + 3*0  + 4*0  + 1*1  + 5*1  + 5*1  + 1*1  + 8*0  = 2
0*0  + 3*0  + 4*0  + 1*0  + 5*1  + 5*1  + 1*1  + 8*1  = 9
0*0  + 3*0  + 4*0  + 1*0  + 5*0  + 5*1  + 1*1  + 8*1  = 4
0*0  + 3*0  + 4*0  + 1*0  + 5*0  + 5*0  + 1*1  + 8*1  = 9
0*0  + 3*0  + 4*0  + 1*0  + 5*0  + 5*0  + 1*0  + 8*1  = 8

After 4 phases: 01029498

Here are the first eight digits of the final output list after 100 phases for
some larger inputs:

    80871224585914546619083218645595 becomes 24176176.
    19617804207202209144916044189917 becomes 73745418.
    69317163492948606335995924319873 becomes 52432133.

After 100 phases of FFT, what are the first eight digits in the final output list?

Your puzzle answer was 42945143.
--- Part Two ---

Now that your FFT is working, you can decode the real signal.

The real signal is your puzzle input repeated 10000 times. Treat this new
signal as a single input list. Patterns are still calculated as before, and 100
phases of FFT are still applied.

The first seven digits of your initial input signal also represent the message
offset. The message offset is the location of the eight-digit message in the
final output list. Specifically, the message offset indicates the number of
digits to skip before reading the eight-digit message. For example, if the
first seven digits of your initial input signal were 1234567, the eight-digit
message would be the eight digits after skipping 1,234,567 digits of the final
output list. Or, if the message offset were 7 and your final output list were
98765432109876543210, the eight-digit message would be 21098765. (Of course,
your real message offset will be a seven-digit number, not a one-digit
number like 7.)

Here is the eight-digit message in the final output list after 100 phases. The
message offset given in each input has been highlighted. (Note that the inputs
given below are repeated 10000 times to find the actual starting input lists.)

    03036732577212944063491565474664 becomes 84462026.
    02935109699940807407585447034323 becomes 78725270.
    03081770884921959731165446850517 becomes 53553731.

After repeating your input signal 10000 times and running 100 phases of FFT,
what is the eight-digit message embedded in the final output list?

Your puzzle answer was 99974970.

*/

#include <cstdio>
#include <ctime>

void phase(const char *input, char *output, int N);

template <int N>
void phase(const char (&input)[N], char (&output)[N])
{
    phase(input, output, N);
}

template <int N>
void fft(const char (&input)[N], int rounds)
{
    char buf1[N-1]={};
    char buf2[N-1]={};
    for (int i = 0; i< N-1; i++)
    {
        buf1[i] = input[i] - '0';
    }
    clock_t t0 = clock();
    while (rounds>0)
    {
        if ((rounds & 1) == 0)
            phase(buf1, buf2); else phase(buf2, buf1);
        rounds--;
    }
    clock_t t1 = clock();
    int millis = ((t1 - t0) * 1000 + 10) / CLOCKS_PER_SEC;
    printf("took %d millis\n", millis);

    for (int i = 0; i < 8; i++) printf("%d", buf1[i]);
    putchar('\n');
}

const char puzzle_input[] = "59787832768373756387231168493208357132958685401595722881580547807942982606755215622050260150447434057354351694831693219006743316964757503791265077635087624100920933728566402553345683177887856750286696687049868280429551096246424753455988979991314240464573024671106349865911282028233691096263590173174821612903373057506657412723502892841355947605851392899875273008845072145252173808893257256280602945947694349746967468068181317115464342687490991674021875199960420015509224944411706393854801616653278719131946181597488270591684407220339023716074951397669948364079227701367746309535060821396127254992669346065361442252620041911746738651422249005412940728";

void part_one()
{
    //fft("12345678", 4);
    //fft("80871224585914546619083218645595", 100);

    fft(puzzle_input, 100);
}

#include <cstdlib>
#include <cstring>

void generate_pattern(int N)
{
    int pattern[] = {0, 1, 0, -1};
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int pi = (j+1) / (i+1);
            //if ((pi & 1) == 0) continue;
            pi = pi % 4;
            printf("%3d, ", pattern[pi]);
            //if (pattern[pi]) printf("%3d, ", pattern[pi]);
            //else printf("[%d], ", pi);
        }
        printf("\n");
    }
}

void phase(const char *input, char *output, int N)
{
#if 0
    //int pattern[] = {0, 1, 0, -1};
    for (int i = 0; i < N; i++)
    {
        int n = 0;
        for (int j = i; j < N; j++)
        {
            int pi = (j+1) / (i+1);
            if ((pi & 1) == 0) continue;
            //pi = pi % 4;
            //n += input[j] * pattern[pi];
            n += input[j] * (((pi&0x3)==1) ? 1 : -1);
            //printf("%2d,", pattern[pi]);
        }
        printf("%d\n", i);
        if (n < 0) n = -n;
        output[i] = n % 10;
    }
#elif 0
    for (int i = 0; i < N; i++)
    {
        int n = 0;
        int j = i;
        while (j < N)
        {
            int pi = (j+1) / (i+1);
            if ((pi&0x3) == 1)
            {
                for (int k = 0; k < i + 1 && j < N; k++, j++)
                {
                    n += input[j];
                }
                j += i;
            }
            else if ((pi&0x3) == 3)
            {
                for (int k = 0; k < i + 1 && j < N; k++, j++)
                {
                    n -= input[j];
                }
                j += i;
            }
            else
            {
                j++;
            }
        }
        printf("%d,", i);
        if (n < 0) n = -n;
        output[i] = n % 10;
    }
#else
    for (int i = 0; i < N; i++)
    {
        int n0 = 0;
        int j0 = i;
        while (j0 < N)
        {
            for (int k = 0; k < i + 1 && j0 < N; k++, j0++)
            {
                n0 += input[j0];
            }
            j0 += i*3 + 3;
        }
        int n1 = 0;
        int j1 = i + (i + 1)*2;
        while (j1 < N)
        {
            for (int k = 0; k < i + 1 && j1 < N; k++, j1++)
            {
                n1 -= input[j1];
            }
            j1 += i*3 + 3;
        }
        if ((i%1000)==0) {printf("%d,", i);fflush(stdout);}
        int n = n0 + n1;
        if (n < 0) n = -n;
        output[i] = n % 10;
    }
#endif
    for (int i = 0; i < N; i++) printf("%d", output[i]);
    putchar('\n');
    fflush(stdout);
}

void fft(const char *input, char *output, int N, int rounds)
{
    char *bufs = (char*)malloc(N * 2);
    char *buf1 = bufs + 0;
    char *buf2 = bufs + N;
    for (int i = 0; i < N; i++)
    {
        buf1[i] = input[i] - '0';
    }
    while (rounds>0)
    {
        printf("Round %d\n", rounds);
        fflush(stdout);
        if ((rounds & 1) == 0)
            phase(buf1, buf2, N); else phase(buf2, buf1, N);
        rounds--;
    }
    memcpy(output, buf1, N);
    free(bufs);
}

template <class T, int N>
int length(T (&x)[N])
{
    return N;
}

// Too slow
void part_two()
{
    int N_0 = length(puzzle_input) - 1;
    int N_1 = 10000;
    int N = N_0 * N_1;

    char *input = (char*)malloc(N);
    char *output = (char*)malloc(N);

    for (int i = 0; i < N_1; i++)
    {
        for (int j = 0; j < N_0; j++)
        {
            input[j+i*N_0] = puzzle_input[j];
        }
    }

    printf("--FFT-- N0=%d, N=%d\n", N_0, N); fflush(stdout);
    fft(input, output, N, 100);
}

void fast_phase(const char *input, int N, char *output)
{
    int carry = 0;
    for (int i = N - 1; i >= 0; i--)
    {
        output[i] = (input[i] + carry) % 10;
        carry = output[i];
    }
}

void fast_fft(const char *input, char *output, int N, int rounds)
{
    char *tmp = (char*)malloc(N);

    char *out = (rounds&1) ? output : tmp;
    fast_phase(input, N, out);

    char *inp = (rounds&1) ? tmp : output;
    while (--rounds > 0)
    {
        char *t = out;
        out = inp;
        inp = t;

        fast_phase(inp, N, out);
    }

    free(tmp);
}

// Only need to deal with the input values from offset to end. Also, offset
// (7 first digits of original input), O, is larger than half of the whole input of N digits, i.e. O > 1/2 N.
// All of the pattern is a upper triangular matrix of ones for the lower right pattern sub matrix, this allows
// further simplifications to the calculation of the output vector.
void part_two_2()
{
    int Offset = 5978783;

    int N_0 = length(puzzle_input) - 1;
    int N_1 = 10000;
    int N = N_0 * N_1;

    char *input = (char*)malloc(N);
    char *output = (char*)malloc(N-Offset);

    for (int i = 0; i < N_1; i++)
    {
        for (int j = 0; j < N_0; j++)
        {
            input[j+i*N_0] = puzzle_input[j] - '0';
        }
    }

    printf("--FFT-- N0=%d, N=%d, N-O=%d\n", N_0, N, N-Offset); fflush(stdout);
    fast_fft(input+Offset, output, N - Offset, 100);

    for (int i = 0; i < 8; i++) printf("%d", output[i]);
    putchar('\n');
}

int main(int argc, char **argv)
{
    //generate_pattern(8);
    //part_one();
    //part_two();
    part_two_2();
    return 0;
}

