#ifndef CPI_META_COMPONENT_H
#define CPI_META_COMPONENT_H

#include "../../conf/global_conf.h"

class cpi_meta_component
{
  public:
    static constexpr unsigned int FIELD_Meta0State = 0b00;
    static constexpr unsigned int FIELD_EMS = 0b01;
    static constexpr unsigned int FIELD_NoOp = 0b11;
    static constexpr unsigned int VALUE_I = 0b00;
    static constexpr unsigned int VALUE_NoOp = 0b01;
    static constexpr unsigned int VALUE_A = 0b10;
    static constexpr unsigned int VALUE_S = 0b11;

  private:
    u32 metafield;
    u32 metavalue;

  public:
    explicit cpi_meta_component(u32 metafield, u32 metavalue) : metafield(metafield), metavalue(metavalue)
    {
        CPP_ASSERT(this->metafield == FIELD_Meta0State || this->metafield == FIELD_EMS || this->metafield == FIELD_NoOp);
        CPP_ASSERT(this->metavalue == VALUE_I || this->metavalue == VALUE_NoOp || this->metavalue == VALUE_A
                   || this->metavalue == VALUE_S);
    }
    cpi_meta_component(const cpi_meta_component &other) = default;
    cpi_meta_component &operator=(const cpi_meta_component &other) = default;
    ~cpi_meta_component() = default;

  public:
    u32 get_metafield() const
    {
        return this->metafield;
    }
    u32 get_metavalue() const
    {
        return this->metavalue;
    }
    std::string get_metafield_str() const
    {
        switch (this->metafield) {
            case FIELD_Meta0State:
                return "Meta0State";
            case FIELD_EMS:
                return "EMS";
            case FIELD_NoOp:
                return "NoOp";
            default:
                return "Unknown";
        }
    }
    std::string get_metavalue_str() const
    {
        switch (this->metavalue) {
            case VALUE_I:
                return "I";
            case VALUE_NoOp:
                return "NoOp";
            case VALUE_A:
                return "A";
            case VALUE_S:
                return "S";
            default:
                return "Unknown";
        }
    }

  private:  // Disabled
    void set_metafield(u32 metafield)
    {
        this->metafield = metafield;
        CPP_ASSERT(this->metafield == FIELD_Meta0State || this->metafield == FIELD_EMS || this->metafield == FIELD_NoOp);
    }
    void set_metavalue(u32 metavalue)
    {
        this->metavalue = metavalue;
        CPP_ASSERT(this->metavalue == VALUE_I || this->metavalue == VALUE_NoOp || this->metavalue == VALUE_A
                   || this->metavalue == VALUE_S);
    }
};

#endif