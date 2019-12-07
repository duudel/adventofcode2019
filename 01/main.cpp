// 776996-20191205-0a4a7f2b

/*

--- Day 1: The Tyranny of the Rocket Equation ---

Santa has become stranded at the edge of the Solar System while delivering
presents to other planets! To accurately calculate his position in space,
safely align his warp drive, and return to Earth in time to save Christmas, he
needs you to bring him measurements from fifty stars.

Collect stars by solving puzzles. Two puzzles will be made available on each
day in the Advent calendar; the second puzzle is unlocked when you complete the
first. Each puzzle grants one star. Good luck!

The Elves quickly load you into a spacecraft and prepare to launch.

At the first Go / No Go poll, every Elf is Go until the Fuel Counter-Upper.
They haven't determined the amount of fuel required yet.

Fuel required to launch a given module is based on its mass. Specifically, to
find the fuel required for a module, take its mass, divide by three, round
down, and subtract 2.

For example:

    For a mass of 12, divide by 3 and round down to get 4, then subtract 2 to get 2.
    For a mass of 14, dividing by 3 and rounding down still yields 4, so the fuel required is also 2.
    For a mass of 1969, the fuel required is 654.
    For a mass of 100756, the fuel required is 33583.

The Fuel Counter-Upper needs to know the total fuel requirement. To find it,
individually calculate the fuel needed for the mass of each module (your puzzle
input), then add together all the fuel values.

What is the sum of the fuel requirements for all of the modules on your spacecraft?

Your puzzle answer was 3262358.
--- Part Two ---

During the second Go / No Go poll, the Elf in charge of the Rocket Equation
Double-Checker stops the launch sequence. Apparently, you forgot to include
additional fuel for the fuel you just added.

Fuel itself requires fuel just like a module - take its mass, divide by three,
round down, and subtract 2. However, that fuel also requires fuel, and that
fuel requires fuel, and so on. Any mass that would require negative fuel should
instead be treated as if it requires zero fuel; the remaining mass, if any, is
instead handled by wishing really hard, which has no mass and is outside the
scope of this calculation.

So, for each module mass, calculate its fuel and add it to the total. Then,
treat the fuel amount you just calculated as the input mass and repeat the
process, continuing until a fuel requirement is zero or negative. For example:

    A module of mass 14 requires 2 fuel. This fuel requires no further fuel (2
    divided by 3 and rounded down is 0, which would call for a negative fuel),
    so the total fuel required is still just 2.  At first, a module of mass
    1969 requires 654 fuel. Then, this fuel requires 216 more fuel (654 / 3 -
    2). 216 then requires 70 more fuel, which requires 21 fuel, which requires
    5 fuel, which requires no further fuel. So, the total fuel required for a
    module of mass 1969 is 654 + 216 + 70 + 21 + 5 = 966.
    The fuel required by a module of mass 100756 and its fuel is: 33583 + 11192
    + 3728 + 1240 + 411 + 135 + 43 + 12 + 2 = 50346.

What is the sum of the fuel requirements for all of the modules on your
spacecraft when also taking into account the mass of the added fuel? (Calculate
the fuel requirements for each module separately, then add them all up at the
end.)

Your puzzle answer was 4890696.

*/

#include <cstdio>
#include <cstdint>

int masses[] = {
  113481
, 140620
, 123826
, 86474
, 71091
, 126880
, 103784
, 140154
, 124024
, 54281
, 80810
, 109441
, 68828
, 144207
, 99151
, 136876
, 99398
, 138555
, 118619
, 133215
, 139302
, 137780
, 136649
, 83358
, 63027
, 75067
, 73974
, 90158
, 94691
, 86847
, 61466
, 81184
, 86043
, 119923
, 116576
, 131380
, 102136
, 143364
, 124421
, 123141
, 138131
, 73274
, 84598
, 61410
, 67240
, 136186
, 63878
, 135804
, 73599
, 84526
, 116178
, 114587
, 58606
, 79162
, 124031
, 120329
, 61270
, 89887
, 54859
, 67618
, 96669
, 56796
, 55725
, 96105
, 68833
, 52417
, 72249
, 53930
, 139995
, 86217
, 131618
, 137145
, 54944
, 76456
, 82141
, 69754
, 102656
, 57461
, 108747
, 79510
, 105715
, 98046
, 116903
, 139339
, 127451
, 135374
, 88468
, 69524
, 76112
, 110928
, 99160
, 137229
, 121433
, 65951
, 56267
, 117209
, 61358
, 73659
, 69633
, 149274
};

int64_t fuel_from_mass(int64_t mass)
{
    int64_t fuel = (mass / 3) - 2;
    return (fuel < 0) ? 0 : fuel;
}

int64_t fuel_for_fuel(int64_t fuel_mass)
{
    int64_t added_fuel = 0;
    do {
        fuel_mass = fuel_from_mass(fuel_mass);
        printf("%lld\n", fuel_mass);
        added_fuel += fuel_mass;
    } while(fuel_mass > 0);
    return added_fuel;
}

int main(int argc, const char **argv)
{
    int64_t total_fuel = 0;
    int64_t added_fuel = 0;
    for (uint64_t i = 0; i < sizeof(masses)/sizeof(int); i++)
    {
        int mass = masses[i];
        int64_t fuel = fuel_from_mass(mass);
        total_fuel += fuel;

        added_fuel += fuel_for_fuel(fuel);
    }
    printf("Total fuel: %lld; added fuel %lld\n", total_fuel, added_fuel);
    printf("Total fuel + fuel for fuel: %lld\n", total_fuel + added_fuel);
    return 0;
}

