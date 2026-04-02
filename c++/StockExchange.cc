/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <vector>

enum class OrderType {
    BID,
    ASK
};

enum class Ticker {
    MSFT,
    TSLA,
    AAPL
};

using Units = unsigned int;
using Price = double;
using UUID = unsigned int;

struct Order {
    UUID id;
    OrderType type;
    Price price;
    Ticker ticker;
    Units   quantity;
    
    bool empty() const { return quantity == 0; }
};

class Orders {
    public:
    void add(Order& order) {
        orders_.push_back(order);
    };
    
    void match(Order& order) {
        for (auto& o : orders_) {
            switch (order.type) {
                case OrderType::BID: {
                    if (order.price >= o.price) {
                        if (order.quantity < o.quantity) {
                            o.quantity -= order.quantity;
                            order.quantity = 0;
                            //std::cout << "Bought " << order.quantity << "units of " << order.ticker << " @ " << o.price;
                            continue;
                        } else if (order.quantity == o.quantity) {
                            o.quantity = 0;
                            order.quantity = 0;
                            //std::cout << "Bought " << order.quantity << "units of " << order.ticker << " @ " << o.price;
                        } else {
                            order.quantity -= o.quantity;
                            o.quantity = 0;
                            //std::cout << "Bought " << o.quantity << "units of " << order.ticker << " @ " << o.price;
                        }
                    }
                }
                case OrderType::ASK: {
                    if (order.price <= o.price) {
                        if (order.quantity < o.quantity) {
                            o.quantity -= order.quantity;
                            order.quantity = 0;
                            //std::cout << "Bought " << order.quantity << "units of " << order.ticker << " @ " << o.price;
                            continue;
                        } else if (order.quantity == o.quantity) {
                            o.quantity = 0;
                            order.quantity = 0;
                            //std::cout << "Bought " << order.quantity << "units of " << order.ticker << " @ " << o.price;
                        } else {
                            order.quantity -= o.quantity;
                            o.quantity = 0;
                            //std::cout << "Bought " << o.quantity << "units of " << order.ticker << " @ " << o.price;
                        }
                    }
                }
            }
            if (order.empty()) {
                break;
            }
        }
        std::erase_if(orders_, [] (Order& o) {return o.quantity == 0;});
    };
    private:
    std::vector<Order> orders_;
};

class Exchange {
public:
    void trade(Order& order);
private:
    Orders buyOrders_;
    Orders sellOrders_;
};

void Exchange::trade(Order& order)  {
    switch(order.type) {
        case OrderType::BID: {
            sellOrders_.match(order);
            if (order.empty() == false) {
                buyOrders_.add(order);
            }
        }
        break;
        case OrderType::ASK: {
            buyOrders_.match(order);
            if (order.empty() == false) {
                sellOrders_.add(order);
            }
        }
        break;
        default:
        break;
    }
}

int main()
{
    std::cout<<"Hello World";
    
    Order order{.type = OrderType::BID};


    return 0;
}
