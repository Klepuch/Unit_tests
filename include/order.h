#pragma once
#include <string>
#include <vector>
#include <stdexcept>

struct Money {
    long long cents = 0;

    Money() = default;
    explicit Money(long long cents_) : cents(cents_) {
        if (cents_ < 0) {
            throw std::invalid_argument("Money cannot be negative");
        }
    }

    Money operator+(const Money& other) const {
        return Money{cents + other.cents};
    }

    Money& operator+=(const Money& other) {
        cents += other.cents;
        return *this;
    }

    bool operator==(const Money& other) const {
        return cents == other.cents;
    }
};

struct OrderItem {
    std::string sku;      
    Money unitPrice;      
    int quantity = 0;     

    Money total() const {
        return Money{unitPrice.cents * quantity}; 
    }
};

Money apply_discount(const Money& original, int discountPercent);

Money apply_vat(const Money& original, int vatPercent);

class Order {
public:
    void add_item(const OrderItem& item);
    std::size_t items_count() const;
    Money subtotal() const;                  
    Money total_with_discount(int discountPercent) const;
    Money total_with_discount_and_vat(int discountPercent, int vatPercent) const;

    bool is_valid() const;

private:
    std::vector<OrderItem> items_;
};
