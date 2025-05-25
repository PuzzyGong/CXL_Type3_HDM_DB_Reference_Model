#ifndef DEVICE_GFAM_DCD_H
#define DEVICE_GFAM_DCD_H

#include "../base/device_gfam_base.h"

class device_gfam_dcd : public device_gfam_base
{
  public:
    explicit device_gfam_dcd(const shared_var_pool &shared_pool) : device_gfam_base(shared_pool)
    {
        init();
    }
    ~device_gfam_dcd() = default;
    bool m2s_process(unsigned long long dpa, unsigned long long &mpa) override; //重载该函数
    bool s2m_process(unsigned long long mpa, unsigned int dpid_6bit, unsigned long long &dpa) override; //重载该函数

  private:
    void init(); //重载该函数
    //****************************** 自定义
    int X;
    //****************************** 自定义
};

#endif