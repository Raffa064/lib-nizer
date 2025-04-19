#define EXPAND(x) x
#define JOIN(a, b, c) a##b##c
#define UNAME(a, b) EXPAND(JOIN(a, _, b))
