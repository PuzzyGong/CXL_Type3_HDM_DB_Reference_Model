#ifndef GLOBAL_CONF_WITH_CHANNEL_H
#define GLOBAL_CONF_WITH_CHANNEL_H

#include "./global_conf.h"

// CPI
#include "../channel_model/channel_cpi_msg/s2m_cpi_bisnp_msg.h"
#include "../channel_model/channel_cpi_msg/s2m_cpi_ndr_msg.h"
#include "../channel_model/channel_cpi_msg/s2m_cpi_drs_msg.h"
#include "../channel_model/channel_cpi_msg/m2s_cpi_req_msg.h"
#include "../channel_model/channel_cpi_msg/m2s_cpi_birsp_msg.h"
#include "../channel_model/channel_cpi_msg/m2s_cpi_rwd_msg.h"

// UIO
#include "../channel_model/channel_uio_msg/s2m_uio_rddata_msg.h"
#include "../channel_model/channel_uio_msg/s2m_uio_wrrsp_msg.h"
#include "../channel_model/channel_uio_msg/m2s_uio_rwaddr_msg.h"

// AXI
#include "../channel_model/channel_axi_msg/d2s_axi_rddata_msg.h"
#include "../channel_model/channel_axi_msg/d2s_axi_wrrsp_msg.h"
#include "../channel_model/channel_axi_msg/s2d_axi_rdaddr_msg.h"
#include "../channel_model/channel_axi_msg/s2d_axi_wraddr_msg.h"

// DUT
#include "../channel_model/channel_dut_msg/m2s_dut_msg.h"

// ALL
#include "../channel_model/m2s_union_four_msg.h"

// TOOLS
#include "../channel_model/tools/print_tool.h"

#endif
