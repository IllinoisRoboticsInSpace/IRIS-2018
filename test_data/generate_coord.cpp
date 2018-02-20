#include<iostream>
#include<string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

struct coord{
  int x=0;
  int y=0;
  double angle =180;


};

int main(){
  srand(time(NULL));
  ofstream file;
  file.open("Coordinates.txt");
  coord map[1000];

  for(int i = 1;i<1000;i++){
    map[i].x = map[i-1].x + (rand()%2) - (rand()%2);
    if(map[i].x<0)
      map[i].x=0;
    map[i].y = map[i-1].y + (rand()%(2)) - (rand()%2);
    if(map[i].y<0)
      map[i].y=0;
    map[i].angle = (int)(map[i-1].angle + (double)(rand()%(5)) - (double)(rand()%(5)))%360;
    if(map[i].angle < 0)
      map[i].angle+=360;
  }
  for(int i = 0;i<999;i++){
    file << "(" << map[i].x << ", " << map[i].y << ", " << map[i].angle << "), ";
  }

  file << "(" << map[999].x << ", " << map[999].y << ", " << map[999].angle << ")\n";
  file.close();
  return 0;
}
