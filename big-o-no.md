# Big O No

---

# The Setup

```c++
// 28 bytes
struct Entity {
    float x_, y_, z_;
    float w_, h_;
    bool alive_;
};

class World {
public:
    std::vector<Entity> entities_;

    // ...
};
```

---

[.code-highlight: all]
[.code-highlight: 5]
[.code-highlight: 8-16]

```c++
void populate(const int count) {
    std::mt19937 gen;
    gen.seed(1);

    std::bernoulli_distribution dist(0.5); // 50/50 alive or dead

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
```
