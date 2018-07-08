#include <stdio.h>

typedef struct {
    float x;
    float y;
} point;

int hello(int x){
    int i = 0;
    point p;
    p.x = 2;

    do{
        puts("Hello, world!");
        i++;
    }while(i < x);
    return 0;
}


int main(int argc, char** argv){
    hello(7);
}