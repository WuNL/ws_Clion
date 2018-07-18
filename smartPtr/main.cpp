#include <iostream>
#include <memory>
#include <cassert>

using namespace std;

int main()
{
    shared_ptr<int> sp(new int(10));
    assert(sp.use_count() == 1);
    //create a weak_ptr from shared_ptr
    weak_ptr<int> wp(sp);
    //not increase the use count
    assert(sp.use_count() == 1);
    //judge wp is invalid
    //expired() is equivalent with use_count() == 0
    if (!wp.expired())
    {
        shared_ptr<int> sp2 = wp.lock();//get a shared_ptr
        *sp2 = 100;
        assert(2 == wp.use_count());
        cout << *sp2 << endl;
    }//out of scope,sp2 destruct automatically,use_count()--;
    assert(wp.use_count() == 1);
    sp.reset();//shared_ptr is invalid
    assert(wp.expired());

    return 0;
}