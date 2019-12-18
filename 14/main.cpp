
/*

--- Day 14: Space Stoichiometry ---

As you approach the rings of Saturn, your ship's low fuel indicator turns on.
There isn't any fuel here, but the rings have plenty of raw material. Perhaps
your ship's Inter-Stellar Refinery Union brand nanofactory can turn these raw
materials into fuel.

You ask the nanofactory to produce a list of the reactions it can perform that
are relevant to this process (your puzzle input). Every reaction turns some
quantities of specific input chemicals into some quantity of an output
chemical. Almost every chemical is produced by exactly one reaction; the only
exception, ORE, is the raw material input to the entire process and is not
produced by a reaction.

You just need to know how much ORE you'll need to collect before you can
produce one unit of FUEL.

Each reaction gives specific quantities for its inputs and output; reactions
cannot be partially run, so only whole integer multiples of these quantities
can be used. (It's okay to have leftover chemicals when you're done, though.)
For example, the reaction 1 A, 2 B, 3 C => 2 D means that exactly 2 units of
chemical D can be produced by consuming exactly 1 A, 2 B and 3 C. You can run
the full reaction as many times as necessary; for example, you could produce 10
D by consuming 5 A, 10 B, and 15 C.

Suppose your nanofactory produces the following list of reactions:

10 ORE => 10 A
1 ORE => 1 B
7 A, 1 B => 1 C
7 A, 1 C => 1 D
7 A, 1 D => 1 E
7 A, 1 E => 1 FUEL

The first two reactions use only ORE as inputs; they indicate that you can
produce as much of chemical A as you want (in increments of 10 units, each 10
costing 10 ORE) and as much of chemical B as you want (each costing 1 ORE). To
produce 1 FUEL, a total of 31 ORE is required: 1 ORE to produce 1 B, then 30
more ORE to produce the 7 + 7 + 7 + 7 = 28 A (with 2 extra A wasted) required
in the reactions to convert the B into C, C into D, D into E, and finally E
into FUEL. (30 A is produced because its reaction requires that it is created
in increments of 10.)

Or, suppose you have the following list of reactions:

9 ORE => 2 A
8 ORE => 3 B
7 ORE => 5 C
3 A, 4 B => 1 AB
5 B, 7 C => 1 BC
4 C, 1 A => 1 CA
2 AB, 3 BC, 4 CA => 1 FUEL

The above list of reactions requires 165 ORE to produce 1 FUEL:

    Consume 45 ORE to produce 10 A.
    Consume 64 ORE to produce 24 B.
    Consume 56 ORE to produce 40 C.
    Consume 6 A, 8 B to produce 2 AB.
    Consume 15 B, 21 C to produce 3 BC.
    Consume 16 C, 4 A to produce 4 CA.
    Consume 2 AB, 3 BC, 4 CA to produce 1 FUEL.

Here are some larger examples:

    13312 ORE for 1 FUEL:

    157 ORE => 5 NZVS
    165 ORE => 6 DCFZ
    44 XJWVT, 5 KHKGT, 1 QDVJ, 29 NZVS, 9 GPVTF, 48 HKGWZ => 1 FUEL
    12 HKGWZ, 1 GPVTF, 8 PSHF => 9 QDVJ
    179 ORE => 7 PSHF
    177 ORE => 5 HKGWZ
    7 DCFZ, 7 PSHF => 2 XJWVT
    165 ORE => 2 GPVTF
    3 DCFZ, 7 NZVS, 5 HKGWZ, 10 PSHF => 8 KHKGT

    180697 ORE for 1 FUEL:

    2 VPVL, 7 FWMGM, 2 CXFTF, 11 MNCFX => 1 STKFG
    17 NVRVD, 3 JNWZP => 8 VPVL
    53 STKFG, 6 MNCFX, 46 VJHF, 81 HVMC, 68 CXFTF, 25 GNMV => 1 FUEL
    22 VJHF, 37 MNCFX => 5 FWMGM
    139 ORE => 4 NVRVD
    144 ORE => 7 JNWZP
    5 MNCFX, 7 RFSQX, 2 FWMGM, 2 VPVL, 19 CXFTF => 3 HVMC
    5 VJHF, 7 MNCFX, 9 VPVL, 37 CXFTF => 6 GNMV
    145 ORE => 6 MNCFX
    1 NVRVD => 8 CXFTF
    1 VJHF, 6 MNCFX => 4 RFSQX
    176 ORE => 6 VJHF

    2210736 ORE for 1 FUEL:

    171 ORE => 8 CNZTR
    7 ZLQW, 3 BMBT, 9 XCVML, 26 XMNCP, 1 WPTQ, 2 MZWV, 1 RJRHP => 4 PLWSL
    114 ORE => 4 BHXH
    14 VRPVC => 6 BMBT
    6 BHXH, 18 KTJDG, 12 WPTQ, 7 PLWSL, 31 FHTLT, 37 ZDVW => 1 FUEL
    6 WPTQ, 2 BMBT, 8 ZLQW, 18 KTJDG, 1 XMNCP, 6 MZWV, 1 RJRHP => 6 FHTLT
    15 XDBXC, 2 LTCX, 1 VRPVC => 6 ZLQW
    13 WPTQ, 10 LTCX, 3 RJRHP, 14 XMNCP, 2 MZWV, 1 ZLQW => 1 ZDVW
    5 BMBT => 4 WPTQ
    189 ORE => 9 KTJDG
    1 MZWV, 17 XDBXC, 3 XCVML => 2 XMNCP
    12 VRPVC, 27 CNZTR => 2 XDBXC
    15 KTJDG, 12 BHXH => 5 XCVML
    3 BHXH, 2 VRPVC => 7 MZWV
    121 ORE => 7 VRPVC
    7 XCVML => 6 RJRHP
    5 BHXH, 4 VRPVC => 5 LTCX

Given the list of reactions in your puzzle input, what is the minimum amount of
ORE required to produce exactly 1 FUEL?

Your puzzle answer was 2556890.
--- Part Two ---

After collecting ORE for a while, you check your cargo hold: 1 trillion
(1000000000000) units of ORE.

With that much ore, given the examples above:

    The 13312 ORE-per-FUEL example could produce 82892753 FUEL.
    The 180697 ORE-per-FUEL example could produce 5586022 FUEL.
    The 2210736 ORE-per-FUEL example could produce 460664 FUEL.

Given 1 trillion ORE, what is the maximum amount of FUEL you can produce?

Your puzzle answer was 1120408.

*/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <ctime>

enum Chem
{
    none, ORE, FUEL
};

struct ChemQuantity
{
    int n;
    int chem;
};

#define MAX_INPUTS 10

struct Reaction
{
    ChemQuantity inputs[MAX_INPUTS];
    ChemQuantity output;
};

struct Reactions
{
    int num;
    Reaction r[128];
};

template <int N>
Reactions make_reactions(Reaction (&&reactions)[N])
{
    Reactions result = {};
    result.num = N;
    memcpy(&result.r, &reactions, sizeof(Reaction)*N);
    return result;
}

namespace sample00
{
    enum Chem
    {
        none,
        ORE,
        FUEL,
        A, B, C, AB, BC, CA,
    };

    static const char* Chem_names[] = {
        "none",
        "ORE",
        "FUEL",
        "A", "B", "C", "AB", "BC", "CA",
    };
    static const auto reactions = make_reactions({
        { {{9, ORE}},                         /* => */ {2, A} },
        { {{8, ORE}},                         /* => */ {3, B} },
        { {{7, ORE}},                         /* => */ {5, C} },
        { {{3, A}, {4, B}},                 /* => */ {1, AB} },
        { {{5, B}, {7, C}},                 /* => */ {1, BC} },
        { {{4, C}, {1, A}},                 /* => */ {1, CA} },
        { {{2, AB}, {3, BC}, {4, CA}},    /* => */ {1, FUEL} },
    });
}

namespace sample01
{
    enum Chem
    {
        none,
        ORE,
        FUEL,
        XJWVT, KHKGT, QDVJ, NZVS, DCFZ, GPVTF, HKGWZ, PSHF,
    };

    static const char* Chem_names[] = {
        "none",
        "ORE",
        "FUEL",
        "XJWVT", "KHKGT", "QDVJ", "NZVS", "DCFZ", "GPVTF", "HKGWZ", "PSHF",
    };

    static const auto reactions = make_reactions({
        { {{157, ORE}},                               /* => */ {5, NZVS} },
        { {{165, ORE}},                               /* => */ {6, DCFZ} },
        { {{ 44, XJWVT}, {5, KHKGT}, {1, QDVJ}, {29, NZVS}, {9, GPVTF}, {48, HKGWZ}}, /* => */ {1, FUEL} },
        { {{ 12, HKGWZ}, {1, GPVTF}, {8, PSHF}},  /* => */ {9, QDVJ} },
        { {{179, ORE}},                               /* => */ {7, PSHF} },
        { {{177, ORE}},                               /* => */ {5, HKGWZ} },
        { {{  7, DCFZ}, {7, PSHF}},                 /* => */ {2, XJWVT} },
        { {{165, ORE}},                               /* => */ {2, GPVTF} },
        { {{  3, DCFZ}, {7, NZVS}, {5, HKGWZ}, {10, PSHF}}, /* => */ {8, KHKGT} },
    });
}

namespace puzzle
{
    enum Chem
    {
        none,
        ORE,
        FUEL,
        LQGXD, TDLQ, VGLV, WBQF, JZKMJ, TRSK,
        MGHZ, ZLDQF, HMVG, JWQH, QFBC, ZXVNM,
        JFJZH, QTPX, LDLWS, NVZPS, QPWF, PRWSM,
        WHWF, LZBQ, MDPJB, KQHW, QBCKX, TMTH,
        WLFTZ, NPMPT, QXHFH, LCDVR, MWFP, MSFV,
        BPDJL, DVGW, KCPSH , FPZXN, FQSK, FBHW,
        XWKXC, PZTGX, NTQZ, DJWCL, WRNJ, TBGJC,
        HKWP, PDRN, BJNS, GNBQ,  HQHD, NJFNC,
        NDCQ, GPFRS, LPQZ, LZKM, QRTH, TDTKT,
        QGVQC, GKPK, KGZT, CSPFK, TSDLM, XDKP,
        PCWS, KVFMS,
    };

    static const char* Chem_names[] =
    {
        "none",
        "ORE",
        "FUEL",
        "LQGXD", "TDLQ", "VGLV", "WBQF", "JZKMJ", "TRSK",
        "MGHZ", "ZLDQF", "HMVG", "JWQH", "QFBC", "ZXVNM",
        "JFJZH", "QTPX", "LDLWS", "NVZPS", "QPWF", "PRWSM",
        "WHWF", "LZBQ", "MDPJB", "KQHW", "QBCKX", "TMTH",
        "WLFTZ", "NPMPT", "QXHFH", "LCDVR", "MWFP", "MSFV",
        "BPDJL", "DVGW", "KCPSH ", "FPZXN", "FQSK", "FBHW",
        "XWKXC", "PZTGX", "NTQZ", "DJWCL", "WRNJ", "TBGJC",
        "HKWP", "PDRN", "BJNS", "GNBQ", " HQHD", "NJFNC",
        "NDCQ", "GPFRS", "LPQZ", "LZKM", "QRTH", "TDTKT",
        "QGVQC", "GKPK", "KGZT", "CSPFK", "TSDLM", "XDKP",
        "PCWS", "KVFMS",
    };

    static const auto reactions = make_reactions({
        { {{ 14, LQGXD}, {6, TDLQ}},                /* => */ {9, VGLV} },
        { {{  1,  WBQF}, {2, JZKMJ}},               /* => */ {5, TRSK} },
        { {{  5,  MGHZ}, {5, ZLDQF}},               /* => */ {8, HMVG} },
        { {{  1,  JWQH}, {1, QFBC}, {2, ZXVNM}},  /* => */ {8, JFJZH} },
        { {{  8,  QTPX}, {8, LDLWS}},               /* => */ {6, NVZPS} },
        { {{  2,  QPWF}, {1, PRWSM}},               /* => */ {5, WHWF} },
        { {{  1,  QPWF}, {8, LDLWS}},               /* => */ {5, LZBQ} },
        { {{127,   ORE}},                             /* => */ {1, MDPJB} },
        { {{  4,  WHWF}},                             /* => */ {4, KQHW} },
        { {{  1, QBCKX}, {3, TMTH}},                /* => */ {4, WLFTZ} },
        { {{ 15, NPMPT}, {4, TMTH}},                /* => */ {6, QFBC} },
        { {{ 12, MDPJB}},                             /* => */ {9, PRWSM} },
        { {{  5, QXHFH}, {3, LCDVR}, {24, MWFP}, {1, MSFV}, {1, BPDJL}, {3, LQGXD}, {2, DVGW}}, /* => */ {2, KCPSH} },
        { {{  6, FPZXN}, {1, FQSK}, {3, TMTH}},   /* => */ {1, FBHW} },
        { {{ 25, PRWSM}},                             /* => */ {1, MGHZ} },
        { {{  6, XWKXC}, {5, TMTH}, {1, PZTGX}},  /* => */ {6, NTQZ} },
        { {{  3, BPDJL}, {3, DJWCL}, {2, JZKMJ}}, /* => */ {7, MWFP} },
        { {{  5, JFJZH}},                             /* => */ {3, DJWCL} },
        { {{ 22,  WRNJ}, {12, TRSK}},               /* => */ {5, TBGJC} },
        { {{  3,  HKWP}},                             /* => */ {1, PDRN} },
        { {{  3,  JWQH}},                             /* => */ {5, JZKMJ} },
        { {{  4,  WBQF}},                             /* => */ {2, BJNS} },
        { {{  1,  GNBQ}},                             /* => */ {9, FQSK} },
        { {{  8,  HMVG}, {1, HQHD}},                /* => */ {5, NJFNC} },
        { {{  7, QBCKX}, {1, FQSK}},                /* => */ {9, NDCQ} },
        { {{  3, XWKXC}, {7, QFBC}, {3, GPFRS}, {2, LPQZ}, {2, LQGXD}, {20, LZKM}, {1, QRTH}}, /* => */ {8, TDTKT} },
        { {{  1,  QTPX}},                             /* => */ {3, LPQZ} },
        { {{  2, QGVQC}, {14, LDLWS}},              /* => */ {1, NPMPT} },
        { {{  1,  QRTH}, {7, BPDJL}},               /* => */ {7, XWKXC} },
        { {{  9, WLFTZ}, {8, TDLQ}},                /* => */ {6, GKPK} },
        { {{  4,  GNBQ}},                             /* => */ {3, QXHFH} },
        { {{  3, TBGJC}, {1, LPQZ}},                /* => */ {3, DVGW} },
        { {{  3,  NDCQ}, {1, KGZT}},                /* => */ {7, FPZXN} },
        { {{ 36, WLFTZ}, {1, KCPSH}, {1, GKPK}, {1, TDTKT}, {3, CSPFK}, {27, JZKMJ}, {5, VGLV}, {39, XWKXC}}, /* => */ {1, FUEL} },
        { {{115,   ORE}},                             /* => */ {7, QGVQC} },
        { {{ 21,  NTQZ}, {11, HQHD}, {33, JFJZH}, {3, NJFNC}, {3, MSFV}, {1, TRSK}, {7, WRNJ}}, /* => */ {9, CSPFK} },
        { {{  3,  DVGW}},                             /* => */ {4, TDLQ} },
        { {{  5, FPZXN}},                             /* => */ {6, WRNJ} },
        { {{ 10, TSDLM}, {17, XDKP}, {3, PDRN}},  /* => */ {2, HQHD} },
        { {{  1,  PCWS}},                             /* => */ {3, PZTGX} },
        { {{  2, QXHFH}},                             /* => */ {5, JWQH} },
        { {{ 17,  KQHW}},                             /* => */ {2, WBQF} },
        { {{139,   ORE}},                             /* => */ {5, LDLWS} },
        { {{  3, TSDLM}},                             /* => */ {9, KGZT} },
        { {{ 16, NPMPT}},                             /* => */ {3, QTPX} },
        { {{  3,  DVGW}, {5, KVFMS}, {3, WLFTZ}}, /* => */ {6, GPFRS} },
        { {{  1, PZTGX}, {2, LCDVR}, {13, TBGJC}}, /* => */ {6, LZKM} },
        { {{  5, ZXVNM}, {2, QXHFH}},               /* => */ {4, MSFV} },
        { {{  4,  XDKP}, {7, FBHW}, {2, PCWS}},   /* => */ {3, LCDVR} },
        { {{  3,  TRSK}},                             /* => */ {7, KVFMS} },
        { {{ 10, LDLWS}},                             /* => */ {9, TMTH} },
        { {{  2, TBGJC}},                             /* => */ {6, LQGXD} },
        { {{  2,  TRSK}},                             /* => */ {6, ZXVNM} },
        { {{  4,  KQHW}, {1, NVZPS}},               /* => */ {8, ZLDQF} },
        { {{  2,  LZBQ}},                             /* => */ {4, QBCKX} },
        { {{  7, QBCKX}},                             /* => */ {6, TSDLM} },
        { {{152,   ORE}},                             /* => */ {3, QPWF} },
        { {{  2, TSDLM}, {8, WHWF}},                /* => */ {3, HKWP} },
        { {{ 19,  FQSK}},                             /* => */ {8, QRTH} },
        { {{ 19,  QTPX}},                             /* => */ {3, GNBQ} },
        { {{  4,  PDRN}, {12, HKWP}, {4, PCWS}},  /* => */ {3, XDKP} },
        { {{  6,  LZBQ}, {19, BJNS}},               /* => */ {5, BPDJL} },
        { {{  5,  HKWP}, {6, NVZPS}},               /* => */ {3, PCWS} },
    });
};

int find_reaction_to_produce_n(Reactions reactions, int chem)
{
    for (int i = 0; i < reactions.num; i++)
    {
        if (reactions.r[i].output.chem == chem)
        {
            return i;
        }
    }
    return -1;
}

void print_reaction(Reaction r, const char **Chem_names)
{
    for (int i = 0; i < MAX_INPUTS; i++)
    {
        ChemQuantity inp = r.inputs[i];
        if (inp.chem == 0) break;

        printf("%d %d, ", inp.n, inp.chem);
    }
    printf("=> %d %s\n", r.output.n, Chem_names[r.output.chem]);
}

struct Amounts
{
    int64_t n[100];
};

int num_ore_required(Reactions reactions, const char **Chem_names, int64_t n, int chem, Amounts *amounts)
{
    if (chem == ORE) return n;

    int ri = find_reaction_to_produce_n(reactions, chem);
    assert(ri != -1);
    Reaction reaction = reactions.r[ri];

    int64_t yield = reaction.output.n;
    int64_t how_many_over = n % yield;
    int64_t multiple = 1;
    if (how_many_over > 0)
    {
        multiple = (n + yield) / yield;
        int64_t left_overs = multiple*yield - n;
        //printf("left overs: %lld %s, multiple: %lld\n", left_overs, Chem_names[reaction.output.chem], multiple);
        amounts->n[reaction.output.chem] += left_overs;
    }
    else
    {
        multiple = n / yield;
        //printf("multiple: %lld \n", multiple);
    }

    //printf("%lld %s required\n", n, Chem_names[chem]);

    int64_t total = 0;
    for (int i = 0; i < MAX_INPUTS; i++)
    {
        ChemQuantity inp = reaction.inputs[i];
        if (inp.chem == none) break;

        int64_t inp_n = inp.n*multiple;
        //printf("Input needed: %lld %s\n", inp_n, Chem_names[inp.chem]);
        if (inp.chem != ORE)
        {
            int64_t left_overs = amounts->n[inp.chem];
            if (left_overs >= inp_n)
            {
                //printf("(1) Left overs of %s: %lld\n", Chem_names[inp.chem], left_overs);
                left_overs -= inp_n;
                inp_n = 0;
            }
            else
            {
                //printf("(2) Left overs of %s: %lld\n", Chem_names[inp.chem], left_overs);
                inp_n -= left_overs;
                left_overs = 0;
            }
            amounts->n[inp.chem] = left_overs;
        }

        if (inp_n > 0)
        {
            total += num_ore_required(reactions, Chem_names, inp_n, inp.chem, amounts);
        }
    }

    return total;
}

void part_one()
{
    //Reactions reactions = sample00::reactions;
    //auto Chem_names = sample00::Chem_names;
    //Reactions reactions = sample01::reactions;
    //auto Chem_names = sample01::Chem_names;
    Reactions reactions = puzzle::reactions;
    auto Chem_names = puzzle::Chem_names;

    Amounts amounts = {};
    int total = num_ore_required(reactions, Chem_names, 1, FUEL, &amounts);
    printf("For 1 FUEL, %d ORE is required\n", total);
}

void part_two()
{
    time_t start_t = time(nullptr);
    Reactions reactions = puzzle::reactions;
    auto Chem_names = puzzle::Chem_names;

    int64_t trillion = 1000'000'000'000LL;
    Amounts amounts = {};
    int64_t fuel = 0;
    int64_t total_ore = 0;
    while (total_ore < trillion)
    {
        int ores = num_ore_required(reactions, Chem_names, 1, FUEL, &amounts);
        if (total_ore + ores <= trillion)
        {
            fuel++;
            total_ore += ores;
        }

        if (total_ore % 10000 == 0)
        {
            printf("%lld ore consumed, %lld fuel produced\n", total_ore, fuel);
            fflush(stdout);
        }
    }
    time_t end_t = time(nullptr);
    time_t elapsed = end_t - start_t;
    printf("For %lld FUEL, %lld ORE is consumed; %lld seconds elapsed\n", fuel, total_ore, elapsed);
}


int main(int argc, char **argv)
{
    //part_one();
    part_two();
    return 0;
}

