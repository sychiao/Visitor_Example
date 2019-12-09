# Visitor 設計範式

###### tags: `Blog`

[TOC]

## 問題情境
我們總是說程式就是資料結構加上演算法，我們幾乎到處都可以看到這樣的蹤跡，刷Leetcode時，起手勢肯定是先查一個STL的資料結構，可能是stack, list, 或是 map。然後才開始寫題目。如果是C++的使用者，這些資料結構的複雜度與使用場景也十分的重要。

在更實際的專案裡，我們可能會自己實作一些資料結構，然而，當我們要實作這組資料結構的行為的時候，我們應該怎麼辦呢？例如說：

```cpp
class object{
}

class good : object{
}

class bad : object{
}
```

這是一個很簡單的例子，如果說我們需要寫一個計算good object數量的函數該怎麼辦呢？我們可以準備一個`lengood`


```cpp
int lengood(list<object*> lst){
    int count = 0;
    for(auto e : lst){
        if(e->isgood()){
            count++;
        }
    }
    return count;
}
```

為了提供這個功能，我們必須替object準備isgood函數

```cpp
class object{
    virtual bool isgood() { return false;}
}

class good : object{
    bool isgood(){return true;}
}

class bad : object{
    bool isgood(){return false;}
}
```

看起來還不錯，不過，什麼是好東西？蘋果顯然是好東西，一天一蘋果，醫生遠離我，貓咪可愛又討喜，也是好東西，所以我定義了`apple`與`class`。抽煙得肺癌，所`smoke`是壞東西。所以我們有以下的`class`，問題不大，透過OO一切都很好。但接下來可能就不是這麼順利了。

```
class apple : good{

}
class cat : good{

}
class smoke : bad{

}
```

我們可能需要一個生活快樂函數`life_score`，遇到apple+1，cat+2,smoke+1。如法炮製，我們加上一個`score`

```cpp
class object{
    virtual bool isgood() { return false;}
    virtual int score() {return 0;}
}

class apple : good{
    int score() {return 1;}
}
class cat : good{
    int score() {return 2;}
}
class smoke : bad{
    int score() {return 1;}
}

int life_scroe(list<object*> lst){
    int score = 0;
    for(auto e : lst){
        score += e->score();
    }
    return score;
}
```

這樣顯然還是不夠的，我們還需要生活健康函數，這個時候讓人過敏的貓咪與香菸都應該扣分，所以我們有`health_score`

```cpp
class object{
    virtual bool isgood() { return false;}
    virtual int score() {return 0;}
    virtual int health() {return 0;}
}

class apple : good{
    int score() {return 1;}
    int health() {return 2;}
}
class cat : good{
    int score() {return 2;}
    int health() {return -1;}
}
class smoke : bad{
    int score() {return 1;}
    int health() {return -2;}
}

int life_scroe(list<object*> lst){
    int score = 0;
    for(auto e : lst){
        score += e->score();
    }
    return score;
}

int health_scroe(list<object*> lst){
    int score = 0;
    for(auto e : lst){
        score += e->health();
    }
    return score;
}
```
OK，接下來我們來做code review，這隻程式的命名顯然有點問題，`score`看起不知道是什麼（是life score呢）。所以，我們應該調整一下命名，改成跟`health`一樣的`life`似乎更好。不幸的是，所有的`score`分散再不同的class，最糟糕的狀況是，分散再不同的檔案，如果你沒有強大的IDE支援，估計你必須要撞幾次編譯錯誤才搞得定了。

## 問題分析
這裡最大的問題在於，沒想到會有另外一個跟分數有關的功能，貿然的做了命名。不過這也不是我們的問題對吧，需求不是一次寫清楚的，這種失誤理所當然。不過，在工程當中這都是很常見的。
另外一個問題，我們每次在新增功能，似乎都需要修改資料結構，這些資料結構的定義散落在不同文件的時候，就很有可能不小心遺漏而導致不正確的結果，這是另一個麻煩。

## 問題的本質
我們前面所遇到的問題到底是怎麼發生的？顯然class是關鍵，我們必須根據不同的class有不同的反應。而class就是type，也就是說，貪得無厭的我們，想要讓程式根據type有不同對應行為的時候，這個問題就發生了，這樣子的機制稱為多型，而且是動態的多型。

我們想想如果現在是C語言的工作者，會如何定義前面的問題呢？我想他依然會準備一個class，然後再裏面放入kind的欄位。這麼一來，`lengood`的函數就會變成：
```cpp
int lengood(list lst){
    int count = 0;
    for(auto e : lst){
        if(e->kind == "good"){
            count++;
        }
    }
    return count;
}
```

這個想法很好，但是真正應用的時候卻沒這麼簡單，我們來看一個很典型，而且visitor最常被運用的例子，語法樹。
```cpp
struct expr {string kind;}
struct num : expr {int val;}
struct add : expr {expr *lhs, *rhs;}
struct mul : expr {expr *lhs, *rhs;}
```

但是，當kind是add或是mul時，val不會有值，反之，lhs及rhs在num下必須為num。接下來我們來實作，計算expr的函數calc:

```cpp
calc(expr e){
    if(e->kind == "num"){
        return e->val;
    }else if(e->kind == "add"){
        return calc(e->lhs) + calc(e->rhs)
    }else if(e->kind == "mul"){
        return calc(e->lhs) * calc(e->rhs)
    }
}
```
這個實作還不錯，透過Kind的標記，但是這玩意是沒有辦法動的，因為e的型別是`expr`他並不知道`val/lhs/rhs`是什麼？那我們只好讓expr有這些資料欄位：

```cpp
struct expr {
    string kind;
    int val;
    expr *lhs, *rhs;
}
```

不過，想必所有C++的用戶都享受過物件導向的美好，這個expr的資料結構實在是太丑了。因此，我們需要一個修正方案，就是轉型。
```cpp
calc(expr e){
    if(e->kind == "num"){
        mul *e = static_cast<val*>(e);
        return e->val;
    }else if(e->kind == "add"){
        mul *e = static_cast<add*>(e);
        return calc(e->lhs) + calc(e->rhs)
    }else if(e->kind == "mul"){
        mul *e = static_cast<mul*>(e);
        return calc(e->lhs) * calc(e->rhs)
    }
}
```

讓我們改寫我們最早的例子吧：

```cpp
class object{
    string kind="object";
}

class good : object{
    string kind="good";
}

class bad : object{
    string kind="bad";
}

class apple : good{
    string kind="apple";
}
class cat : good{
    string kind="cat";
}
class smoke : bad{
    string kind="smoke";
}

int life_scroe(list<object*> lst){
    int score = 0;
    for(auto e : lst){
        if(e->kind == "num")
            score += 1;
        else if(e->kind == "add")
            score += 2;
        else if(e->kind == "mul")
            score += 1;
    }
    return score;
}

int lengood(list lst){
    int count = 0;
    for(auto e : lst){
        if(e->kind == "good"){
            count++;
        }
    }
    return count;
}
```

> **完美**.....

個毛線，我們的`lengood`再一次的壞掉了，因為我們把繼承`good`kind的人替換成`apple`之類的東西了，要修好這個解決方案，你的kind必須也是一個資料結構，記錄他們之間的類型關係才行。修修補補，補補修修。為什麼C\++沒有一個可以完美解決這些問題的終極方案呢？我只不過要拿個type而已啊。穩的沒有問題，其實可以這個方法就是`dynamic_cast`：
```cpp
calc(expr e){
    if(val *e = dynamic_cast<val*>(e)){
        return e->val;
    }else if(add *e = dynamic_cast<add*>(e)){
        return calc(e->lhs) + calc(e->rhs)
    }else if(mul *e = dynamic_cast<mul*>(e)){
        return calc(e->lhs) * calc(e->rhs)
    }
}
```
前一個例子我們也可以如法炮製
```cpp
int lengood(list<object*> lst){
    int score = 0;
    for(auto e : lst){
      if(auto *ce = dynamic_cast<good*>(e))
        score += 1;
    }
    return score;

}

int life_scroe(list<object*> lst){
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
```
`dynamic_cast`可以很好的應付這個問題，這個解法乾淨俐落，比起維護一個`kind`這個機制更合理。不過呢，dynamic_cast實際上依賴一個稱為RTTI的技術，在使用這個方法的時候將會有大量個效能損失。但其實，我們有一個很經典但卻十分有用的機制，稱為Visitor Design Pattern。

## Kind的升級版
我們現在已經知道了對付這問題的技巧與方法了（就是C的老方法），就是讓class自己本身告訴我你是什麼type。不過，這樣子的作法有個不算缺點的缺點，就是醜。而且顯然會對應一組巨大的if else表，看起來神煩，一點都不OO。所以我們發明了一個新的Kind。叫作`accept`函數。

```cpp
class object{
    virtual bool accept(bVisitor *V) = 0;
}

class good : object{
    bool accept(bVisitor *V){return V->good();}
}

class bad : object{
    bool accept(bVisitor *V){return V->bad();}
}
```

我們讓accept自己呼叫good()函數以及bad()函數，這麼一來我們的功能都可以實作再名為bVisitor的類別裡。而且我們只要繼承bVisitor，改掉good()與bad()的功能就可以解決所有的問題了呢。

```cpp
class bVisitor{
    virtual bool good() = 0
    virtual bool bad() = 0
}

class isgood : bVisitor{
    bool good() { return true;}
    bool bad() { return false;}
}
```

那麼，我們的expr class也可依樣畫葫蘆，不過，有點不同的是，當我們計算的時候，還需要索取資料結構的內容，而不是只要判對型別。所以，我們還必須讓函數本身可以吃一個參數，也就是自己。

```cpp
class expr {virtual int accept() = 0}
class num : expr {
    int val;
    int accept(IVisitor V){return V.num(this);}
}
class add : expr {
    expr lhs,rhs;
    int accept(IVisitor V){return V.add(this);}
}
class mul : expr {
    expr lhs,rhs;
    int accept(IVisitor V){return V.expr(this);}
}

class calc : IVisitor {
    int num(num *n) {return n->val;}
    int add(add *n) {return n->lhs->accept(calc) + n->rhs->accept(calc);}
    int mul(mul *n) {return n->lhs->accept(calc) * n->lhs->accept(calc);}
}
```

在大部分的語言當中，我們允許函數的重載，所以顯然的Visitor內的函數也不需要特別稱呼不同的名字了，我們就一致的叫作`visit()`。所以這組程式就變成了：

```cpp
class expr {virtual int accept() = 0}
class num : expr {
    int val;
    int accept(IVisitor *V){return V.visit(this);}
    //visit匹配visit(num *n)
}
class add : expr {
    expr lhs,rhs;
    int accept(IVisitor *V){return V.visit(this);}
}
class mul : expr {
    expr lhs,rhs;
    int accept(IVisitor *V){return V.visit(this);}
}

class calc : IVisitor {
    int visit(num *n) {return n->val;}
    int visit(add *n) {return n->lhs->accept(calc) + n->rhs->accept(calc);}
    int visit(mul *n) {return n->lhs->accept(calc) * n->lhs->accept(calc);}
}
```

## 比較
對於這個動態多型的問題，我們有三種處理策略
* virtual function
* dynamic_cast
* visitor pattern


首先，先比較速度，可以參考我的實作 [^github]
| virtual function | dynamic_cast | visitor pattern |
| -------- | -------- | -------- |
| 0.885s     | 0.998s     | 0.918s     |

[^github]:https://github.com/sychiao/Visitor_Example

我們後兩者，主要是想要在新增功能的時候減少修改資料結構的開銷。儘管dynamic_cast的解法簡單漂亮，但是付出的代價卻是很大的。而visitor很好的調整了他們之間的平衡。

## 結論與後續
Visitor各種意義上非常的平凡，但是卻包裝的十分抽象。正是因為他的設計如此單純，而且副作用巨大（想想當我想要改資料結構的時候，我必須要付出的代價可能是所有visitor都要跟著改)，所以他充滿爭議，也有很多後續的變體。

然而，話說從頭，其實我們就是渴望一個簡單一致的多型介面，我們很明確的用dynamic_cast展示了C++動態多型的真實威力，以及被稱為RTTI的代價。

以LLVM來說，他放棄了標準庫的RTTI而自己從0打造了一個新的（就像我們的Kind一樣），而這麼做的C++ Library不再少數，每個C++ Library的作者都有想要無限加速Library的衝動。

然而，Visitor展示了一個非常好的折衷方案，無需RTTI的快速，實作上也就只比STL的dynamic_cast複雜一點點而已。


## Reference
1. A Little Java, A few Patterns
http://www.codemaster.blog/2008/04/scalas-pattern-matching-visitor-pattern.html


## 注
而在C++當中我們則會有expr.calc()。

```cpp
class expr {virtual int calc() = 0}
class num : expr {
    int val;
    int calc(){return val;}
}
class add : expr {
    expr lhs,rhs;
    int calc(){return lhs.calc() + rhs.calc();}
}
class mul : expr {
    expr lhs,rhs;
    int calc(){return lhs.calc() * rhs.calc();}
}
```


現在我們來想想，C++或的什麼好處，當我們新增一個資料結構，例如說指數
```cpp
class exp : expr{}
```
下面這個方法提供了很高的彈性，這個class只要實作一組calc一切的一切就解決了，相反的，C的版本要替所有的算法函數提供判斷與修改，哇歐，浩大工程，但是相反的，當我要替calc實作新的功能的時候，我也必須付出巨大的代價，因為資料結構的所有人都必須做出對應的修改。

>「稳定的数据结构和易变的操作耦合问题」

此時這句話是不是開始變得明朗了呢，正是因為資料結構與算法會互相影響，所以我們有不同的「設計方法」，讓我們的資料結構更改變得容易，或是算法添加變得容易。

https://gieseanw.wordpress.com/2018/12/29/stop-reimplementing-the-virtual-table-and-start-using-double-dispatch/
