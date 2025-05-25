#ifndef SHARED_VAR_POOL_H
#define SHARED_VAR_POOL_H

#include <iostream>       // 用于std::cout
#include <unordered_map>  // 用于std::unordered_map
#include <string>         // 用于std::string
#include <vector>         // 用于std::vector
#include <cstdint>
#include <cstring>
#include <array>

#include "../../conf/global_conf.h"

class shared_var_pool
{
  public:
    shared_var_pool();
    ~shared_var_pool();

    shared_var_pool(const std::string &file_name){};

    // const类型，禁止直接通过更改此指针指向的内容操作内存，只能通过set

    /*decoder related var pool*/
    inline const void *get_ld_cap_reg(int ld_id) const
    {
        validate_ld(ld_id);
        return ld_cap_regs[ld_id].data();
    }

    inline bool set_ld_cap_reg(int ld_id, const void *cap_reg_value)
    {
        bool res = true;
        validate_ld(ld_id);
        if (!cap_reg_value) {
            std::cout << "cap_reg_value pointer cannot be null" << std::endl;
            return false;
        }
        std::memcpy(ld_cap_regs[ld_id].data(), cap_reg_value, CAP_REG_BYTE_SIZE);
        return res;
    }

    inline const void *get_ld_range_reg(int ld_id) const
    {
        validate_ld(ld_id);
        return ld_range_regs[ld_id].data();
    }

    inline bool set_ld_range_reg(int ld_id, const void *range_reg_value)
    {
        bool res = true;
        validate_ld(ld_id);
        if (!range_reg_value) {
            std::cout << "range_reg_value pointer cannot be null" << std::endl;
            return false;
        }
        std::memcpy(ld_range_regs[ld_id].data(), range_reg_value, RANGE_REG_BYTE_SIZE);
        return true;
    }

    /*dcd related var pool*/
    inline bool set_dc_config(int dc_region_index, const std::string &var_name, uint64_t value)
    {
        bool res = false;
        validate_dc_region(dc_region_index);
        if (dc_config_reg[dc_region_index].find(var_name) != dc_config_reg[dc_region_index].end()) {
            dc_config_reg[dc_region_index][var_name] = value;
            return true;
        }

        return res;
    }

    inline uint64_t get_dc_config(int dc_region_index, const std::string &var_name)
    {
        validate_dc_region(dc_region_index);
        if (dc_config_reg[dc_region_index].find(var_name) != dc_config_reg[dc_region_index].end()) {
            return dc_config_reg[dc_region_index][var_name];
        }
        return UNKNOWN_DC_CONFG;
    }

    inline bool add_ld_bit_map(int dc_region_index, uint16_t ld_priv_mask)
    {
        bool res = true;
        validate_dc_region(dc_region_index);
        ld_bit_map[dc_region_index].push_back(ld_priv_mask);
        return res;
    }

    inline bool set_ld_bit_map(int dc_region_index, int inner_block_index, uint16_t ld_priv_mask)
    {
        bool res = true;
        validate_block_index(dc_region_index, inner_block_index);
        ld_bit_map[dc_region_index][inner_block_index] = ld_priv_mask;
        return res;
    }

    inline uint16_t get_ld_bit_map(int dc_region_index, int inner_block_index)
    {
        uint16_t res;
        // double check
        validate_block_index(dc_region_index, inner_block_index);
        res = ld_bit_map[dc_region_index][inner_block_index];
        return res;
    }

    inline bool set_mgt_config(int pid_index, uint8_t value)
    {
        bool res = true;
        validate_pid(pid_index);
        mgt[pid_index] = value;
        return res;
    }

    inline uint8_t get_mgt_config(int pid_index)
    {
        uint8_t res;
        validate_pid(pid_index);
        res = mgt[pid_index];
        // res contain 6 valid bits
        return res;
    }

    inline bool set_sat_config(int pid_index, uint32_t value)
    {
        bool res = true;
        validate_pid(pid_index);
        sat[pid_index] = value;
        // res have 32 valid bits
        return res;
    }

    inline uint32_t get_sat_config(int pid_index)
    {
        uint32_t res;
        validate_pid(pid_index);
        res = sat[pid_index];
        return res;
    }

    /*decoder related var pool*/
  private:
    static constexpr int LD_MAX_NUM = 16;
    // LD_CAP_REG
    static constexpr int DECODER_PER_LD = 8;
    static constexpr int SING_CAP_REG_BIT_SIZE = 32;
    static constexpr int CAP_REG_COMMON_OFFSET = 0x10;
    static constexpr int CAP_REG_BIT_SIZE = (CAP_REG_COMMON_OFFSET + (DECODER_PER_LD * SING_CAP_REG_BIT_SIZE));
    static constexpr int CAP_REG_BYTE_SIZE = CAP_REG_BIT_SIZE / 8;
    // LD_RANGE_REG
    static constexpr int RANGE_PER_LD = 2;
    static constexpr int REG_TYPES_PER_RANGE = 4;
    static constexpr int SING_RANGE_REG_BIT_SIZE = 32;
    static constexpr int RANGE_REG_BIT_SIZE = (RANGE_PER_LD * REG_TYPES_PER_RANGE * SING_RANGE_REG_BIT_SIZE);
    static constexpr int RANGE_REG_BYTE_SIZE = RANGE_REG_BIT_SIZE / 8;
    // regs
    std::array<std::array<uint8_t, CAP_REG_BYTE_SIZE>, LD_MAX_NUM> ld_cap_regs{};
    std::array<std::array<uint8_t, RANGE_REG_BYTE_SIZE>, LD_MAX_NUM> ld_range_regs{};

    /*dcd related var pool*/
  private:
    static constexpr int PID_MAX_NUM = 64;
    static constexpr int DC_REGION_MAX_NUM = 8;
    static constexpr uint64_t UNKNOWN_DC_CONFG = 0xFFFFFFFF;

    // key: var name
    std::array<std::unordered_map<std::string, uint64_t>, DC_REGION_MAX_NUM> dc_config_reg{};
    std::array<uint8_t, PID_MAX_NUM> mgt{};
    std::array<uint32_t, PID_MAX_NUM> sat{};
    std::array<std::vector<uint16_t>, DC_REGION_MAX_NUM> ld_bit_map{};

    /*validation related API*/
  private:
    void validate_ld(int ld_num) const
    {
        if (ld_num < 0 || ld_num >= LD_MAX_NUM) {
            throw std::out_of_range("ld_num out of range");
        }
    }
    void validate_dc_region(int dc_region_index)
    {
        if (dc_region_index < 0 || dc_region_index >= DC_REGION_MAX_NUM) {
            throw std::out_of_range("dc_region_index out of range");
        }
    }
    void validate_block_index(int dc_region_index, int inner_block_index)
    {
        int size = 0;
        if (dc_region_index < 0 || dc_region_index >= DC_REGION_MAX_NUM) {
            throw std::out_of_range("dc_region_index out of range");
        }
        size = ld_bit_map[dc_region_index].size();
        if (inner_block_index >= size || inner_block_index < 0) {
            throw std::out_of_range("inner_block_index out of range");
        }
    }
    void validate_pid(int pid_index)
    {
        if (pid_index < 0 || pid_index >= PID_MAX_NUM) {
            throw std::out_of_range("pid_index out of range , adpative to 0~63");
        }
    }
};

#endif