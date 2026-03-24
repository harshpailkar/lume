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
};

struct LimitLevel
{
    int64_t price;
    uint64_t totalQuantity;
    std::list<Order> orders;
};

struct OrderBookEntry
{
    std::list<Order>::iterator it;
    LimitLevel *level;
};

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
};

void OrderBook::addOrder(uint64_t orderId, bool isBuy, int64_t price, uint32_t quantity) {
    if(isBuy) {
        while(quantity > 0 && !asks.empty() && price >= asks.begin()->first) {
            auto &bestAskLevel = asks.begin()->second;
            auto &ordersAtLevel = bestAskLevel.orders;

            while(quantity > 0 && !ordersAtLevel.empty()) {
                Order &restingOrder = ordersAtLevel.front();
                uint32_t matchQuantity = min(quantity, restingOrder.quantity);
                quantity -= matchQuantity;
                restingOrder.quantity -= matchQuantity;
                bestAskLevel.totalQuantity -= matchQuantity;

                if (restingOrder.quantity == 0) {
                    ordersAtLevel.pop_front();
                    orderMap.erase(restingOrder.orderId);
                }
            }

            if(ordersAtLevel.empty()) {
                asks.erase(asks.begin());
            }
        }
    } else {
        while(quantity > 0 && !bids.empty() && price <= bids.begin()->first) {
            auto &bestBidLevel = bids.begin()->second;
            auto &ordersAtLevel = bestBidLevel.orders;

            while(quantity > 0 && !ordersAtLevel.empty()) {
                Order &restingOrder = ordersAtLevel.front();
                uint32_t matchQuantity = std::min(quantity, restingOrder.quantity);
                quantity -= matchQuantity;
                restingOrder.quantity -= matchQuantity;
                bestBidLevel.totalQuantity -= matchQuantity;

                if (restingOrder.quantity == 0) {
                    ordersAtLevel.pop_front();
                    orderMap.erase(restingOrder.orderId);
                }
            }

            if(ordersAtLevel.empty()) {
                bids.erase(bids.begin());
            }
        }
    }

    if(quantity > 0) {
        Order newOrder = {orderId, isBuy, price, quantity};
        if(isBuy) {
            auto &level = bids[price];
            level.price = price;
            level.totalQuantity += quantity;
            level.orders.push_back(newOrder);
            orderMap[orderId] = {--level.orders.end(), &level};
        } else {
            auto &level = asks[price];
            level.price = price;
            level.totalQuantity += quantity;
            level.orders.push_back(newOrder);
            orderMap[orderId] = {--level.orders.end(), &level};
        }
    }
}
