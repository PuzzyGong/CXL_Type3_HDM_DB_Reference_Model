#ifndef ALL_TOOL_H
#define ALL_TOOL_H

#include "../conf/global_conf.h"
#include "./axi_tool.h"
#include "./bi_tool.h"
#include "../channel_cpi_msg/m2s_cpi_req_msg.h"
#include "../channel_cpi_msg/m2s_cpi_rwd_msg.h"
#include "../channel_uio_msg/m2s_uio_rwaddr_msg.h"

class all_tool
{
  private:
    static constexpr unsigned int ERROR_SIZE = 50;
    static constexpr unsigned int WARNING_SIZE = ERROR_SIZE + 1;
    limited_unordered_map<u32, m2s_cpi_req_msg> m2s_cpi_req_msg_map;
    limited_unordered_map<u32, m2s_cpi_rwd_msg> m2s_cpi_rwd_msg_map;
    limited_unordered_map<u32, m2s_uio_rwaddr_msg> m2s_uio_rwaddr_msg_map;
    axi_tool axi_tool_inst;
    bi_tool bi_tool_inst;

  public:
    explicit all_tool()
        : m2s_cpi_req_msg_map(ERROR_SIZE, WARNING_SIZE, "m2s_cpi_req_msg_map"),
          m2s_cpi_rwd_msg_map(ERROR_SIZE, WARNING_SIZE, "m2s_cpi_rwd_msg_map"),
          m2s_uio_rwaddr_msg_map(ERROR_SIZE, WARNING_SIZE, "m2s_uio_rwaddr_msg_map"), axi_tool_inst(), bi_tool_inst()
    {
    }
    ~all_tool() = default;

    void end_of_function(const std::list<s2m_cpi_bisnp_msg> &bisnp_msg_list,
                         const std::list<s2d_axi_rdaddr_msg> &axi_rdaddr_msg_list,
                         const std::list<s2d_axi_wraddr_msg> &axi_wraddr_msg_list)
    {
        bi_tool_inst.end_of_function(bisnp_msg_list);
        axi_tool_inst.end_of_function(axi_rdaddr_msg_list);
        axi_tool_inst.end_of_function(axi_wraddr_msg_list);
    }

    void start_of_function(const m2s_cpi_birsp_msg &birsp_msg,
                           const m2s_cpi_req_msg &req_msg,
                           const m2s_cpi_rwd_msg &rwd_msg,
                           const m2s_uio_rwaddr_msg &uio_rwaddr_msg,
                           const d2s_axi_rddata_msg &axi_rddata_msg,
                           const d2s_axi_wrrsp_msg &axi_wrrsp_msg,

                           m2s_cpi_req_msg &req_msg__BY__axi_rddata_msg,
                           m2s_cpi_rwd_msg &rwd_msg__BY__axi_rddata_msg,
                           m2s_uio_rwaddr_msg &uio_rwaddr_msg__BY__axi_rddata_msg,
                           m2s_cpi_req_msg &req_msg__BY__axi_wrrsp_msg,
                           m2s_cpi_rwd_msg &rwd_msg__BY__axi_wrrsp_msg,
                           m2s_uio_rwaddr_msg &uio_rwaddr_msg__BY__axi_wrrsp_msg,
                           s2m_cpi_bisnp_msg &bisnp_msg__BY__birsp_msg)
    {
        bisnp_msg__BY__birsp_msg = bi_tool_inst.start_of_function(birsp_msg);
        s2d_axi_rdaddr_msg tmp_s2d_axi_rdaddr_msg = axi_tool_inst.start_of_function(axi_rddata_msg);
        s2d_axi_wraddr_msg tmp_s2d_axi_wraddr_msg = axi_tool_inst.start_of_function(axi_wrrsp_msg);

        if (req_msg.get_valid()){
            m2s_cpi_req_msg_map.insert(req_msg.get_tag(), req_msg);
        }
        if (rwd_msg.get_valid()){
            m2s_cpi_rwd_msg_map.insert(rwd_msg.get_tag(), rwd_msg);
        }
        if (uio_rwaddr_msg.get_valid()){
            m2s_uio_rwaddr_msg_map.insert(uio_rwaddr_msg.get_tagidx(), uio_rwaddr_msg);
        }

        if (m2s_cpi_req_msg_map.exists(tmp_s2d_axi_rdaddr_msg.tag)){
            req_msg__BY__axi_rddata_msg = m2s_cpi_req_msg_map.fetch(tmp_s2d_axi_rdaddr_msg.tag);
        }
        else{
            req_msg__BY__axi_rddata_msg.set_valid(false);
        }
        if (m2s_cpi_rwd_msg_map.exists(tmp_s2d_axi_rdaddr_msg.tag)){
            rwd_msg__BY__axi_rddata_msg = m2s_cpi_rwd_msg_map.fetch(tmp_s2d_axi_rdaddr_msg.tag);
        }
        else{
            rwd_msg__BY__axi_rddata_msg.set_valid(false);
        }
        if (m2s_uio_rwaddr_msg_map.exists(tmp_s2d_axi_rdaddr_msg.tag)){
            uio_rwaddr_msg__BY__axi_rddata_msg = m2s_uio_rwaddr_msg_map.fetch(tmp_s2d_axi_rdaddr_msg.tag);
        }
        else{
            uio_rwaddr_msg__BY__axi_rddata_msg.set_valid(false);
        }
        if (m2s_cpi_req_msg_map.exists(tmp_s2d_axi_wraddr_msg.tag)){
            req_msg__BY__axi_wrrsp_msg = m2s_cpi_req_msg_map.fetch(tmp_s2d_axi_wraddr_msg.tag);
        }
        else{
            req_msg__BY__axi_wrrsp_msg.set_valid(false);
        }
        if (m2s_cpi_rwd_msg_map.exists(tmp_s2d_axi_wraddr_msg.tag)){
            rwd_msg__BY__axi_wrrsp_msg = m2s_cpi_rwd_msg_map.fetch(tmp_s2d_axi_wraddr_msg.tag);
        }
        else{
            rwd_msg__BY__axi_wrrsp_msg.set_valid(false);
        }
        if (m2s_uio_rwaddr_msg_map.exists(tmp_s2d_axi_wraddr_msg.tag)){
            uio_rwaddr_msg__BY__axi_wrrsp_msg = m2s_uio_rwaddr_msg_map.fetch(tmp_s2d_axi_wraddr_msg.tag);
        }
        else{
            uio_rwaddr_msg__BY__axi_wrrsp_msg.set_valid(false);
        }
    }
};

#endif