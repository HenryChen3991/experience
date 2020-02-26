
#include <stdio.h>
#include <stdlib.h>

typedef struct Ball {
    char *color;
    double radius;
    double (*volumn)(struct Ball*); // 函式指標
} Ball;

// 計算實例體積
double volumn(Ball *this) {
    double r = this->radius; // 模擬 this 指標 的行為
    return r * r * 3.14;
}

Ball* new_ball(char *color, double radius) {
    Ball *ball = (Ball*) malloc(sizeof(Ball));
    ball->volumn = volumn; // 連結函式
    ball->color = color;
    ball->radius = radius;
    return ball;
}

//using double pointer
void nBall(char *color,double radius,Ball **_ball){
    Ball *ball = (Ball*) malloc(sizeof(Ball));
    ball->volumn = volumn; // 連結函式
    ball->color = color;
    ball->radius = radius;
    
    *_ball = ball;
}


int main(void) {
    //Ball *ball = new_ball("red", 5.0);
    Ball *ball = NULL;
    nBall("red",5.0,&ball);
    printf("ball 實例的體積: %.2f\n", ball->volumn(ball));
    Ball *ptr2 = ball;
    nBall("blue",6.0,&ball);
    printf("ball 實例的體積: %.2f\n", ball->volumn(ball));
    printf("ball 實例的體積: %.2f\n", ptr2->volumn(ptr2));

    return 0;
}
