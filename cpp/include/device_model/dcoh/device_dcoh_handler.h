#ifndef DEVICE_DCOH_HANDLER_H
#define DEVICE_DCOH_HANDLER_H

#include "../../../conf/global_conf_with_channel.h"
#include "../../shared_var_pool/shared_var_pool.h"
#include "./device_snoop_filter.h"

class device_dcoh_handler
{
  public:
    virtual bool handle(bool &axi_loop,
                        bool DCOH_OR_ParGen,
                        device_snoop_filter &sf,

                        m2s_cpi_birsp_msg &cpi_birsp,
                        s2m_cpi_bisnp_msg &cpi_bisnp,
                        m2s_cpi_req_msg &cpi_req,
                        m2s_cpi_rwd_msg &cpi_rwd,
                        m2s_uio_rwaddr_msg &uio_rwaddr,
                        d2s_axi_rddata_msg &axi_rddata,
                        d2s_axi_wrrsp_msg &axi_wrrsp,

                        limited_list<s2m_cpi_bisnp_msg> &cpi_bisnp_list,
                        limited_list<s2m_cpi_ndr_msg> &cpi_ndr_list,
                        limited_list<s2m_cpi_drs_msg> &cpi_drs_list,
                        limited_list<s2m_uio_rddata_msg> &uio_rddata_list,
                        limited_list<s2m_uio_wrrsp_msg> &uio_wrrsp_list,
                        limited_list<s2d_axi_rdaddr_msg> &axi_rdaddr_list,
                        limited_list<s2d_axi_wraddr_msg> &axi_wraddr_list) = 0;

    bool common(u32 &index,
                int &way,
                bool &axi_loop,
                device_snoop_filter &sf,
                u64 addr,
                limited_list<s2m_cpi_bisnp_msg> &cpi_bisnp_list)
    {
        std::string str = sf.probe_address(addr, index, way);
        if (str == "HitButBusy" || str == "MissAndAllBusy") {
            axi_loop = true;
            return true;
        } else if (str == "MissWithFreeWay") {
            std::list<u32> hostid_list;
            sf.update_state(index, way, 0, Unchange, I, hostid_list);
            for (const auto &hostid : hostid_list) {
                cpi_bisnp_list.push_back(s2m_cpi_bisnp_msg(true,                       // u32 valid,
                                                           cpi_req.get_flitmode(),     // u32 flitmode,
                                                           s2m_cpi_bisnp_msg::SnpInv,  // u32 opcode,
                                                           sf.bitag_add(),             // u32 bitag,
                                                           cpi_req.get_address(),      // u32 address,
                                                           HDM_PID,                    // u32 spid_ldid,
                                                           hostid,                     // u32 dpid,
                                                           cpi_req.get_tag()           // u32 tag
                                                           ));
            }
        } else {
            sf.update_LRUcnt(index, way);
            return false;
        }
    }
    ~device_dcoh_handler()
    {
    }
};

class device_dcoh_handler_MemRd_SnpInv_A : public device_dcoh_handler
{
  public:
    bool handle(bool &axi_loop,
                bool DCOH_OR_ParGen,
                device_snoop_filter &sf,

                m2s_cpi_birsp_msg &cpi_birsp,
                s2m_cpi_bisnp_msg &cpi_bisnp,
                m2s_cpi_req_msg &cpi_req,
                m2s_cpi_rwd_msg &cpi_rwd,
                m2s_uio_rwaddr_msg &uio_rwaddr,
                d2s_axi_rddata_msg &axi_rddata,
                d2s_axi_wrrsp_msg &axi_wrrsp,

                limited_list<s2m_cpi_bisnp_msg> &cpi_bisnp_list,
                limited_list<s2m_cpi_ndr_msg> &cpi_ndr_list,
                limited_list<s2m_cpi_drs_msg> &cpi_drs_list,
                limited_list<s2m_uio_rddata_msg> &uio_rddata_list,
                limited_list<s2m_uio_wrrsp_msg> &uio_wrrsp_list,
                limited_list<s2d_axi_rdaddr_msg> &axi_rdaddr_list,
                limited_list<s2d_axi_wraddr_msg> &axi_wraddr_list) override
    {
        if (!(cpi_req.get_valid() &&                                 //
              cpi_req.get_memopcode() == m2s_cpi_req_msg::MemInv &&  //
              cpi_req.get_snptype() == m2s_cpi_req_msg::SnpInv &&    //
              cpi_req.get_metavalue() == m2s_cpi_req_msg::VALUE_A    //
              ))
            return false;

        if (DCOH_OR_ParGen == 0) {
            u32 index;
            int way;
            if (device_dcoh_handler::common(index, way, axi_loop, sf, cpi_req.get_address(), cpi_bisnp_list)) {
                return true;
            } else {
                std::list<u32> hostid_list;
                sf.update_state(index, way, cpi_req.get_hostid(), Wait_E, I, hostid_list);
                for (const auto &hostid : hostid_list) {
                    cpi_bisnp_list.push_back(s2m_cpi_bisnp_msg(true,                       // u32 valid,
                                                               cpi_req.get_flitmode(),     // u32 flitmode,
                                                               s2m_cpi_bisnp_msg::SnpInv,  // u32 opcode,
                                                               sf.bitag_add(),             // u32 bitag,
                                                               cpi_req.get_address(),      // u32 address,
                                                               HDM_PID,                    // u32 spid_ldid,
                                                               hostid,                     // u32 dpid,
                                                               cpi_req.get_tag()           // u32 tag
                                                               ));
                }

                if (!sf.is_busy(index, way)) {
                    axi_rdaddr_list.push_back(s2d_axi_rdaddr_msg(true,                   // u32 valid,
                                                                 sf.rid_add(),           // u32 id,
                                                                 cpi_req.get_address(),  // u64 address,
                                                                 0,                      // u64 user,
                                                                 cpi_req.get_tag()       // u32 tag
                                                                 ));
                    sf.update_state(index, way, cpi_req.get_hostid(), EM, I, hostid_list);
                }
            }
        }

        else {
            if (axi_rddata.get_valid()) {
                cpi_ndr_list.push_back(s2m_cpi_ndr_msg(true,                       // u32 valid,
                                                       cpi_req.get_flitmode(),     // u32 flitmode,
                                                       s2m_cpi_ndr_msg::CmpE,      // u32 opcode,
                                                       cpi_req.get_metafield(),    // u32 metafield,
                                                       axi_rddata.get_metadata(),  // u32 metadata,
                                                       cpi_req.get_tag(),          // u32 tag,
                                                       HDM_PID,                    // u32 spid_ldid,
                                                       cpi_req.get_hostid()        // u32 dpid
                                                       ));
                return true;
            }
        }
    }
};

class device_dcoh_handler_noop_memwr_emd : public device_dcoh_handler_base  // 我举得还是emd 合了比较好
{
  public:
    bool handle(m2s_cpi_birsp_msg &cpi_birsp_msg,    // io
                m2s_cpi_req_msg &cpi_req_msg,        // io
                m2s_cpi_rwd_msg &cpi_rwd_msg,        // io
                m2s_uio_rwaddr_msg &uio_rwaddr_msg,  // io
                m2s_dut_msg &dut_msg,                // io

                limited_list<s2m_cpi_bisnp_msg> &bisnp_msg,    // o
                limited_list<s2m_cpi_ndr_msg> &ndr_msg,        // o
                limited_list<s2m_cpi_drs_msg> &drs_msg,        // o
                limited_list<s2m_uio_rddata_msg> &rddata_msg,  // o
                limited_list<s2m_uio_wrrsp_msg> &wrrsp_msg,    // o

                d2s_axi_rddata_msg &axi_rddata_msg,  // io
                d2s_axi_wrrsp_msg &axi_wrrsp_msg,    // io

                limited_list<s2d_axi_rdaddr_msg> &axi_rdaddr_msg,  // o
                limited_list<s2d_axi_wraddr_msg> &axi_wraddr_msg,  // o

                device_snoop_filter_line &snoop_filter_line,  // io
                ) override
    {
        /*
            到这必是valid
        */
        if (poison == 1)
            丢弃；

        if (is_valid, no_valid_but_waynum, no_valid_and_no_way, snoop_filter_elecment = snoop_filter_line.func(cpi_req_msg.address))

            if (no_valid_but_waynum) {
                FUNCTION(&snoop_filter_elecment)
                {  // io
                    bisnp_msg, ndr_msg, drs_msg, rddata_msg wrrsp_msg, axi_rdaddr_msg axi_wraddr_msg.push_back() 准备好输出
                }

                return false 代表消息回环
            }

            else if (no_valid_and_no_way) {
                return false 代表消息回环
            }

            else {
                其他 LRU_cnt--,
                    为0则取消 valid

                        snoop_filter_elecment.FUNC(const &msg)

                            int host_self,
                    list<int> host_others = snoop_filter_elecment.FUNCTION(
                        self_change : NeedToWrite_WillBeS others_change : I)  // FUNC 中有CPP_EEROR 可能报错, 没有足够host空间了

                                            if (host_others.isnotempty) CPP_ERROR

                                            FUNCTION_check(snoop_filter_elecment)
                {  // 和上面是一个函数
                    snoop_filter_elecment
                        .FUNCTION(self_change : NeedToWrite_WillBeS others_change : UnChanged)

                            axi_wraddr_msg.push_back（）带有emd的
                }
            }
    }
};

class device_dcoh_handler_birsp : public device_dcoh_handler_base
{
  public:
    bool handle(m2s_cpi_birsp_msg &cpi_birsp_msg,    // io
                m2s_cpi_req_msg &cpi_req_msg,        // io
                m2s_cpi_rwd_msg &cpi_rwd_msg,        // io
                m2s_uio_rwaddr_msg &uio_rwaddr_msg,  // io
                m2s_dut_msg &dut_msg,                // io

                limited_list<s2m_cpi_bisnp_msg> &bisnp_msg,    // o
                limited_list<s2m_cpi_ndr_msg> &ndr_msg,        // o
                limited_list<s2m_cpi_drs_msg> &drs_msg,        // o
                limited_list<s2m_uio_rddata_msg> &rddata_msg,  // o
                limited_list<s2m_uio_wrrsp_msg> &wrrsp_msg,    // o

                d2s_axi_rddata_msg &axi_rddata_msg,  // io
                d2s_axi_wrrsp_msg &axi_wrrsp_msg,    // io

                limited_list<s2d_axi_rdaddr_msg> &axi_rdaddr_msg,  // o
                limited_list<s2d_axi_wraddr_msg> &axi_wraddr_msg,  // o

                device_snoop_filter_line &snoop_filter_line,  // io
                ) override
    {
        if (!XXX.valid) {
            丢弃；
        }

        if (is_valid, no_valid_but_waynum, no_valid_and_no_way,
            snoop_filter_elecment = snoop_filter_line.func(cpi_birsp_msg.address))

            if (!is_valid) {
                CPP_ERROR()
            } else {
                int host_self_zhua ngtai = snoop_filter_elecment.FUNCTION2()
                {
                }
                if (host_self_zhuangtai != Rsp那两个) {
                    CPP_ERROR()
                } else if (Need_For_RspI) {
                    if
                        else CPP_ERROR
                } else if (Need_For_RspIS) {
                    if
                        else CPP_ERROR
                }

                FUNCTION_check(snoop_filter_elecment)
                {  // 和上面是一个函数
                    snoop_filter_elecment
                        .FUNCTION(self_change : NeedToWrite_WillBeS others_change : UnChanged)

                            axi_wraddr_msg.push_back
                }
            }
    }

    class device_dcoh_handler_axi_rddata : public device_dcoh_handler_base
    {
        if (is_valid,
            no_valid_but_waynum,
            no_valid_and_no_way,
            snoop_filter_elecment = snoop_filter_line.func(cpi_birsp_msg.address))

            if (!is_valid) {
                CPP_ERROR()
            } else {
                if (rsp != 0) {
                }
            }
    }
};

class device_dcoh_handler_axi

#endif
