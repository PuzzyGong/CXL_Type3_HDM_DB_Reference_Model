#ifndef DEVICE_EMPTYDDR_H
#define DEVICE_EMPTYDDR_H

#include "device_base.h"

class device_emptyddr : public device_base
{
  private:
    static constexpr unsigned int MAP_MAX_SIZE = 1024;
    std::unordered_map<u64, std::pair<u512, u64>> ddr;
    static constexpr unsigned int LIST_MAX_SIZE = 40;
    std::list<d2s_axi_rddata_msg> axi_rddata_msg_list;
    std::list<d2s_axi_wrrsp_msg> axi_wrrsp_msg_list;

  public:
    device_emptyddr(const shared_var_pool &var_pool) : device_base(var_pool) {};
    ~device_emptyddr();

    void process(m2s_cpi_birsp_msg &cpi_birsp_msg,        // io
                 m2s_cpi_req_msg &cpi_req_msg,            // io
                 m2s_cpi_rwd_msg &cpi_rwd_msg,            // io
                 m2s_uio_rwaddr_msg &uio_rwaddr_msg,  // io
                 m2s_dut_msg &dut_msg,                // io

                 limited_list<s2m_cpi_bisnp_msg> &cpi_bisnp_msg_list,        // o
                 limited_list<s2m_cpi_ndr_msg> &cpi_ndr_msg_list,            // o
                 limited_list<s2m_cpi_drs_msg> &cpi_drs_msg_list,            // o
                 limited_list<s2m_uio_rddata_msg> &uio_rddata_msg_list,  // o
                 limited_list<s2m_uio_wrrsp_msg> &uio_wrrsp_msg_list,    // o

                 d2s_axi_rddata_msg &axi_rddata_msg,  // io
                 d2s_axi_wrrsp_msg &axi_wrrsp_msg,    // io

                 limited_list<s2d_axi_rdaddr_msg> &axi_rdaddr_msg_list,  // o
                 limited_list<s2d_axi_wraddr_msg> &axi_wraddr_msg_list   // o
                 ) override
    {
        if (axi_rddata_msg.get_valid() == true || axi_wrrsp_msg.get_valid() == true) {
            CPP_ERROR("in FILE device_emptyddr.h : axi_rddata_msg or axi_wrrsp_msg can not be valid in emptyddr Mode");
        }

        if (axi_rddata_msg_list.size() > 0) {
            axi_rddata_msg = axi_rddata_msg_list.front();
            axi_rddata_msg_list.pop_front();
        }
        if (axi_wrrsp_msg_list.size() > 0) {
            axi_wrrsp_msg = axi_wrrsp_msg_list.front();
            axi_wrrsp_msg_list.pop_front();
        }

        device_base::process(cpi_birsp_msg, cpi_req_msg, cpi_rwd_msg, uio_rwaddr_msg, dut_msg, cpi_bisnp_msg_list, cpi_ndr_msg_list, cpi_drs_msg_list,
                             uio_rddata_msg_list, uio_wrrsp_msg_list, axi_rddata_msg, axi_wrrsp_msg, axi_rdaddr_msg_list,
                             axi_wraddr_msg_list);

        for (const auto &axi_rdaddr_msg : axi_rdaddr_msg_list) {
            if (axi_rdaddr_msg.get_valid() == true) {
                if (axi_rddata_msg_list.size() >= LIST_MAX_SIZE) {
                    CPP_ERROR("in FILE device_emptyddr.h : axi_rddata_msg_list is full");
                }

                u512 data;
                u64 user;
                u64 key = axi_rdaddr_msg.get_address();
                auto it = ddr.find(key);
                if (it != ddr.end()) {
                    u512 data = it->second.first;
                    u64 user = it->second.second;
                } else {
                    data = u512(0);
                    user = 0;
                }
                axi_rddata_msg_list.push_back(d2s_axi_rddata_msg(true,                     // u32 valid,
                                                                 axi_rdaddr_msg.get_id(),  // u64 address,
                                                                 data,                     // u512 data,
                                                                 0,                        // u32 resp,
                                                                 user                      // u64 user
                                                                 ));
            }
        }
        for (const auto &axi_wraddr_msg : axi_wraddr_msg_list) {
            if (axi_wraddr_msg.get_valid() == true) {
                if (axi_wrrsp_msg_list.size() >= LIST_MAX_SIZE) {
                    CPP_ERROR("in FILE device_emptyddr.h : axi_wrrsp_msg_list is full");
                }
                u64 key = axi_wraddr_msg.get_address();
                ddr[key] = std::make_pair(axi_wraddr_msg.get_data(), axi_wraddr_msg.get_user());

                u64 now_unknown_user = 0;  // Unknown
                axi_wrrsp_msg_list.push_back(d2s_axi_wrrsp_msg(true, axi_wraddr_msg.get_id(), 0,
                                                               now_unknown_user));  // u32 valid, u32 id, u32 resp, u64 user
            }
        }
    }
};

#endif