#ifndef CPI_SNP_COMPONENT_H
#define CPI_SNP_COMPONENT_H

#include "../../conf/global_conf.h"

class cpi_snp_component
{
  public:
    static constexpr unsigned int NoOp = 0b000;
    static constexpr unsigned int SnpData = 0b001;
    static constexpr unsigned int SnpCur = 0b010;
    static constexpr unsigned int SnpInv = 0b011;

  private:
    u32 snptype;

  public:
    explicit cpi_snp_component(u32 snptype) : snptype(snptype)
    {
        CPP_ASSERT(this->snptype == NoOp || this->snptype == SnpData || this->snptype == SnpCur || this->snptype == SnpInv);
    }
    cpi_snp_component(const cpi_snp_component &other) = default;
    cpi_snp_component &operator=(const cpi_snp_component &other) = default;
    ~cpi_snp_component() = default;

  public:
    u32 get_snptype() const
    {
        return this->snptype;
    }
    std::string get_snptype_str() const
    {
        switch (this->snptype) {
            case NoOp:
                return "NoOp";
            case SnpData:
                return "SnpData";
            case SnpCur:
                return "SnpCur";
            case SnpInv:
                return "SnpInv";
            default:
                return "Unknown";
        }
    }

  private:  // Disabled
    void set_snptype(u32 snptype)
    {
        this->snptype = snptype;
        CPP_ASSERT(this->snptype == NoOp || this->snptype == SnpData || this->snptype == SnpCur || this->snptype == SnpInv);
    }
};

#endif