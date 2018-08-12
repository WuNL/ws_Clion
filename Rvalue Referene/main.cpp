#include <iostream>

using namespace std;

int g_cont = 0;
int g_copycont = 0;
int g_dest = 0;

class B
{
public:
    B()
    {
        cout<<"构造函数"<<endl;
    }
    ~B()
    {
        cout<<"析构函数"<<endl;
    }

    B(const B& b)
    {
        cout<<"拷贝构造函数"<<endl;
    }
    B& operator=(const B& b)
    {
        cout<<"赋值构造函数"<<endl;
    }
};

B func1(B& b)
{
    return b;
}

B func2(B b)
{
    return b;
}

B& func3(B b)
{
    return b;
}

B getB()
{
    return B();
}

int main()
{
//    B b1,b2;
//    cout<<endl;
//    cout<<"以下是func1的结果："<<endl;
//    b2 = func1(b1);
//
//    cout<<endl;
//    cout<<"以下是func2的结果："<<endl;
//    b2 = func2(b1);
//    cout<<endl;
//
//    cout<<endl;
//    cout<<"以下是func3的结果："<<endl;
//    b2 = func3(b1);
//    cout<<endl;

    B b3 = getB();
    return 0;
}