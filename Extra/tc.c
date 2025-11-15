#include "tc.h"
#include "iic.h"
#include "panel.h"

uint16_t sta_ylj;
uint16_t status = 0, int_sys_status = 0;
uint8_t sys_int = 0, sys_status = 0;
uint8_t hdmi_int = 0;

void tc_task(void)
{
	HAL_Delay(500);
	HAL_GPIO_WritePin(GPIOA,RESETN_Pin,GPIO_PIN_RESET);//reset tc
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);//EN 1V1
	HAL_Delay(450);
	HAL_GPIO_WritePin(GPIOA,RESETN_Pin,GPIO_PIN_SET);//set tc
	HAL_Delay(100);
	
	RS1();
	
	while(1)
	{
		int_status_check();
	}
}

void Waitx1ms(int16_t ms)
{
	HAL_Delay(ms);
}

void Waitx1us(int16_t ms)
{
	HAL_Delay(1);
}

void RS1(void) 
{
	// Initialization for Stand-by (RS1)				
	// Software Reset				
	i2c1_uh2cd_write16(0x0004,0x0004); // ConfCtl0				
	i2c1_uh2cd_write16(0x0002,0x3F00); // SysCtl				
	i2c1_uh2cd_write16(0x0002,0x0000); // SysCtl				
	i2c1_uh2cd_write16(0x0006,0x0000); // ConfCtl1				
	// HDMI Interrupt Mask, Clear				
	i2c1_uh2cd_write16(0x0016,0x0F3F); // IntMask				
	i2c1_uh2cd_write8(0x8502,0xFF); // SYS_INT				
	i2c1_uh2cd_write8(0x850B,0xFF); // MISC_INT				
	i2c1_uh2cd_write16(0x0014,0x0F3F); // IntStatus				
	i2c1_uh2cd_write8(0x8512,0xFE); // SYS_INTM				
	i2c1_uh2cd_write8(0x851B,0xFD); // MISC_INTM				
	// HDMI PHY				
	i2c1_uh2cd_write8(0x8410,0x03); // PHY CTL				
	i2c1_uh2cd_write8(0x8413,0x3F); // PHY_ENB				
	i2c1_uh2cd_write8(0x8420,0x07); // EQ_BYPS				
	i2c1_uh2cd_write8(0x84F0,0x31); // APLL_CTL				
	i2c1_uh2cd_write8(0x84F4,0x01); // DDCIO_CTL				
	// HDMI Clock				
	i2c1_uh2cd_write16(0x8540,0x12C0); // SYS_FREQ0_1				
	i2c1_uh2cd_write8(0x8630,0x00); // LOCKDET_FREQ0				
	i2c1_uh2cd_write16(0x8631,0x0753); // LOCKDET_REF1_2				
	i2c1_uh2cd_write8(0x8670,0x02); // NCO_F0_MOD				
	i2c1_uh2cd_write16(0x8A0C,0x12C0); // CSC_SCLK0_1				
	// EDID				
	i2c1_uh2cd_write8(0x85E0,0x01); // EDID_MODE				
	i2c1_uh2cd_write16(0x85E3,0x0100); // EDID_LEN1_2				
	// EDID Data				
	i2c1_uh2cd_write8(0x8C00,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C01,0xFF); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C02,0xFF); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C03,0xFF); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C04,0xFF); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C05,0xFF); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C06,0xFF); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C07,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C08,0x52); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C09,0x62); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C0A,0x88); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C0B,0x88); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C0C,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C0D,0x88); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C0E,0x88); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C0F,0x88); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C10,0x1C); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C11,0x15); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C12,0x01); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C13,0x03); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C14,0x80); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C15,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C16,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C17,0x78); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C18,0x0A); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C19,0x0D); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C1A,0xC9); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C1B,0xA0); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C1C,0x57); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C1D,0x47); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C1E,0x98); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C1F,0x27); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C20,0x12); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C21,0x48); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C22,0x4C); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C23,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C24,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C25,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C26,0x01); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C27,0x01); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C28,0x01); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C29,0x01); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C2A,0x01); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C2B,0x01); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C2C,0x01); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C2D,0x01); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C2E,0x01); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C2F,0x01); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C30,0x01); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C31,0x01); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C32,0x01); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C33,0x01); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C34,0x01); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C35,0x01); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C36,0x28); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C37,0x3C); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C38,0x38); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C39,0xFA); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C3A,0x40); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C3B,0x80); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C3C,0x0C); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C3D,0x70); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C3E,0x64); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C3F,0x32); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C40,0x44); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C41,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C42,0xC0); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C43,0x78); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C44,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C45,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C46,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C47,0x18); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C48,0x28); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C49,0x3C); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C4A,0x38); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C4B,0xFA); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C4C,0x40); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C4D,0x80); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C4E,0x0C); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C4F,0x70); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C50,0x64); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C51,0x32); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C52,0x44); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C53,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C54,0xC0); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C55,0x78); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C56,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C57,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C58,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C59,0x18); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C5A,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C5B,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C5C,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C5D,0xFC); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C5E,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C5F,0x54); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C60,0x6F); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C61,0x73); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C62,0x68); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C63,0x69); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C64,0x62); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C65,0x61); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C66,0x2D); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C67,0x55); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C68,0x48); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C69,0x32); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C6A,0x44); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C6B,0x0A); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C6C,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C6D,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C6E,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C6F,0xFD); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C70,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C71,0x14); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C72,0x78); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C73,0x01); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C74,0xFF); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C75,0x1D); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C76,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C77,0x0A); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C78,0x20); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C79,0x20); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C7A,0x20); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C7B,0x20); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C7C,0x20); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C7D,0x20); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C7E,0x01); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C7F,0x71); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C80,0x02); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C81,0x03); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C82,0x1A); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C83,0x71); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C84,0x47); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C85,0xC6); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C86,0x46); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C87,0x46); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C88,0x46); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C89,0x46); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C8A,0x46); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C8B,0x46); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C8C,0x23); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C8D,0x09); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C8E,0x07); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C8F,0x01); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C90,0x83); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C91,0x01); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C92,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C93,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C94,0x65); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C95,0x03); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C96,0x0C); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C97,0x00); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C98,0x10); // EDID_RAM				
	i2c1_uh2cd_write8(0x8C99,0x00); // EDID_RAM				
	// Complete EDID data - shortened for brevity
	i2c1_uh2cd_write8(0x8CFF,0x93); // EDID_RAM				
	// HDCP Setting				
	// HDMI SYSTEM				
	i2c1_uh2cd_write8(0x8543,0x02); // DDC_CTL				
	i2c1_uh2cd_write8(0x8544,0x10); // HPD_CTL				
	// HDMI Audio Setting				
	i2c1_uh2cd_write8(0x8600,0x00); // AUD_Auto_Mute				
	i2c1_uh2cd_write8(0x8602,0xF3); // Auto_CMD0				
	i2c1_uh2cd_write8(0x8603,0x02); // Auto_CMD1				
	i2c1_uh2cd_write8(0x8604,0x0C); // Auto_CMD2				
	i2c1_uh2cd_write8(0x8606,0x05); // BUFINIT_START				
	i2c1_uh2cd_write8(0x8607,0x00); // FS_MUTE				
	i2c1_uh2cd_write8(0x8652,0x02); // SDO_MODE1				
	i2c1_uh2cd_write32(0x8671,0x020C49BA); // NCO_48F0A_D				
	i2c1_uh2cd_write32(0x8675,0x01E1B089); // NCO_44F0A_D				
	i2c1_uh2cd_write8(0x8680,0x00); // AUD_MODE				
	// Enable Interrupt				
	i2c1_uh2cd_write16(0x0016,0x0F1F); // IntMask				
	// Enter Sleep				
	i2c1_uh2cd_write16(0x0002,0x0001); // SysCtl
}

void RS2(void)
{
	// Initialization for Ready (RS2)
	// Enable Interrupt
	i2c1_uh2cd_write16(0x0016, 0x0D3F); // IntMask
	// Set HPDO to "H"
	i2c1_uh2cd_write8(0x854A, 0x01); // INIT_END
}

void RS3(void)
{			
	// MIPI Output Enable(RS3)				
	// Check HDMI resolution/format				
	Waitx1ms(1000);				
	// MIPI Output Setting				
	// Stop Video and Audio				
	i2c1_uh2cd_write16(0x0004,0x0C34); // ConfCtl0				
	// Soft Reset CSI-TX and Splitter Block				
	i2c1_uh2cd_write16(0x0002,0x1200); // SysCtl				
	i2c1_uh2cd_write16(0x0002,0x0000); // SysCtl				
	// Video Color Format Setting				
	i2c1_uh2cd_write8(0x8A02,0x42); // VOUT_SYNC0				
	// DSI-TX0 Transition Timing				
	i2c1_uh2cd_write32(0x0108,0x00000001); // DSI_TX_CLKEN				
	i2c1_uh2cd_write32(0x010C,0x00000001); // DSI_TX_CLKSEL				
	i2c1_uh2cd_write32(0x02A0,0x00000001); // MIPI_PLL_CONTROL				
	i2c1_uh2cd_write32(0x02AC,0x000090CF); // MIPI_PLL_CNF				
	Waitx1ms(1);				
	i2c1_uh2cd_write32(0x02A0,0x00000003); // MIPI_PLL_CONTROL				
	i2c1_uh2cd_write32(0x0118,0x00000014); // LANE_ENABLE				
	i2c1_uh2cd_write32(0x0120,0x00001770); // LINE_INIT_COUNT				
	i2c1_uh2cd_write32(0x0124,0x00000000); // HSTX_TO_COUNT				
	i2c1_uh2cd_write32(0x0128,0x00000101); // FUNC_ENABLE				
	// Additional DSI configuration...
	i2c1_uh2cd_write32(0x011C,0x00000001); // DSITX_START				
	// Command Transmission Before Video Start				
	i2c1_uh2cd_write16(0x0500,0x0004); // CMD_SEL				
	i2c1_uh2cd_write32(0x0110,0x00000016); // MODE_CONFIG				
	i2c1_uh2cd_write32(0x0310,0x00000016); // MODE_CONFIG	
	// LCD Initialization
	Panel_Init();

	// Split Control				
	i2c1_uh2cd_write16(0x5000,0x0100); // STX0_CTL				
	i2c1_uh2cd_write16(0x500C,0x0000); // STX0_FPX				
	i2c1_uh2cd_write16(0x500E,0x021B); // STX0_LPX				
	i2c1_uh2cd_write16(0x5080,0x0100); // STX1_CTL				
	i2c1_uh2cd_write16(0x508C,0x021C); // STX1_FPX				
	i2c1_uh2cd_write16(0x508E,0x0437); // STX1_LPX				
	i2c1_uh2cd_write16(0x7080,0x0080); // STX_DBG				
	i2c1_uh2cd_write16(0x5008,0x0CA8); // STX0_WC				
	i2c1_uh2cd_write16(0x5088,0x0CA8); // STX1_WC				
	// Clear interrupts and start video				
	i2c1_uh2cd_write8(0x8502,0xFF); // SYS_INT				
	i2c1_uh2cd_write8(0x850B,0xFF); // MISC_INT				
	i2c1_uh2cd_write16(0x0014,0x0F3F); // IntStatus				
	i2c1_uh2cd_write16(0x0016,0x0D3F); // IntMask				
	// Start Video TX				
	i2c1_uh2cd_write16(0x0004,0x0C37); // ConfCtl0				
	i2c1_uh2cd_write16(0x0006,0x0000); // ConfCtl1				
	// Command Transmission After Video Start.				
	i2c1_uh2cd_write32(0x0110,0x00000006); // MODE_CONFIG				
	i2c1_uh2cd_write32(0x0310,0x00000006); // MODE_CONFIG
	Waitx1ms(200);
	
	i2c1_uh2cd_write16(0x0504,0x0005); // DCSCMD_Q				
	i2c1_uh2cd_write16(0x0504,0x0029); // DCSCMD_Q				
	HAL_Delay(1);
}

void RS4(void)
{
	// MIPI Output Disable(RS4)
	// Set Display OFF
	i2c1_uh2cd_write16(0x0504, 0x0005); // DCSCMD_Q
	i2c1_uh2cd_write16(0x0504, 0x0028); // DCSCMD_Q
	Waitx1ms(100);

	i2c1_uh2cd_write16(0x0004, 0x0C34); // ConfCtl0
	// Enable Interrupt
	i2c1_uh2cd_write16(0x0016, 0x0D3F); // IntMask
}

void RS5(void)
{
	// MIPI Output Disable & Sleep(RS5)
	// Set Display OFF
	i2c1_uh2cd_write16(0x0504, 0x0005); // DCSCMD_Q
	i2c1_uh2cd_write16(0x0504, 0x0028); // DCSCMD_Q
	Waitx1ms(100);
	// In Sleep
	i2c1_uh2cd_write16(0x0504, 0x0005); // DCSCMD_Q
	i2c1_uh2cd_write16(0x0504, 0x0010); // DCSCMD_Q
	Waitx1ms(200);

	i2c1_uh2cd_write16(0x0004, 0x0C34); // ConfCtl0
	// Enable Interrupt
	i2c1_uh2cd_write16(0x0016, 0x0F1F); // IntMask
	// Enter Sleep
	i2c1_uh2cd_write16(0x0002, 0x0001); // SysCtl
}

void int_status_check(void)
{
	i2c1_uh2cd_write16(0x0002, 0x0000); // SysCtl
	Waitx1us(10);

	//Check Interrupt
	status = i2c1_uh2cd_read16(0x0014);

	//Check bit5 (SYS_INT interrupt).
	if (status & 0x20) {
		int_sys_status = i2c1_uh2cd_read16(0x001A);
		//Check 0x001A<0>=1 (HPDI Change interrupt)
		if (int_sys_status & 0x01) {
			RS2();
		}
	}

	//Check bit9 (HDMI-RX interrupt).
	if (status & 0x0200) {
		//Check 0x850B<1>=1 & 0x8520<7>=1
		hdmi_int = i2c1_uh2cd_read8(0x850B);
		//Check if bit1=1 (Sync Change interrupt)
		if (hdmi_int & 0x02) {
			sys_status = i2c1_uh2cd_read8(0x8520); // SYS_STATUS
			if (sys_status & 0x80) 
			{
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET);//EN 1V8
				HAL_Delay(200);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);//RST Panel E4
				HAL_Delay(200);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
				HAL_Delay(10);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
				HAL_Delay(100);
				//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);//EN VP_VN
				//HAL_Delay(200);
				RS3();
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET);//EN Backlight
			}
			//Check disconnect
			if (sys_status & 0x01) 
			{
				if (!(sys_status & 0x80)) 
				{
					HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);//DIS Backlight
					RS4();
					HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);//DIS 1V8
					HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);//RST Panel
				}
			}
		}
	}
	//Check DDC interrupt  
	sys_int = i2c1_uh2cd_read8(0x8502); // SYS_INT
	if (sys_int & 0x01) {
		sys_status = i2c1_uh2cd_read8(0x8520); // SYS_STATUS
		if (!(sys_status & 0x01)) 
		{
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);//DIS Backlight
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);//DIS 1V8
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);//RST Panel
			Waitx1ms(10);
			RS5();
		} 
		else if(sys_status & 0x01) 
		{
			RS2();
		}
	}
}