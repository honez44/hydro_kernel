/* 
 * This software is contributed or developed by KYOCERA Corporation.
 * (C) 2012 KYOCERA Corporation
 *
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>

#define DIAG_SLATE_F            (246)
#define DIAG_OEM_F              (250)

#define DIAG_ENABLE             (0)
#define DIAG_DISABLE            (1)

static int diag_state = DIAG_DISABLE;
static int const_state = 0;

int oemmsg_process_pkt(int cmd, int sub_id, int sub_cmd)
{
    if(cmd == DIAG_SLATE_F)
    {
        if((sub_id == 0x31)||(sub_id == 0x25)||(sub_id == 0x37))
        {
            return 1;
        }
    }
    else if(cmd == DIAG_OEM_F)
    {
        return 1;
    }
    return 0;
}

int oemmsg_diag_state(void)
{
    if(const_state == 1)
    {
        return DIAG_ENABLE;
    }
    else
    {
        return diag_state;
    }
}

void oemmsg_diag_state_change(void *buf)
{
    char *rsp_buf;

    rsp_buf = buf;

    if(*rsp_buf == DIAG_OEM_F && const_state == 0)
    {
        rsp_buf += 2;
        if(*rsp_buf == 0x61 || *rsp_buf == 0x63)
        {
            diag_state = DIAG_ENABLE;
            if(*rsp_buf == 0x63)
            {
                const_state = 1;
            }
        }
        else if(*rsp_buf == 0x62)
        {
            diag_state = DIAG_DISABLE;
        }
    }
}

void oemmsg_set_diag_enable(void)
{
    const_state = 1;
}

