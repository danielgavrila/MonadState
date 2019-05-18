#include <iostream>
#include <chrono>
#include "runit.hpp"
#include "state.hpp"

using namespace std;
using namespace std::chrono;
using namespace monadstate;
using namespace orders;

void testRejected()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    auto ordId=orderID {0};
    auto clId=clientID{20001};
    auto initState= initial {ordId,clId};
    auto start=states{initState};
    runIt<final>(start);
    std::cout << "Final reached\n" ;

}

void testFullfilled()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    auto ordId=orderID {0};
    auto clId=clientID{20002};
    auto initState= initial {ordId,clId};
    auto start=states{initState};
    runIt<final>(start);
    std::cout << "Final reached\n" ;


}


int main([[maybe_unused]] int argc,[[maybe_unused]]  char **argv)
{
    testRejected();
    testFullfilled();


    return 0;
}
