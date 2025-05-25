#ifndef U512_H
#define U512_H

#include <array>

class u512
{
  private:
    static constexpr int SIZE = 16;
    std::array<unsigned int, SIZE> data;

  public:
    explicit u512(bool allOne = false)
    {
        if (allOne)
            data.fill(0xFFFFFFFF);
        else
            data.fill(0);
    }
    explicit u512(unsigned int i0,
                  unsigned int i1,
                  unsigned int i2,
                  unsigned int i3,
                  unsigned int i4,
                  unsigned int i5,
                  unsigned int i6,
                  unsigned int i7,
                  unsigned int i8,
                  unsigned int i9,
                  unsigned int i10,
                  unsigned int i11,
                  unsigned int i12,
                  unsigned int i13,
                  unsigned int i14,
                  unsigned int i15)
    {
        data = {i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15};
    }
    ~u512() = default;

    u512 &operator=(const u512 &other)
    {
        if (this != &other) {
            data = other.data;
        }
        return *this;
    }
    bool operator==(const u512 &other) const
    {
        return data == other.data;
    }
    bool operator!=(const u512 &other) const
    {
        return !(*this == other);
    }

    // 按 index 取出第 index 个 32bit 块（从0~15）
    unsigned int get(int index) const
    {
        if (index < 0 || index >= SIZE) {
            return 0;
        }
        return data[index];
    }

    // 按 index 设置第 index 个 32bit 块（从0~15）
    u512 set(int index, unsigned int value)
    {
        if (index < 0 || index >= SIZE) {
            return;
        }
        data[index] = value;
        return *this;
    }
};

#endif