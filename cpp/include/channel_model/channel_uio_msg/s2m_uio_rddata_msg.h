#ifndef S2M_UIO_RDDATA_MSG_H
#define S2M_UIO_RDDATA_MSG_H

#include "../valid_component.h"

#include "../tools/print_tool.h"

class s2m_uio_rddata_msg : public valid_component
{
  private:
    static constexpr unsigned int RDERR_WIDTH = 1;
    static constexpr unsigned int RDTAGINDX_WIDTH = 8;
    u512 rddata;
    u32 rderr : 1;
    u32 rdtagidx : 8;

  public:
    explicit s2m_uio_rddata_msg(u32 valid, u512 rddata, u32 rderr, u32 rdtagidx)
        : valid_component(valid), rddata(rddata), rderr(rderr), rdtagidx(rdtagidx)
    {
        CPP_ASSERT(rderr < (1 << RDERR_WIDTH));
        CPP_ASSERT(rdtagidx < (1 << RDTAGINDX_WIDTH));
    }
    explicit s2m_uio_rddata_msg() : s2m_uio_rddata_msg(false, u512(), 0, 0)
    {
    }
    s2m_uio_rddata_msg(const s2m_uio_rddata_msg &other) = default;
    s2m_uio_rddata_msg &operator=(const s2m_uio_rddata_msg &other) = default;
    ~s2m_uio_rddata_msg() = default;

  public:
    u512 get_rddata() const
    {
        return rddata;
    }
    u32 get_rderr() const
    {
        return rderr;
    }
    u32 get_rdtagidx() const
    {
        return rdtagidx;
    }

  private:  // Disabled
    void set_rddata(u512 rddata)
    {
        this->rddata = rddata;
    }
    void set_rderr(u32 rderr)
    {
        this->rderr = rderr;
        CPP_ASSERT(rderr < (1 << RDERR_WIDTH));
    }
    void set_rdtagidx(u32 rdtagidx)
    {
        this->rdtagidx = rdtagidx;
        CPP_ASSERT(rdtagidx < (1 << RDTAGINDX_WIDTH));
    }

  public:
    void print() const
    {
        print_tool::start(print_tool::Direction::S2M, 0, get_rdtagidx(), "uio   :  rddata");
        print_tool::elecment("tag  :", "<", ">", get_rdtagidx(), 3);
        print_tool::elecment("data :", "[", "]", get_rddata().get(0), 3);
#ifndef SIMPLE_PRINT
        // to be added
#endif
        std::cout << std::endl;
    }
};

#endif