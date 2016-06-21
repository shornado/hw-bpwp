/* drivers/video/decon_display/tc358764_mipi_lcd.c
 *
 * Samsung SoC MIPI LCD driver.
 *
 * Copyright (c) 2013 Samsung Electronics
 *
 * Haowei Li, <haowei.li@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/delay.h>
#include <linux/gpio.h>
#include <video/mipi_display.h>
#include <linux/platform_device.h>

#include "decon_mipi_dsi.h"
#include "decon_display_driver.h"
#include "decon_mipi_dsi_lowlevel.h"
#include "regs-mipidsim.h"

static struct mipi_dsim_device *dsim_base;

static struct {
    unsigned short  reg;    // Control register address
    unsigned int    data;   // Register data value
}   sInitCode[] = {
    
    // DSI Basic Parameters
    {   0x013C, 0x00050006  },  // PPI_TX_RX_TA BTA Parameters
    {   0x0114, 0x00000004  },  // PPI_LPTXTIMCNT
    {   0x0164, 0x00000005  },  // PPI_D0S_CLRSIPOCOUNT
    {   0x0168, 0x00000005  },  // PPI_D1S_CLRSIPOCOUNT
    {   0x016C, 0x00000005  },  // PPI_D2S_CLRSIPOCOUNT
    {   0x0170, 0x00000005  },  // PPI_D3S_CLRSIPOCOUNT
    {   0x0134, 0x0000001F  },  // PPI_LANEENABLE
    {   0x0210, 0x0000001F  },  // DSI_LANEENABLE
    {   0x0104, 0x00000001  },  // PPI_SARTPPI
    {   0x0204, 0x00000001  },  // DSI_SARTPPI
    
    // Timming and mode setting
    {   0x0450, 0x03F00120  },  // VPCTRL
    {   0x0454, 0x00400030  },  // HTIM1
    {   0x0458, 0x00100500  },  // HTIM2
    {   0x045C, 0x000C0001  },  // VTIM1
    {   0x0460, 0x00010320  },  // VTIM2
    {   0x0464, 0x00000001  },  // VFUEN
    {   0x04A0, 0x00448406  },  // LVPHY0
    {   0x04A0, 0x00048406  },  // LVPHY0
    {   0x0504, 0x00000004  },  // SYSRST
    
    // LVDS Color mapping setting
    {   0x0480, 0x03020100  },  // LVMX0003
    {   0x0484, 0x08050704  },  // LVMX0407
    {   0x0488, 0x0F0E0A09  },  // LVMX0811
    {   0x048C, 0x100D0C0B  },  // LVMX1215
    {   0x0490, 0x12111716  },  // LVMX1619
    {   0x0494, 0x1B151413  },  // LVMX2023
    {   0x0498, 0x061A1918  },  // LVMX2427
    
    // LVDS Enable
    {   0x049C, 0x00000001  },  // LVCFG
};

struct decon_lcd tc358764_lcd_info = {
	.mode = VIDEO_MODE,

	.vfp = 0x4,
	.vbp = 0x4,
	.hfp = 0x4,
	.hbp = 0x4,

	.vsa = 0x4,
	.hsa = 0x4,

	.xres = 1280,
	.yres = 800,

	.width = 1280,
	.height = 800,

	/* Mhz */
	.hs_clk = 480,
	.esc_clk = 7,
	.fps = 60,
};

struct decon_lcd * decon_get_lcd_info(void)
{
	return &tc358764_lcd_info;
}
EXPORT_SYMBOL(decon_get_lcd_info);

static int tc358764_probe(struct mipi_dsim_device *dsim)
{
	dsim_base = dsim;

	return 1;
}

static void init_lcd(struct mipi_dsim_device *dsim)
{
    unsigned char   cmd[6], i;

	for (i = 0; i < ARRAY_SIZE(sInitCode); i++) {
	    cmd[0] = sInitCode[i].reg;	        cmd[1] = sInitCode[i].reg  >> 8;
	    cmd[2] = sInitCode[i].data;         cmd[3] = sInitCode[i].data >> 8;
	    cmd[4] = sInitCode[i].data >> 16;   cmd[5] = sInitCode[i].data >> 24;

#if 0
        printk("reg = 0x%04X, data = 0x%08X\n", sInitCode[i].reg, sInitCode[i].data);
        printk("cmd = 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n", cmd[0]
                                                                  , cmd[1]
                                                                  , cmd[2]
                                                                  , cmd[3]
                                                                  , cmd[4]
                                                                  , cmd[5]);
#endif	   
        if(s5p_mipi_dsi_wr_data(dsim, MIPI_DSI_GENERIC_LONG_WRITE, 
                                (unsigned int)cmd, sizeof(cmd)) == -1)   {
            printk("write error!! reg = 0x%04X, data = 0x%08X\n", sInitCode[i].reg, sInitCode[i].data);
        }
    	
    	mdelay(10);    
	}

	mdelay(10);
}

static int tc358764_displayon(struct mipi_dsim_device *dsim)
{
	s5p_mipi_dsi_enable_hs_clock(dsim, 1);
	s5p_mipi_dsi_set_cpu_transfer_mode(dsim, 1);
	s5p_mipi_dsi_clear_int_status(dsim, INTSRC_SFR_FIFO_EMPTY);
	init_lcd(dsim);
	s5p_mipi_dsi_set_cpu_transfer_mode(dsim, 0);

	return 1;
}

static int tc358764_suspend(struct mipi_dsim_device *dsim)
{
	return 1;
}

static int tc358764_resume(struct mipi_dsim_device *dsim)
{
	return 1;
}

struct mipi_dsim_lcd_driver tc358764_mipi_lcd_driver = {
	.probe		= tc358764_probe,
	.displayon	= tc358764_displayon,
	.suspend	= tc358764_suspend,
	.resume		= tc358764_resume,
};
