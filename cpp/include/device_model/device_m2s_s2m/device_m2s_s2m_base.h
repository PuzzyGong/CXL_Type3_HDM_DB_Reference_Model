#ifndef DEVICE_M2S_S2M_BASE_H
#define DEVICE_M2S_S2M_BASE_H

#include "base/device_gfam_base.h"
#include "base/device_ldfam_base.h"
#include "decoder/gfam/device_gfam_decoders.h"
#include "decoder/ldfam/device_ldfam_decoders.h"
#include "dcd/gfam/device_gfam_dcd.h"
#include "dcd/ldfam/device_ldfam_dcd.h"

/*
    device_gfam_base, device_ldfam_base
    device_gfam_decoders, device_ldfam_decoders
    device_gfam_dcd, device_ldfam_dcd
    中选择两个组合
*/
class device_m2s_s2m_base
{
  private:
    shared_var_pool var_pool;
    std::unique_ptr<device_gfam_base> gfam_instance;
    std::unique_ptr<device_ldfam_base> ldfam_instance;

  public:
    explicit device_m2s_s2m_base(const shared_var_pool &var_pool, bool decoder_OR_dcd, bool first_base, bool second_base)
        : var_pool(var_pool)
    {
        if (decoder_OR_dcd == 0) {
            if (first_base == 0) {
                gfam_instance = std::make_unique<device_gfam_decoders>(var_pool);
            } else {
                gfam_instance = std::make_unique<device_gfam_base>(var_pool);
            }
            if (second_base == 0) {
                ldfam_instance = std::make_unique<device_ldfam_decoders>(var_pool);
            } else {
                ldfam_instance = std::make_unique<device_ldfam_base>(var_pool);
            }
        } else {
            if (first_base == 0) {
                gfam_instance = std::make_unique<device_gfam_dcd>(var_pool);
            } else {
                gfam_instance = std::make_unique<device_gfam_base>(var_pool);
            }
            if (second_base == 0) {
                ldfam_instance = std::make_unique<device_ldfam_dcd>(var_pool);
            } else {
                ldfam_instance = std::make_unique<device_ldfam_base>(var_pool);
            }
        }
    }

    virtual void m2s_process(m2s_cpi_birsp_msg &cpi_birsp_msg,    // io
                             m2s_cpi_req_msg &cpi_req_msg,        // io
                             m2s_cpi_rwd_msg &cpi_rwd_msg,        // io
                             m2s_uio_rwaddr_msg &uio_rwaddr_msg,  // io

                             limited_list<s2m_cpi_bisnp_msg> &cpi_bisnp_msg_list,    // o
                             limited_list<s2m_cpi_ndr_msg> &cpi_ndr_msg_list,        // o
                             limited_list<s2m_cpi_drs_msg> &cpi_drs_msg_list,        // o
                             limited_list<s2m_uio_rddata_msg> &uio_rddata_msg_list,  // o
                             limited_list<s2m_uio_wrrsp_msg> &uio_wrrsp_msg_list     // o
    )
    {
        cpi_bisnp_msg_list.clear();
        cpi_ndr_msg_list.clear();
        cpi_drs_msg_list.clear();
        uio_rddata_msg_list.clear();
        uio_wrrsp_msg_list.clear();

        u512 databody_error(1);
        u64 enable_error = 0xFFFFFFFFFFFFFFF;
        u32 trp_error = 1;
        u32 emd_error = 0xFFFFFFFF;

        /*
            目前不考虑 Poison_Enable 是 0 还是 1，而且只返回 poison = 1
        */
        u32 poison = 1;

        /*
            目前不考虑 MemDataNXM_Capable 是 0 还是 1，而且只返回 DRS，不返回 NDR
        */
        if (cpi_req_msg.get_valid()) {
            u64 dpa = 0;
            if (cpi_req_msg.get_flitmode() == FLITMODE_68) {
                if (ldfam_instance->m2s_process(cpi_req_msg.get_address(), cpi_req_msg.get_ldid(), dpa))
                    cpi_req_msg.set_address(dpa);
                else {
                    cpi_req_msg.set_valid(false);
                    cpi_drs_msg_list.push_back(s2m_cpi_drs_msg(1,                               //(u32 valid,
                                                               cpi_req_msg.get_flitmode(),      // u32 flitmode,
                                                               s2m_cpi_drs_msg::MemDataNXM,     // u32 opcode,
                                                               cpi_meta_component::FIELD_NoOp,  // u32 metafield,
                                                               cpi_meta_component::VALUE_NoOp,  // u32 metadata,
                                                               cpi_req_msg.get_tag(),           // u32 tag,
                                                               cpi_req_msg.get_ldid(),          // u32 spid_ldid,
                                                               cpi_req_msg.get_spid(),          // u32 dpid,
                                                               databody_error,                  // u512 databody,
                                                               enable_error,                    // u64 enable,
                                                               poison                           // u32 poison)
                                                               ));
                }
            } else if (cpi_req_msg.get_flitmode() == FLITMODE_256) {
                if (ldfam_instance->m2s_process(cpi_req_msg.get_address(), cpi_req_msg.get_ldid(), dpa))
                    cpi_req_msg.set_address(dpa);
                else {
                    cpi_req_msg.set_valid(false);
                    cpi_drs_msg_list.push_back(s2m_cpi_drs_msg(1,                               //(u32 valid,
                                                               cpi_req_msg.get_flitmode(),      // u32 flitmode,
                                                               s2m_cpi_drs_msg::MemDataNXM,     // u32 opcode,
                                                               cpi_meta_component::FIELD_NoOp,  // u32 metafield,
                                                               cpi_meta_component::VALUE_NoOp,  // u32 metadata,
                                                               cpi_req_msg.get_tag(),           // u32 tag,
                                                               cpi_req_msg.get_ldid(),          // u32 spid_ldid,
                                                               cpi_req_msg.get_spid(),          // u32 dpid,
                                                               databody_error,                  // u512 databody,
                                                               enable_error,                    // u64 enable,
                                                               poison,                          // u32 poison,
                                                               trp_error,                       // u32 trp,
                                                               emd_error                        // u32 emd)
                                                               ));
                }
            } else if (cpi_req_msg.get_flitmode() == FLITMODE_PBR) {
                if (gfam_instance->m2s_process(cpi_req_msg.get_address(), dpa))
                    cpi_req_msg.set_address(dpa);
                else {
                    cpi_req_msg.set_valid(false);
                    cpi_drs_msg_list.push_back(s2m_cpi_drs_msg(1,                               //(u32 valid,
                                                               cpi_req_msg.get_flitmode(),      // u32 flitmode,
                                                               s2m_cpi_drs_msg::MemDataNXM,     // u32 opcode,
                                                               cpi_meta_component::FIELD_NoOp,  // u32 metafield,
                                                               cpi_meta_component::VALUE_NoOp,  // u32 metadata,
                                                               cpi_req_msg.get_tag(),           // u32 tag,
                                                               cpi_req_msg.get_ldid(),          // u32 spid_ldid,
                                                               cpi_req_msg.get_spid(),          // u32 dpid,
                                                               databody_error,                  // u512 databody,
                                                               enable_error,                    // u64 enable,
                                                               poison,                          // u32 poison,
                                                               trp_error,                       // u32 trp,
                                                               emd_error                        // u32 emd)
                                                               ));
                }
            }
        }

        /*
            RwD 目前处理和 Req 完全一致
        */
        if (cpi_rwd_msg.get_valid()) {
            u64 dpa = 0;
            if (cpi_rwd_msg.get_flitmode() == FLITMODE_68) {
                if (ldfam_instance->m2s_process(cpi_rwd_msg.get_address(), cpi_rwd_msg.get_ldid(), dpa))
                    cpi_rwd_msg.set_address(dpa);
                else {
                    cpi_rwd_msg.set_valid(false);
                    cpi_drs_msg_list.push_back(s2m_cpi_drs_msg(1,                               //(u32 valid,
                                                               cpi_rwd_msg.get_flitmode(),      // u32 flitmode,
                                                               s2m_cpi_drs_msg::MemDataNXM,     // u32 opcode,
                                                               cpi_meta_component::FIELD_NoOp,  // u32 metafield,
                                                               cpi_meta_component::VALUE_NoOp,  // u32 metadata,
                                                               cpi_rwd_msg.get_tag(),           // u32 tag,
                                                               cpi_rwd_msg.get_ldid(),          // u32 spid_ldid,
                                                               cpi_rwd_msg.get_spid(),          // u32 dpid,
                                                               databody_error,                  // u512 databody,
                                                               0xFFFFFFFFFFFFFFF,               // u64 enable,
                                                               poison                           // u32 poison)
                                                               ));
                }
            } else if (cpi_rwd_msg.get_flitmode() == FLITMODE_256) {
                if (ldfam_instance->m2s_process(cpi_rwd_msg.get_address(), cpi_rwd_msg.get_ldid(), dpa))
                    cpi_rwd_msg.set_address(dpa);
                else {
                    cpi_rwd_msg.set_valid(false);
                    cpi_drs_msg_list.push_back(s2m_cpi_drs_msg(1,                               //(u32 valid,
                                                               cpi_rwd_msg.get_flitmode(),      // u32 flitmode,
                                                               s2m_cpi_drs_msg::MemDataNXM,     // u32 opcode,
                                                               cpi_meta_component::FIELD_NoOp,  // u32 metafield,
                                                               cpi_meta_component::VALUE_NoOp,  // u32 metadata,
                                                               cpi_rwd_msg.get_tag(),           // u32 tag,
                                                               cpi_rwd_msg.get_ldid(),          // u32 spid_ldid,
                                                               cpi_rwd_msg.get_spid(),          // u32 dpid,
                                                               databody_error,                  // u512 databody,
                                                               enable_error,                    // u64 enable,
                                                               poison,                          // u32 poison,
                                                               trp_error,                       // u32 trp,
                                                               emd_error                        // u32 emd)
                                                               ));
                }
            } else if (cpi_rwd_msg.get_flitmode() == FLITMODE_PBR) {
                if (gfam_instance->m2s_process(cpi_rwd_msg.get_address(), dpa))
                    cpi_rwd_msg.set_address(dpa);
                else {
                    cpi_rwd_msg.set_valid(false);
                    cpi_drs_msg_list.push_back(s2m_cpi_drs_msg(1,                               //(u32 valid,
                                                               cpi_rwd_msg.get_flitmode(),      // u32 flitmode,
                                                               s2m_cpi_drs_msg::MemDataNXM,     // u32 opcode,
                                                               cpi_meta_component::FIELD_NoOp,  // u32 metafield,
                                                               cpi_meta_component::VALUE_NoOp,  // u32 metadata,
                                                               cpi_rwd_msg.get_tag(),           // u32 tag,
                                                               cpi_rwd_msg.get_ldid(),          // u32 spid_ldid,
                                                               cpi_rwd_msg.get_spid(),          // u32 dpid,
                                                               databody_error,                  // u512 databody,
                                                               enable_error,                    // u64 enable,
                                                               poison,                          // u32 poison,
                                                               trp_error,                       // u32 trp,
                                                               emd_error                        // u32 emd)
                                                               ));
                }
            }
        }

        if (uio_rwaddr_msg.get_valid()) {
            u64 dpa = 0;
            if (gfam_instance->m2s_process(uio_rwaddr_msg.get_address(), dpa))
                uio_rwaddr_msg.set_address(dpa);
            else {
                uio_rwaddr_msg.set_valid(false);
                if (uio_rwaddr_msg.get_opcode() == 0)
                    uio_rddata_msg_list.push_back(s2m_uio_rddata_msg(
                        1, databody_error, 1, uio_rwaddr_msg.get_tagidx()));  // u32 valid, u512 rddata, u32 rderr, u32 rdtagidx))
                else
                    uio_wrrsp_msg_list.push_back(
                        s2m_uio_wrrsp_msg(1, 1, uio_rwaddr_msg.get_tagidx()));  // u32 valid, u32 wrderr, u32 wrtagidx
            }
        }
    }

    virtual void s2m_process(limited_list<s2m_cpi_bisnp_msg> &cpi_bisnp_msg_list,    // io
                             limited_list<s2m_cpi_ndr_msg> &cpi_ndr_msg_list,        // io
                             limited_list<s2m_cpi_drs_msg> &cpi_drs_msg_list,        // io
                             limited_list<s2m_uio_rddata_msg> &uio_rddata_msg_list,  // io
                             limited_list<s2m_uio_wrrsp_msg> &uio_wrrsp_msg_list     // io
    )
    {
        /*
            目前，默认 bisnp 消息都是正确的
        */

        cpi_bisnp_msg_list.clear();
        cpi_ndr_msg_list.clear();
        cpi_drs_msg_list.clear();
        uio_rddata_msg_list.clear();
        uio_wrrsp_msg_list.clear();

        for (auto &bisnp_msg : cpi_bisnp_msg_list) {
            u64 hpa = 0;
            if (bisnp_msg.get_flitmode() == FLITMODE_PBR) {
                if (gfam_instance->s2m_process(bisnp_msg.get_address(), bisnp_msg.get_dpid(), hpa))
                    bisnp_msg.set_address(hpa);
                else {
                }
            } else if (bisnp_msg.get_flitmode() == FLITMODE_68 || bisnp_msg.get_flitmode() == FLITMODE_256) {
                if (ldfam_instance->s2m_process(bisnp_msg.get_address(), bisnp_msg.get_ldid(), hpa))
                    bisnp_msg.set_address(hpa);
                else {
                }
            }
        }
    }
};

#endif
