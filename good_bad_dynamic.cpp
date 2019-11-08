#include <iostream>
#include <list>

using namespace std;

struct object{
    virtual bool type() { return false;}
};

struct good : object{
    virtual bool type() { return false;}
};
struct bad : object{
    virtual bool type() { return false;}
};

struct apple : good{
    virtual bool type() { return false;}
};
struct cat : good{
    virtual bool type() { return false;}
};
struct smoke : bad{
    virtual bool type() { return false;}
};

int lengood(list<object*> lst){
    int score = 0;
    for(auto e : lst){
      if(auto *ce = dynamic_cast<good*>(e))
        score += 1;
    }
    return score;

}

int life_score(list<object*> lst){
    int score = 0;
    for(auto e : lst){
      if(auto *ce = dynamic_cast<apple*>(e))
        score += 1;
      else if(auto *ce = dynamic_cast<cat*>(e))
        score += 2;
      else if(auto *ce = dynamic_cast<smoke*>(e))
        score += 1;
    }
    return score;
}

int health_score(list<object*> lst){
    int score = 0;
    for(auto e : lst){
      if(auto *ce = dynamic_cast<apple*>(e))
        score += 2;
      else if(auto *ce = dynamic_cast<cat*>(e))
        score += -1;
      else if(auto *ce = dynamic_cast<smoke*>(e))
        score += -2;
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


