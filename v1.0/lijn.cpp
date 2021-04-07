#include "lijn.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Lijn::Lijn(){
    Lijn::p1 = Point2f(NULL,NULL);
    Lijn::p2 = Point2f(NULL,NULL);
}
Lijn::Lijn(Point2f p1, Point2f p2){
    Lijn::p1 = p1;
     Lijn::p2 = p2;
}

void Lijn::getLijn(Point2f *p1, Point2f *p2){
        *p1 = Lijn::p1;
        *p2 = Lijn::p2;
}

Point2f Lijn::getP1(){
        return p1;
    }

Point2f Lijn::getP2(){
    return p2;
}

float Lijn::getLengte(){
    return abs(sqrt(pow(p2.x - p1.x,2)+pow(p2.y-p1.y,2)));
}

float Lijn::getLengteX(){
    return abs(sqrt(pow(Lijn::getLengte(),2)-pow(p2.y-p1.y,2)));
}

float Lijn::getLengteY(){
    return abs(sqrt(pow(Lijn::getLengte(),2)-pow(p2.x-p1.x,2)));
}

Point2f Lijn::getMidPunt(){
    float x,y;
    if(p1.x >= p2.x){
        x=p2.x + Lijn::getLengteX()/2;
    }else{
        x = p1.x+ Lijn::getLengteX()/2;
    }

    if (p1.y >= p2.y){
        y = p2.y+ Lijn::getLengteY()/2;
    }else{
        y = p1.y + Lijn::getLengteY()/2;
    }

    return Point2f(x,y);
    
}

bool Lijn::isEmpty(){
    if (p1.x == NULL){
        return true;
    }
    return false;
}

float Lijn::getHoekTovXAs(bool rad){
    float x,y;
    x = p2.x-p1.x;
    y = p2.y-p1.y;
    if (rad == true){
        return atan2(y,x);
    }
    return atan2(y,x)*(180/M_PI);
}

void Lijn::printLijn(){
    cout << "punt1: ";
}


