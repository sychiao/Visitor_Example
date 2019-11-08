#include <iostream>
#include <list>

using namespace std;

struct object{
    virtual bool isgood() { return false;}
    virtual int score() {return 0;}
    virtual int health() {return 0;}
};

struct good : object{
  bool isgood(){return true;}
};
struct bad : object{};

struct apple : good{
    int score() {return 1;}
    int health() {return 2;}
};
struct cat : good{
    int score() {return 2;}
    int health() {return -1;}
};
struct smoke : bad{
    int score() {return 1;}
    int health() {return -2;}
};

int lengood(list<object*> lst){
  int count = 0;
  for(auto e : lst){
    if(e->isgood())
      count++;
  }
  return count;
}

int life_score(list<object*> lst){
    int score = 0;
    for(auto e : lst){
        score += e->score();
    }
    return score;
}

int health_score(list<object*> lst){
    int score = 0;
    for(auto e : lst){
        score += e->health();
    }
    return score;
}

int main(){
 for(int i=0;i<100000;i++){
  list<object*> lst = {
          new apple(),new cat(),new apple(),new smoke(),new cat(),new cat(),new smoke(),
          new apple(),new cat(),new apple(),new smoke(),new cat(),new cat(),new smoke(),
          new apple(),new cat(),new apple(),new smoke(),new cat(),new cat(),new smoke()
  };
  std::cout << life_score(lst) << " " << health_score(lst) << " " <<  lengood(lst);
 }
 return 0;
}


