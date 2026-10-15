#include <HUH/Types/pair.h>

#include <gtest/gtest.h>

#include <concepts>
#include <cstddef>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

///
/// DISCLAIMER CHATGPT Generated tests I just needed some tests while i looked over it i don't claim that it tests all
/// paths
///

namespace {

template<typename T, std::size_t Index>
concept HasIndexGet = requires(T&& value) { std::forward<T>(value).template Get<Index>(); };

using IntPair = HUH::Pair<int, double>;

static_assert(HasIndexGet<IntPair&, 0>);
static_assert(HasIndexGet<IntPair&&, 1>);
static_assert(!HasIndexGet<IntPair&, 2>);

static_assert(std::same_as<decltype(std::declval<IntPair&>().Get<0>()), int&>);
static_assert(std::same_as<decltype(std::declval<const IntPair&>().Get<0>()), const int&>);
static_assert(std::same_as<decltype(std::declval<volatile IntPair&>().Get<0>()), volatile int&>);
static_assert(std::same_as<decltype(std::declval<const volatile IntPair&>().Get<0>()), const volatile int&>);
static_assert(std::same_as<decltype(std::declval<IntPair&&>().Get<0>()), int&&>);
static_assert(std::same_as<decltype(std::declval<const IntPair&&>().Get<0>()), const int&&>);
static_assert(std::same_as<decltype(std::declval<volatile IntPair&&>().Get<0>()), volatile int&&>);
static_assert(std::same_as<decltype(std::declval<const volatile IntPair&&>().Get<0>()), const volatile int&&>);

struct MemberComparable {
    int value{};

    MemberComparable() = default;
    explicit MemberComparable(int v) : value(v) {}

    bool operator==(const MemberComparable& rhs) const { return value == rhs.value; }
    bool operator!=(const MemberComparable& rhs) const { return value != rhs.value; }
    bool operator<(const MemberComparable& rhs) const { return value < rhs.value; }
    bool operator<=(const MemberComparable& rhs) const { return value <= rhs.value; }
    bool operator>(const MemberComparable& rhs) const { return value > rhs.value; }
    bool operator>=(const MemberComparable& rhs) const { return value >= rhs.value; }
};

template<typename T>
struct AssignmentBox {
    T value{};

    AssignmentBox() = default;
    explicit AssignmentBox(T v) : value(v) {}

    template<typename U>
    AssignmentBox& operator=(const AssignmentBox<U>& rhs) {
        value = static_cast<T>(rhs.value);
        return *this;
    }
};

struct LifetimeTracked {
    static inline int alive = 0;
    static inline int constructed = 0;
    static inline int destroyed = 0;

    int value{};

    explicit LifetimeTracked(int v = 0) : value(v) {
        ++alive;
        ++constructed;
    }
    LifetimeTracked(const LifetimeTracked& rhs) : value(rhs.value) {
        ++alive;
        ++constructed;
    }
    LifetimeTracked(LifetimeTracked&& rhs) noexcept : value(rhs.value) {
        rhs.value = -1;
        ++alive;
        ++constructed;
    }
    LifetimeTracked& operator=(const LifetimeTracked& rhs) {
        value = rhs.value;
        return *this;
    }
    LifetimeTracked& operator=(LifetimeTracked&& rhs) noexcept {
        value = rhs.value;
        rhs.value = -1;
        return *this;
    }
    ~LifetimeTracked() {
        --alive;
        ++destroyed;
    }

    static void Reset() {
        alive = 0;
        constructed = 0;
        destroyed = 0;
    }
};

struct Accumulator {
    int base{};
    int Add(int value) const { return base + value; }
};

template<typename Lhs, typename Rhs>
void TryAssign(Lhs& lhs, const Rhs& rhs) {
    if constexpr (std::is_assignable_v<Lhs&, const Rhs&>) {
        lhs = rhs;
    }
}

template<typename Lhs, typename Rhs>
void TryMoveAssign(Lhs& lhs, Rhs&& rhs) {
    if constexpr (std::is_assignable_v<Lhs&, Rhs&&>) {
        lhs = std::forward<Rhs>(rhs);
    }
}

TEST(PairConstruction, DefaultAndValueConstruction) {
    HUH::Pair<int, std::string> defaulted{};
    EXPECT_EQ(defaulted.first, 0);
    EXPECT_TRUE(defaulted.second.empty());

    HUH::Pair<int, std::string> pair(7, std::string("hello"));
    EXPECT_EQ(pair.first, 7);
    EXPECT_EQ(pair.second, "hello");
}

TEST(PairConstruction, ConvertingConstructionFromAnotherPair) {
    HUH::Pair<int, float> source(9, 1.25F);
    HUH::Pair<long, double> converted(source);
    EXPECT_EQ(converted.first, 9L);
    EXPECT_DOUBLE_EQ(converted.second, 1.25);

    const HUH::Pair<int, std::string> constSource(2, std::string("copy"));
    HUH::Pair<long, std::string> fromConst(constSource);
    EXPECT_EQ(fromConst.first, 2L);
    EXPECT_EQ(fromConst.second, "copy");
}

TEST(PairAccess, FirstSecondAndGetStayInSync) {
    HUH::Pair<int, std::string> pair(1, std::string("a"));
    EXPECT_EQ(pair.Get<0>(), pair.first);
    EXPECT_EQ(pair.Get<1>(), pair.second);
    pair.Get<0>() = 10;
    pair.Get<1>() = "b";
    EXPECT_EQ(pair.first, 10);
    EXPECT_EQ(pair.second, "b");
}

TEST(PairAccess, CvQualifiedObjectsCanBeRead) {
    const HUH::Pair<int, int> constPair(1, 2);
    volatile HUH::Pair<int, int> volatilePair(3, 4);
    const volatile HUH::Pair<int, int> cvPair(5, 6);

    EXPECT_EQ(constPair.Get<0>(), 1);
    EXPECT_EQ(volatilePair.Get<1>(), 4);
    EXPECT_EQ(cvPair.Get<0>(), 5);
    EXPECT_EQ(cvPair.Get<1>(), 6);
}

TEST(PairReferences, ReferenceElementsAliasTheirOriginalObjects) {
    int number = 3;
    std::string text = "before";
    HUH::Pair<int&, std::string&> pair(number, text);

    pair.first = 4;
    pair.Get<1>() = "after";
    EXPECT_EQ(number, 4);
    EXPECT_EQ(text, "after");
    EXPECT_EQ(&pair.Get<0>(), &number);
    EXPECT_EQ(&pair.Get<1>(), &text);
}

TEST(PairReferences, ConstPairDoesNotMakeReferredObjectsConst) {
    int value = 1;
    const HUH::Pair<int&, int&> pair(value, value);
    pair.Get<0>() = 8;
    EXPECT_EQ(value, 8);
    static_assert(std::same_as<decltype(std::declval<const HUH::Pair<int&, int&>&>().Get<0>()), int&>);
}

TEST(PairReferences, ReferenceElementAssignmentIsReportedExplicitly) {
    using RefPair = HUH::Pair<int&, int&>;
    int a = 1;
    int b = 7;
    RefPair lhs(a, a);
    RefPair rhs(b, b);
    const bool assignable = std::is_assignable_v<RefPair&, const RefPair&>;
    EXPECT_TRUE(assignable) << "Pair<int&, int&> should ideally assign through its reference elements";
    TryAssign(lhs, rhs);
    if (assignable) {
        EXPECT_EQ(a, 7);
    } else {
        EXPECT_EQ(a, 1);
    }
}

TEST(PairAssignment, SameTypeCopyAndMoveAssignment) {
    HUH::Pair<int, std::string> source(4, std::string("copy"));
    HUH::Pair<int, std::string> destination(0, std::string("old"));
    destination = source;
    EXPECT_EQ(destination.first, 4);
    EXPECT_EQ(destination.second, "copy");

    HUH::Pair<int, std::string> moveSource(9, std::string("move"));
    destination = std::move(moveSource);
    EXPECT_EQ(destination.first, 9);
    EXPECT_EQ(destination.second, "move");
}

TEST(PairAssignment, HeterogeneousAssignmentWithElementAssignmentOperators) {
    HUH::Pair<AssignmentBox<long>, AssignmentBox<long>> destination(AssignmentBox<long>(0), AssignmentBox<long>(0));
    HUH::Pair<AssignmentBox<int>, AssignmentBox<int>> source(AssignmentBox<int>(11), AssignmentBox<int>(22));

    const bool copyAssignable = std::is_assignable_v<decltype(destination)&, const decltype(source)&>;
    EXPECT_TRUE(copyAssignable) << "Heterogeneous Pair assignment should have an unambiguous overload";
    TryAssign(destination, source);
    if (copyAssignable) {
        EXPECT_EQ(destination.first.value, 11L);
        EXPECT_EQ(destination.second.value, 22L);
    }

    HUH::Pair<AssignmentBox<int>, AssignmentBox<int>> moveSource(AssignmentBox<int>(31), AssignmentBox<int>(42));
    const bool moveAssignable = std::is_assignable_v<decltype(destination)&, decltype(moveSource)&&>;
    EXPECT_TRUE(moveAssignable) << "Heterogeneous Pair rvalue assignment should have an unambiguous overload";
    TryMoveAssign(destination, std::move(moveSource));
    if (moveAssignable) {
        EXPECT_EQ(destination.first.value, 31L);
        EXPECT_EQ(destination.second.value, 42L);
    }
}

TEST(PairMoveOnly, MoveOnlyElementsCanBeMovedAfterDefaultConstruction) {
    // Pair's current (T1 first, T2 second) constructor copies its parameters
    // into the members, so this test initializes the public fields separately.
    HUH::Pair<std::unique_ptr<int>, std::string> source{};
    source.first = std::make_unique<int>(123);
    source.second = "move me";

    HUH::Pair<std::unique_ptr<int>, std::string> destination(std::move(source));
    ASSERT_NE(destination.first, nullptr);
    EXPECT_EQ(*destination.first, 123);
    EXPECT_EQ(destination.second, "move me");
    EXPECT_EQ(source.first, nullptr);
}

TEST(PairStructuredBinding, LvalueBindingsReferToElements) {
    HUH::Pair<int, std::string> pair(3, std::string("original"));
    auto& [number, text] = pair;
    number = 5;
    text = "updated";
    EXPECT_EQ(pair.first, 5);
    EXPECT_EQ(pair.second, "updated");
}

TEST(PairTraits, TupleSizeAndElementTypes) {
    using P = HUH::Pair<int, const std::string&>;
    static_assert(std::tuple_size_v<P> == 2);
    static_assert(std::same_as<std::tuple_element_t<0, P>, int>);
    static_assert(std::same_as<std::tuple_element_t<1, P>, const std::string&>);
    SUCCEED();
}

TEST(PairApply, BeforeAndAfterPlaceElementsInTheCorrectOrder) {
    HUH::Pair<int, int> pair(3, 4);
    const auto before = pair.ApplyBefore([](int a, int b, int suffix) { return a * 100 + b * 10 + suffix; }, 5);
    const auto after = pair.ApplyAfter([](int prefix, int a, int b) { return prefix * 100 + a * 10 + b; }, 2);
    EXPECT_EQ(before, 345);
    EXPECT_EQ(after, 234);
}

TEST(PairApply, PreservesLvalueReferencesInCallableResults) {
    HUH::Pair<int, int> pair(7, 8);
    int& first = pair.ApplyBefore([](int& a, int&) -> int& { return a; });
    first = 99;
    EXPECT_EQ(pair.first, 99);
}

TEST(PairApply, WorksWithConstVolatileAndRvaluePairs) {
    const HUH::Pair<int, int> constPair(1, 2);
    volatile HUH::Pair<int, int> volatilePair(3, 4);
    const volatile HUH::Pair<int, int> cvPair(5, 6);

    EXPECT_EQ(constPair.ApplyBefore([](int a, int b) { return a + b; }), 3);
    EXPECT_EQ(volatilePair.ApplyBefore([](int a, int b) { return a + b; }), 7);
    EXPECT_EQ(cvPair.ApplyAfter([](int a, int b, int suffix) { return a + b + suffix; }, 10), 21);

    HUH::Pair<std::unique_ptr<int>, int> movable{};
    movable.first = std::make_unique<int>(77);
    movable.second = 1;
    const int result = std::move(movable).ApplyBefore([](std::unique_ptr<int> value, int) { return *value; });
    EXPECT_EQ(result, 77);
    EXPECT_EQ(movable.first, nullptr);
}

TEST(PairApply, SupportsMemberFunctionPointersThroughStdInvoke) {
    Accumulator accumulator{10};
    HUH::Pair<Accumulator*, int> args(&accumulator, 5);
    EXPECT_EQ(args.ApplyBefore(&Accumulator::Add), 15);

    HUH::Pair<int, int> oneValue(8, 0);
    EXPECT_EQ(oneValue.ApplyAfter([](Accumulator* acc, int a, int b) { return acc->base + a + b; }, &accumulator), 18);
}

TEST(PairLifetime, OwnedElementsAreDestroyedExactlyOnce) {
    LifetimeTracked::Reset();
    {
        HUH::Pair<LifetimeTracked, LifetimeTracked> pair(LifetimeTracked(1), LifetimeTracked(2));
        EXPECT_EQ(LifetimeTracked::alive, 2);
        {
            auto copy = pair;
            EXPECT_EQ(LifetimeTracked::alive, 4);
            auto moved = std::move(copy);
            EXPECT_EQ(LifetimeTracked::alive, 6);
            EXPECT_EQ(moved.first.value, 1);
            EXPECT_EQ(moved.second.value, 2);
        }
        EXPECT_EQ(LifetimeTracked::alive, 2);
    }
    EXPECT_EQ(LifetimeTracked::alive, 0);
    EXPECT_EQ(LifetimeTracked::constructed, LifetimeTracked::destroyed);
}

TEST(PairComparisons, EqualityAndInequality) {
    using P = HUH::Pair<MemberComparable, MemberComparable>;
    const P a(MemberComparable(1), MemberComparable(2));
    const P b(MemberComparable(1), MemberComparable(2));
    const P c(MemberComparable(1), MemberComparable(3));

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
}

TEST(PairComparisons, RelationalOperatorsUseComponentWiseSemantics) {
    using P = HUH::Pair<MemberComparable, MemberComparable>;
    const P a(MemberComparable(1), MemberComparable(2));
    const P b(MemberComparable(3), MemberComparable(4));
    const P mixed(MemberComparable(0), MemberComparable(4));

    EXPECT_TRUE(a < b);
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(b > a);
    EXPECT_TRUE(b >= a);
    EXPECT_FALSE(a < mixed);// This is component-wise, not std::pair's lexicographic order.
    EXPECT_FALSE(a <= mixed);
}

}// namespace
