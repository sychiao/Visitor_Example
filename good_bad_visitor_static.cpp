#include <iostream>
#include <list>

using namespace std;

struct visitor{
  virtual bool good() = 0;
  virtual bool apple() = 0;
  virtual bool cat() = 0;
  virtual bool smoke() = 0;
  virtual bool bad() = 0;
};

template<typename T>
struct visitor_CRTP: public visitor{
  bool good(){
      return static_cast<T*>(this)->good();
  }
  bool bad(){
      return static_cast<T*>(this)->bad();
  }
};

struct object{
    virtual bool accept(visitor *V) = 0;
};

template<typename T>
struct object_CRTP: public object {
    bool accept(visitor *V){
    	static_cast<T*>(this)->accept(V);
    }
};

struct good : object_CRTP<good>{
   bool accept(visitor *V){
   	V->good();
   }
};

struct bad : object_CRTP<bad>{
   bool accept(visitor *V){
   	V->bad();
   }
};

struct apple : object_CRTP<apple>{
   bool accept(visitor *V){V->apple();}
};

struct cat : object_CRTP<cat>{
   bool accept(visitor *V){V->cat();}
};

struct smoke : object_CRTP<smoke>{
   bool accept(visitor *V){V->smoke();}
};

struct isgood : visitor_CRTP<isgood>{
  bool good() {return true;}
  bool bad()  {return false;}
  bool apple()  {return true;}
  bool cat()  {return true;}
  bool smoke()  {return false;}
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

struct life : public visitor_CRTP<life>{
  int score = 0;
  bool good()  override {return true;}
  bool bad()   override {return false;}
  bool apple() override {score+=1;return true;}
  bool cat()   override {score+=2;return true;}
  bool smoke() override {score+=1;return false;}
};

struct health : public visitor_CRTP<health>{
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
  //std::cout << lengood(lst);
  std::cout << life_score(lst) << " " << health_score(lst) << " " <<  lengood(lst) << "\n";
 }
 return 0; 
}
