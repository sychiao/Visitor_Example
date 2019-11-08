#include <iostream>

using namespace std;

struct Expr{
  virtual std::string type() const = 0;
};

struct Val : Expr{
  std::string type() const override {return "Val";}
  Val(int a):val(a){}
  int val;
};


struct Add : Expr{
  std::string type() const override {return "Add";}
  Add(Expr *l,Expr *r):lhs(l),rhs(r){}
  struct Expr *lhs, *rhs;
};


struct Mul : Expr{
  std::string type() const override {return "Mul";}
  Mul(Expr *l,Expr *r):lhs(l),rhs(r){}
  struct Expr *lhs,*rhs;
};

int calc(Expr *e){
    if(Val *a = dynamic_cast<Val*>(e)){
        return a->val;
    }else if(Add *a = dynamic_cast<Add*>(e)){
        return calc(a->lhs) + calc(a->rhs);
    }else if(Mul *a = dynamic_cast<Mul*>(e)){
        return calc(a->lhs) * calc(a->rhs);
    }else{
      return 0;
    }
}

int main(){
  Add *e = new Add(new Val(2),new Val(15));
  int a = calc(e);
  cout << "Ans:" << a << endl;
  return 0;
}
