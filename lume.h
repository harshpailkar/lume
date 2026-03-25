#ifndef LUME_H
#define LUME_H

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
    
    void printBestPrices() const {
        if (!bids.empty()) std::cout << "Best Bid: " << bids.begin()->first << " | ";
        if (!asks.empty()) std::cout << "Best Ask: " << asks.begin()->first;
        std::cout << std::endl;
    }
};

#endif
