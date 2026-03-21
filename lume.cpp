#include <iostream>
#include <map>
#include <list>
#include <unordered_map>

struct Order
{
    uint64_t orderId;
    bool isBuy;
    int64_t price;
    uint32_t quantity;
}

struct LimitLevel
{
    int64_t price;
    uint64_t totalQuantity;
    std::list<Order> orders;
}

struct OrderBookEntry
{
    std::list<Order>::iterator it;
    LimitLevel *level;
}

class OrderBook
{
private:
    map<int64_t, LimitLevel, std::greater<int64_t> > bids;
    map<int64_t, LimitLevel, std::less<int64_t> > asks;
    unordered_map<uint64_t, OrderBookEntry> orderMap;

public:
    void addOrder(uint64_t orderId, bool isBuy, int64_t price, uint32_t quantity);
    void cancelOrder(uint64_t orderId);
    void matchOrders();
}