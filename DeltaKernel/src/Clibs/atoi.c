int atoi(const char *str);
long atol(const char *str);
void itoa(int val, char *buf, int base);
void utoa(int val, char *buf, int base);
int isalnum(int c);
int isalpha(int c);
#define isascii(c)  ((c) < 128)
int isblank(int c);
int isdigit(int c);
int isupper(int c);
int islower(int c);
int isprint(int c);
int isspace(int c);

int toupper(int c);
int tolower(int c);


int isalnum(int c)
{
    return isalpha(c) || isdigit(c);
}

int isalpha(int c)
{
    return isupper(c) || islower(c);
}

int isblank(int c)
{
    return c == ' ' || c == '\t';
}

int isdigit(int c)
{
    return c >= '0' && c <= '9';
}

int isupper(int c)
{
    return c >= 'A' && c <= 'Z';
}

int islower(int c)
{
    return c >= 'a' && c <= 'z';
}

int isgraph(int c)
{
    return c > ' ' && c <= '~';
}

int isprint(int c)
{
    return isgraph(c) || c == ' ';
}

int isspace(int c)
{
    return isblank(c) || c == '\f' || c == '\n' || c == '\r' || c == '\v';
}

int toupper(int c)
{
    return islower(c) ? (c + ('A' - 'a')) : c;
}

int tolower(int c)
{
    return isupper(c) ? (c + ('A' - 'a')) : c;
}
int atoi(const char *str) {
    return atol(str);
}

long atol(const char *str) {
    long acc = 0;
    int sign = 1;

    while(isspace(*str)) str++;

    if(*str == '+') {
        str++;
    } 
    else if(*str == '-') {
        sign = -1;
        str++;
    }

    while(*str && isdigit(*str)) {
        acc = acc * 10 + (*str - '0');
        str++;
    }

    return sign > 0 ? acc : -acc;
}

void itoa(int val, char *buf, int base) {
    static char rbuf[16];
    char *ptr = rbuf;
    int neg = 0;

    if(val < 0) {
        neg = 1;
        val = -val;
    }

    if(val == 0) {
        *ptr++ = '0';
    }

    while(val) {
        int digit = (int)((long)val % (long)base);
        *ptr++ = digit < 10 ? (digit + '0') : (digit - 10 + 'a');
        val /= base;
    }

    if(neg) {
        *ptr++ = '-';
    }

    ptr--;

    while(ptr >= rbuf) {
        *buf++ = *ptr--;
    }

    *buf = 0;
}

// In the previous version it was 'unsigned int val'
void utoa(int val, char *buf, int base) {
    static char rbuf[16];
    char *ptr = rbuf;

    if(val == 0) {
        *ptr++ = '0';
    }

    while(val) {
        int digit = (int)((long)val % (long)base);
        *ptr++ = digit < 10 ? (digit + '0') : (digit - 10 + 'a');
        val /= base;
    }

    ptr--;

    while(ptr >= rbuf) {
        *buf++ = *ptr--;
    }

    *buf = 0;
}