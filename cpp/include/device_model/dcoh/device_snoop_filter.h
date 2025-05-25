#ifndef DEVICE_SNOOP_FILTER_H
#define DEVICE_SNOOP_FILTER_H

#include "../../shared_var_pool/shared_var_pool.h"

// 列数，行数如果发生变动，记得修改 probe_address 方法
#define DEVICE_SNOOP_FILTER__WAY_NUM 4          // 列数
#define DEVICE_SNOOP_FILTER__DEPTH (16 * 1024)  // 行数
#define DEVICE_SNOOP_FILTER__HOST_NUM 3         // 文档里面是 4
#define DEVICE_SNOOP_FILTER__LRU_CNT_INIT 8     // LRU CNT
#define DEVICE_SNOOP_FILTER__LRU_CNT_MAX 15

#define CHECK_INDEX_WAY_VALID(idx, wy)                                                                                             \
    if ((idx) < DEVICE_SNOOP_FILTER__DEPTH && (wy) < DEVICE_SNOOP_FILTER__WAY_NUM)                                                 \
        CPP_ERROR("in file device_snoop_filter.h, index or way is out of range");

typedef enum : unsigned short
{
    Unchange,
    I,
    S,
    EM,
    Wait_I,
    Wait_IS,
    Wait_E

} CacheLineState_Enum;

class device_snoop_filter
{
  private:
    std::vector<std::vector<device_snoop_filter_elecment>> sf;
    u32 bitag_now;
    u32 rid_now;
    u32 wid_now;

  public:
    u32 bitag_add()
    {
        if (bitag_now == (1 << 12) - 1)
            bitag_now = 0;
        else
            ++bitag_now;
    }
    u32 rid_add()
    {
        if (rid_now == (1 << 4) - 1)
            rid_now = 0;
        else
            ++rid_now;
    }
    u32 wid_add()
    {
        if (wid_now == (1 << 4) - 1)
            wid_now = 0;
        else
            ++wid_now;
    }

    device_snoop_filter() : sf(DEVICE_SNOOP_FILTER__DEPTH, std::vector<device_snoop_filter_elecment>(DEVICE_SNOOP_FILTER__WAY_NUM))
    {
    }

    bool is_busy(u32 index, int way)
    {
        CHECK_INDEX_WAY_VALID(index, way);
        return sf[index][way].is_busy();
    }

    /**
     * @brief 给定地址，判断其在 snoop filter 中的命中及 busy 状态。
     * @return 命中与 busy 状态分类：
     *         - "HitAndNotBusy"
     *         - "HitButBusy"
     *         - "MissWithFreeWay"
     *         - "MissAndAllBusy"
     */
    std::string probe_address(u64 address, u32 &index, int &way)
    {
        u32 tag = (address >> 20) & 0xFFFFFFFF;
        index = (address >> 6) & 0x3FFF;

        int min_lru = DEVICE_SNOOP_FILTER__LRU_CNT_MAX + 1;
        int min_lru_way = -1;

        for (int w = 0; w < DEVICE_SNOOP_FILTER__WAY_NUM; ++w) {
            const device_snoop_filter_elecment &ele = sf[index][w];

            if (ele.LRUcnt_AND_valid > 0 && ele.tag == tag) {
                way = w;
                if (!ele.is_busy())
                    return "HitAndNotBusy";
                else
                    return "HitButBusy";
            }

            if (!ele.is_busy() && ele.LRUcnt_AND_valid > 0) {
                if (ele.LRUcnt_AND_valid < min_lru) {
                    min_lru = ele.LRUcnt_AND_valid;
                    min_lru_way = w;
                }
            }
        }

        if (min_lru_way >= 0) {
            way = min_lru_way;
            return "MissWithFreeWay";
        }

        way = -1;
        return "MissAndAllBusy";
    }

    /**
     * @brief 访问某一 index + way，对应 elecment 的 LRU 值加一（不超过 MAX），
     *        同一行其它 elecment 的 LRU 值减一（不低于 0）
     * @param index 行号
     * @param way 列号
     */
    void update_LRUcnt(u32 index, int way)
    {
        CHECK_INDEX_WAY_VALID(index, way);
        for (int w = 0; w < DEVICE_SNOOP_FILTER__WAY_NUM; ++w) {
            device_snoop_filter_elecment &ele = sf[index][w];
            if (w == way) {
                if (ele.LRUcnt_AND_valid < DEVICE_SNOOP_FILTER__LRU_CNT_MAX)
                    ++ele.LRUcnt_AND_valid;
            } else {
                if (ele.LRUcnt_AND_valid > 0)
                    --ele.LRUcnt_AND_valid;
            }
        }
    }

    /**
     * @brief 更新 elecment 中的 state 和 hostid，或统一清除非 I。
     *
     * @param index 行号
     * @param way 列号
     * @param host 新的主机编号（仅当 new_state != Unchange 时生效）
     * @param replace_state 替换其它非 I 状态的目标值
     * @param new_state 要写入的新状态（Unchange 表示不写入，仅替换）
     * @param replaced_hostids 被替换为 replace_state 的 hostid 列表输出
     * @return bool 若成功更新，返回 true；若无空位写入则返回 false（仅 new_state != Unchange 时）
     */
    bool update_state(u32 index,
                      int way,
                      u32 host,
                      CacheLineState_Enum replace_state,
                      CacheLineState_Enum new_state,
                      std::list<u32> &replaced_hostids)
    {
        CHECK_INDEX_WAY_VALID(index, way);
        device_snoop_filter_elecment &ele = sf[index][way];
        replaced_hostids.clear();

        if (new_state == Unchange) {
            // 忽略 host，统一将所有非 I 的状态改为 replace_state，并记录 hostid
            for (int i = 0; i < DEVICE_SNOOP_FILTER__HOST_NUM; ++i) {
                if (ele.state[i] != I) {
                    ele.state[i] = replace_state;
                    replaced_hostids.push_back(ele.hostid[i]);
                }
            }
            return true;
        }

        int target_idx = -1;

        // 1. 先找已有的 hostid[i] == host 且状态非 I 的槽
        for (int i = 0; i < DEVICE_SNOOP_FILTER__HOST_NUM; ++i) {
            if (ele.hostid[i] == host && ele.state[i] != I) {
                ele.state[i] = new_state;
                target_idx = i;
                break;
            }
        }

        // 2. 没找到，就找空位 state == I
        if (target_idx == -1) {
            for (int i = 0; i < DEVICE_SNOOP_FILTER__HOST_NUM; ++i) {
                if (ele.state[i] == I) {
                    ele.state[i] = new_state;
                    ele.hostid[i] = host;
                    target_idx = i;
                    break;
                }
            }
        }

        if (target_idx == -1) {
            // 没有空位可分配
            return false;
        }

        // 3. 替换除当前 host 槽外的其它非 I 状态，并记录 hostid
        for (int i = 0; i < DEVICE_SNOOP_FILTER__HOST_NUM; ++i) {
            if (i == target_idx)
                continue;
            if (ele.state[i] != I) {
                replaced_hostids.push_back(ele.hostid[i]);
                ele.state[i] = replace_state;
            }
        }

        return true;
    }
};

class device_snoop_filter_elecment
{
  public:
    u32 LRUcnt_AND_valid;
    u32 tag;  // tag == address[51:20], index == address[19:6] ; address_valid == address[51:6]

    u32 hostid[DEVICE_SNOOP_FILTER__HOST_NUM];
    CacheLineState_Enum state[DEVICE_SNOOP_FILTER__HOST_NUM];

    u32 bitag_valid[DEVICE_SNOOP_FILTER__HOST_NUM];
    u32 bitag[DEVICE_SNOOP_FILTER__HOST_NUM];

    /**
     * @brief 判断该 elecment 是否 busy：有效且存在任意 Wait 状态
     */
    bool is_busy() const
    {
        if (LRUcnt_AND_valid == 0)
            return false;
        for (int i = 0; i < DEVICE_SNOOP_FILTER__HOST_NUM; ++i) {
            if (state[i] == Wait_I || state[i] == Wait_IS || state[i] == Wait_E)
                return true;
        }
        return false;
    }
};

#endif
