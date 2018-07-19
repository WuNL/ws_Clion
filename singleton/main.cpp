#include <iostream>
#include "Singleton.h"

using namespace std;

class test
{
public:
    test():data(1){}
    void setData(int d)
    {
        data = d;
    }
    int getData()
    {
        return data;
    }
private:
    int data;
};

int main()
{
    test& t = Singleton<test>::instance();
    cout<<t.getData()<<endl;
    t.setData(2);

    test& t1 = Singleton<test>::instance();
    cout<<t1.getData()<<endl;

    std::cout << "Hello, World!" << std::endl;
    return 0;
}