#include <iostream>
#include <list>

using namespace std;

class Visitor{
public:
  virtual bool good() = 0 ;
  virtual bool bad() {return false;}
  virtual bool apple()  {return false;}
  virtual bool cat()  {return false;}
  virtual bool smoke()  {return false;}
};


struct object{
    virtual bool accept(Visitor *V) = 0;
};


struct good : object{
  bool accept(Visitor *V) {return V->good();}
};
struct bad : object{
  bool accept(Visitor *V) {return V->bad();}
};

struct apple : good{
  bool accept(Visitor *V) {return V->apple();}
};
struct cat : good{
  bool accept(Visitor *V) {return V->cat();}
};
struct smoke : bad{
  bool accept(Visitor *V) {return V->smoke();}
};

struct isgood : public Visitor{
  bool good() override {return true;}
  bool bad()  override {return false;}
  bool apple()override {return true;}
  bool cat()  override {return true;}
  bool smoke()override {return false;}
};

int lengood(list<object*> lst){
    int score = 0;
    isgood *V = new isgood();
    for(auto e : lst){
      if(e->accept(V))
        score += 1;
    }
    return score;

}

struct life : public Visitor{
  int score = 0;
  bool good()  override {return true;}
  bool bad()   override {return false;}
  bool apple() override {score+=1;return true;}
  bool cat()   override {score+=2;return true;}
  bool smoke() override {score+=1;return false;}
};

struct health : public Visitor{
  int score = 0;
  bool good()  override {return true;}
  bool bad()   override {return false;}
  bool apple() override {score+=2;return true;}
  bool cat()   override {score+=-1;return true;}
  bool smoke() override {score+=-2;return false;}
};


int life_score(list<object*> lst){
    life *V = new life();
    for(auto e : lst){
      e->accept(V);
    }
    return V->score;
}
int health_score(list<object*> lst){
    health *V = new health();
    for(auto e : lst){
      e->accept(V);
    }
    return V->score;
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
