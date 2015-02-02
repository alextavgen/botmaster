/* Jasp0r patch for fixing corrupted frame issue.
 * Code taken from gspca mainline and injected here to preserve controls.
 * Based on:
 * M.Ebrahimi patch for power line frequency control
 * <m.ebrahimi@ieee.org>
 * Based on:
 * Bear24rw PS3eye camera patch for V4L brightness, contrast, hue, AWB control
 * http://bear24rw.blogspot.com
 * Based on:
 * Kaswy PS3eye camera mod for Multitouch use
 * http://kaswy.free.fr
 * Based on: 
 * ov534/ov772x gspca driver
 * Copyright (C) 2008 Antonio Ospite <ospite@studenti.unina.it>
 * Copyright (C) 2008 Jim Paris <jim@jtan.com>
 * Copyright (C) 2009 Jean-Francois Moine http://moinejf.free.fr
 *
 * Based on a prototype written by Mark Ferrell <majortrips@gmail.com>
 * USB protocol reverse engineered by Jim Paris <jim@jtan.com>
 * https://jim.sh/svn/jim/devl/playstation/ps3/eye/test/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#define MODULE_NAME "ov534"

#include "gspca.h"

//+/* 
//+ * register offset 
//+ */ 
//+#define GAIN        0x00 /* AGC - Gain control gain setting */ 
//+#define BLUE        0x01 /* AWB - Blue channel gain setting */ 
//+#define RED         0x02 /* AWB - Red   channel gain setting */ 
//+#define GREEN       0x03 /* AWB - Green channel gain setting */ 
//+#define COM1        0x04 /* Common control 1 */ 
//+#define BAVG        0x05 /* U/B Average Level */ 
//+#define GAVG        0x06 /* Y/Gb Average Level */ 
//+#define RAVG        0x07 /* V/R Average Level */ 
//+#define AECH        0x08 /* Exposure Value - AEC MSBs */ 
//+#define COM2        0x09 /* Common control 2 */ 
//+#define PID         0x0A /* Product ID Number MSB */ 
//+#define VER         0x0B /* Product ID Number LSB */ 
//+#define COM3        0x0C /* Common control 3 */ 
//+#define COM4        0x0D /* Common control 4 */ 
//+#define COM5        0x0E /* Common control 5 */ 
//+#define COM6        0x0F /* Common control 6 */ 
//+#define AEC         0x10 /* Exposure Value */ 
//+#define CLKRC       0x11 /* Internal clock */ 
//+#define COM7        0x12 /* Common control 7 */ 
//+#define COM8        0x13 /* Common control 8 */ 
//+#define COM9        0x14 /* Common control 9 */ 
//+#define COM10       0x15 /* Common control 10 */ 
//+#define HSTART      0x17 /* Horizontal sensor size */ 
//+#define HSIZE       0x18 /* Horizontal frame (HREF column) end high 8-bit */ 
//+#define VSTART      0x19 /* Vertical frame (row) start high 8-bit */ 
//+#define VSIZE       0x1A /* Vertical sensor size */ 
//+#define PSHFT       0x1B /* Data format - pixel delay select */ 
//+#define MIDH        0x1C /* Manufacturer ID byte - high */ 
//+#define MIDL        0x1D /* Manufacturer ID byte - low  */ 
//+#define LAEC        0x1F /* Fine AEC value */ 
//+#define COM11       0x20 /* Common control 11 */ 
//+#define BDBASE      0x22 /* Banding filter Minimum AEC value */ 
//+#define DBSTEP      0x23 /* Banding filter Maximum Setp */ 
//+#define AEW         0x24 /* AGC/AEC - Stable operating region (upper limit) */ 
//+#define AEB         0x25 /* AGC/AEC - Stable operating region (lower limit) */ 
//+#define VPT         0x26 /* AGC/AEC Fast mode operating region */ 
//+#define HOUTSIZE    0x29 /* Horizontal data output size MSBs */ 
//+#define EXHCH       0x2A /* Dummy pixel insert MSB */ 
//+#define EXHCL       0x2B /* Dummy pixel insert LSB */ 
//+#define VOUTSIZE    0x2C /* Vertical data output size MSBs */ 
//+#define ADVFL       0x2D /* LSB of insert dummy lines in Vertical direction */ 
//+#define ADVFH       0x2E /* MSG of insert dummy lines in Vertical direction */ 
//+#define YAVE        0x2F /* Y/G Channel Average value */ 
//+#define LUMHTH      0x30 /* Histogram AEC/AGC Luminance high level threshold */ 
//+#define LUMLTH      0x31 /* Histogram AEC/AGC Luminance low  level threshold */ 
//+#define HREF        0x32 /* Image start and size control */ 
//+#define DM_LNL      0x33 /* Dummy line low  8 bits */ 
//+#define DM_LNH      0x34 /* Dummy line high 8 bits */ 
//+#define ADOFF_B     0x35 /* AD offset compensation value for B  channel */ 
//+#define ADOFF_R     0x36 /* AD offset compensation value for R  channel */ 
//+#define ADOFF_GB    0x37 /* AD offset compensation value for Gb channel */ 
//+#define ADOFF_GR    0x38 /* AD offset compensation value for Gr channel */ 
//+#define OFF_B       0x39 /* Analog process B  channel offset value */
//+#define OFF_R       0x3A /* Analog process R  channel offset value */
//+#define OFF_GB      0x3B /* Analog process Gb channel offset value */
//+#define OFF_GR      0x3C /* Analog process Gr channel offset value */
//+#define COM12       0x3D /* Common control 12 */ 
//+#define COM13       0x3E /* Common control 13 */ 
//+#define COM14       0x3F /* Common control 14 */ 
//+#define COM15       0x40 /* Common control 15*/ 
//+#define COM16       0x41 /* Common control 16 */ 
//+#define TGT_B       0x42 /* BLC blue channel target value */ 
//+#define TGT_R       0x43 /* BLC red  channel target value */ 
//+#define TGT_GB      0x44 /* BLC Gb   channel target value */ 
//+#define TGT_GR      0x45 /* BLC Gr   channel target value */ 
//+#define LCC0        0x46 /* Lens correction control 0 */ 
//+#define LCC1        0x47 /* Lens correction option 1 - X coordinate */ 
//+#define LCC2        0x48 /* Lens correction option 2 - Y coordinate */ 
//+#define LCC3        0x49 /* Lens correction option 3 */ 
//+#define LCC4        0x4A /* Lens correction option 4 - radius of the circular */ 
//+#define LCC5        0x4B /* Lens correction option 5 */ 
//+#define LCC6        0x4C /* Lens correction option 6 */ 
//+#define FIXGAIN     0x4D /* Analog fix gain amplifer */ 
//+#define AREF0       0x4E /* Sensor reference control */ 
//+#define AREF1       0x4F /* Sensor reference current control */ 
//+#define AREF2       0x50 /* Analog reference control */ 
//+#define AREF3       0x51 /* ADC    reference control */ 
//+#define AREF4       0x52 /* ADC    reference control */ 
//+#define AREF5       0x53 /* ADC    reference control */ 
//+#define AREF6       0x54 /* Analog reference control */ 
//+#define AREF7       0x55 /* Analog reference control */ 
//+#define UFIX        0x60 /* U channel fixed value output */ 
//+#define VFIX        0x61 /* V channel fixed value output */ 
//+#define AW_BB_BLK   0x62 /* AWB option for advanced AWB */ 
//+#define AW_B_CTRL0  0x63 /* AWB control byte 0 */ 
//+#define DSP_CTRL1   0x64 /* DSP control byte 1 */ 
//+#define DSP_CTRL2   0x65 /* DSP control byte 2 */ 
//+#define DSP_CTRL3   0x66 /* DSP control byte 3 */ 
//+#define DSP_CTRL4   0x67 /* DSP control byte 4 */ 
//+#define AW_B_BIAS   0x68 /* AWB BLC level clip */ 
//+#define AW_BCTRL1   0x69 /* AWB control  1 */ 
//+#define AW_BCTRL2   0x6A /* AWB control  2 */ 
//+#define AW_BCTRL3   0x6B /* AWB control  3 */ 
//+#define AW_BCTRL4   0x6C /* AWB control  4 */ 
//+#define AW_BCTRL5   0x6D /* AWB control  5 */ 
//+#define AW_BCTRL6   0x6E /* AWB control  6 */ 
//+#define AW_BCTRL7   0x6F /* AWB control  7 */ 
//+#define AW_BCTRL8   0x70 /* AWB control  8 */ 
//+#define AW_BCTRL9   0x71 /* AWB control  9 */ 
//+#define AW_BCTRL10  0x72 /* AWB control 10 */ 
//+#define AW_BCTRL11  0x73 /* AWB control 11 */ 
//+#define AW_BCTRL12  0x74 /* AWB control 12 */ 
//+#define AW_BCTRL13  0x75 /* AWB control 13 */ 
//+#define AW_BCTRL14  0x76 /* AWB control 14 */ 
//+#define AW_BCTRL15  0x77 /* AWB control 15 */ 
//+#define AW_BCTRL16  0x78 /* AWB control 16 */ 
//+#define AW_BCTRL17  0x79 /* AWB control 17 */ 
//+#define AW_BCTRL18  0x7A /* AWB control 18 */ 
//+#define AW_BCTRL19  0x7B /* AWB control 19 */ 
//+#define AW_BCTRL20  0x7C /* AWB control 20 */ 
//+#define AW_BCTRL21  0x7D /* AWB control 21 */ 
//+#define GAM1        0x7E /* Gamma Curve  1st segment input end point */ 
//+#define GAM2        0x7F /* Gamma Curve  2nd segment input end point */ 
//+#define GAM3        0x80 /* Gamma Curve  3rd segment input end point */ 
//+#define GAM4        0x81 /* Gamma Curve  4th segment input end point */ 
//+#define GAM5        0x82 /* Gamma Curve  5th segment input end point */ 
//+#define GAM6        0x83 /* Gamma Curve  6th segment input end point */ 
//+#define GAM7        0x84 /* Gamma Curve  7th segment input end point */ 
//+#define GAM8        0x85 /* Gamma Curve  8th segment input end point */ 
//+#define GAM9        0x86 /* Gamma Curve  9th segment input end point */ 
//+#define GAM10       0x87 /* Gamma Curve 10th segment input end point */ 
//+#define GAM11       0x88 /* Gamma Curve 11th segment input end point */ 
//+#define GAM12       0x89 /* Gamma Curve 12th segment input end point */ 
//+#define GAM13       0x8A /* Gamma Curve 13th segment input end point */ 
//+#define GAM14       0x8B /* Gamma Curve 14th segment input end point */ 
//+#define GAM15       0x8C /* Gamma Curve 15th segment input end point */ 
//+#define SLOP        0x8D /* Gamma curve highest segment slope */ 
//+#define DNSTH       0x8E /* De-noise threshold */ 
//+#define EDGE0       0x8F /* Edge enhancement control 0 */ 
//+#define EDGE1       0x90 /* Edge enhancement control 1 */ 
//+#define DNSOFF      0x91 /* Auto De-noise threshold control */ 
//+#define EDGE2       0x92 /* Edge enhancement strength low  point control */ 
//+#define EDGE3       0x93 /* Edge enhancement strength high point control */ 
//+#define MTX1        0x94 /* Matrix coefficient 1 */ 
//+#define MTX2        0x95 /* Matrix coefficient 2 */ 
//+#define MTX3        0x96 /* Matrix coefficient 3 */ 
//+#define MTX4        0x97 /* Matrix coefficient 4 */ 
//+#define MTX5        0x98 /* Matrix coefficient 5 */ 
//+#define MTX6        0x99 /* Matrix coefficient 6 */ 
//+#define MTX_CTRL    0x9A /* Matrix control */ 
//+#define BRIGHT      0x9B /* Brightness control */ 
//+#define CNTRST      0x9C /* Contrast contrast */ 
//+#define CNTRST_CTRL 0x9D /* Contrast contrast center */ 
//+#define UVAD_J0     0x9E /* Auto UV adjust contrast 0 */ 
//+#define UVAD_J1     0x9F /* Auto UV adjust contrast 1 */ 
//+#define SCAL0       0xA0 /* Scaling control 0 */ 
//+#define SCAL1       0xA1 /* Scaling control 1 */ 
//+#define SCAL2       0xA2 /* Scaling control 2 */ 
//+#define FIFODLYM    0xA3 /* FIFO manual mode delay control */ 
//+#define FIFODLYA    0xA4 /* FIFO auto   mode delay control */ 
//+#define SDE         0xA6 /* Special digital effect control */ 
//+#define USAT        0xA7 /* U component saturation control */ 
//+#define VSAT        0xA8 /* V component saturation control */ 
//+#define HUE0        0xA9 /* Hue control 0 */ 
//+#define HUE1        0xAA /* Hue control 1 */ 
//+#define SIGN        0xAB /* Sign bit for Hue and contrast */ 
//+#define DSPAUTO     0xAC /* DSP auto function ON/OFF control */ 
//

#define OV534_REG_ADDRESS	0xf1	/* sensor address */
#define OV534_REG_SUBADDR	0xf2
#define OV534_REG_WRITE		0xf3
#define OV534_REG_READ		0xf4
#define OV534_REG_OPERATION	0xf5
#define OV534_REG_STATUS	0xf6

#define OV534_OP_WRITE_3	0x37
#define OV534_OP_WRITE_2	0x33
#define OV534_OP_READ_2		0xf9

#define CTRL_TIMEOUT 500
#define VGA	0
#define QVGA	1


MODULE_AUTHOR("Antonio Ospite <ospite@studenti.unina.it>");
MODULE_DESCRIPTION("GSPCA/OV534 USB Camera Driver(kaswy mod for MT use V0.5 / bear24rw patch V0.1)" \
                   " / ebrahimi patch V0.1");
MODULE_LICENSE("GPL");

static int videomode = 1;
static int autogain = 0;
static int gain = 20;
static int exposure = 120;
static int redblc = 128;
static int blueblc = 128;
static int sharpness = 0;
static int vflip = 0;
static int hflip = 0;
static int hue = 143;
static int awb = 0;
static int brightness =  20;
static int contrast =  37;
static int freqfltr = 0;

/* specific webcam descriptor */
struct sd {
	struct gspca_dev gspca_dev;	/* !! must be the first item */
	__u32 last_pts;
	u16 last_fid;
	u8 frame_rate;
	u8 gain;
	u8 exposure;
	u8 redblc;
	u8 blueblc;
	u8 autogain;
	u8 sharpness;
	u8 hflip;
	u8 vflip;
	u8 hue;
	u8 awb;
	u8 brightness;
	u8 contrast;
        u8 freqfltr;
};

/* V4L2 controls supported by the driver */
static int sd_setgain(struct gspca_dev *gspca_dev, __s32 val);
static int sd_getgain(struct gspca_dev *gspca_dev, __s32 *val);
static int sd_setexposure(struct gspca_dev *gspca_dev, __s32 val);
static int sd_getexposure(struct gspca_dev *gspca_dev, __s32 *val);
static int sd_setredblc(struct gspca_dev *gspca_dev, __s32 val);
static int sd_getredblc(struct gspca_dev *gspca_dev, __s32 *val);
static int sd_setblueblc(struct gspca_dev *gspca_dev, __s32 val);
static int sd_getblueblc(struct gspca_dev *gspca_dev, __s32 *val);
static int sd_setautogain(struct gspca_dev *gspca_dev, __s32 val);
static int sd_getautogain(struct gspca_dev *gspca_dev, __s32 *val);
static int sd_setsharpness(struct gspca_dev *gspca_dev, __s32 val);
static int sd_getsharpness(struct gspca_dev *gspca_dev, __s32 *val);
static int sd_sethflip(struct gspca_dev *gspca_dev, __s32 val);
static int sd_gethflip(struct gspca_dev *gspca_dev, __s32 *val);
static int sd_setvflip(struct gspca_dev *gspca_dev, __s32 val);
static int sd_getvflip(struct gspca_dev *gspca_dev, __s32 *val);
static int sd_sethue(struct gspca_dev *gspca_dev, __s32 val);
static int sd_gethue(struct gspca_dev *gspca_dev, __s32 *val);
static int sd_setawb(struct gspca_dev *gspca_dev, __s32 val);
static int sd_getawb(struct gspca_dev *gspca_dev, __s32 *val);
static int sd_setbrightness(struct gspca_dev *gspca_dev, __s32 val);
static int sd_getbrightness(struct gspca_dev *gspca_dev, __s32 *val);
static int sd_setcontrast(struct gspca_dev *gspca_dev, __s32 val);
static int sd_getcontrast(struct gspca_dev *gspca_dev, __s32 *val);
static int sd_setfreqfltr(struct gspca_dev *gspca_dev, __s32 val);
static int sd_getfreqfltr(struct gspca_dev *gspca_dev, __s32 *val);
static int sd_querymenu(struct gspca_dev *gspca_dev,
                        struct v4l2_querymenu *menu);

static struct ctrl sd_ctrls[] = {
{
	{
		.id      = V4L2_CID_GAIN,
		.type    = V4L2_CTRL_TYPE_INTEGER,
		.name    = "Main Gain",
		.minimum = 0,
		.maximum = 63,
		.step    = 1,
		.default_value = 20,
	},
	.set = sd_setgain,
	.get = sd_getgain,
},

{
	{
		.id      = V4L2_CID_EXPOSURE,
		.type    = V4L2_CTRL_TYPE_INTEGER,
		.name    = "Exposure",
		.minimum = 0,
		.maximum = 255,
		.step    = 1,
		.default_value = 255,
	},
	.set = sd_setexposure,
	.get = sd_getexposure,
},

{
	{
		.id      = V4L2_CID_BRIGHTNESS,
		.type    = V4L2_CTRL_TYPE_INTEGER,
		.name    = "Brightness",
		.minimum = 0,
		.maximum = 255,
		.step    = 1,
		.default_value = 8,
	},
	.set = sd_setbrightness,
	.get = sd_getbrightness,
},

{
	{
		.id      = V4L2_CID_CONTRAST,
		.type    = V4L2_CTRL_TYPE_INTEGER,
		.name    = "Contrast",
		.minimum = 0,
		.maximum = 255,
		.step    = 1,
		.default_value = 0x20,
	},
	.set = sd_setcontrast,
	.get = sd_getcontrast,
},

{
	{
		.id      = V4L2_CID_RED_BALANCE,
		.type    = V4L2_CTRL_TYPE_INTEGER,
		.name    = "Red Balance",
		.minimum = 0,
		.maximum = 255,
		.step    = 1,
		.default_value = 128, 
	},
	.set = sd_setredblc,
	.get = sd_getredblc,
},

{
	{
		.id      = V4L2_CID_BLUE_BALANCE,
		.type    = V4L2_CTRL_TYPE_INTEGER,
		.name    = "Blue Balance",
		.minimum = 0,
		.maximum = 255,
		.step    = 1,
		.default_value = 128,
	},
	.set = sd_setblueblc,
	.get = sd_getblueblc,
},

{
	{
		.id      = V4L2_CID_HUE,
		.type    = V4L2_CTRL_TYPE_INTEGER,
		.name    = "Hue",
		.minimum = 0,
		.maximum = 255,
		.step    = 1,
		.default_value = 165, 
	},
	.set = sd_sethue,
	.get = sd_gethue,
},

{
	{
		.id      = V4L2_CID_AUTOGAIN,
		.type    = V4L2_CTRL_TYPE_BOOLEAN,
		.name    = "Autogain",
		.minimum = 0,
		.maximum = 1,
		.step    = 1,
		.default_value = 1,
	},
	.set = sd_setautogain,
	.get = sd_getautogain,
},

{
	{
		.id      = V4L2_CID_AUTO_WHITE_BALANCE,
		.type    = V4L2_CTRL_TYPE_BOOLEAN,
		.name    = "Auto White Balance",
		.minimum = 0,
		.maximum = 1,
		.step    = 1,
		.default_value = 0,
	},
	.set = sd_setawb,
	.get = sd_getawb,
},

{
	{
		.id      = V4L2_CID_SHARPNESS,
		.type    = V4L2_CTRL_TYPE_INTEGER,
		.name    = "Sharpness",
		.minimum = 0,
		.maximum = 63,
		.step    = 1,
		.default_value = 0,
	},
	.set = sd_setsharpness,
	.get = sd_getsharpness,
},

{
	{
		.id      = V4L2_CID_HFLIP,
		.type    = V4L2_CTRL_TYPE_BOOLEAN,
		.name    = "HFlip",
		.minimum = 0,
		.maximum = 1,
		.step    = 1,
		.default_value = 0,
	},
	.set = sd_sethflip,
	.get = sd_gethflip,
},

{
	{
		.id      = V4L2_CID_VFLIP,
		.type    = V4L2_CTRL_TYPE_BOOLEAN,
		.name    = "VFlip",
		.minimum = 0,
		.maximum = 1,
		.step    = 1,
		.default_value = 0,
	},
	.set = sd_setvflip,
	.get = sd_getvflip,
},

{
        {
            .id      = V4L2_CID_POWER_LINE_FREQUENCY,
            .type    = V4L2_CTRL_TYPE_MENU,
            .name    = "Light Frequency Filter",
            .minimum = 0,
            .maximum = 1,
            .step    = 1,
            .default_value = 1,
        },
        .set = sd_setfreqfltr,
        .get = sd_getfreqfltr,
},

};

static const struct v4l2_pix_format vga_mode[] = {
	{640, 480, V4L2_PIX_FMT_YUYV, V4L2_FIELD_NONE,
	 .bytesperline = 640 * 2,
	 .sizeimage = 640 * 480 * 2,
	 .colorspace = V4L2_COLORSPACE_SRGB,
	 .priv = 0},
};

static const struct v4l2_pix_format qvga_mode[] = {
	{320, 240, V4L2_PIX_FMT_YUYV, V4L2_FIELD_NONE,
	 .bytesperline = 320 * 2,
	 .sizeimage = 320 * 240 * 2,
	 .colorspace = V4L2_COLORSPACE_SRGB,
	 .priv = 0},
};



static const __u8 ov534_reg_initdata[][2] = {
	{ 0xe7, 0x3a },

	{ OV534_REG_ADDRESS, 0x42 }, /* select OV772x sensor */

	{ 0xc2, 0x0c },
	{ 0x88, 0xf8 },
	{ 0xc3, 0x69 },
	{ 0x89, 0xff },
	{ 0x76, 0x03 },
	{ 0x92, 0x01 },
	{ 0x93, 0x18 },
	{ 0x94, 0x10 },
	{ 0x95, 0x10 },
	{ 0xe2, 0x00 },
	{ 0xe7, 0x3e },

	{ 0x96, 0x00 },

	{ 0x97, 0x20 },
	{ 0x97, 0x20 },
	{ 0x97, 0x20 },
	{ 0x97, 0x0a },
	{ 0x97, 0x3f },
	{ 0x97, 0x4a },
	{ 0x97, 0x20 },
	{ 0x97, 0x15 },
	{ 0x97, 0x0b },

	{ 0x8e, 0x40 },
	{ 0x1f, 0x81 },
	{ 0x34, 0x05 },
	{ 0xe3, 0x04 },
	{ 0x88, 0x00 },
	{ 0x89, 0x00 },
	{ 0x76, 0x00 },
	{ 0xe7, 0x2e },
	{ 0x31, 0xf9 },
	{ 0x25, 0x42 },
	{ 0x21, 0xf0 },

	{ 0x1c, 0x00 },
	{ 0x1d, 0x40 },
	{ 0x1d, 0x02 }, /* payload size 0x0200 * 4 = 2048 bytes */
	{ 0x1d, 0x00 }, /* payload size */

// -------------

//	{ 0x1d, 0x01 },/* frame size */		// kwasy
//	{ 0x1d, 0x4b },/* frame size */
//	{ 0x1d, 0x00 }, /* frame size */


//	{ 0x1d, 0x02 },/* frame size */		// macam
//	{ 0x1d, 0x57 },/* frame size */
//	{ 0x1d, 0xff }, /* frame size */

	{ 0x1d, 0x02 },/* frame size */		// jfrancois / linuxtv.org/hg/v4l-dvb
	{ 0x1d, 0x58 },/* frame size */
	{ 0x1d, 0x00 }, /* frame size */

// ---------

	{ 0x1c, 0x0a },
	{ 0x1d, 0x08 }, /* turn on UVC header */
	{ 0x1d, 0x0e }, /* .. */

	{ 0x8d, 0x1c },
	{ 0x8e, 0x80 },
	{ 0xe5, 0x04 },

// ----------------
//	{ 0xc0, 0x28 },//	kwasy / macam
//	{ 0xc1, 0x1e },//

	{ 0xc0, 0x50 },		// jfrancois
	{ 0xc1, 0x3c },
	{ 0xc2, 0x0c }, 


	
};

static const __u8 ov772x_reg_initdata[][2] = {

	{0x12, 0x80 },
	{0x11, 0x01 },
	{0x11, 0x01 },
	{0x11, 0x01 },
	{0x11, 0x01 },
	{0x11, 0x01 },
	{0x11, 0x01 },
	{0x11, 0x01 },
	{0x11, 0x01 },
	{0x11, 0x01 },
	{0x11, 0x01 },
	{0x11, 0x01 },

	{0x3d, 0x03 },
	{0x17, 0x26 },
	{0x18, 0xa0 },
	{0x19, 0x07 },
	{0x1a, 0xf0 },
	{0x32, 0x00 },
	{0x29, 0xa0 },
	{0x2c, 0xf0 },
	{0x65, 0x20 },
	{0x11, 0x01 },
	{0x42, 0x7f },
	{0x63, 0xAA }, 	// AWB
	{0x64, 0xff },
	{0x66, 0x00 },
	{0x13, 0xf0 },	// COM8  - jfrancois 0xf0	orig x0f7
	{0x0d, 0x41 },
	{0x0f, 0xc5 },
	{0x14, 0x11 },

	{0x22, 0x7f },
	{0x23, 0x03 },
	{0x24, 0x40 },
	{0x25, 0x30 },
	{0x26, 0xa1 },
	{0x2a, 0x00 },
	{0x2b, 0x00 }, 
	{0x6b, 0xaa },
	{0x13, 0xff },	// COM8 - jfrancois 0xff orig 0xf7

	{0x90, 0x05 },
	{0x91, 0x01 },
	{0x92, 0x03 },
	{0x93, 0x00 },
	{0x94, 0x60 },
	{0x95, 0x3c },
	{0x96, 0x24 },
	{0x97, 0x1e },
	{0x98, 0x62 },
	{0x99, 0x80 },
	{0x9a, 0x1e },
	{0x9b, 0x08 },
	{0x9c, 0x20 },
	{0x9e, 0x81 },

	{0xa6, 0x04 },
	{0x7e, 0x0c },
	{0x7f, 0x16 },
	{0x80, 0x2a },
	{0x81, 0x4e },
    {0x82, 0x61 },
	{0x83, 0x6f },
	{0x84, 0x7b },
	{0x85, 0x86 },
	{0x86, 0x8e },
	{0x87, 0x97 },
	{0x88, 0xa4 },
	{0x89, 0xaf },
	{0x8a, 0xc5 },
	{0x8b, 0xd7 },
	{0x8c, 0xe8 },
	{0x8d, 0x20 },

	{0x0c, 0x90 },

	{0x2b, 0x00 }, 
	{0x22, 0x7f },
	{0x23, 0x03 },
	{0x11, 0x01 },
	{0x0c, 0xd0 },
	{0x64, 0xff },
	{0x0d, 0x41 },

	{0x14, 0x41 },
	{0x0e, 0xcd },
	{0xac, 0xbf },
	{0x8e, 0x00 },	// De-noise threshold - jfrancois 0x00 - orig 0x04
	{0x0c, 0xd0 }

};

static void ov534_reg_write(struct gspca_dev *gspca_dev, u16 reg, u8 val)
{
	struct usb_device *udev = gspca_dev->dev;
	int ret;

	PDEBUG(D_USBO, "reg=0x%04x, val=0%02x", reg, val);
	gspca_dev->usb_buf[0] = val;
	ret = usb_control_msg(udev,
			      usb_sndctrlpipe(udev, 0),
			      0x01,
			      USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
			      0x00, reg, gspca_dev->usb_buf, 1, CTRL_TIMEOUT);
	if (ret < 0)
		PDEBUG(D_ERR, "write failed");
}

static u8 ov534_reg_read(struct gspca_dev *gspca_dev, u16 reg)
{
	struct usb_device *udev = gspca_dev->dev;
	int ret;

	ret = usb_control_msg(udev,
			      usb_rcvctrlpipe(udev, 0),
			      0x01,
			      USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
			      0x00, reg, gspca_dev->usb_buf, 1, CTRL_TIMEOUT);
	PDEBUG(D_USBI, "reg=0x%04x, data=0x%02x", reg, gspca_dev->usb_buf[0]);
	if (ret < 0)
		PDEBUG(D_ERR, "read failed");
	return gspca_dev->usb_buf[0];
}

/* Two bits control LED: 0x21 bit 7 and 0x23 bit 7.
 * (direction and output)? */
static void ov534_set_led(struct gspca_dev *gspca_dev, int status)
{
	u8 data;

	PDEBUG(D_CONF, "led status: %d", status);

	data = ov534_reg_read(gspca_dev, 0x21);
	data |= 0x80;
	ov534_reg_write(gspca_dev, 0x21, data);

	data = ov534_reg_read(gspca_dev, 0x23);
	if (status)
		data |= 0x80;
	else
		data &= ~0x80;

	ov534_reg_write(gspca_dev, 0x23, data);
	
	if (!status) {
		data = ov534_reg_read(gspca_dev, 0x21);
		data &= ~0x80;
		ov534_reg_write(gspca_dev, 0x21, data);
	}
}

static int sccb_check_status(struct gspca_dev *gspca_dev)
{
	u8 data;
	int i;

	for (i = 0; i < 5; i++) {
		data = ov534_reg_read(gspca_dev, OV534_REG_STATUS);

		switch (data) {
		case 0x00:
			return 1;
		case 0x04:
			return 0;
		case 0x03:
			break;
		default:
			PDEBUG(D_ERR, "sccb status 0x%02x, attempt %d/5",
			       data, i + 1);
		}
	}
	return 0;
}

static void sccb_reg_write(struct gspca_dev *gspca_dev, u8 reg, u8 val)
{
	PDEBUG(D_USBO, "reg: 0x%02x, val: 0x%02x", reg, val);
	ov534_reg_write(gspca_dev, OV534_REG_SUBADDR, reg);
	ov534_reg_write(gspca_dev, OV534_REG_WRITE, val);
	ov534_reg_write(gspca_dev, OV534_REG_OPERATION, OV534_OP_WRITE_3);

	if (!sccb_check_status(gspca_dev))
		PDEBUG(D_ERR, "sccb_reg_write failed");
}


static u8 sccb_reg_read(struct gspca_dev *gspca_dev, u16 reg)
{
	ov534_reg_write(gspca_dev, OV534_REG_SUBADDR, reg);
	ov534_reg_write(gspca_dev, OV534_REG_OPERATION, OV534_OP_WRITE_2);
	if (!sccb_check_status(gspca_dev))
		PDEBUG(D_ERR, "sccb_reg_read failed 1");

	ov534_reg_write(gspca_dev, OV534_REG_OPERATION, OV534_OP_READ_2);
	if (!sccb_check_status(gspca_dev))
		PDEBUG(D_ERR, "sccb_reg_read failed 2");

	return ov534_reg_read(gspca_dev, OV534_REG_READ);
}

/* set videomode */

static void ov534_set_res(struct gspca_dev *gspca_dev, int res)
{
	struct cam *cam;
	cam = &gspca_dev->cam;
	if (res == 1) { //set to qvga
	    cam->cam_mode = qvga_mode;
	    cam->nmodes = ARRAY_SIZE(qvga_mode);
	    ov534_reg_write(gspca_dev, 0x1c, 0x00);
	    ov534_reg_write(gspca_dev, 0x1d, 0x40);
	    ov534_reg_write(gspca_dev, 0x1d, 0x02);
	    ov534_reg_write(gspca_dev, 0x1d, 0x00);
	    ov534_reg_write(gspca_dev, 0x1d, 0x01);
	    ov534_reg_write(gspca_dev, 0x1d, 0x4b);
	    ov534_reg_write(gspca_dev, 0x1d, 0x00);
	    ov534_reg_write(gspca_dev, 0xc0, 0x28);
	    ov534_reg_write(gspca_dev, 0xc1, 0x1e);

    	sccb_reg_write(gspca_dev, 0x12, 0x40);
	    sccb_reg_write(gspca_dev, 0x17, 0x3f);
	    sccb_reg_write(gspca_dev, 0x18, 0x50);
	    sccb_reg_write(gspca_dev, 0x19, 0x03);
	    sccb_reg_write(gspca_dev, 0x1a, 0x78);

	    sccb_reg_write(gspca_dev, 0x29, 0x50);
	    sccb_reg_write(gspca_dev, 0x2c, 0x78);
	    sccb_reg_write(gspca_dev, 0x65, 0x2f);		// http://forums.ps2dev.org/viewtopic.php?p=74541#74541

	}else{ //set to vga default
	    cam->cam_mode = vga_mode;
	    cam->nmodes = ARRAY_SIZE(vga_mode);
	    ov534_reg_write(gspca_dev, 0x1c, 0x00);
	    ov534_reg_write(gspca_dev, 0x1d, 0x40);
	    ov534_reg_write(gspca_dev, 0x1d, 0x02);
	    ov534_reg_write(gspca_dev, 0x1d, 0x00);
	    ov534_reg_write(gspca_dev, 0x1d, 0x02);
	    ov534_reg_write(gspca_dev, 0x1d, 0x58);
	    ov534_reg_write(gspca_dev, 0x1d, 0x00);
	    ov534_reg_write(gspca_dev, 0xc0, 0x50);
	    ov534_reg_write(gspca_dev, 0xc1, 0x3c);

    	sccb_reg_write(gspca_dev, 0x12, 0x00); 
	    sccb_reg_write(gspca_dev, 0x17, 0x26);
	    sccb_reg_write(gspca_dev, 0x18, 0xa0);
	    sccb_reg_write(gspca_dev, 0x19, 0x07);
	    sccb_reg_write(gspca_dev, 0x1a, 0xf0);

	    sccb_reg_write(gspca_dev, 0x29, 0xa0);
	    sccb_reg_write(gspca_dev, 0x2c, 0xf0);
	    sccb_reg_write(gspca_dev, 0x65, 0x20);		// http://forums.ps2dev.org/viewtopic.php?p=74541#74541

	}
}


static void ov534_set_videomode(struct gspca_dev *gspca_dev,int videomode)
{
	struct sd *sd = (struct sd *) gspca_dev;
	int fr = sd->frame_rate;

	switch (videomode) {
	//640x480
	case 0: //640x480@15 
		ov534_set_res(gspca_dev,VGA);
		sccb_reg_write(gspca_dev, 0x11, 0x03);
		sccb_reg_write(gspca_dev, 0x0d, 0x41);
		ov534_reg_write(gspca_dev, 0xe5, 0x04);
		fr = 15;
		break;
		
	case 1: //640x480@30 default
		ov534_set_res(gspca_dev,VGA);
		sccb_reg_write(gspca_dev, 0x11, 0x04);
		sccb_reg_write(gspca_dev, 0x0d, 0x81);
		ov534_reg_write(gspca_dev, 0xe5, 0x02);
		fr = 30;
		break;

	case 2: //640x480@40
		ov534_set_res(gspca_dev,VGA);
		sccb_reg_write(gspca_dev, 0x11, 0x02);
		sccb_reg_write(gspca_dev, 0x0d, 0xc1);
		ov534_reg_write(gspca_dev, 0xe5, 0x04);
		fr = 40;
		break;
	
	case 3: //640x480@50
		ov534_set_res(gspca_dev,VGA);
		sccb_reg_write(gspca_dev, 0x11, 0x01);
		sccb_reg_write(gspca_dev, 0x0d, 0x41);
		ov534_reg_write(gspca_dev, 0xe5, 0x02);
		fr = 50;
		break;

	case 4: //640x480@60
		ov534_set_res(gspca_dev,VGA);
		sccb_reg_write(gspca_dev, 0x11, 0x01);
		sccb_reg_write(gspca_dev, 0x0d, 0xc1);
		ov534_reg_write(gspca_dev, 0xe5, 0x04);
		fr = 60;
		break;

		
	//320x240
	case 10: //320x240@30
		ov534_set_res(gspca_dev,QVGA);
		sccb_reg_write(gspca_dev, 0x11, 0x04);
		sccb_reg_write(gspca_dev, 0x0d, 0x41);
		ov534_reg_write(gspca_dev, 0xe5, 0x04);
		fr = 30;
		break;
	case 11: //320x240@40
		ov534_set_res(gspca_dev,QVGA);
		sccb_reg_write(gspca_dev, 0x11, 0x03);
		sccb_reg_write(gspca_dev, 0x0d, 0x41);
		ov534_reg_write(gspca_dev, 0xe5, 0x04);
		fr = 40;
		break;
	case 12: //320x240@50
		ov534_set_res(gspca_dev,QVGA);
		sccb_reg_write(gspca_dev, 0x11, 0x02);
		sccb_reg_write(gspca_dev, 0x0d, 0x41);
		ov534_reg_write(gspca_dev, 0xe5, 0x04);
		fr = 50;
		break;
	case 13: //320x240@60
		ov534_set_res(gspca_dev,QVGA);
		sccb_reg_write(gspca_dev, 0x11, 0x04);
		sccb_reg_write(gspca_dev, 0x0d, 0xc1);
		ov534_reg_write(gspca_dev, 0xe5, 0x04);
		fr = 60;
		break;
	case 14: //320x240@75
		ov534_set_res(gspca_dev,QVGA);
		sccb_reg_write(gspca_dev, 0x11, 0x03);
		sccb_reg_write(gspca_dev, 0x0d, 0xc1);
		ov534_reg_write(gspca_dev, 0xe5, 0x04);
		fr = 75;
		break;

	case 15: //320x240@100
		ov534_set_res(gspca_dev,QVGA);
		sccb_reg_write(gspca_dev, 0x11, 0x02);
		sccb_reg_write(gspca_dev, 0x0d, 0xc1);
		ov534_reg_write(gspca_dev, 0xe5, 0x04);
		fr = 100;
		break;
	case 16: //320x240@125
		ov534_set_res(gspca_dev,QVGA);
		sccb_reg_write(gspca_dev, 0x11, 0x02);
		sccb_reg_write(gspca_dev, 0x0d, 0x81);
		ov534_reg_write(gspca_dev, 0xe5, 0x02);
		fr = 125;
		break;
	
	}
	PDEBUG(D_PROBE, "set_videomode set to: %d", videomode);
	
	sd->frame_rate = fr;
}

/* setup method */
static void ov534_setup(struct gspca_dev *gspca_dev)
{
	int i;

	/* Initialize bridge chip */
	for (i = 0; i < ARRAY_SIZE(ov534_reg_initdata); i++)
		ov534_reg_write(gspca_dev, ov534_reg_initdata[i][0],
				ov534_reg_initdata[i][1]);

	PDEBUG(D_PROBE, "sensor is ov%02x%02x",
		sccb_reg_read(gspca_dev, 0x0a),
		sccb_reg_read(gspca_dev, 0x0b));

	ov534_set_led(gspca_dev, 1);

	/* Initialize sensor */
	for (i = 0; i < ARRAY_SIZE(ov772x_reg_initdata); i++)
		sccb_reg_write(gspca_dev, ov772x_reg_initdata[i][0],
			       ov772x_reg_initdata[i][1]);

	ov534_reg_write(gspca_dev, 0xe0, 0x09);
	ov534_set_led(gspca_dev, 0);
}

/* this function is called at probe time */
static int sd_config(struct gspca_dev *gspca_dev,
		     const struct usb_device_id *id)
{
	struct cam *cam;

	cam = &gspca_dev->cam;

	cam->cam_mode = vga_mode;
	cam->nmodes = ARRAY_SIZE(vga_mode);
	cam->bulk = 1;
	cam->bulk_size = 16384;
	cam->bulk_nurbs = 2;

	return 0;
}

/* this function is called at probe and resume time */
static int sd_init(struct gspca_dev *gspca_dev)
{
	ov534_setup(gspca_dev);
	ov534_set_videomode(gspca_dev,videomode);

	sd_setautogain(gspca_dev,autogain);
	sd_setawb(gspca_dev, awb);
	sd_setgain(gspca_dev,gain);
	sd_setredblc(gspca_dev,redblc);
	sd_setblueblc(gspca_dev,blueblc);
	sd_sethue(gspca_dev, hue);
	sd_setexposure(gspca_dev,exposure);
	sd_setbrightness(gspca_dev, brightness);
	sd_setcontrast(gspca_dev, contrast);
	sd_setsharpness(gspca_dev,sharpness);
	sd_setvflip(gspca_dev,vflip);	
	sd_sethflip(gspca_dev,hflip);	
        sd_setfreqfltr(gspca_dev,freqfltr);

	return 0;
}

static int sd_start(struct gspca_dev *gspca_dev)
{
	/* start streaming data */
	ov534_set_led(gspca_dev, 1);
	ov534_reg_write(gspca_dev, 0xe0, 0x00);

	return 0;
}

static void sd_stopN(struct gspca_dev *gspca_dev)
{
	/* stop streaming data */
	ov534_reg_write(gspca_dev, 0xe0, 0x09);
	ov534_set_led(gspca_dev, 0);
}

/* Values for bmHeaderInfo (Video and Still Image Payload Headers, 2.4.3.3) */
#define UVC_STREAM_EOH	(1 << 7)
#define UVC_STREAM_ERR	(1 << 6)
#define UVC_STREAM_STI	(1 << 5)
#define UVC_STREAM_RES	(1 << 4)
#define UVC_STREAM_SCR	(1 << 3)
#define UVC_STREAM_PTS	(1 << 2)
#define UVC_STREAM_EOF	(1 << 1)
#define UVC_STREAM_FID	(1 << 0)

static void sd_pkt_scan(struct gspca_dev *gspca_dev, struct gspca_frame *frame,
			__u8 *data, int len)
{
	struct sd *sd = (struct sd *) gspca_dev;
	__u32 this_pts;
	u16 this_fid;
	int remaining_len = len;
	int payload_len;

	payload_len = gspca_dev->cam.bulk ? 2048 : 2040;
	do {
		len = min(remaining_len, payload_len);

		/* Payloads are prefixed with a UVC-style header.  We
		   consider a frame to start when the FID toggles, or the PTS
		   changes.  A frame ends when EOF is set, and we've received
		   the correct number of bytes. */

		/* Verify UVC header.  Header length is always 12 */
		if (data[0] != 12 || len < 12) {
			PDEBUG(D_PACK, "bad header");
			goto discard;
		}

		/* Check errors */
		if (data[1] & UVC_STREAM_ERR) {
			PDEBUG(D_PACK, "payload error");
			goto discard;
		}

		/* Extract PTS and FID */
		if (!(data[1] & UVC_STREAM_PTS)) {
			PDEBUG(D_PACK, "PTS not present");
			goto discard;
		}

		this_pts = (data[5] << 24) | (data[4] << 16)
						| (data[3] << 8) | data[2];
		this_fid = (data[1] & UVC_STREAM_FID) ? 1 : 0;

		/* If PTS or FID has changed, start a new frame. */
		if (this_pts != sd->last_pts || this_fid != sd->last_fid) {
			if (gspca_dev->last_packet_type == INTER_PACKET)
				frame = gspca_frame_add(gspca_dev,
							LAST_PACKET, frame,
							NULL, 0);
			sd->last_pts = this_pts;
			sd->last_fid = this_fid;
			gspca_frame_add(gspca_dev, FIRST_PACKET, frame,
					data + 12, len - 12);
		/* If this packet is marked as EOF, end the frame */
		} else if (data[1] & UVC_STREAM_EOF) {
			sd->last_pts = 0;
			if (frame->data_end - frame->data + (len - 12) !=
			    gspca_dev->width * gspca_dev->height * 2) {
				PDEBUG(D_PACK, "wrong sized frame");
				goto discard;
			}
			frame = gspca_frame_add(gspca_dev, LAST_PACKET, frame,
						data + 12, len - 12);
		} else {

			/* Add the data from this payload */
			gspca_frame_add(gspca_dev, INTER_PACKET, frame,
						data + 12, len - 12);
		}


		/* Done this payload */
		goto scan_next;

discard:
		/* Discard data until a new frame starts. */
		gspca_frame_add(gspca_dev, DISCARD_PACKET, frame, NULL, 0);
scan_next:
		remaining_len -= len;
		data += len;
	} while (remaining_len > 0);
}
	
static int sd_setgain(struct gspca_dev *gspca_dev, __s32 val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    sd->gain = val;
    switch(val & 0x30){
	case 0x00:
	    val &=0x0F;
	    break;
	case 0x10:
	    val &=0x0F;
	    val |=0x30;
	    break;
	case 0x20:
	    val &=0x0F;
	    val |=0x70;
	    break;
	case 0x30:
	    val &=0x0F;
	    val |=0xF0;
	    break;
    }
    sccb_reg_write(gspca_dev, 0x00, val);
    return 0;
}

static int sd_getgain(struct gspca_dev *gspca_dev, __s32 *val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    *val = sd->gain;
    return 0;
}


static int sd_setexposure(struct gspca_dev *gspca_dev, __s32 val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    sd->exposure = val;
    sccb_reg_write(gspca_dev, 0x08, val>>7);
    sccb_reg_write(gspca_dev, 0x10, val<<1);
    return 0;
}

static int sd_getexposure(struct gspca_dev *gspca_dev, __s32 *val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    *val = sd->exposure;
    return 0;
}

static int sd_setbrightness(struct gspca_dev *gspca_dev, __s32 val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    sd->brightness = val;
    sccb_reg_write(gspca_dev, 0x9B, val);
    return 0;
}

static int sd_getbrightness(struct gspca_dev *gspca_dev, __s32 *val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    *val = sd->brightness;
    return 0;
}

static int sd_setcontrast(struct gspca_dev *gspca_dev, __s32 val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    sd->contrast = val;
    sccb_reg_write(gspca_dev, 0x9C, val);
    return 0;
}

static int sd_getcontrast(struct gspca_dev *gspca_dev, __s32 *val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    *val = sd->contrast;
    return 0;
}

static int sd_setredblc(struct gspca_dev *gspca_dev, __s32 val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    sd->redblc = val;
    sccb_reg_write(gspca_dev, 0x43, val);
    return 0;
}

static int sd_getredblc(struct gspca_dev *gspca_dev, __s32 *val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    *val = sd->redblc;
    return 0;
}

static int sd_setblueblc(struct gspca_dev *gspca_dev, __s32 val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    sd->blueblc = val;
    sccb_reg_write(gspca_dev, 0x42, val);
    return 0;
}

static int sd_getblueblc(struct gspca_dev *gspca_dev, __s32 *val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    *val = sd->blueblc;
    return 0;
}

static int sd_sethue(struct gspca_dev *gspca_dev, __s32 val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    sd->hue = val;
    sccb_reg_write(gspca_dev, 0x01, val);
    return 0;
}

static int sd_gethue(struct gspca_dev *gspca_dev, __s32 *val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    *val = sd->hue;
    return 0;
}

static int sd_setautogain(struct gspca_dev *gspca_dev, __s32 val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    sd->autogain = val;
    if (val == 1) {
	sccb_reg_write(gspca_dev, 0x13, 0xf7); //AGC,AEC,AWB ON
	sccb_reg_write(gspca_dev, 0x64, sccb_reg_read(gspca_dev, 0x64)|0x03);
    }else{
	sccb_reg_write(gspca_dev, 0x13, 0xf0); //AGC,AEC,AWB OFF
	sccb_reg_write(gspca_dev, 0x64, sccb_reg_read(gspca_dev, 0x64)&0xFC); 
    }
    return 0;
}

static int sd_getautogain(struct gspca_dev *gspca_dev, __s32 *val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    *val = sd->autogain;
    return 0;
}

static int sd_setawb(struct gspca_dev *gspca_dev, __s32 val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    sd->awb = val;
    if (val == 1) {
		sccb_reg_write(gspca_dev, 0x63, 0xe0); //AWB ON
    }else{
		sccb_reg_write(gspca_dev, 0x63, 0xAA); //AWB OFF
    }
    return 0;
}

static int sd_getawb(struct gspca_dev *gspca_dev, __s32 *val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    *val = sd->awb;
    return 0;
}

static int sd_setsharpness(struct gspca_dev *gspca_dev, __s32 val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    sd->sharpness = val;
    sccb_reg_write(gspca_dev, 0x91, val); //vga noise
    sccb_reg_write(gspca_dev, 0x8E, val); //qvga noise
    return 0;
}

static int sd_getsharpness(struct gspca_dev *gspca_dev, __s32 *val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    *val = sd->sharpness;
    return 0;
}

static int sd_sethflip(struct gspca_dev *gspca_dev, __s32 val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    sd->hflip = val;
    if (val == 0) {
	sccb_reg_write(gspca_dev, 0x0c, sccb_reg_read(gspca_dev, 0x0c)|0x40);
    }else{
	sccb_reg_write(gspca_dev, 0x0c, sccb_reg_read(gspca_dev, 0x0c)&0xBF);
    }
    return 0;
}

static int sd_gethflip(struct gspca_dev *gspca_dev, __s32 *val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    *val = sd->hflip;
    return 0;
}
static int sd_setvflip(struct gspca_dev *gspca_dev, __s32 val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    sd->vflip = val;
    if (val == 0) {
	sccb_reg_write(gspca_dev, 0x0c, sccb_reg_read(gspca_dev, 0x0c)|0x80);
    }else{
	sccb_reg_write(gspca_dev, 0x0c, sccb_reg_read(gspca_dev, 0x0c)&0x7F);
    }
    return 0;
}

static int sd_getvflip(struct gspca_dev *gspca_dev, __s32 *val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    *val = sd->vflip;
    return 0;
}

static int sd_setfreqfltr(struct gspca_dev *gspca_dev, __s32 val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    sd->freqfltr = val;
    if (val == 0) {
        sccb_reg_write(gspca_dev, 0x2b, 0x9e);
    }else if (val == 1){
        sccb_reg_write(gspca_dev, 0x2b, 0x00);
    }
    return 0;
}

static int sd_getfreqfltr(struct gspca_dev *gspca_dev, __s32 *val)
{
    struct sd *sd = (struct sd *) gspca_dev;
    *val = sd->freqfltr;
    return 0;
}

static int sd_querymenu(struct gspca_dev *gspca_dev,
                        struct v4l2_querymenu *menu)
{
        switch (menu->id) {
        case V4L2_CID_POWER_LINE_FREQUENCY:
                switch (menu->index) {
                case 0:         /* V4L2_CID_POWER_LINE_FREQUENCY_50HZ */
                        strcpy((char *) menu->name, "50 Hz");
                        return 0;
                case 1:         /* V4L2_CID_POWER_LINE_FREQUENCY_60HZ */
                        strcpy((char *) menu->name, "60 Hz");
                        return 0;
                }
                break;
        }
        return -EINVAL;
}


/* get stream parameters (framerate) */
static int sd_get_streamparm(struct gspca_dev *gspca_dev,
			     struct v4l2_streamparm *parm)
{
	struct v4l2_captureparm *cp = &parm->parm.capture;
	struct v4l2_fract *tpf = &cp->timeperframe;
	struct sd *sd = (struct sd *) gspca_dev;

	if (parm->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

	cp->capability |= V4L2_CAP_TIMEPERFRAME;
	tpf->numerator = 1;
	tpf->denominator = sd->frame_rate;

	return 0;
}

/* set stream parameters (framerate) */
static int sd_set_streamparm(struct gspca_dev *gspca_dev,
			     struct v4l2_streamparm *parm)
{
	struct v4l2_captureparm *cp = &parm->parm.capture;
	struct v4l2_fract *tpf = &cp->timeperframe;
	struct sd *sd = (struct sd *) gspca_dev;

	if (parm->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

	/* Set requested framerate */
	sd->frame_rate = tpf->denominator / tpf->numerator;

	/* Return the actual framerate */
	tpf->numerator = 1;
	tpf->denominator = sd->frame_rate;

	return 0;
}

/* sub-driver description */
static const struct sd_desc sd_desc = {
	.name     = MODULE_NAME,
	.ctrls    = sd_ctrls,
	.nctrls   = ARRAY_SIZE(sd_ctrls),
	.config   = sd_config,
	.init     = sd_init,
	.start    = sd_start,
	.stopN    = sd_stopN,
	.pkt_scan = sd_pkt_scan,
	.querymenu = sd_querymenu,
	.get_streamparm = sd_get_streamparm,
	.set_streamparm = sd_set_streamparm,
};

/* -- module initialisation -- */
static const __devinitdata struct usb_device_id device_table[] = {
	{USB_DEVICE(0x1415, 0x2000)},	/* Sony HD Eye for PS3 (SLEH 00201) */
	{}
};

MODULE_DEVICE_TABLE(usb, device_table);

/* -- device connect -- */
static int sd_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	return gspca_dev_probe(intf, id, &sd_desc, sizeof(struct sd),
			       THIS_MODULE);
}

static struct usb_driver sd_driver = {
	.name       = MODULE_NAME,
	.id_table   = device_table,
	.probe      = sd_probe,
	.disconnect = gspca_disconnect,
#ifdef CONFIG_PM
	.suspend    = gspca_suspend,
	.resume     = gspca_resume,
#endif
};

/* -- module insert / remove -- */
static int __init sd_mod_init(void)
{
	int ret;
	ret = usb_register(&sd_driver);
	if (ret < 0)
		return ret;
	PDEBUG(D_PROBE, "registered");
	return 0;
}

static void __exit sd_mod_exit(void)
{
	usb_deregister(&sd_driver);
	PDEBUG(D_PROBE, "deregistered");
}

module_init(sd_mod_init);
module_exit(sd_mod_exit);

module_param(videomode, int, 0000);
module_param(autogain, bool, 0000);
module_param(awb, bool, 0000);
module_param(gain, int, 0000);
module_param(redblc, int, 0000);
module_param(blueblc, int, 0000);
module_param(hue, int, 0000);
module_param(exposure, int, 0000);
module_param(brightness, int, 0000);
module_param(contrast, int, 0000);
module_param(sharpness, int, 0000);
module_param(vflip, bool, 0000);
module_param(hflip, bool, 0000);
module_param(freqfltr, int, 0000);

MODULE_PARM_DESC(videomode, " = xx //Set the videomode(see doc)");
MODULE_PARM_DESC(autogain, " = [0|1] //Autogain (agc,aec,awb)");
MODULE_PARM_DESC(gain, " = [0..63] //main gain");
MODULE_PARM_DESC(awb, " = [0|1] //Auto White Balance");
MODULE_PARM_DESC(exposure, " = [0..255] //Exposure");
MODULE_PARM_DESC(brightness, " = [0..255] //Brightness");
MODULE_PARM_DESC(contrast, " = [0..255] //Contrast");
MODULE_PARM_DESC(redblc, " = [0..255] //Red Balance");
MODULE_PARM_DESC(blueblc, " = [0..255] //Blue Balance");
MODULE_PARM_DESC(hue, " = [0..255] //Hue");
MODULE_PARM_DESC(sharpness, " = [0..63] //Sharpness");
MODULE_PARM_DESC(vflip, " = [0|1] //Vertical flip");
MODULE_PARM_DESC(hflip, " = [0|1] //Horizontal mirror");
MODULE_PARM_DESC(freqfltr, " = [0|1] //Light frequency filter (50 Hz|60 Hz)");

