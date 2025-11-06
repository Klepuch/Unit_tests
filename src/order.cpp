#include "order.h"
Money apply_discount(const Money& original, int discountPercent) {
    if (discountPercent < 0 || discountPercent > 100) {
        throw std::invalid_argument("discountPercent must be between 0 and 100");
    }
    long long discount = original.cents * discountPercent / 100; 
    return Money{original.cents - discount};
}

Money apply_vat(const Money& original, int vatPercent) {
    if (vatPercent < 0 || vatPercent > 100) {
        throw std::invalid_argument("vatPercent must be between 0 and 100");
    }
    long long vat = original.cents * vatPercent / 100;
    return Money{original.cents + vat};
}

void Order::add_item(const OrderItem& item) {
    if (item.quantity <= 0) {
        throw std::invalid_argument("quantity must be > 0");
    }
    items_.push_back(item);
}

std::size_t Order::items_count() const {
    return items_.size();
}

Money Order::subtotal() const {
    Money sum{0};
    for (const auto& it : items_) {
        sum += it.total(); 
    }
    return sum;
}

Money Order::total_with_discount(int discountPercent) const {
    Money sub = subtotal();
    return apply_discount(sub, discountPercent);
}

Money Order::total_with_discount_and_vat(int discountPercent, int vatPercent) const {
    Money discounted = total_with_discount(discountPercent); 
    return apply_vat(discounted, vatPercent);
}

bool Order::is_valid() const {
    if (items_.empty()) return false;
    for (const auto& it : items_) {
        if (it.sku.empty()) return false;
        if (it.quantity <= 0) return false; 
        if (it.unitPrice.cents <= 0) return false; 
    }
    return true;
}
