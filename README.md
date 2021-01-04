# Steps
```c
flex our_c.l

bison -d our_c.y

gcc lex.yy.c our_c.tab.c
```