#include <HUH/Types/tuple.h>

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

template<typename T, typename Element>
concept HasTypeGet = requires(T&& value) { std::forward<T>(value).template Get<Element>(); };

using IntTuple = HUH::Tuple<int, double>;
using DuplicateTuple = HUH::Tuple<int, int>;

static_assert(HasIndexGet<IntTuple&, 0>);
static_assert(HasIndexGet<IntTuple&&, 1>);
static_assert(!HasIndexGet<IntTuple&, 2>);
static_assert(HasTypeGet<IntTuple&, int>);
static_assert(!HasTypeGet<DuplicateTuple&, int>);

static_assert(std::same_as<decltype(std::declval<IntTuple&>().Get<0>()), int&>);
static_assert(std::same_as<decltype(std::declval<const IntTuple&>().Get<0>()), const int&>);
static_assert(std::same_as<decltype(std::declval<volatile IntTuple&>().Get<0>()), volatile int&>);
static_assert(std::same_as<decltype(std::declval<const volatile IntTuple&>().Get<0>()), const volatile int&>);
static_assert(std::same_as<decltype(std::declval<IntTuple&&>().Get<0>()), int&&>);
static_assert(std::same_as<decltype(std::declval<const IntTuple&&>().Get<0>()), const int&&>);
static_assert(std::same_as<decltype(std::declval<volatile IntTuple&&>().Get<0>()), volatile int&&>);
static_assert(std::same_as<decltype(std::declval<const volatile IntTuple&&>().Get<0>()), const volatile int&&>);

// The implementation invokes comparison/assignment operators as members
// (value.operator==(other), value.operator=(other)). This wrapper lets those
// operators be tested without making the current tests fail to compile for int.
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

TEST(TupleConstruction, EmptyTupleAndElementCounts) {
    HUH::Tuple<> empty;
    static_assert(std::tuple_size_v<decltype(empty)> == 0);
    EXPECT_EQ(empty.ApplyBefore([] { return 42; }), 42);
    EXPECT_EQ(empty.ApplyAfter([](int n) { return n + 1; }, 41), 42);
}

TEST(TupleConstruction, ValuesAndDifferentElementTypes) {
    HUH::Tuple<int, std::string, double> tuple(17, std::string("hello"), 2.5);
    EXPECT_EQ(tuple.Get<0>(), 17);
    EXPECT_EQ(tuple.Get<1>(), "hello");
    EXPECT_DOUBLE_EQ(tuple.Get<2>(), 2.5);
}

TEST(TupleAccess, IndexAndUniqueTypeGet) {
    HUH::Tuple<int, std::string, double> tuple(12, std::string("value"), 4.5);
    EXPECT_EQ(tuple.Get<int>(), 12);
    EXPECT_EQ(tuple.Get<std::string>(), "value");
    EXPECT_DOUBLE_EQ(tuple.Get<double>(), 4.5);
    tuple.Get<std::string>() = "changed";
    EXPECT_EQ(tuple.Get<1>(), "changed");
}

TEST(TupleAccess, DuplicateTypesRemainAccessibleByIndex) {
    HUH::Tuple<int, int, int> tuple(1, 2, 3);
    EXPECT_EQ(tuple.Get<0>(), 1);
    EXPECT_EQ(tuple.Get<1>(), 2);
    EXPECT_EQ(tuple.Get<2>(), 3);
}

TEST(TupleAccess, CvQualifiedObjectsCanBeRead) {
    const HUH::Tuple<int, int> constTuple(10, 20);
    volatile HUH::Tuple<int, int> volatileTuple(30, 40);
    const volatile HUH::Tuple<int, int> cvTuple(50, 60);

    EXPECT_EQ(constTuple.Get<0>(), 10);
    EXPECT_EQ(volatileTuple.Get<1>(), 40);
    EXPECT_EQ(cvTuple.Get<0>(), 50);
    EXPECT_EQ(cvTuple.Get<1>(), 60);
}

TEST(TupleReferences, ReferenceElementsAliasTheOriginalObject) {
    int number = 10;
    std::string text = "before";
    HUH::Tuple<int&, std::string&> refs(number, text);

    refs.Get<0>() = 20;
    refs.Get<std::string&>() = "after";

    EXPECT_EQ(number, 20);
    EXPECT_EQ(text, "after");
    EXPECT_EQ(&refs.Get<0>(), &number);
    EXPECT_EQ(&refs.Get<1>(), &text);
}

TEST(TupleReferences, ConstTupleDoesNotMakeReferencedObjectsConst) {
    int number = 1;
    const HUH::Tuple<int&> refs(number);
    refs.Get<0>() = 2;
    EXPECT_EQ(number, 2);
    static_assert(std::same_as<decltype(std::declval<const HUH::Tuple<int&>&>().Get<0>()), int&>);
}

TEST(TupleReferences, ReferenceElementAssignmentsAreReportedExplicitly) {
    // A defaulted same-type assignment can be deleted by reference members.
    // Keep the test compilable on either implementation: if assignment is
    // supported, verify that it assigns through the references.
    using RefTuple = HUH::Tuple<int&>;
    int a = 1;
    int b = 9;
    RefTuple lhs(a);
    RefTuple rhs(b);
    const bool assignable = std::is_assignable_v<RefTuple&, const RefTuple&>;
    EXPECT_TRUE(assignable) << "Tuple<int&> should ideally assign through its reference element";
    TryAssign(lhs, rhs);
    if (assignable) {
        EXPECT_EQ(a, 9);
    } else {
        EXPECT_EQ(a, 1);
    }
}

TEST(TupleConversion, ConvertingCopyAndMoveConstruction) {
    HUH::Tuple<int, float> source(7, 3.5F);
    HUH::Tuple<long, double> copy(source);
    EXPECT_EQ(copy.Get<0>(), 7L);
    EXPECT_DOUBLE_EQ(copy.Get<1>(), 3.5);

    HUH::Tuple<int, std::string> movable(8, std::string("move me"));
    HUH::Tuple<long, std::string> moved(std::move(movable));
    EXPECT_EQ(moved.Get<0>(), 8L);
    EXPECT_EQ(moved.Get<1>(), "move me");
}

TEST(TupleAssignment, SameTypeCopyAndMoveAssignment) {
    HUH::Tuple<int, std::string> source(4, std::string("copy"));
    HUH::Tuple<int, std::string> destination(0, std::string("old"));
    destination = source;
    EXPECT_EQ(destination.Get<0>(), 4);
    EXPECT_EQ(destination.Get<1>(), "copy");

    HUH::Tuple<int, std::string> moveSource(9, std::string("move"));
    destination = std::move(moveSource);
    EXPECT_EQ(destination.Get<0>(), 9);
    EXPECT_EQ(destination.Get<1>(), "move");
}

TEST(TupleAssignment, HeterogeneousAssignmentWithElementAssignmentOperators) {
    HUH::Tuple<AssignmentBox<long>, AssignmentBox<long>> destination(AssignmentBox<long>(0), AssignmentBox<long>(0));
    HUH::Tuple<AssignmentBox<int>, AssignmentBox<int>> source(AssignmentBox<int>(11), AssignmentBox<int>(22));

    destination = source;
    EXPECT_EQ(destination.Get<0>().value, 11L);
    EXPECT_EQ(destination.Get<1>().value, 22L);

    HUH::Tuple<AssignmentBox<int>, AssignmentBox<int>> moveSource(AssignmentBox<int>(31), AssignmentBox<int>(42));
    destination = std::move(moveSource);
    EXPECT_EQ(destination.Get<0>().value, 31L);
    EXPECT_EQ(destination.Get<1>().value, 42L);
}

TEST(TupleMoveOnly, UniquePointersCanBeOwnedAndMoved) {
    HUH::Tuple<std::unique_ptr<int>> source(std::make_unique<int>(123));
    ASSERT_NE(source.Get<0>(), nullptr);
    EXPECT_EQ(*source.Get<0>(), 123);

    HUH::Tuple<std::unique_ptr<int>> destination;
    destination = std::move(source);
    ASSERT_NE(destination.Get<0>(), nullptr);
    EXPECT_EQ(*destination.Get<0>(), 123);
    EXPECT_EQ(source.Get<0>(), nullptr);
}

TEST(TupleStructuredBinding, LvalueBindingsReferToElements) {
    HUH::Tuple<int, std::string> tuple(3, std::string("original"));
    auto& [number, text] = tuple;
    number = 5;
    text = "updated";
    EXPECT_EQ(tuple.Get<0>(), 5);
    EXPECT_EQ(tuple.Get<1>(), "updated");
}

TEST(TupleTraits, TupleSizeAndElementTypes) {
    using T = HUH::Tuple<int, const std::string&, double>;
    static_assert(std::tuple_size_v<T> == 3);
    static_assert(std::same_as<std::tuple_element_t<0, T>, int>);
    static_assert(std::same_as<std::tuple_element_t<1, T>, const std::string&>);
    static_assert(std::same_as<std::tuple_element_t<2, T>, double>);
    SUCCEED();
}

TEST(TupleApply, BeforeAndAfterPlaceElementsInTheCorrectOrder) {
    HUH::Tuple<int, int> tuple(3, 4);
    const auto before = tuple.ApplyBefore([](int a, int b, int suffix) { return a * 100 + b * 10 + suffix; }, 5);
    const auto after = tuple.ApplyAfter([](int prefix, int a, int b) { return prefix * 100 + a * 10 + b; }, 2);
    EXPECT_EQ(before, 345);
    EXPECT_EQ(after, 234);
}

TEST(TupleApply, PreservesLvalueReferencesInCallableResults) {
    HUH::Tuple<int, int> tuple(7, 8);
    int& first = tuple.ApplyBefore([](int& a, int&) -> int& { return a; });
    first = 99;
    EXPECT_EQ(tuple.Get<0>(), 99);
}

TEST(TupleApply, WorksWithConstVolatileAndRvalueTuples) {
    const HUH::Tuple<int, int> constTuple(1, 2);
    volatile HUH::Tuple<int, int> volatileTuple(3, 4);
    const volatile HUH::Tuple<int, int> cvTuple(5, 6);

    EXPECT_EQ(constTuple.ApplyBefore([](int a, int b) { return a + b; }), 3);
    EXPECT_EQ(volatileTuple.ApplyBefore([](int a, int b) { return a + b; }), 7);
    EXPECT_EQ(cvTuple.ApplyAfter([](int a, int b, int suffix) { return a + b + suffix; }, 10), 21);

    HUH::Tuple<std::unique_ptr<int>> movable(std::make_unique<int>(77));
    const int result = std::move(movable).ApplyBefore([](std::unique_ptr<int> value) { return *value; });
    EXPECT_EQ(result, 77);
    EXPECT_EQ(movable.Get<0>(), nullptr);
}

TEST(TupleApply, SupportsMemberFunctionPointersThroughStdInvoke) {
    Accumulator accumulator{10};
    HUH::Tuple<Accumulator*, int> args(&accumulator, 5);
    EXPECT_EQ(args.ApplyBefore(&Accumulator::Add), 15);

    HUH::Tuple<int> oneArgument(8);
    EXPECT_EQ(oneArgument.ApplyAfter(&Accumulator::Add, &accumulator), 18);
}

TEST(TupleLifetime, OwnedElementsAreDestroyedExactlyOnce) {
    LifetimeTracked::Reset();
    {
        HUH::Tuple<LifetimeTracked, LifetimeTracked> tuple(LifetimeTracked(1), LifetimeTracked(2));
        EXPECT_EQ(LifetimeTracked::alive, 2);
        {
            auto copy = tuple;
            EXPECT_EQ(LifetimeTracked::alive, 4);
            auto moved = std::move(copy);
            EXPECT_EQ(LifetimeTracked::alive, 6);
            EXPECT_EQ(moved.Get<0>().value, 1);
            EXPECT_EQ(moved.Get<1>().value, 2);
        }
        EXPECT_EQ(LifetimeTracked::alive, 2);
    }
    EXPECT_EQ(LifetimeTracked::alive, 0);
    EXPECT_EQ(LifetimeTracked::constructed, LifetimeTracked::destroyed);
}

TEST(TupleComparisons, EqualityAndInequality) {
    using T = HUH::Tuple<MemberComparable, MemberComparable>;
    const T a(MemberComparable(1), MemberComparable(2));
    const T b(MemberComparable(1), MemberComparable(2));
    const T c(MemberComparable(1), MemberComparable(3));

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
}

TEST(TupleComparisons, RelationalOperatorsUseComponentWiseSemantics) {
    using T = HUH::Tuple<MemberComparable, MemberComparable>;
    const T a(MemberComparable(1), MemberComparable(2));
    const T b(MemberComparable(3), MemberComparable(4));
    const T mixed(MemberComparable(0), MemberComparable(4));

    EXPECT_TRUE(a < b);
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(b > a);
    EXPECT_TRUE(b >= a);
    EXPECT_FALSE(a < mixed);// The second element is not less: this is not lexicographic ordering.
    EXPECT_FALSE(a <= mixed);
}

}// namespace
