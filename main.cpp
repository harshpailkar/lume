#include <iostream> 
#include "lume.h"

int main() {
    OrderBook lume;

    // 1. Setup a "Deep" Market
    std::cout << "Step 1: Building liquidity..." << std::endl;
    lume.addOrder(1, true, 10000, 10);  // Buy 10 @ 100.00
    lume.addOrder(2, true, 9950, 20);   // Buy 20 @ 99.50
    lume.addOrder(3, true, 9900, 50);   // Buy 50 @ 99.00
    lume.addOrder(4, false, 10100, 10); // Sell 10 @ 101.00
    
    std::cout << "Initial Market -> ";
    lume.printBestPrices(); // Should show Bid: 10000 | Ask: 10100

    // 2. Trigger the "Flash Crash"
    std::cout << "\nStep 2: Massive Sell Order (80 shares @ 98.00) hitting the book!" << std::endl;
    lume.addOrder(5, false, 9800, 80); 

    std::cout << "Post-Crash Market -> ";
    lume.printBestPrices(); // Should show Bid: 9900 | Ask: 10100 (if all matches correctly)

    // 3. Test Cancellation
    std::cout << "\nStep 3: Testing Cancellation on remaining Sell order..." << std::endl;
    lume.cancelOrder(4); 

    std::cout << "Final Market -> ";
    lume.printBestPrices(); 

    std::cout << "\nSimulation Complete. No crashes detected!" << std::endl;

    return 0;
}
