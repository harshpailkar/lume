#include <iostream>
#include <map>
#include <list>
#include <unordered_map>
#include <algorithm>

struct Order {
    uint64_t orderId;
    bool isBuy;
    int64_t price;
    uint32_t quantity;
};

struct LimitLevel {
    int64_t price;
    uint64_t totalQuantity = 0;
    std::list<Order> orders;
};

struct OrderBookEntry {
    std::list<Order>::iterator it;
    LimitLevel *level;
};

class OrderBook {
private:
    std::map<int64_t, LimitLevel, std::greater<int64_t>> bids;
    std::map<int64_t, LimitLevel, std::less<int64_t>> asks;
    std::unordered_map<uint64_t, OrderBookEntry> orderMap;

public:
    void addOrder(uint64_t orderId, bool isBuy, int64_t price, uint32_t quantity);
    void cancelOrder(uint64_t orderId);
};

void OrderBook::addOrder(uint64_t orderId, bool isBuy, int64_t price, uint32_t quantity) {
    if (isBuy) {
        while (quantity > 0 && !asks.empty() && price >= asks.begin()->first) {
            auto &bestAskLevel = asks.begin()->second;
            auto &ordersAtLevel = bestAskLevel.orders;

            while (quantity > 0 && !ordersAtLevel.empty()) {
                Order &restingOrder = ordersAtLevel.front();
                uint32_t matchQuantity = std::min(quantity, restingOrder.quantity);
                
                quantity -= matchQuantity;
                restingOrder.quantity -= matchQuantity;
                bestAskLevel.totalQuantity -= matchQuantity;

                if (restingOrder.quantity == 0) {
                    orderMap.erase(restingOrder.orderId); // Erase from map before popping
                    ordersAtLevel.pop_front();
                }
            }
            if (ordersAtLevel.empty()) asks.erase(asks.begin());
        }
    } else {
        while (quantity > 0 && !bids.empty() && price <= bids.begin()->first) {
            auto &bestBidLevel = bids.begin()->second;
            auto &ordersAtLevel = bestBidLevel.orders;

            while (quantity > 0 && !ordersAtLevel.empty()) {
                Order &restingOrder = ordersAtLevel.front();
                uint32_t matchQuantity = std::min(quantity, restingOrder.quantity);
                
                quantity -= matchQuantity;
                restingOrder.quantity -= matchQuantity;
                bestBidLevel.totalQuantity -= matchQuantity;

                if (restingOrder.quantity == 0) {
                    orderMap.erase(restingOrder.orderId);
                    ordersAtLevel.pop_front();
                }
            }
            if (ordersAtLevel.empty()) bids.erase(bids.begin());
        }
    }

    if (quantity > 0) {
        if (isBuy) {
            auto &level = bids[price];
            level.price = price;
            level.totalQuantity += quantity;
            level.orders.push_back({orderId, isBuy, price, quantity});
            orderMap[orderId] = {--level.orders.end(), &level};
        } else {
            auto &level = asks[price];
            level.price = price;
            level.totalQuantity += quantity;
            level.orders.push_back({orderId, isBuy, price, quantity});
            orderMap[orderId] = {--level.orders.end(), &level};
        }
    }
}

void OrderBook::cancelOrder(uint64_t orderId) {
    auto it = orderMap.find(orderId);
    if (it == orderMap.end()) return;

    OrderBookEntry &entry = it->second;
    entry.level->totalQuantity -= entry.it->quantity;
    
    int64_t price = entry.it->price;
    bool wasBuy = entry.it->isBuy;

    entry.level->orders.erase(entry.it);

    if (entry.level->orders.empty()) {
        if (wasBuy) bids.erase(price);
        else asks.erase(price);
    }
    orderMap.erase(it);
}
