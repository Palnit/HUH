#include <HUH/Types/array.h>

#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

///
/// DISCLAIMER CHATGPT Generated tests I just needed some tests while i looked over it i don't claim that it tests all
/// paths
///

namespace {

template<typename T>
using Array = HUH::Array<T>;

struct Tracked {
    inline static int live = 0;
    inline static int constructed = 0;
    inline static int destroyed = 0;
    inline static int copied = 0;
    inline static int moved = 0;

    int value = 0;

    Tracked() { OnConstruct(); }
    explicit Tracked(int v) : value(v) { OnConstruct(); }

    Tracked(const Tracked& other) : value(other.value) {
        OnConstruct();
        ++copied;
    }

    Tracked(Tracked&& other) noexcept : value(other.value) {
        OnConstruct();
        ++moved;
        other.value = -1;
    }

    Tracked& operator=(const Tracked& other) {
        value = other.value;
        return *this;
    }

    Tracked& operator=(Tracked&& other) noexcept {
        if (this != &other) {
            value = other.value;
            other.value = -1;
        }
        return *this;
    }

    ~Tracked() {
        --live;
        ++destroyed;
    }

    static void ResetEvents() {
        constructed = 0;
        destroyed = 0;
        copied = 0;
        moved = 0;
    }

private:
    static void OnConstruct() {
        ++live;
        ++constructed;
    }
};

struct MoveOnly {
    std::unique_ptr<int> value;

    explicit MoveOnly(int v) : value(std::make_unique<int>(v)) {}
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
    MoveOnly(MoveOnly&&) noexcept = default;
    MoveOnly& operator=(MoveOnly&&) noexcept = default;
};

struct alignas(64) OverAligned {
    int value;
    explicit OverAligned(int v) : value(v) {}
};

struct ThrowingConstructor {
    explicit ThrowingConstructor(int) { throw std::runtime_error("deliberate test exception"); }
};

struct AllocationState {
    std::size_t allocations = 0;
    std::size_t deallocations = 0;
    std::size_t outstanding = 0;
    bool failNextAllocation = false;
};

// Matches the Allocate(bytes)/Deallocate(pointer) interface used by HUH::Array.
// Shared state lets tests observe allocations after allocator moves.
struct CountingAllocator {
    std::shared_ptr<AllocationState> state = std::make_shared<AllocationState>();

    CountingAllocator() = default;
    explicit CountingAllocator(std::shared_ptr<AllocationState> s) : state(std::move(s)) {}

    void* Allocate(std::size_t bytes) {
        if (state->failNextAllocation) {
            state->failNextAllocation = false;
            throw std::bad_alloc();
        }
        void* p = std::malloc(bytes == 0 ? 1 : bytes);
        if (p == nullptr) {
            throw std::bad_alloc();
        }
        ++state->allocations;
        ++state->outstanding;
        return p;
    }

    HUH_FORCE_INLINE static void* Reallocate(void* ptr, const size_t size) { return std::realloc(ptr, size); }

    void Deallocate(void* p) noexcept {
        if (p != nullptr) {
            ++state->deallocations;
            --state->outstanding;
            std::free(p);
        }
    }
};

TEST(ArrayTest, DefaultConstructionIsEmpty) {
    Array<int> a;

    EXPECT_EQ(a.Size(), 0u);
    EXPECT_EQ(a.size(), 0u);
    EXPECT_EQ(a.Capacity(), 0u);
    EXPECT_EQ(a.capacity(), 0u);
    EXPECT_TRUE(a.IsEmpty());
    EXPECT_TRUE(a.empty());
    EXPECT_EQ(a.data(), nullptr);
    EXPECT_EQ(a.GetData(), nullptr);
    EXPECT_EQ(a.begin(), a.end());
    EXPECT_EQ(a.rbegin(), a.rend());
}

TEST(ArrayTest, CountFillAndInitializerListConstructors) {
    Array<int> defaults(5);
    ASSERT_EQ(defaults.size(), 5u);
    for (const int value : defaults) {
        EXPECT_EQ(value, 0);
    }

    Array<int> filled(4, 17);
    ASSERT_EQ(filled.size(), 4u);
    for (const int value : filled) {
        EXPECT_EQ(value, 17);
    }

    Array<int> listed{2, 4, 6, 8};
    ASSERT_EQ(listed.size(), 4u);
    EXPECT_EQ(listed[0], 2);
    EXPECT_EQ(listed[1], 4);
    EXPECT_EQ(listed[2], 6);
    EXPECT_EQ(listed[3], 8);
}

TEST(ArrayTest, EmplaceReturnsIndexAndPreservesValuesWhenGrowing) {
    Array<int> a;
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(a.Emplace(i * 3), static_cast<std::size_t>(i));
        EXPECT_EQ(a[static_cast<std::size_t>(i)], i * 3);
    }

    ASSERT_EQ(a.size(), 100u);
    for (std::size_t i = 0; i < a.size(); ++i) {
        EXPECT_EQ(a[i], static_cast<int>(i * 3));
    }
    EXPECT_GE(a.capacity(), a.size());
}

TEST(ArrayTest, ReserveDoesNotChangeSizeOrConstructElements) {
    ASSERT_EQ(Tracked::live, 0);
    Tracked::ResetEvents();
    {
        Array<Tracked> a;
        a.Reserve(80);
        EXPECT_EQ(a.size(), 0u);
        EXPECT_GE(a.capacity(), 80u);
        EXPECT_EQ(Tracked::live, 0);
        EXPECT_EQ(Tracked::constructed, 0);

        a.Emplace(123);
        Tracked* oldData = a.data();
        a.Reserve(20);
        EXPECT_EQ(a.data(), oldData);
        EXPECT_EQ(a[0].value, 123);
        EXPECT_EQ(a.size(), 1u);
    }
    EXPECT_EQ(Tracked::live, 0);
    EXPECT_EQ(Tracked::constructed, Tracked::destroyed);
}

TEST(ArrayTest, ReserveOneThenGrowDoesNotWritePastAllocation) {
    Array<int> a;
    a.Reserve(1);
    a.Emplace(10);
    a.Emplace(20);// Important edge case: capacity 1 must grow beyond 1.
    a.Emplace(30);

    ASSERT_EQ(a.size(), 3u);
    EXPECT_EQ(a[0], 10);
    EXPECT_EQ(a[1], 20);
    EXPECT_EQ(a[2], 30);
    EXPECT_GE(a.capacity(), 3u);
}

TEST(ArrayTest, ClearDestroysElementsRetainsCapacityAndAllowsReuse) {
    ASSERT_EQ(Tracked::live, 0);
    Tracked::ResetEvents();
    {
        Array<Tracked> a;
        a.Emplace(1);
        a.Emplace(2);
        a.Emplace(3);
        const auto oldCapacity = a.capacity();

        a.Clear();
        EXPECT_TRUE(a.empty());
        EXPECT_EQ(a.size(), 0u);
        EXPECT_EQ(a.capacity(), oldCapacity);
        EXPECT_EQ(Tracked::live, 0);

        a.Emplace(9);
        ASSERT_EQ(a.size(), 1u);
        EXPECT_EQ(a[0].value, 9);
    }
    EXPECT_EQ(Tracked::live, 0);
    EXPECT_EQ(Tracked::constructed, Tracked::destroyed);
}

TEST(ArrayTest, CopyConstructionAssignmentAndSelfAssignment) {
    Array<int> source{1, 2, 3, 4};
    Array<int> copy(source);
    EXPECT_EQ(copy, source);

    Array<int> assigned{90, 91};
    assigned = source;
    EXPECT_EQ(assigned, source);

    assigned = assigned;
    EXPECT_EQ(assigned, source);

    Array<int> empty;
    assigned = empty;
    EXPECT_TRUE(assigned.empty());
    EXPECT_EQ(assigned, empty);
}

TEST(ArrayTest, MoveConstructionAndMoveAssignmentLeaveSourceEmpty) {
    Array<int> source{1, 2, 3};
    Array<int> moved(std::move(source));

    EXPECT_TRUE(source.empty());
    EXPECT_EQ(source.size(), 0u);
    EXPECT_EQ(source.capacity(), 0u);
    ASSERT_EQ(moved.size(), 3u);
    EXPECT_EQ(moved[0], 1);
    EXPECT_EQ(moved[2], 3);

    Array<int> destination{80, 81, 82};
    destination = std::move(moved);
    EXPECT_TRUE(moved.empty());
    ASSERT_EQ(destination.size(), 3u);
    EXPECT_EQ(destination[0], 1);
    EXPECT_EQ(destination[1], 2);
    EXPECT_EQ(destination[2], 3);
}

TEST(ArrayTest, MoveAssignmentReleasesDestinationBuffer) {
    auto state = std::make_shared<AllocationState>();
    {
        HUH::Array<int, CountingAllocator> destination(0, CountingAllocator{state});
        destination.Emplace(99);
        EXPECT_EQ(state->outstanding, 1u);

        HUH::Array<int, CountingAllocator> source(0, CountingAllocator{state});
        source.Emplace(1);
        source.Emplace(2);
        EXPECT_EQ(state->outstanding, 2u);

        destination = std::move(source);
        EXPECT_TRUE(source.empty());
        EXPECT_EQ(destination.size(), 2u);
        EXPECT_EQ(destination[0], 1);
        EXPECT_EQ(state->outstanding, 1u) << "move assignment must free the old destination allocation";
    }
    EXPECT_EQ(state->outstanding, 0u);
    EXPECT_EQ(state->allocations, state->deallocations);
}

TEST(ArrayTest, SelfMoveAssignmentDoesNotLeakItsAllocation) {
    auto state = std::make_shared<AllocationState>();
    {
        HUH::Array<int, CountingAllocator> a(0, CountingAllocator{state});
        a.Emplace(7);
        ASSERT_EQ(state->outstanding, 1u);
        a = std::move(a);
        // The post-self-move contents need not be asserted; the allocation must
        // still be owned and released (or have been released) by the container.
    }
    EXPECT_EQ(state->outstanding, 0u);
    EXPECT_EQ(state->allocations, state->deallocations);
}

TEST(ArrayTest, FailedReserveLeavesTheOriginalArrayStateIntact) {
    auto state = std::make_shared<AllocationState>();
    {
        HUH::Array<int, CountingAllocator> a(0, CountingAllocator{state});
        a.Reserve(4);
        a.Emplace(73);
        int* const oldData = a.data();
        const auto oldCapacity = a.capacity();

        state->failNextAllocation = true;
        EXPECT_THROW(a.Reserve(32), std::bad_alloc);
        EXPECT_EQ(a.data(), oldData);
        EXPECT_EQ(a.capacity(), oldCapacity);
        ASSERT_EQ(a.size(), 1u);
        EXPECT_EQ(a[0], 73);
    }
    EXPECT_EQ(state->outstanding, 0u);
    EXPECT_EQ(state->allocations, state->deallocations);
}

TEST(ArrayTest, MoveOnlyElementsSurviveGrowth) {
    Array<MoveOnly> a;
    for (int i = 0; i < 40; ++i) {
        a.Emplace(i + 100);
    }

    ASSERT_EQ(a.size(), 40u);
    for (int i = 0; i < 40; ++i) {
        ASSERT_NE(a[static_cast<std::size_t>(i)].value, nullptr);
        EXPECT_EQ(*a[static_cast<std::size_t>(i)].value, i + 100);
    }
}

TEST(ArrayTest, EmplaceCanCopyAnElementThatAliasesTheArrayDuringGrowth) {
    Array<int> a{10, 20, 30, 40};
    ASSERT_EQ(a.size(), a.capacity());

    a.Emplace(a[0]);
    ASSERT_EQ(a.size(), 5u);
    EXPECT_EQ(a[4], 10);
}

TEST(ArrayTest, ResizeGrowingDefaultConstructsAndShrinkingDestroysTail) {
    ASSERT_EQ(Tracked::live, 0);
    Tracked::ResetEvents();
    {
        Array<Tracked> a;
        a.Emplace(1);
        a.Emplace(2);
        a.Emplace(3);

        a.Resize(6);
        ASSERT_EQ(a.size(), 6u);
        EXPECT_EQ(a[0].value, 1);
        EXPECT_EQ(a[1].value, 2);
        EXPECT_EQ(a[2].value, 3);
        EXPECT_EQ(a[3].value, 0);
        EXPECT_EQ(a[4].value, 0);
        EXPECT_EQ(a[5].value, 0);

        a.Resize(2);
        ASSERT_EQ(a.size(), 2u);
        EXPECT_EQ(a[0].value, 1);
        EXPECT_EQ(a[1].value, 2);
        EXPECT_EQ(Tracked::live, 2);
    }
    EXPECT_EQ(Tracked::live, 0);
    EXPECT_EQ(Tracked::constructed, Tracked::destroyed);
}

TEST(ArrayTest, ResizeWithFillValue) {
    Array<int> a{1, 2};
    a.Resize(5, 9);

    ASSERT_EQ(a.size(), 5u);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 2);
    EXPECT_EQ(a[2], 9);
    EXPECT_EQ(a[3], 9);
    EXPECT_EQ(a[4], 9);

    a.Resize(3, 7);
    ASSERT_EQ(a.size(), 3u);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 2);
    EXPECT_EQ(a[2], 9);
}

TEST(ArrayTest, ShrinkToSizeChangesCapacityNotElements) {
    Array<int> a;
    a.Reserve(100);
    for (int i = 0; i < 10; ++i) {
        a.Emplace(i);
    }

    a.ShrinkToSize();
    EXPECT_EQ(a.capacity(), a.size());
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(a[static_cast<std::size_t>(i)], i);
    }
}

TEST(ArrayTest, InsertInitializerListAtBeginningMiddleAndEnd) {
    Array<int> a{1, 4};
    a.Insert(1, {2, 3});
    ASSERT_EQ(a.size(), 4u);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 2);
    EXPECT_EQ(a[2], 3);
    EXPECT_EQ(a[3], 4);

    a.Insert(0, {7, 8});
    ASSERT_EQ(a.size(), 6u);
    EXPECT_EQ(a[0], 7);
    EXPECT_EQ(a[1], 8);
    EXPECT_EQ(a[2], 1);

    a.Insert(a.size(), {9, 10});
    ASSERT_EQ(a.size(), 8u);
    EXPECT_EQ(a[6], 9);
    EXPECT_EQ(a[7], 10);
}

TEST(ArrayTest, ForwardIteratorIncrementDecrementAndOffset) {
    Array<int> a{11, 22, 33, 44};

    auto it = a.begin();
    EXPECT_EQ(*it, 11);
    EXPECT_EQ(*(it + 2), 33);
    ++it;
    EXPECT_EQ(*it, 22);
    it += 2;
    EXPECT_EQ(*it, 44);
    --it;
    EXPECT_EQ(*it, 33);
    it -= 2;
    EXPECT_EQ(*it, 11);

    auto end = a.end();
    --end;
    EXPECT_EQ(*end, 44);
}

TEST(ArrayTest, ReverseIteratorsVisitElementsFromBackToFront) {
    Array<int> a{1, 2, 3, 4};
    std::vector<int> actual;
    for (auto it = a.rbegin(); it != a.rend(); ++it) {
        actual.push_back(*it);
    }
    EXPECT_EQ(actual, (std::vector<int>{4, 3, 2, 1}));
}

TEST(ArrayTest, EqualityAndFindFunctions) {
    Array<int> a{5, 10, 15, 20};
    Array<int> same{5, 10, 15, 20};
    Array<int> other{5, 10, 15};

    EXPECT_TRUE(a == same);
    EXPECT_FALSE(a != same);
    EXPECT_FALSE(a == other);
    EXPECT_TRUE(a != other);

    EXPECT_EQ(a.FindIndex(15), 2u);
    EXPECT_EQ(a.FindIndex(99), static_cast<std::size_t>(-1));
    EXPECT_EQ(a.FindIndexByPred([](int value) { return value > 10; }), 2u);
    EXPECT_EQ(a.FindIndexByPred([](int value) { return value < 0; }), static_cast<std::size_t>(-1));
}

TEST(ArrayTest, ConstAccessAndByteCount) {
    Array<int> mutableArray{3, 4, 5};
    const Array<int>& a = mutableArray;

    EXPECT_EQ(a[1], 4);
    EXPECT_EQ(a.NumBytes(), 3u * sizeof(int));
    EXPECT_EQ(a.data(), mutableArray.data());
}

TEST(ArrayTest, StorageIsAlignedForOverAlignedTypes) {
    Array<OverAligned> a;
    a.Emplace(42);

    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(a.data()) % alignof(OverAligned), 0u);
    EXPECT_EQ(a[0].value, 42);
}

TEST(ArrayTest, FailedEmplaceDoesNotIncreaseSize) {
    Array<ThrowingConstructor> a;
    a.Reserve(4);

    EXPECT_THROW(a.Emplace(1), std::runtime_error);
    EXPECT_EQ(a.size(), 0u);
}

TEST(ArrayTest, RepeatedReserveClearAndDestructionBalanceAllocations) {
    auto state = std::make_shared<AllocationState>();
    {
        HUH::Array<int, CountingAllocator> a(0, CountingAllocator{state});
        for (int round = 0; round < 20; ++round) {
            a.Reserve(static_cast<std::size_t>(round * 8 + 1));
            for (int i = 0; i < round; ++i) {
                a.Emplace(i);
            }
            a.Clear();
        }
    }
    EXPECT_EQ(state->outstanding, 0u);
    EXPECT_EQ(state->allocations, state->deallocations);
}

// The current scalar Insert(index, value) overload has a compile-time bug in
// the attached implementation: it forwards its arguments in reverse order.
// Enable this test after fixing that overload to keep the regression covered.
#ifdef HUH_ARRAY_ENABLE_SCALAR_INSERT_REGRESSION
TEST(ArrayTest, ScalarInsertAtEveryPosition) {
    Array<int> a{1, 3};
    a.Insert(1, 2);
    EXPECT_EQ(a, (Array<int>{1, 2, 3}));
    a.Insert(0, 0);
    EXPECT_EQ(a, (Array<int>{0, 1, 2, 3}));
    a.Insert(a.size(), 4);
    EXPECT_EQ(a, (Array<int>{0, 1, 2, 3, 4}));
}
#endif

}// namespace
