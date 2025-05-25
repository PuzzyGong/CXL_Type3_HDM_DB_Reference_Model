#ifndef BI_TOOL_H
#define BI_TOOL_H

#include "../conf/global_conf.h"
#include "../channel_cpi_msg/s2m_cpi_bisnp_msg.h"
#include "../channel_cpi_msg/m2s_cpi_birsp_msg.h"

/*
    【功能描述】
    缓存 CPI 协议中从主机发出的 s2m_cpi_bisnp_msg 请求，
    并在 m2s_cpi_birsp_msg 响应返回时按 bitag + hostid 查找并取出原始请求。

    【使用说明】
    - 在收到一批 BI 请求消息时，调用 end_of_function() 插入映射。
    - 当收到对应 BI 响应时，调用 start_of_function() 根据 bitag 和 hostid 联合键获取。
*/
class bi_tool
{
  private:
    static constexpr unsigned int ERROR_SIZE = 50;
    static constexpr unsigned int WARNING_SIZE = ERROR_SIZE + 1;
    limited_unordered_map<u64, s2m_cpi_bisnp_msg> s2m_cpi_bisnp_msg_list;

  public:
    explicit bi_tool() : s2m_cpi_bisnp_msg_list(ERROR_SIZE, WARNING_SIZE, "s2m_cpi_bisnp_msg_list")
    {
    }
    ~bi_tool() = default;

    void end_of_function(const std::list<s2m_cpi_bisnp_msg> &msg_list)
    {
        for (const auto &msg : msg_list) {
            s2m_cpi_bisnp_msg_list.insert(msg.get_bitag() << 32 | msg.get_hostid(), msg);
        }
    }
    s2m_cpi_bisnp_msg start_of_function(const m2s_cpi_birsp_msg &msg)
    {
        return s2m_cpi_bisnp_msg_list.fetch(msg.get_bitag() << 32 | msg.get_hostid());
    }
};

#endif