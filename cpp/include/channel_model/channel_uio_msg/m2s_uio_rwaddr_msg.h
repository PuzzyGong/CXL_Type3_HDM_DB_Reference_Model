#ifndef M2S_UIO_RWADDR_MSG_H
#define M2S_UIO_RWADDR_MSG_H

#include "../valid_component.h"

#include "../tools/print_tool.h"

class m2s_uio_rwaddr_msg : public valid_component
{
  private:
    static constexpr unsigned int OPCODE_WIDTH = 1;
    static constexpr unsigned int SPID_WIDTH = 12;
    static constexpr unsigned int DPID_WIDTH = 12;
    static constexpr unsigned int ADDRESS_WIDTH = 46;
    static constexpr unsigned int METAVALUE_WIDTH = 2;
    static constexpr unsigned int TAGIDX_WIDTH = 8;
    u32 opcode;  // 0: read, 1: write
    u32 spid;
    u32 dpid;
    u64 address;
    u512 data;
    u32 metavalue;
    u32 tagidx;

  public:
    explicit m2s_uio_rwaddr_msg(u32 valid, u32 opcode, u32 spid, u32 dpid, u64 address, u512 data, u32 metavalue, u32 tagidx)
        : valid_component(valid), opcode(opcode), spid(spid), dpid(dpid), address(address), data(data), metavalue(metavalue),
          tagidx(tagidx)
    {
        CPP_ASSERT(opcode < (1 << OPCODE_WIDTH));
        CPP_ASSERT(spid < (1 << SPID_WIDTH));
        CPP_ASSERT(dpid < (1 << DPID_WIDTH));
        CPP_ASSERT(address < (1 << ADDRESS_WIDTH));
        CPP_ASSERT(metavalue < (1 << METAVALUE_WIDTH));
        CPP_ASSERT(tagidx < (1 << TAGIDX_WIDTH));
    }
    explicit m2s_uio_rwaddr_msg() : m2s_uio_rwaddr_msg(false, 0, 0, 0, 0, u512(0), 0, 0)
    {
    }
    m2s_uio_rwaddr_msg(const m2s_uio_rwaddr_msg &other) = default;
    m2s_uio_rwaddr_msg &operator=(const m2s_uio_rwaddr_msg &other) = default;
    ~m2s_uio_rwaddr_msg() = default;

  public:
    u32 get_opcode() const
    {
        return opcode;
    }
    u32 get_spid() const
    {
        return spid;
    }
    u32 get_dpid() const
    {
        return dpid;
    }
    u64 get_address() const
    {
        return address;
    }
    u32 get_address_low() const
    {
        return address & 0xffffffff;
    }
    u32 get_address_high() const
    {
        return address >> 32;
    }
    u512 get_data() const
    {
        return data;
    }
    u32 get_metavalue() const
    {
        return metavalue;
    }
    u32 get_tagidx() const
    {
        return tagidx;
    }

  public:
    void set_spid(u32 spid)
    {
        this->spid = spid;
        CPP_ASSERT(spid < (1 << 12));
    }
    void set_address(u64 address)
    {
        this->address = address;
        CPP_ASSERT(address < (1 << 46));
    }

  private:  // Disabled
    void set_opcode(u32 opcode)
    {
        this->opcode = opcode;
        CPP_ASSERT(opcode < (1 << OPCODE_WIDTH));
    }
    void set_dpid(u32 dpid)
    {
        this->dpid = dpid;
        CPP_ASSERT(dpid < (1 << DPID_WIDTH));
    }
    void set_data(u512 data)
    {
        this->data = data;
    }
    void set_metavalue(u32 metavalue)
    {
        this->metavalue = metavalue;
        CPP_ASSERT(metavalue < (1 << METAVALUE_WIDTH));
    }
    void set_tagidx(u32 tagidx)
    {
        this->tagidx = tagidx;
        CPP_ASSERT(tagidx < (1 << TAGIDX_WIDTH));
    }

  public:
    void print() const
    {
        print_tool::start(print_tool::Direction::M2S, 0, get_tagidx(), "uio   :  " + opcode ? "write" : "read");
        print_tool::elecment("tag  :", "<", ">", get_tagidx(), 3);
        print_tool::elecment("host :", "$", "$", get_spid(), 3);
        print_tool::elecment("addr :", "(", ")", get_address(), 3);
#ifndef SIMPLE_PRINT
        // to be added
#endif
        std::cout << std::endl;
    }
};

#endif