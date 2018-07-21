#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>
#include <vector>
#include <cstdio>
#include <ctime>
#include <stdint.h>

class TSomeData
{
private:
    int data;
public:
    TSomeData(int d)
            : data(d)
    {
        // Empty
    }
};

const int TEST_ITERATIONS = 100000;

typedef std::vector<boost::shared_ptr<TSomeData> > TVectorOfShared;
typedef std::vector<std::unique_ptr<TSomeData> > TVectorOfUnique;
typedef boost::ptr_vector<TSomeData> TPtrVector;

int main()
{
    clock_t start;
    clock_t end;

    start = ::clock();
    TVectorOfUnique vectorOfUnique;
    for (int i = 0; i < TEST_ITERATIONS; ++i) {
        std::unique_ptr<TSomeData> data(new TSomeData(i));
        vectorOfUnique.push_back(std::move(data));
    }
    end = ::clock();
    printf("Vector of unique:\n  Time executed: %u\n",
           static_cast<uint32_t>((end - start) / (CLOCKS_PER_SEC/1000)));

    start = ::clock();
    TVectorOfShared vectorOfShared;
    for (int i = 0; i < TEST_ITERATIONS; ++i) {
        // Test vector of shared_ptr
        boost::shared_ptr<TSomeData> data(new TSomeData(i));
        vectorOfShared.push_back(data);
    }
    end = ::clock();
    printf("Vector of shared:\n  Time executed: %u\n",
           static_cast<uint32_t>((end - start) / (CLOCKS_PER_SEC/1000)));

    start = ::clock();
    TPtrVector ptrVector;
    for (int i = 0; i < TEST_ITERATIONS; ++i) {
        // Test ptr_vector
        TSomeData* data = new TSomeData(i);
        ptrVector.push_back(data);
    }
    end = ::clock();
    printf("PtrVector:\n  Time executed: %u\n",
           static_cast<uint32_t>((end - start) / (CLOCKS_PER_SEC/1000)));

    TSomeData* lek;
    lek = new TSomeData(1);
    return 0;
}