#ifndef S_VSF_H
#define S_VSF_H

#include "define.h"
#include "s_decoder.h"
#include "s_vsf_line.h"

#define mapper_size 32
#define s2m_snp_not_alloc_bitag_size 32
#define m2s_req_list_size 16
#define vsf_line_size 1024
class S_VSF_Type  //DCOH
{
private:
    shared_v_pool pool;
    std::unordered_map<u32, S2M_Snp_type> mapper;
    u32 mapper_bitag_now;

    limited_list<S2M_Snp_type> s2m_snp_not_alloc_bitag;
    limited_list<M2S_Req_Type> m2s_req_list;
    std::unordered_map<u64, S_VSF_Line_Type> vsf_line;
    S_Decoder decoder;
    void main_product(
        const M2S_Rsp_type &m2s_rsp,
        const M2S_RwD_Type &m2s_rwd,
        limited_list<S2M_Snp_type> &s2m_snp,
        limited_list<S2M_NDR_Type> &s2m_ndr,
        limited_list<S2M_DRS_Type> &s2m_drs,
        const Rd_Data_Type &rd_data,
        limited_list<Rd_Addr_Type> &rd_addr,
        limited_list<Wr_Type> &wr);


        {
            M2S_Req_Type m2s_req,

            M2S_Rsp_type m2s_rsp;
            M2S_RwD_Type m2s_rwd;
            Rd_Data_Type rd_data;
            limited_list<Rd_Addr_Type> rd_addr;
            limited_list<Wr_Type> wr;
            limited_list<S2M_Snp_type> s2m_snp;

            Flit_mode Test::get_shared_pool().get_flit_mode();
            LD_OR_G_mode Test::get_shared_pool().get_ld_g_mode();
            

            //第一步decoder
            u64 hpa = = m2s_req.get_addr();
            u32 ld_id = m2s_req.get_ld_id();
            u64 dpa;
            if(m2s_req.get_valid()){
                bool res = m2s_process(const u64 &hpa, const u32 &ld_id, u64 &dpa);
                if(res)
                {
                    m2s_req.set_addr(dpa);
                }
                else{
                    m2s_req.set_valid(0);
                    s2m_ndr_list.add(s2m_ndr.set_all());
                }
            }
            //这里不打印

            

            //第二步
        }
        shared_v_pool pool = Test::get_shared_pool();
        pool.set_var(123);

public:
get_shared_pool()
{
    return pool;
}
get_shared_pool()
    S_VSF_Type() : mapper_bitag_now(0) {}
    inline void product(
        const M2S_Req_Type &m2s_req,
        const M2S_Rsp_type &m2s_rsp,
        const M2S_RwD_Type &m2s_rwd,
        limited_list<S2M_Snp_type> &s2m_snp,
        limited_list<S2M_NDR_Type> &s2m_ndr,
        limited_list<S2M_DRS_Type> &s2m_drs,

        const Rd_Data_Type &rd_data,
        limited_list<Rd_Addr_Type> &rd_addr,
        limited_list<Wr_Type> &wr,

        const CXL_UIO_req &cxl_uio_req,
        limited_list<CXL_UIO_rsp> &cxl_uio_rsp,



        const CXL_io_Req_Type &cxl_io_req,  //
        limited_list<CXL_io_Rsp_Type> &cxl_io_rsp, //
        const Victim_Type &victim,   
        const Victim_Type &victim1,  
        const Victim_Type &victim2,  
        const S2M_Snp_type &s2m_snp_provide_bitag  //
    )
    {
        // pre_product(m2s_req, m2s_rsp, m2s_rwd, s2m_snp, s2m_ndr, s2m_drs, rd_data, rd_addr, wr, cxl_io_req, cxl_io_rsp, victim, s2m_snp_provide_bitag);
        main_product(m2s_req, m2s_rsp, m2s_rwd, s2m_snp, s2m_ndr, s2m_drs, rd_data, rd_addr, wr, cxl_io_req, cxl_io_rsp, victim, s2m_snp_provide_bitag);
        // post_product(m2s_req, m2s_rsp, m2s_rwd, s2m_snp, s2m_ndr, s2m_drs, rd_data, rd_addr, wr, cxl_io_req, cxl_io_rsp, victim, s2m_snp_provide_bitag);
    };
    void main_product(
        const M2S_Req_Type &m2s_req,
        const M2S_Rsp_type &m2s_rsp,
        const M2S_RwD_Type &m2s_rwd,
        limited_list<S2M_Snp_type> &s2m_snp,
        limited_list<S2M_NDR_Type> &s2m_ndr,
        limited_list<S2M_DRS_Type> &s2m_drs,

        const Rd_Data_Type &rd_data,
        limited_list<Rd_Addr_Type> &rd_addr,
        limited_list<Wr_Type> &wr,

        const CXL_io_Req_Type &cxl_io_req,
        limited_list<CXL_io_Rsp_Type> &cxl_io_rsp,
        const Victim_Type &victim,
        const S2M_Snp_type &s2m_snp_provide_bitag);
};

#endif



//注意除了2DECODER 和 2DCD 和 DCOH ，还有 SPID 和AXI 交织模块 要搞，每个地址转化模块都要有dummy
//直接用test中的变量定义 要 HPA = DPA，还是经过转化模块
//直接用test中的变量定义 要不要采用AXI回环，如果回环的话, File 接口和其他接口对应 AXI 就无效
// 回环加一个函数或者类

// 注意，如果4个Way都在 waitfor，可能会有wayfull 回环，会不会影响功能？？？
// 记得DCOH 插入一个 fromDUT 的 message，也在 test 中指定。

// 三个模式device； device+host； device+host+sv，  对应三个test类。
// 

// 只有DCOH中 的 fromDUTmessage模式下 才会有list buffer来缓冲数据，添加元素前，需要检查容量是否会爆炸
// DCOH中每一个相应一个子类！！！
// DCOH   Metadata ？？？
// DCOH   PTr ？？？

// 里面的暂存值都要打印出来！！！

//BIID 目前策略是和ld-id保持一致，要在DCOH中体现

// limited_list 输出的函数，函数开头要clear  
// 
// 多个main和makefile

// 是不是所有函數都有析構