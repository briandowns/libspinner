#define ATTR_NORMAL         0
#define ATTR_BOLD           1
#define ATTR_UNDERLINE      4
#define ATTR_BLINK          5
#define ATTR_REVERSE_VID    7
#define ATTR_REVERSE_NONDIS 8

#define FG_BLACK   30
#define FG_RED     31
#define FG_GREEN   32
#define FG_YELLOW  33
#define FG_BLUE    34
#define FG_MAGENTA 35
#define FG_CYAN    36
#define FG_WHITE   37

#define BK_BLACK   40
#define BK_RED     41
#define BK_GREEN   42
#define BK_YELLOW  43
#define BK_BLUE    44
#define BK_MAGENTA 45
#define BK_CYAN    46
#define BK_WHITE   47

#define BLACK_STRING(s)                                        \
    sprintf(s, "\033;%d;%s\033;%d", FG_BLACK, s, ATTR_NORMAL); \

#define PRINT_BLACK(s)                                     \
    printf("\033;%d;%s\033;%d", FG_BLACK, s, ATTR_NORMAL); \
