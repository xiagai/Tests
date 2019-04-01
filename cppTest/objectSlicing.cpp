// clang编译器
// C++中将派生类转换成基类会生成一个该类基类成分的临时副本
// C++中调用派生类的基类成分应该使用基类指针或引用
#include <iostream>
using namespace std;

class A {
public:
    virtual void foo() { 
    	std::cout << "A" << std::endl;
    	a = -1;
    }
    int a;
};

class B : public A {
public:
    virtual void foo() { 
    	std::cout << "B" << std::endl;
    }
    void bar ()  { 
      //A::foo(); // 输出 A -1 2
      static_cast<A>(*this).foo(); // 输出 A 1 2
    	//static_cast<A*>(this)->foo(); // 输出 B 1 2
    	//static_cast<A&>(*this).foo(); // 输出 B 1 2
    	cout << a << " " << b << endl;
    }
    int b;
};

int main () {
    B b;
    b.a = 1;
    b.b = 2;
    b.bar();
    
}
