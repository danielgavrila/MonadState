#include <iostream>
#include <chrono>
#include "state.hpp"
using namespace std;
using namespace std::chrono;

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
