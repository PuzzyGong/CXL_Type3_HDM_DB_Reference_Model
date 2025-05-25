#ifndef M2S_UNION_FOUR_MSG_H
#define M2S_UNION_FOUR_MSG_H

#include "./channel_cpi_msg/m2s_cpi_birsp_msg.h"
#include "./channel_cpi_msg/m2s_cpi_req_msg.h"
#include "./channel_cpi_msg/m2s_cpi_rwd_msg.h"
#include "./channel_uio_msg/m2s_uio_rwaddr_msg.h"

class m2s_union_four_msg
{
  public:
    static constexpr unsigned int CHOOSE_M2S_CPI_BIRSP_MSG = 1;
    static constexpr unsigned int CHOOSE_M2S_CPI_REQ_MSG = 2;
    static constexpr unsigned int CHOOSE_M2S_CPI_RWD_MSG = 3;
    static constexpr unsigned int CHOOSE_M2S_UIO_RWADDR_MSG = 4;

  private:
    u32 chooose;
    m2s_cpi_birsp_msg cpi_birsp_msg;
    m2s_cpi_req_msg cpi_req_msg;
    m2s_cpi_rwd_msg cpi_rwd_msg;
    m2s_uio_rwaddr_msg uio_rwaddr_msg;

  public:
    explicit m2s_union_four_msg(m2s_cpi_birsp_msg cpi_birsp_msg)
    {
        this->chooose = CHOOSE_M2S_CPI_BIRSP_MSG;
        this->cpi_birsp_msg = cpi_birsp_msg;
    }
    explicit m2s_union_four_msg(m2s_cpi_req_msg cpi_req_msg)
    {
        this->chooose = CHOOSE_M2S_CPI_REQ_MSG;
        this->cpi_req_msg = cpi_req_msg;
    }
    explicit m2s_union_four_msg(m2s_cpi_rwd_msg cpi_rwd_msg)
    {
        this->chooose = CHOOSE_M2S_CPI_RWD_MSG;
        this->cpi_rwd_msg = cpi_rwd_msg;
    }
    explicit m2s_union_four_msg(m2s_uio_rwaddr_msg uio_rwaddr_msg)
    {
        this->chooose = CHOOSE_M2S_UIO_RWADDR_MSG;
        this->uio_rwaddr_msg = uio_rwaddr_msg;
    }

    bool is_m2s_cpi_birsp_msg()
    {
        return this->chooose == CHOOSE_M2S_CPI_BIRSP_MSG;
    }
    bool is_m2s_cpi_req_msg()
    {
        return this->chooose == CHOOSE_M2S_CPI_REQ_MSG;
    }
    bool is_m2s_cpi_rwd_msg()
    {
        return this->chooose == CHOOSE_M2S_CPI_RWD_MSG;
    }
    bool is_m2s_uio_rwaddr_msg()
    {
        return this->chooose == CHOOSE_M2S_UIO_RWADDR_MSG;
    }

    m2s_cpi_birsp_msg get_m2s_cpi_birsp_msg()
    {
        if (this->chooose != CHOOSE_M2S_CPI_BIRSP_MSG) {
            CPP_ERROR(
                "in file m2s_union_four_msg.h, function get_m2s_cpi_birsp_msg(), the chooose is not CHOOSE_M2S_CPI_BIRSP_MSG");
        }
        return this->cpi_birsp_msg;
    }
    m2s_cpi_req_msg get_m2s_cpi_req_msg()
    {
        if (this->chooose != CHOOSE_M2S_CPI_REQ_MSG) {
            CPP_ERROR("in file m2s_union_four_msg.h, function get_m2s_cpi_req_msg(), the chooose is not CHOOSE_M2S_CPI_REQ_MSG");
        }
        return this->cpi_req_msg;
    }
    m2s_cpi_rwd_msg get_m2s_cpi_rwd_msg()
    {
        if (this->chooose != CHOOSE_M2S_CPI_RWD_MSG) {
            CPP_ERROR("in file m2s_union_four_msg.h, function get_m2s_cpi_rwd_msg(), the chooose is not CHOOSE_M2S_CPI_RWD_MSG");
        }
        return this->cpi_rwd_msg;
    }
    m2s_uio_rwaddr_msg get_m2s_uio_rwaddr_msg()
    {
        if (this->chooose != CHOOSE_M2S_UIO_RWADDR_MSG) {
            CPP_ERROR(
                "in file m2s_union_four_msg.h, function get_m2s_uio_rwaddr_msg(), the chooose is not CHOOSE_M2S_UIO_RWADDR_MSG");
        }
        return this->uio_rwaddr_msg;
    }
};

#endif