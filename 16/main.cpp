
#include <cstdio>

template <int N>
void phase(const int (&input)[N], int (&output)[N])
{
    int pattern[] = {0, 1, 0, -1};
    for (int i = 0; i < N; i++)
    {
        int n = 0;
        for (int j = 0; j < N; j++)
        {
            int pi = (j+1) / (i+1);
            pi = pi % 4;
            n += input[j] * pattern[pi];
            //printf("%2d,", pattern[pi]);
        }
        //printf("\n");
        if (n < 0) n = -n;
        output[i] = n % 10;
    }
    for (int i = 0; i < N; i++) printf("%d", output[i]);
    putchar('\n');
}

template <int N>
void fft(const char (&input)[N], int rounds)
{
    int buf1[N-1]={};
    int buf2[N-1]={};
    for (int i = 0; i< N-1; i++)
    {
        buf1[i] = input[i] - '0';
    }
    while (rounds>0)
    {
        if ((rounds & 1) == 0)
            phase(buf1, buf2); else phase(buf2, buf1);
        rounds--;
    }
}

void part_one()
{
    //fft("12345678", 4);
    //fft("80871224585914546619083218645595", 100);

    const char input[] = "59787832768373756387231168493208357132958685401595722881580547807942982606755215622050260150447434057354351694831693219006743316964757503791265077635087624100920933728566402553345683177887856750286696687049868280429551096246424753455988979991314240464573024671106349865911282028233691096263590173174821612903373057506657412723502892841355947605851392899875273008845072145252173808893257256280602945947694349746967468068181317115464342687490991674021875199960420015509224944411706393854801616653278719131946181597488270591684407220339023716074951397669948364079227701367746309535060821396127254992669346065361442252620041911746738651422249005412940728";
    fft(input, 100);
}

int main(int argc, char **argv)
{
    part_one();
    return 0;
}
