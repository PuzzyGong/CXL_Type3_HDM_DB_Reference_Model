#include "../include/s_g_decoders.h"

bool s_g_decoders::m2s_process(const u64 &hpa, u64 &dpa)
{
    dpa = hpa + X;
    if (dpa > 0x3FFFFFFFFFFF)
        return false;
    return true;
}

bool s_g_decoders::s2m_process(const u64 &dpa, const u32 &dpid_6bit, u64 &hpa)
{
    hpa = dpa - X;
    if (dpa < X)
        return false;
    return true;
}
void s_g_decoders::rd_config(s_g_decoders_rd_Type &config)
{
    if( config.valid) {

    config.payload[0] = X % 0x100000000;
    config.payload[1] = X / 0x100000000;
    }
}

void s_g_decoders::wr_config(const s_g_decoders_wr_Type &config)
{
    if (config.valid) {
        X = config.payload[0] + config.payload[1] * 0x100000000;
    }
}