#include <vector>
#include <random>
#include <iostream>

#include "benchmark/benchmark.h"

struct Entity {
    float x_, y_, z_;
    float w_, h_;
    bool alive_;
};

class World {
public:
    void populate(const int count) {
        std::mt19937 gen;
        gen.seed(1);

        std::bernoulli_distribution dist;

        entities_.reserve(count);
        std::generate_n(
            std::back_inserter(entities_),
            count, [&]{
            return Entity {
                0.0f, 0.0f, 0.0f,
                1.0f, 1.0f,
                dist(gen)
            };
        });
    }

    void removeDeadEntities_erase() {
        for (auto it = entities_.begin(); it != entities_.end();) {
            if (!it->alive_) {
                it = entities_.erase(it);
            } else {
                ++it;
            }
        }
    }

    void removeDeadEntities_erase_reverse() {
        for (auto it = entities_.rbegin(); it != entities_.rend(); ++it) {
            if (!it->alive_) {
                entities_.erase(std::next(it).base());
            }
        }
    }

    void removeDeadEntities_erase_reverse_index() {
        for (int i = entities_.size() - 1; i >= 0; --i) {
            if (!entities_[i].alive_) {
                entities_.erase(entities_.begin() + i);
            }
        }
    }

    void removeDeadEntities_copy_for() {
        std::vector<Entity> entities;
        entities.reserve(entities_.size()); // over allocate
        // entities.reserve(
        //     std::count_if(entities_.cbegin(), entities_.cend(),
        //         [](const Entity& entity) { return entity.alive_; }));
        for (const auto& entity : entities_) {
            if (entity.alive_) {
                entities.push_back(entity);
            }
        }
        // entities_ = entities;
        entities_ = std::move(entities);
    }

    void removeDeadEntities_copy_if() {
        std::vector<Entity> entities;
        entities.reserve(entities_.size()); // over allocate
        std::copy_if(
            entities_.begin(), entities_.end(),
            std::back_inserter(entities),
            [](const Entity& entity) { return entity.alive_; });
        entities_ = std::move(entities);
    }

    void removeDeadEntities_swap() {
        for (int i = 0; i < entities_.size();) {
            if (!entities_[i].alive_) {
                // entities_[i] = entities_.back();
                std::swap(entities_[i], entities_.back());
                entities_.pop_back();
            } else {
                i++;
            }
        }
    }

    void removeDeadEntities_remove() {
        entities_.erase(
            std::remove_if(
                entities_.begin(), entities_.end(),
                [](const Entity& entity) { return !entity.alive_; }),
            entities_.end());
    }

    void removeDeadEntities_partition() {
        entities_.erase(
            std::partition(
                entities_.begin(), entities_.end(),
                [](const Entity& e) { return e.alive_; }),
            entities_.end());
    }

    void depopulate() {
        std::vector<Entity>().swap(entities_);
    }

    std::vector<Entity> entities_;
};

#if 0
int main(int argc, char** argv) {

    std::cout << sizeof(Entity) << "\n";

    // sanity check

    const int EntityCount = 100000;

    {
        World world;
        world.populate(EntityCount);
        world.removeDeadEntities_erase();
        std::cout << world.entities_.size() << "\n";
        world.depopulate();
    }

    {
        World world;
        world.populate(EntityCount);
        world.removeDeadEntities_erase_reverse();
        std::cout << world.entities_.size() << "\n";
        world.depopulate();
    }

    {
        World world;
        world.populate(EntityCount);
        world.removeDeadEntities_erase_reverse_index();
        std::cout << world.entities_.size() << "\n";
        world.depopulate();
    }

    {
        World world;
        world.populate(EntityCount);
        world.removeDeadEntities_copy_for();
        std::cout << world.entities_.size() << "\n";
        world.depopulate();
    }

    {
        World world;
        world.populate(EntityCount);
        world.removeDeadEntities_copy_if();
        std::cout << world.entities_.size() << "\n";
        world.depopulate();
    }

    {
        World world;
        world.populate(EntityCount);
        world.removeDeadEntities_swap();
        std::cout << world.entities_.size() << "\n";
        world.depopulate();
    }

    {
        World world;
        world.populate(EntityCount);
        world.removeDeadEntities_remove();
        std::cout << world.entities_.size() << "\n";
        world.depopulate();
    }

    {
        World world;
        world.populate(EntityCount);
        world.removeDeadEntities_partition();
        std::cout << world.entities_.size() << "\n";
        world.depopulate();
    }
}
#endif

#if 1

class WorldFixture
    : public benchmark::Fixture
{
public:
  void SetUp(const ::benchmark::State& state) {
      world.populate(state.range(0));
  }

  void TearDown(const ::benchmark::State& state) {
      world.depopulate();
  }

  World world;
};

static const int EntityCountBegin = 10000;
static const int EntityCountEnd = 200000;
static const int RangeMultiplier = 2;

BENCHMARK_DEFINE_F(WorldFixture, Erase)(benchmark::State& state)
{
    for (auto _ : state)
    {
        world.removeDeadEntities_erase();
    }
}

BENCHMARK_REGISTER_F(WorldFixture, Erase)
    ->Unit(benchmark::kMillisecond)
    ->RangeMultiplier(RangeMultiplier)
    ->Range(EntityCountBegin, EntityCountEnd);

BENCHMARK_DEFINE_F(WorldFixture, EraseReverse)(benchmark::State& state)
{
    for (auto _ : state)
    {
        world.removeDeadEntities_erase_reverse();
    }
}

BENCHMARK_REGISTER_F(WorldFixture, EraseReverse)
    ->Unit(benchmark::kMillisecond)
    ->RangeMultiplier(RangeMultiplier)
    ->Range(EntityCountBegin, EntityCountEnd);

BENCHMARK_DEFINE_F(WorldFixture, EraseReverseIndex)(benchmark::State& state)
{
    for (auto _ : state)
    {
        world.removeDeadEntities_erase_reverse_index();
    }
}

BENCHMARK_REGISTER_F(WorldFixture, EraseReverseIndex)
    ->Unit(benchmark::kMillisecond)
    ->RangeMultiplier(RangeMultiplier)
    ->Range(EntityCountBegin, EntityCountEnd);

BENCHMARK_DEFINE_F(WorldFixture, CopyFor)(benchmark::State& state)
{
    for (auto _ : state)
    {
        world.removeDeadEntities_copy_for();
    }
}

BENCHMARK_REGISTER_F(WorldFixture, CopyFor)
    ->Unit(benchmark::kMillisecond)
    ->RangeMultiplier(RangeMultiplier)
    ->Range(EntityCountBegin, EntityCountEnd);

BENCHMARK_DEFINE_F(WorldFixture, CopyIf)(benchmark::State& state)
{
    for (auto _ : state)
    {
        world.removeDeadEntities_copy_if();
    }
}

BENCHMARK_REGISTER_F(WorldFixture, CopyIf)
    ->Unit(benchmark::kMillisecond)
    ->RangeMultiplier(RangeMultiplier)
    ->Range(EntityCountBegin, EntityCountEnd);

BENCHMARK_DEFINE_F(WorldFixture, Swap)(benchmark::State& state)
{
    for (auto _ : state)
    {
        world.removeDeadEntities_swap();
    }
}

BENCHMARK_REGISTER_F(WorldFixture, Swap)
    ->Unit(benchmark::kMillisecond)
    ->RangeMultiplier(RangeMultiplier)
    ->Range(EntityCountBegin, EntityCountEnd);

BENCHMARK_DEFINE_F(WorldFixture, Remove)(benchmark::State& state)
{
    for (auto _ : state)
    {
        world.removeDeadEntities_remove();
    }
}

BENCHMARK_REGISTER_F(WorldFixture, Remove)
    ->Unit(benchmark::kMillisecond)
    ->RangeMultiplier(RangeMultiplier)
    ->Range(EntityCountBegin, EntityCountEnd);

BENCHMARK_DEFINE_F(WorldFixture, Partition)(benchmark::State& state)
{
    for (auto _ : state)
    {
        world.removeDeadEntities_partition();
    }
}

BENCHMARK_REGISTER_F(WorldFixture, Partition)
    ->Unit(benchmark::kMillisecond)
    ->RangeMultiplier(RangeMultiplier)
    ->Range(EntityCountBegin, EntityCountEnd);

#endif
