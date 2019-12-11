
/*

--- Day 10: Monitoring Station ---

You fly into the asteroid belt and reach the Ceres monitoring station. The
Elves here have an emergency: they're having trouble tracking all of the
asteroids and can't be sure they're safe.

The Elves would like to build a new monitoring station in a nearby area of
space; they hand you a map of all of the asteroids in that region (your puzzle
input).

The map indicates whether each position is empty (.) or contains an asteroid
(#). The asteroids are much smaller than they appear on the map, and every
asteroid is exactly in the center of its marked position. The asteroids can be
described with X,Y coordinates where X is the distance from the left edge and Y
is the distance from the top edge (so the top-left corner is 0,0 and the
position immediately to its right is 1,0).

Your job is to figure out which asteroid would be the best place to build a new
monitoring station. A monitoring station can detect any asteroid to which it
has direct line of sight - that is, there cannot be another asteroid exactly
between them. This line of sight can be at any angle, not just lines aligned to
the grid or diagonally. The best location is the asteroid that can detect the
largest number of other asteroids.

For example, consider the following map:

.#..#
.....
#####
....#
...##

The best location for a new monitoring station on this map is the highlighted
asteroid at 3,4 because it can detect 8 asteroids, more than any other
location. (The only asteroid it cannot detect is the one at 1,0; its view of
this asteroid is blocked by the asteroid at 2,2.) All other asteroids are worse
locations; they can detect 7 or fewer other asteroids. Here is the number of
other asteroids a monitoring station on each asteroid could detect:

.7..7
.....
67775
....7
...87

Here is an asteroid (#) and some examples of the ways its line of sight might
be blocked. If there were another asteroid at the location of a capital letter,
the locations marked with the corresponding lowercase letter would be blocked
and could not be detected:

#.........
...A......
...B..a...
.EDCG....a
..F.c.b...
.....c....
..efd.c.gb
.......c..
....f...c.
...e..d..c

Here are some larger examples:

    Best is 5,8 with 33 other asteroids detected:

    ......#.#.
    #..#.#....
    ..#######.
    .#.#.###..
    .#..#.....
    ..#....#.#
    #..#....#.
    .##.#..###
    ##...#..#.
    .#....####

    Best is 1,2 with 35 other asteroids detected:

    #.#...#.#.
    .###....#.
    .#....#...
    ##.#.#.#.#
    ....#.#.#.
    .##..###.#
    ..#...##..
    ..##....##
    ......#...
    .####.###.

    Best is 6,3 with 41 other asteroids detected:

    .#..#..###
    ####.###.#
    ....###.#.
    ..###.##.#
    ##.##.#.#.
    ....###..#
    ..#.#..#.#
    #..#.#.###
    .##...##.#
    .....#.#..

    Best is 11,13 with 210 other asteroids detected:

    .#..##.###...#######
    ##.############..##.
    .#.######.########.#
    .###.#######.####.#.
    #####.##.#.##.###.##
    ..#####..#.#########
    ####################
    #.####....###.#.#.##
    ##.#################
    #####.##.###..####..
    ..######..##.#######
    ####.##.####...##..#
    .#####..#.######.###
    ##...#.##########...
    #.##########.#######
    .####.#.###.###.#.##
    ....##.##.###..#####
    .#.#.###########.###
    #.#.#.#####.####.###
    ###.##.####.##.#..##

Find the best location for a new monitoring station. How many other asteroids
can be detected from that location?

Your puzzle answer was 253.
--- Part Two ---

Once you give them the coordinates, the Elves quickly deploy an Instant
Monitoring Station to the location and discover the worst: there are simply too
many asteroids.

The only solution is complete vaporization by giant laser.

Fortunately, in addition to an asteroid scanner, the new monitoring station
also comes equipped with a giant rotating laser perfect for vaporizing
asteroids. The laser starts by pointing up and always rotates clockwise,
    vaporizing any asteroid it hits.

If multiple asteroids are exactly in line with the station, the laser only has
enough power to vaporize one of them before continuing its rotation. In other
words, the same asteroids that can be detected can be vaporized, but if
vaporizing one asteroid makes another one detectable, the newly-detected
asteroid won't be vaporized until the laser has returned to the same position
by rotating a full 360 degrees.

For example, consider the following map, where the asteroid with the new
monitoring station (and laser) is marked X:

.#....#####...#..
##...##.#####..##
##...#...#.#####.
..#.....X...###..
..#.#.....#....##

The first nine asteroids to get vaporized, in order, would be:

.#....###24...#..
##...##.13#67..9#
##...#...5.8####.
..#.....X...###..
..#.#.....#....##

Note that some asteroids (the ones behind the asteroids marked 1, 5, and 7)
won't have a chance to be vaporized until the next full rotation. The laser
continues rotating; the next nine to be vaporized are:

.#....###.....#..
##...##...#.....#
##...#......1234.
..#.....X...5##..
..#.9.....8....76

The next nine to be vaporized are then:

.8....###.....#..
56...9#...#.....#
34...7...........
..2.....X....##..
..1..............

Finally, the laser completes its first full rotation (1 through 3), a second
rotation (4 through 8), and vaporizes the last asteroid (9) partway through its
third rotation:

......234.....6..
......1...5.....7
.................
........X....89..
.................

In the large example above (the one with the best monitoring station location
at 11,13):

    The 1st asteroid to be vaporized is at 11,12.
    The 2nd asteroid to be vaporized is at 12,1.
    The 3rd asteroid to be vaporized is at 12,2.
    The 10th asteroid to be vaporized is at 12,8.
    The 20th asteroid to be vaporized is at 16,0.
    The 50th asteroid to be vaporized is at 16,9.
    The 100th asteroid to be vaporized is at 10,16.
    The 199th asteroid to be vaporized is at 9,6.
    The 200th asteroid to be vaporized is at 8,2.
    The 201st asteroid to be vaporized is at 10,9.
    The 299th and final asteroid to be vaporized is at 11,1.

The Elves are placing bets on which will be the 200th asteroid to be vaporized.
Win the bet by determining which asteroid that will be; what do you get if you
multiply its X coordinate by 100 and then add its Y coordinate? (For example,
8,2 becomes 802.)

Your puzzle answer was 815.

*/

#include <cstdio>
#include <cstdlib>
#include <cstring>

int abs(int x)
{
    return (x<0) ? -x : x;
}

template <class T>
T get(const T *data, int width, int x, int y)
{
    return data[y*width + x];
}

int gcd(int a, int b)
{
    int temp;
    while (b != 0)
    {
        temp = a % b;

        a = b;
        b = temp;
    }
    return a;
}

void get_step(int target_x, int target_y, int x, int y, int *step_x, int *step_y)
{
    int dx = target_x - x;
    int dy = target_y - y;
    int dd = gcd(abs(dx), abs(dy));
    *step_x = dx / dd;
    *step_y = dy / dd;
}

bool is_visible(const char *data, int width, int height, int sx, int sy, int ax, int ay)
{
    int step_x, step_y;
    get_step(sx, sy, ax, ay, &step_x, &step_y);

    int x = ax + step_x;
    int y = ay + step_y;
    while ((x != sx) || (y != sy))
    {
        if (data[y*width + x] == '#') return false;
        //if (get(data, width, x, y) == '#') return false;
        x += step_x;
        y += step_y;
    }
    return true;
}

int find_visible_num(const char *data, int width, int height, int sx, int sy)
{
    int num_visible = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (data[y*width + x] == '.') continue;
            if ((x == sx) && (y == sy)) continue;

            if (is_visible(data, width, height, sx, sy, x, y)) num_visible++;
        }
    }
    return num_visible;
}

void find_best(const char *data, int width, int height, int *best_x, int *best_y)
{
    int *nums = (int*)calloc(width*height, sizeof(int));
    int best_num = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (data[y*width + x] == '.') continue;
            int num_visible = find_visible_num(data, width, height, x, y);
            nums[y*width + x] = num_visible;
            if (num_visible > best_num)
            {
                *best_x = x;
                *best_y = y;
                best_num = num_visible;
            }
        }
    }
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            printf("%3d", nums[y*width + x]);
        }
        printf("\n");
    }
    free(nums);
    printf("Most asteroids visible: %d at (%d,%d)\n", best_num, *best_x, *best_y);
}

void find_best(const char *data, int width, int height)
{
    int best_x = 0, best_y = 0;
    find_best(data, width, height, &best_x, &best_y);
}

void print_asteroids(const char *asteroids, int width, int height, int lx, int ly, int bx, int by)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (y == ly && x == lx)
            {
                putchar('X');
            }
            else if (y == by && x == bx)
            {
                putchar('O');
            }
            else
            {
                putchar(asteroids[y*width + x]);
            }
        }
        putchar('\n');
    }
}

#include <algorithm>

int quadrant(int dx, int dy)
{
    if (dx > 0)
    {
        if (dy >= 0)
        {
            return 3;
        }
        else
        {
            return 2;
        }
    }
    else
    {
        if (dy >= 0)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
}

template <class T>
void sort_by_angle(T *pts, int N, int ox, int oy)
{
    std::sort(pts, pts + N, [ox, oy](T a, T b) {
        int a_dx = ox - a.x;
        int a_dy = oy - a.y;
        int b_dx = ox - b.x;
        int b_dy = oy - b.y;
        int aq = quadrant(a_dx, a_dy);
        int bq = quadrant(b_dx, b_dy);
        get_step(a.x, a.y, ox, oy, &a_dx, &a_dy);
        get_step(b.x, b.y, ox, oy, &b_dx, &b_dy);
        if (aq == bq)
        {
            if (aq == 0 || aq == 2)
            {
                return abs(a_dx) * abs(b_dy) < abs(b_dx) * abs(a_dy);
            }
            else if (aq == 1 || aq == 3)
            {
                return abs(a_dx) * abs(b_dy) > abs(b_dx) * abs(a_dy);
            }
        }
        return aq < bq;
    });
}

void vaporize(const char *data, int width, int height, int laser_x, int laser_y)
{
    fflush(stdout);
    char *asteroids = strdup(data);

    int asteroids_vaporized = 0;
    int vaporized_x = 0;
    int vaporized_y = 0;

    struct { int x, y; } targets[25*25];
    int tgts = 0;
    for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
    {
        if (!(x == laser_x && y == laser_y)) targets[tgts++] = {x, y};
    }

    sort_by_angle(targets, tgts, laser_x, laser_y);

    int *buffer = (int*)calloc(tgts, sizeof(int));
    for (int i = 0; i < tgts; i++)
    {
        int x = targets[i].x;
        int y = targets[i].y;
        buffer[y*width + x] = i;
    }
    int i = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            printf("%4d", buffer[i++]);
        }
        printf("\n");
    }

    struct { int x, y; } steps[25*25];

    int step_n = 0;
    for (int i = 0; i < tgts; i++)
    {
        int sx, sy;
        get_step(targets[i].x, targets[i].y, laser_x, laser_y, &sx, &sy);
        if (step_n == 0)
        {
            steps[step_n++] = {sx, sy};
        }
        else if (steps[step_n-1].x != sx || steps[step_n-1].y != sy)
        {
            steps[step_n++] = {sx, sy};
        }
    }

    memset(buffer, 0, step_n * sizeof(int));
    for (int i = 0; i < step_n; i++)
    {
        int x = laser_x + steps[i].x;
        int y = laser_y + steps[i].y;
        buffer[y*width + x] = i+1;
    }

    i = 0;
    printf("\n");
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int b = buffer[i++];
            if (b)
                printf("%4d", b);
            else
                printf("    ");
        }
        printf("\n");
    }
    free(buffer);

    int half_rounds = 0;

    int step_i = 0;
    while (asteroids_vaporized < 200 && half_rounds < 2000)
    {
        int step_x, step_y;
        step_x = steps[step_i].x;
        step_y = steps[step_i].y;
        //get_step(target_x, target_y, laser_x, laser_y, &step_x, &step_y);
        int beam_x = laser_x + step_x;
        int beam_y = laser_y + step_y;
        while ((0 <= beam_x && beam_x < width) && (0 <= beam_y && beam_y < height))
        {
            if (asteroids[beam_y*width + beam_x] == '#')
            {
                asteroids[beam_y*width + beam_x] = '.';
                vaporized_x = beam_x;
                vaporized_y = beam_y;
                asteroids_vaporized++;
                printf("%d. asteroid vaporized at (%d, %d)\n", asteroids_vaporized, vaporized_x, vaporized_y);
                fflush(stdout);
                print_asteroids(asteroids, width, height, laser_x, laser_y, beam_x, beam_y);
                break;
            }
            beam_x += step_x;
            beam_y += step_y;
        }

        step_i++;
        if (step_i == step_n)
        {
            half_rounds++;
            step_i = 0;
        }
    }

    if (asteroids_vaporized == 200)
    {
        printf("200th vaporized asteroid was at (%d, %d) (%d)\n", vaporized_x, vaporized_y, half_rounds);
    }
    else
    {
        printf("Did not find 200 asteroids to vaporize :(\n");
    }

    free(asteroids);
}


namespace sample01
{
    const char *data = ".#..#"
                       "....."
                       "#####"
                       "....#"
                       "...##";
    int width = 5;
    int height = 5;

    void test() { find_best(data, width, height); }
}

namespace sample02
{
    const char *data = "......#.#."
                       "#..#.#...."
                       "..#######."
                       ".#.#.###.."
                       ".#..#....."
                       "..#....#.#"
                       "#..#....#."
                       ".##.#..###"
                       "##...#..#."
                       ".#....####";
    int width = 10;
    int height = 10;

    void test() { find_best(data, width, height); }
}

namespace sample03
{
    const char *data = "#.#...#.#."
                       ".###....#."
                       ".#....#..."
                       "##.#.#.#.#"
                       "....#.#.#."
                       ".##..###.#"
                       "..#...##.."
                       "..##....##"
                       "......#..."
                       ".####.###.";
    int width = 10;
    int height = 10;

    void test() { find_best(data, width, height); }
}

namespace sample04
{
    const char *data = ".#..#..###"
                       "####.###.#"
                       "....###.#."
                       "..###.##.#"
                       "##.##.#.#."
                       "....###..#"
                       "..#.#..#.#"
                       "#..#.#.###"
                       ".##...##.#"
                       ".....#.#..";
    int width = 10;
    int height = 10;

    void test() { find_best(data, width, height); }
}

namespace sample05
{
    const char *data = ".#..##.###...#######"
                       "##.############..##."
                       ".#.######.########.#"
                       ".###.#######.####.#."
                       "#####.##.#.##.###.##"
                       "..#####..#.#########"
                       "####################"
                       "#.####....###.#.#.##"
                       "##.#################"
                       "#####.##.###..####.."
                       "..######..##.#######"
                       "####.##.####...##..#"
                       ".#####..#.######.###"
                       "##...#.##########..."
                       "#.##########.#######"
                       ".####.#.###.###.#.##"
                       "....##.##.###..#####"
                       ".#.#.###########.###"
                       "#.#.#.#####.####.###"
                       "###.##.####.##.#..##";
    int width = 20;
    int height = 20;

    void test()
    {
        int laser_x = 0, laser_y = 0;
        find_best(data, width, height, &laser_x, &laser_y);
        vaporize(data, width, height, laser_x, laser_y);
    }
}

namespace sample06
{
    const char *data = ".#....#####...#.."
                       "##...##.#####..##"
                       "##...#...#.#####."
                       "..#.....X...###.."
                       "..#.#.....#....##";
    int width = 17;
    int height = 5;
    void test()
    {
        int laser_x = 0, laser_y = 0;
        find_best(data, width, height, &laser_x, &laser_y);
        vaporize(data, width, height, laser_x, laser_y);
    }
}

namespace puzzle
{
    const char *asteroid_map = "#..#.#.#.######..#.#...##"
                               "##.#..#.#..##.#..######.#"
                               ".#.##.#..##..#.#.####.#.."
                               ".#..##.#.#..#.#...#...#.#"
                               "#...###.##.##..##...#..#."
                               "##..#.#.#.###...#.##..#.#"
                               "###.###.#.##.##....#####."
                               ".#####.#.#...#..#####..#."
                               ".#.##...#.#...#####.##..."
                               "######.#..##.#..#.#.#...."
                               "###.##.#######....##.#..#"
                               ".####.##..#.##.#.#.##...#"
                               "##...##.######..##..#.###"
                               "...###...#..#...#.###..#."
                               ".#####...##..#..#####.###"
                               ".#####..#.#######.###.##."
                               "#...###.####.##.##.#.##.#"
                               ".#.#.#.#.#.##.#..#.#..###"
                               "##.#.####.###....###..##."
                               "#..##.#....#..#..#.#..#.#"
                               "##..#..#...#..##..####..#"
                               "....#.....##..#.##.#...##"
                               ".##..#.#..##..##.#..##..#"
                               ".##..#####....#####.#.#.#"
                               "#..#..#..##...#..#.#.#.##";

    int width = 25;
    int height = 25;

    void part_one()
    {
        find_best(asteroid_map, width, height);
    }

    void part_two()
    {
        int laser_x = 0, laser_y = 0;
        find_best(asteroid_map, width, height, &laser_x, &laser_y);
        vaporize(asteroid_map, width, height, laser_x, laser_y);
    }
}

int main()
{
    //sample01::test();
    //sample02::test();
    //sample03::test();
    //sample04::test();
    //sample05::test();
    //sample06::test();
    //puzzle::part_one();
    puzzle::part_two();
    return 0;
}
