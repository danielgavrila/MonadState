#pragma once

#include "monad.hpp"

namespace ms=monadstate;

namespace orders
{

struct item{
    std::string name;
    double price;
    int qty;
};

using items=std::vector<item>;

using orderID=int;
using clientID=int;
using itemID=int;

using tvctints=std::vector<itemID>;


enum class statesEnum
{
    initial,
    unprocessedOrder,
    rejectedOrder,
    acceptedOrder,
    pendingOrder,
    fullfilledOrder,
    final
};


struct databaseIds{
    //statesEnum val_;
    orderID orderDBID_;
    clientID clientDBID_;
};


item getItem(itemID idDB)
{
    switch(idDB){
    case 0:
        return item {"Apple MacBook", 1999,2};
    case 1:
        return item {"iPhoneX", 999,1};
    default:
        return item {"Apple Watch", 499,1};

    }
}
struct order{
    items items_;
    orderID orderId_;
    clientID clientId_;
};

items getItems (const tvctints &v)
{
    auto ret=items();
    ranges::transform(v,ranges::back_inserter(ret),[](auto &&o)->item
    {
        return getItem(o);
    });

    return ret;
}

order getOrder(orderID orderidDB,clientID clientidDB)
{
    switch(orderidDB){
    case 0:
    {
        auto its=getItems({0,1,4});
        return order {its, orderidDB,clientidDB};
    }
    case 1:
    {
        auto its=getItems({0});
        return order {its, orderidDB,clientidDB};
    }

    default:
    {
        auto its=getItems({1});
        return order {its, orderidDB,clientidDB};
    }

    }
}


struct initial{
    databaseIds dbIds_;
};



struct unprocessedOrder
{
    databaseIds dbIds_;
    order order_;
};

struct rejectedOrder
{
    databaseIds dbIds_;
    std::string reason_;
};

struct acceptedOrder
{
    databaseIds dbIds_;
    order order_;
};

struct pendingOrder{
    databaseIds dbIds_;
    order order_;
    items missing_;
};

struct fullfilledOrder
{
    databaseIds dbIds_;
    order order_;
};

struct final{
    databaseIds dbIds_;
};

using states= std::variant<
std::monostate,
initial,
unprocessedOrder,
rejectedOrder,
acceptedOrder,
pendingOrder,
fullfilledOrder,
final
>;




inline states transition (const initial & s)
{
    auto ids=s.dbIds_;
    auto its=getOrder(ids.orderDBID_,ids.clientDBID_);
    auto ord=unprocessedOrder{ids,its};

    return ms::mreturn<states>(ord);
}

inline states transition (const unprocessedOrder & s)
{
    auto ids=s.dbIds_;
    if (ids.clientDBID_==20001)
    {
        auto ord=rejectedOrder{ids,"Client is bankrupt "};
        return ms::mreturn<states>(ord);

    }
    else {
        auto ord=acceptedOrder{ids,s.order_};
        return ms::mreturn<states>(ord);

    }

}

states transition (const rejectedOrder & s)
{
    auto ord=final{s.dbIds_};
    return ms::mreturn<states>(ord);
}

states transition (const acceptedOrder & s)
{
    auto ord=fullfilledOrder{s.dbIds_,s.order_};

    return ms::mreturn<states>(ord);
}

states transition (const pendingOrder & s)
{
    return ms::mreturn<states>(s);
}

states transition (const fullfilledOrder & s)
{
    auto ord=final{s.dbIds_};
    return ms::mreturn<states>(ord);
}


states transition (const final & s)
{

    return ms::mreturn<states>(s);
}


}
//////////////////////////////////////////////////
