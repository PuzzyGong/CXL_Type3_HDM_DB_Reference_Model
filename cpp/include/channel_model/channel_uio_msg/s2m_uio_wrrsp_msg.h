#ifndef S2M_UIO_WRRSP_MSG_H
#define S2M_UIO_WRRSP_MSG_H

#include "../valid_component.h"

#include "../tools/print_tool.h"

class s2m_uio_wrrsp_msg : public valid_component
{
  private:
    static constexpr unsigned int WRERR_WIDTH = 1;
    static constexpr unsigned int WRTAGINDX_WIDTH = 8;
    u32 wrerr;
    u32 wrtagidx;

  public:
    explicit s2m_uio_wrrsp_msg(u32 valid, u32 wrderr, u32 wrtagidx) : valid_component(valid), wrerr(wrderr), wrtagidx(wrtagidx)
    {
        CPP_ASSERT(wrerr < (1 << WRERR_WIDTH));
        CPP_ASSERT(wrtagidx < (1 << WRTAGINDX_WIDTH));
    }
    explicit s2m_uio_wrrsp_msg() : s2m_uio_wrrsp_msg(false, 0, 0)
    {
    }
    s2m_uio_wrrsp_msg(const s2m_uio_wrrsp_msg &other) = default;
    s2m_uio_wrrsp_msg &operator=(const s2m_uio_wrrsp_msg &other) = default;
    ~s2m_uio_wrrsp_msg() = default;

  public:
    u32 get_wrerr() const
    {
        return wrerr;
    }
    u32 get_wrtagidx() const
    {
        return wrtagidx;
    }

  private:  // Disabled
    void set_wrerr(u32 wrderr)
    {
        wrerr = wrderr;
        CPP_ASSERT(wrerr < (1 << WRERR_WIDTH));
    }
    void set_wrtagidx(u32 wrtagidx)
    {
        this->wrtagidx = wrtagidx;
        CPP_ASSERT(wrtagidx < (1 << WRTAGINDX_WIDTH));
    }

    void print() const
    {
        print_tool::start(print_tool::Direction::S2M, 0, get_wrtagidx(), "uio   :  wrrsp");
        print_tool::elecment("tag  :", "<", ">", get_wrtagidx(), 3);
#ifndef SIMPLE_PRINT
        // to be added
#endif
        std::cout << std::endl;
    }
};

#endif