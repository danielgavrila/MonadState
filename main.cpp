#include <iostream>
#include <chrono>
#include "runit.hpp"
#include "state.hpp"

using namespace std;
using namespace std::chrono;
using namespace monadstate;
using namespace orders;

int main([[maybe_unused]] int argc,[[maybe_unused]]  char **argv)
{
    std::cout << "File name: " << "\n";
    auto ordId=orderID {0};
    auto clId=clientID{20001};

    auto initState= initial {ordId,clId};


    auto start=states{initState};
    runIt<final>(start);


    return 0;
}
