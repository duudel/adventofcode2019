// 776996-20191205-0a4a7f2b

#include <cstdio>
#include <cstdint>
#include <cstdlib>

enum Direction
{
    DIR_Right,
    DIR_Down,
    DIR_Left,
    DIR_Up,
};

struct PathSpans
{
    Direction *direction;
    int *length;

    int size;
};

struct PathSegment
{
    int x0, y0;
    int x1, y1;
};

template <int N>
PathSpans parse(const char *(&strings)[N])
{
    Direction *dirs = (Direction*)malloc(sizeof(Direction)*N);
    int *lens = (int*)malloc(sizeof(int)*N);
    for (int i = 0; i < N; i++)
    {
        const char *s = strings[i];
        switch (s[0])
        {
            case 'R': dirs[i] = DIR_Right; break;
            case 'D': dirs[i] = DIR_Down; break;
            case 'L': dirs[i] = DIR_Left; break;
            case 'U': dirs[i] = DIR_Up; break;
        }
        lens[i] = atoi(s + 1);
    }
    return PathSpans {
        .direction = dirs,
        .length = lens,
        .size = N
    };
};

namespace sample01
{
    const char* path1[] = {
        "R75","D30","R83","U83","L12","D49","R71","U7","L72"
    };
    const char* path2[] = {
        "U62","R66","U55","R34","D71","R55","D58","R83"
    };
}

namespace sample02
{
    const char* path1[] = {
        "R98","U47","R26","D63","R33","U87","L62","D20","R33","U53","R51"
    };
    const char* path2[] = {
        "U98","R91","D20","R16","D67","R40","U7","R15","U6","R7"
    };
}

namespace input
{
    const char *path1[] = {
"R1000","D940","L143","D182","L877","D709","L253","U248","L301","U434","R841","U715","R701","U92","R284","U115","R223","U702","R969","U184","L992","U47","L183","U474","L437","D769","L71","U96","R14","U503","R144","U432","R948","U96","L118","D696","R684","U539","L47","D851","L943","U606","L109","D884","R157","U946","R75","U702","L414","U347","R98","D517","L963","D177","R467","D142","L845","U427","R357","D528","L836","D222","L328","U504","R237","U99","L192","D147","L544","D466","R765","U845","L267","D217","L138","U182","R226","U466","R785","U989","R55","D822","L101","U292","R78","U962","R918","U218","L619","D324","L467","U885","L658","U890","L764","D747","R369","D930","L264","D916","L696","U698","R143","U537","L922","U131","R141","D97","L76","D883","R75","D657","R859","U503","R399","U33","L510","D318","L455","U128","R146","D645","L147","D651","L388","D338","L998","U321","L982","U150","R123","U834","R913","D200","L455","D479","L38","U860","L471","U945","L946","D365","L377","U816","R988","D597","R181","D253","R744","U472","L345","U495","L187","D443","R924","D536","R847","U430","L145","D827","L152","D831","L886","D597","R699","D751","R638","D580","L488","D566","L717","D220","L965","D587","L638","D880","L475","D165","L899","U388","R326","D568","R940","U550","R788","D76","L189","D641","R629","D383","L272","D840","L441","D709","L424","U158","L831","D576","R96","D401","R425","U525","L378","D907","L645","U609","L336","D232","L259","D280","L523","U938","R190","D9","L284","U941","L254","D657","R572","U443","L850","U508","L742","D661","L977","U910","L190","U626","R140","U762","L673","U741","R317","D518","R111","U28","R598","D403","R465","D684","R79","U725","L556","U302","L367","U306","R632","D550","R89","D292","R561","D84","L923","D109","L865","D880","L387","D24","R99","U934","L41","U29","L225","D12","L818","U696","R652","U327","L69","D773","L618","U803","L433","D467","R840","D281","R161","D400","R266","D67","L205","D94","R551","U332","R938","D759","L437","D515","L480","U774","L373","U478","R963","D863","L735","U138","L580","U72","L770","U968","L594"
    };
    const char *path2[] = {
"L990","D248","L833","U137","L556","U943","R599","U481","R963","U812","L825","U421","R998","D847","R377","D19","R588","D657","R197","D354","L548","U849","R30","D209","L745","U594","L168","U5","L357","D135","R94","D686","R965","U838","R192","U428","L861","U354","R653","U543","L633","D508","R655","U575","R709","D53","L801","D709","L92","U289","L466","D875","R75","D448","R576","D972","L77","U4","L267","D727","L3","D687","R743","D830","L803","D537","L180","U644","L204","U407","R866","U886","R560","D848","R507","U470","R38","D652","R806","D283","L836","D629","R347","D679","R609","D224","L131","D616","L687","U181","R539","D829","L598","D55","L806","U208","R886","U794","L268","D365","L145","U690","R50","D698","L140","D512","L551","U845","R351","U724","R405","D245","L324","U181","L824","U351","R223","D360","L687","D640","L653","U158","R786","D962","R931","D151","R939","D34","R610","U684","L694","D283","R402","D253","R388","D195","R732","U809","R246","D571","L820","U742","L507","U967","L886","D693","L273","U558","L914","D122","R146","U788","R83","U149","R241","U616","R326","U40","L192","D845","L577","U803","L668","D443","R705","D793","R443","D883","L715","U757","R767","D360","L289","D756","R696","D236","L525","U872","L332","U203","L152","D234","R559","U191","R340","U926","L746","D128","R867","D562","L100","U445","L489","D814","R921","D286","L378","D956","L36","D998","R158","D611","L493","U542","R932","U957","R55","D608","R790","D388","R414","U670","R845","D394","L572","D612","R842","U792","R959","U7","L285","U769","L410","D940","L319","D182","R42","D774","R758","D457","R10","U82","L861","D901","L310","D217","R644","U305","R92","U339","R252","U460","R609","D486","R553","D798","R809","U552","L183","D238","R138","D147","L343","D597","L670","U237","L878","U872","R789","U268","L97","D313","R22","U343","R907","D646","L36","D516","L808","U622","L927","D982","L810","D149","R390","U101","L565","U488","L588","U426","L386","U305","R503","U227","R969","U201","L698","D850","R800","D961","R387","U632","R543","D541","R750","D174","R543","D237","R487","D932","R220"
    };
}

void print_path(PathSpans p)
{
    for (int i = 0; i < p.size; i++)
    {
        switch (p.direction[i])
        {
            case DIR_Right: printf("R"); break;
            case DIR_Down: printf("D"); break;
            case DIR_Left: printf("L"); break;
            case DIR_Up: printf("U"); break;
        }
        printf("-%d,", p.length[i]);
    }
    printf("\n");
}

PathSegment* to_segments(PathSpans p, int &segment_num)
{
    segment_num = p.size;
    PathSegment *segments = (PathSegment*)malloc(sizeof(PathSegment) * p.size);
    int x = 0, y = 0;
    for (int i = 0; i < p.size; i++)
    {
        PathSegment *s = &segments[i];
        s->x0 = x;
        s->y0 = y;
        int len = p.length[i];
        switch (p.direction[i])
        {
            case DIR_Right: x += len; break;
            case DIR_Down: y -= len; break;
            case DIR_Left: x -= len; break;
            case DIR_Up: y += len; break;
        }
        s->x1 = x;
        s->y1 = y;
    }
    return segments;
}

int min(int a, int b)
{
    return (a < b) ? a : b;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

bool vertical(PathSegment s1)
{
    return s1.x0 == s1.x1;
}

bool horizontal(PathSegment s1)
{
    return s1.y0 == s1.y1;
}

bool segments_intersect(PathSegment s1, PathSegment s2, int *hit_x, int *hit_y)
{
    bool can_have_intersection = false;
    PathSegment v;
    PathSegment h;
    if (vertical(s1) && horizontal(s2))
    {
        v = s1;
        h = s2;
        can_have_intersection = true;
    }
    else if (vertical(s2) && horizontal(s1))
    {
        v = s2;
        h = s1;
        can_have_intersection = true;
    }

    if (can_have_intersection)
    {
        int x = v.x0;
        int y = h.y0;

        int y0 = min(v.y0, v.y1);
        int y1 = max(v.y0, v.y1);

        int x0 = min(h.x0, h.x1);
        int x1 = max(h.x0, h.x1);

        if ((y0 <= y && y <= y1) &&
            (x0 <= x && x <= x1))
        {
            *hit_x = x;
            *hit_y = y;
            return true;
        }
    }
    return false;
}

int manhattan(int x, int y)
{
    return abs(x) + abs(y);
}

int get_closest_intersection(PathSpans p1, PathSpans p2)
{
    print_path(p1);
    print_path(p2);
    int s1_num = 0, s2_num = 0;
    PathSegment* segs1 = to_segments(p1, s1_num);
    PathSegment* segs2 = to_segments(p2, s2_num);

    int closest_distance = INT_MAX;
    for (int n1 = 0; n1 < s1_num; n1++)
    {
        PathSegment s1 = segs1[n1];
        for (int n2 = 0; n2 < s2_num; n2++)
        {
            PathSegment s2 = segs2[n2];
            int x, y;
            if (segments_intersect(s1, s2, &x, &y))
            {
                int len = manhattan(x, y);
                if ((len > 0) && (len < closest_distance))
                {
                    closest_distance = len;
                }
            }
        }
    }
    free(segs1);
    free(segs2);
    return closest_distance;
}

void test(PathSpans path1, PathSpans path2, int expected_result)
{
    int result = get_closest_intersection(path1, path2);
    printf("Got %d, expected %d\n", result, expected_result);
}

void test01()
{
    PathSpans path1 = parse(sample01::path1);
    PathSpans path2 = parse(sample01::path2);
    test(path1, path2, 159);
}

void test02()
{
    PathSpans path1 = parse(sample02::path1);
    PathSpans path2 = parse(sample02::path2);
    test(path1, path2, 135);
}

void test03()
{
    PathSpans path1 = parse(input::path1);
    PathSpans path2 = parse(input::path2);
    test(path1, path2, 0);
}

int segment_length(PathSegment s)
{
    return manhattan(s.x1 - s.x0, s.y1 - s.y0);
}

int solve_steps(PathSpans path1, PathSpans path2)
{
    print_path(path1);
    print_path(path2);
    int s1_num = 0, s2_num = 0;
    PathSegment* segs1 = to_segments(path1, s1_num);
    PathSegment* segs2 = to_segments(path2, s2_num);

    int least_steps = INT_MAX;
    int steps1 = 0;
    for (int n1 = 0; n1 < s1_num; n1++)
    {
        PathSegment s1 = segs1[n1];

        int steps2 = 0;
        for (int n2 = 0; n2 < s2_num; n2++)
        {
            PathSegment s2 = segs2[n2];
            int x, y;
            if (segments_intersect(s1, s2, &x, &y))
            {
                int steps_to_seg1 = manhattan(s1.x0 - x, s1.y0 - y);
                int steps_to_seg2 = manhattan(s2.x0 - x, s2.y0 - y);
                int steps = steps1 + steps2 + steps_to_seg1 + steps_to_seg2;
                if ((steps > 0) && (steps < least_steps))
                {
                    least_steps = steps;
                }
            }
            steps2 += segment_length(s2);
        }
        steps1 += segment_length(s1);
    }
    free(segs1);
    free(segs2);
    return least_steps;
}

void test_steps(PathSpans path1, PathSpans path2, int expected_result)
{
    int result = solve_steps(path1, path2);
    printf("Got %d, expected %d\n", result, expected_result);
}

void solve_sample01()
{
    PathSpans path1 = parse(sample01::path1);
    PathSpans path2 = parse(sample01::path2);
    test_steps(path1, path2, 610);
}

void solve_sample02()
{
    PathSpans path1 = parse(sample02::path1);
    PathSpans path2 = parse(sample02::path2);
    test_steps(path1, path2, 410);
}

void solve_input()
{
    PathSpans path1 = parse(input::path1);
    PathSpans path2 = parse(input::path2);
    test_steps(path1, path2, 0);
}

int main(int argc, const char **argv)
{
    // Part one
    //test01();
    //test02();
    //test03();
    // Part two
    solve_sample01();
    solve_sample02();
    solve_input();
    return 0;
}

