// 776996-20191205-0a4a7f2b

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

