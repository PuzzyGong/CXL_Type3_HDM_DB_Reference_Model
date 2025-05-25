#ifndef CPI_TAG_COMPONENT_H
#define CPI_TAG_COMPONENT_H

#include "../../conf/global_conf.h"

class cpi_tag_component
{
  private:
    static constexpr unsigned int TAG_WIDTH = 16;
    u32 tag;

  public:
    explicit cpi_tag_component(u32 tag) : tag(tag)
    {
        CPP_ASSERT(this->tag < (1 << TAG_WIDTH));
    }
    cpi_tag_component(const cpi_tag_component &other) = default;
    cpi_tag_component &operator=(const cpi_tag_component &other) = default;
    ~cpi_tag_component() = default;

  public:
    u32 get_tag() const
    {
        return this->tag;
    }

  private:  // Disabled
    void set_tag(u32 tag)
    {
        this->tag = tag;
        CPP_ASSERT(this->tag < (1 << TAG_WIDTH));
    }
};

#endif