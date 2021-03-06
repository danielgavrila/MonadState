#pragma once
#include "monad.hpp"

namespace monadstate{

template <typename F,typename S>
int runIt( S  start)
{
auto tpl=tupleNextState<S>();
using namespace std::chrono;
for(;;)
{
    //std::cout << "Sleep"<< std::endl;
    std::this_thread::sleep_for(1s); //time is used as an "event" that triggers the transition of state
    //add here  the logic that comes from "external world" to update the
    //state after a real event is received
    //one possible interface is
    //start=updateState(start);

    auto idx=findIndexInTuple(tpl,start);
    assert(idx!=std::numeric_limits<size_t>::max());
    auto next=S();
    applyIndex(tpl,idx,[&next,start=start](auto &&arg)
    {
        next=mbind(start,arg);
    }
    );
    assert(false==std::holds_alternative<std::monostate>(next));
    if(guard<F>(next))
    {
        break;
    }
    start=next;
}
return 0;
}


}
