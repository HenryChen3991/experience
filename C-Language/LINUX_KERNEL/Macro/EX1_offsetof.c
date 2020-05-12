#include <stdio.h>

#define offsetof(TYPE, MEMBER)  ((size_t)&((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({                              \
                const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
                (type *)( (char *)__mptr - offsetof(type,member) );})

typedef struct foo_s {
        int             m_int;
        short           m_short;
	double          m_double;
        long long       m_longlong;
        char            m_char;
} foo_t;

int
main(int argc, char *argv[])
{
        foo_t ox = {0x12345678, 0x1234, 1234.1234 ,0xfedcba9876543210, 'c'};
        char *p3 = &ox.m_char;
        foo_t *p = container_of(p3, foo_t, m_char);

        printf("foo_t ox (%p) sizeof(foo_t) = %d\n", &ox, sizeof (foo_t));
        printf("foo_t *p (%p) p3(%p)\n", p, p3);
        printf("foo_t->m_int      = %#x\t\t(%d)(%p)\n",
            p->m_int, offsetof(foo_t, m_int), &ox.m_int);
        printf("foo_t->m_short    = %#x\t\t(%d)(%p)\n",
            p->m_short, offsetof(foo_t, m_short), &ox.m_short);
        printf("foo_t->m_char     = %c\t\t\t(%d)(%p)\n",
            p->m_char, offsetof(foo_t, m_char), &ox.m_char);
        printf("foo_t->m_double   = %#g\t\t(%d)(%p)\n",
            p->m_double, offsetof(foo_t, m_double), &ox.m_double);
        printf("foo_t->m_longlong = %#llx\t(%d)(%p)\n",
            p->m_longlong, offsetof(foo_t, m_longlong), &ox.m_longlong);

        return 0;
}
