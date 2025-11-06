#include "gtest/gtest.h"
#include "order.h"

TEST(MoneyTest, CanCreateWithPositiveValue) {
    Money m{12345}; 
    ASSERT_EQ(m.cents, 12345);
}

TEST(MoneyTest, ConstructorThrowsOnNegativeValue) {
    ASSERT_THROW(Money{-100}, std::invalid_argument);
}

TEST(MoneyTest, AdditionOperatorWorks) {
    Money m1{1000};
    Money m2{250};
    Money result = m1 + m2;
    ASSERT_EQ(result.cents, 1250);
}

TEST(MoneyTest, AdditionAssignmentOperatorWorks) {
    Money m1{1000};
    Money m2{250};
    m1 += m2;
    ASSERT_EQ(m1.cents, 1250);
}

TEST(MoneyTest, EqualityOperatorWorks) {
    Money m1{500};
    Money m2{500};
    Money m3{600};
    ASSERT_TRUE(m1 == m2);
    ASSERT_FALSE(m1 == m3);
}

TEST(DiscountTest, ZeroPercentDiscountReturnsSameAmount) {
    Money original{10000};
    Money discounted = apply_discount(original, 0);
    ASSERT_EQ(discounted.cents, 10000);
}

TEST(DiscountTest, HundredPercentDiscountReturnsZero) {
    Money original{10000};
    Money discounted = apply_discount(original, 100);
    ASSERT_EQ(discounted.cents, 0);
}

TEST(DiscountTest, ThrowsOnInvalidDiscountPercentage) {
    Money original{10000};
    ASSERT_THROW(apply_discount(original, -1), std::invalid_argument);
    ASSERT_THROW(apply_discount(original, 101), std::invalid_argument);
}

TEST(DiscountTest, ChecksRoundingLogic) {
    Money original{999};
    Money discounted = apply_discount(original, 10);
    ASSERT_EQ(discounted.cents, 900);
}

TEST(DiscountTest, AppliesStandardDiscount) {
    Money original{10000};
    Money discounted = apply_discount(original, 20);
    ASSERT_EQ(discounted.cents, 8000);
}

TEST(VATTest, ZeroPercentVatReturnsSameAmount) {
    Money original{10000};
    Money withVat = apply_vat(original, 0);
    ASSERT_EQ(withVat.cents, 10000);
}

TEST(VATTest, StandardVatAppliedCorrectly) {
    Money original{10000};
    Money withVat = apply_vat(original, 20);
    ASSERT_EQ(withVat.cents, 12000);
}

TEST(VATTest, ThrowsOnInvalidVatPercentage) {
    Money original{10000};
    ASSERT_THROW(apply_vat(original, -1), std::invalid_argument);
    ASSERT_THROW(apply_vat(original, 101), std::invalid_argument);
}

TEST(VATTest, ChecksRoundingLogic) {
    Money original{999};
    Money withVat = apply_vat(original, 10);
    ASSERT_EQ(withVat.cents, 1098);
}

TEST(OrderItemTest, TotalCalculationIsCorrect) {
    OrderItem item{"SKU123", Money{2500}, 4}; 
    ASSERT_EQ(item.total().cents, 10000);
}

class OrderTest : public ::testing::Test {
protected:
    Order order;
    OrderItem item1{"SKU001", Money{10000}, 2}; 
    OrderItem item2{"SKU002", Money{500}, 5};   

    void SetUp() override {
        order.add_item(item1);
        order.add_item(item2);
    }
};

TEST_F(OrderTest, AddItemIncreasesCount) {
    ASSERT_EQ(order.items_count(), 2);
    OrderItem item3{"SKU003", Money{100}, 1};
    order.add_item(item3);
    ASSERT_EQ(order.items_count(), 3);
}

TEST(OrderTest, AddItemThrowsOnZeroQuantity) {
    Order o;
    OrderItem invalidItem{"SKU_ERR", Money{1000}, 0};
    ASSERT_THROW(o.add_item(invalidItem), std::invalid_argument);
}

TEST_F(OrderTest, SubtotalCalculatesSumCorrectly) {
    ASSERT_EQ(order.subtotal().cents, 22500);
}

TEST_F(OrderTest, TotalWithDiscountCalculatesCorrectly) {
    Money discounted = order.total_with_discount(10);
    ASSERT_EQ(discounted.cents, 20250);
}

TEST_F(OrderTest, TotalWithDiscountAndVatCalculatesCorrectly) {
    Money finalTotal = order.total_with_discount_and_vat(10, 20);
    ASSERT_EQ(finalTotal.cents, 24300);
}

TEST(OrderValidationTest, IsValidReturnsFalseForEmptyOrder) {
    Order o;
    ASSERT_FALSE(o.is_valid());
}

TEST(OrderValidationTest, IsValidReturnsFalseForItemWithoutSku) {
    Order o;
    OrderItem invalidItem{"", Money{1000}, 1}; 
    o.add_item(invalidItem);
    ASSERT_FALSE(o.is_valid());
}

TEST(OrderValidationTest, IsValidReturnsFalseForItemWithZeroPrice) {
    Order o;
    OrderItem invalidItem{"SKU_ZERO_PRICE", Money{0}, 1};
    o.add_item(invalidItem); 
    ASSERT_FALSE(o.is_valid());
}

TEST(OrderValidationTest, IsValidReturnsTrueForValidOrder) {
    Order o;
    OrderItem validItem{"SKU_OK", Money{1}, 1};
    o.add_item(validItem);
    ASSERT_TRUE(o.is_valid()); 
}
