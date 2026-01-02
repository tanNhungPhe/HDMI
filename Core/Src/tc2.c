#include "tc.h"
#include "iic.h"
#include "panel.h"

void Panel_Init_2(void)
{
	i2c1_uh2cd_write16(0x0504,0x0015);
	i2c1_uh2cd_write16(0x0504,0x00ff);

	i2c1_uh2cd_write16(0x0504,0x0015);
	i2c1_uh2cd_write16(0x0504,0x01fb);

	HAL_Delay(30);

	i2c1_uh2cd_write16(0x0504,0x0015);
	i2c1_uh2cd_write16(0x0504,0x00ff);

	i2c1_uh2cd_write16(0x0504,0x0015);
	i2c1_uh2cd_write16(0x0504,0x08d3);

	i2c1_uh2cd_write16(0x0504,0x0015);
	i2c1_uh2cd_write16(0x0504,0x0ed4);

	i2c1_uh2cd_write16(0x0504,0x0005); // DCSCMD_Q
	i2c1_uh2cd_write16(0x0504,0x0011); // DCSCMD_Q
	HAL_Delay(120);
}

void Debug_Check_GPIO_And_Registers(void)
{
	printf("\r\n========== DEBUG CHECK ==========\r\n");
	
	// Check GPIO E pins
	printf("\r\n--- GPIO E Status ---\r\n");
	printf("E1  (1V1 VDD_HDMI):    %d\r\n", HAL_GPIO_ReadPin(E1_GPIO_Port, E1_Pin));
	printf("E2  (1V2 VDD_MIPI):    %d\r\n", HAL_GPIO_ReadPin(E2_GPIO_Port, E2_Pin));
	printf("E3  (3V3 VDD33_HDMI):  %d\r\n", HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1));
	printf("E4  (1V8 VDD_DISP):    %d\r\n", HAL_GPIO_ReadPin(E4_GPIO_Port, E4_Pin));
	printf("E5  (VDD_BL V3):       %d\r\n", HAL_GPIO_ReadPin(E5_GPIO_Port, E5_Pin));
	printf("E6  (3V3 VDD_DISP):    %d\r\n", HAL_GPIO_ReadPin(E6_GPIO_Port, E6_Pin));
	printf("E7  (VDD_OLED V1/V2):  %d\r\n", HAL_GPIO_ReadPin(E7_GPIO_Port, E7_Pin));
	printf("E8  (VDD_BL V3):       %d\r\n", HAL_GPIO_ReadPin(E8_GPIO_Port, E8_Pin));
	printf("E9  (Feedback):        %d\r\n", HAL_GPIO_ReadPin(E9_GPIO_Port, E9_Pin));
	printf("E10 (VDD_OLED V1/V2):  %d\r\n", HAL_GPIO_ReadPin(E10_GPIO_Port, E10_Pin));
	
	// Check Panel Reset pins
	printf("\r\n--- Panel Reset Pins ---\r\n");
	printf("A (RST Panel):         %d\r\n", HAL_GPIO_ReadPin(A_GPIO_Port, A_Pin));
	printf("B (RST Panel):         %d\r\n", HAL_GPIO_ReadPin(B_GPIO_Port, B_Pin));
	printf("C (RST Panel):         %d\r\n", HAL_GPIO_ReadPin(C_GPIO_Port, C_Pin));
	printf("D (RST Panel):         %d\r\n", HAL_GPIO_ReadPin(D_GPIO_Port, D_Pin));
	
	// Check TC358870 critical registers
	printf("\r\n--- TC358870 Registers ---\r\n");
	uint16_t sys_status = i2c1_uh2cd_read16(0x0004);
	printf("SYS_STATUS (0x0004):   0x%04X\r\n", sys_status);
	
	uint16_t clk_status = i2c1_uh2cd_read16(0x0006);
	printf("CLK_STATUS (0x0006):   0x%04X\r\n", clk_status);
	
	uint8_t lane_status = i2c1_uh2cd_read8(0x0290);
	printf("LANE_STATUS (0x0290):  0x%02X ", lane_status);
	if (lane_status == 0) {
		printf("❌ LANES NOT ENABLED!\r\n");
	} else {
		printf("✓ OK\r\n");
	}
	
	uint32_t lane_tx0 = i2c1_uh2cd_read32(0x0118);
	printf("LANE_EN TX0 (0x0118):  0x%08X ", lane_tx0);
	if (lane_tx0 == 0) {
		printf("❌ NOT ENABLED!\r\n");
	} else {
		printf("✓ OK\r\n");
	}
	
	uint32_t lane_tx1 = i2c1_uh2cd_read32(0x0318);
	printf("LANE_EN TX1 (0x0318):  0x%08X ", lane_tx1);
	if (lane_tx1 == 0) {
		printf("❌ NOT ENABLED!\r\n");
	} else {
		printf("✓ OK\r\n");
	}
	
	uint8_t hdmi_sys_status = i2c1_uh2cd_read8(0x8520);
	printf("HDMI SYS_STATUS (0x8520): 0x%02X ", hdmi_sys_status);
	if (hdmi_sys_status & 0x80) {
		printf("✓ SYNC DETECTED\r\n");
	} else {
		printf("❌ NO SYNC\r\n");
	}
	
	uint8_t hdmi_int = i2c1_uh2cd_read8(0x850B);
	printf("HDMI_INT (0x850B):     0x%02X\r\n", hdmi_int);
	
	uint16_t conf_ctl0 = i2c1_uh2cd_read16(0x0004);
	printf("ConfCtl0 (0x0004):     0x%04X ", conf_ctl0);
	if ((conf_ctl0 & 0x0003) == 0x03) {
		printf("✓ RGB888 + Video ON\r\n");
	} else {
		printf("❌ Video not enabled correctly\r\n");
	}
	
	// Check MIPI PLL status
	uint32_t pll_tx0 = i2c1_uh2cd_read32(0x02A0);
	printf("MIPI PLL TX0 (0x02A0): 0x%08X ", pll_tx0);
	if ((pll_tx0 & 0x03) == 0x03) {
		printf("✓ PLL ON\r\n");
	} else {
		printf("❌ PLL OFF\r\n");
	}
	
	uint32_t pll_tx1 = i2c1_uh2cd_read32(0x04A0);
	printf("MIPI PLL TX1 (0x04A0): 0x%08X ", pll_tx1);
	if ((pll_tx1 & 0x03) == 0x03) {
		printf("✓ PLL ON\r\n");
	} else {
		printf("❌ PLL OFF\r\n");
	}
	
	uint32_t DSI_ACKERROR = i2c1_uh2cd_read8(0x01B4);
	printf("DSI_ACKERROR(0x01B4): 0x%08X ", DSI_ACKERROR);

	printf("\r\n=================================\r\n\r\n");
}

void HDMI_Startup(void)
{
	printf("HDMI_Startup: Starting...\r\n");
	
	// Software Reset
	i2c1_uh2cd_write16(0x0004,0x0004); // ConfCtl0
	i2c1_uh2cd_write16(0x0002,0x3F01); // SysCtl
	i2c1_uh2cd_write16(0x0002,0x0080); // SysCtl
	i2c1_uh2cd_write16(0x0006,0x0008); // ConfCtl1
	
	printf("HDMI_Startup: Software reset done\r\n");
	// DSI-TX0 Transition Timing
	i2c1_uh2cd_write32(0x0108,0x00000001); // DSI_TX_CLKEN
	i2c1_uh2cd_write32(0x010C,0x00000001); // DSI_TX_CLKSEL
	i2c1_uh2cd_write32(0x02A0,0x00000001); // MIPI_PLL_CONTROL
	i2c1_uh2cd_write32(0x02AC,0x000090CF); // MIPI_PLL_CNF
	HAL_Delay(1);
	i2c1_uh2cd_write32(0x02A0,0x00000003); // MIPI_PLL_CONTROL
	i2c1_uh2cd_write32(0x0118,0x00000014); // LANE_ENABLE
	i2c1_uh2cd_write32(0x0120,0x00001770); // LINE_INIT_COUNT
	i2c1_uh2cd_write32(0x0124,0x00000000); // HSTX_TO_COUNT
	i2c1_uh2cd_write32(0x0128,0x00000101); // FUNC_ENABLE
	i2c1_uh2cd_write32(0x0130,0x00010000); // DSI_TATO_COUNT
	i2c1_uh2cd_write32(0x0134,0x00005000); // DSI_PRESP_BTA_COUNT
	i2c1_uh2cd_write32(0x0138,0x00010000); // DSI_PRESP_LPR_COUNT
	i2c1_uh2cd_write32(0x013C,0x00010000); // DSI_PRESP_LPW_COUNT
	i2c1_uh2cd_write32(0x0140,0x00010000); // DSI_PRESP_HSR_COUNT
	i2c1_uh2cd_write32(0x0144,0x00010000); // DSI_PRESP_HSW_COUNT
	i2c1_uh2cd_write32(0x0148,0x00001000); // DSI_PR_TO_COUNT
	i2c1_uh2cd_write32(0x014C,0x00010000); // DSI_LRX-H_TO_COUNT
	i2c1_uh2cd_write32(0x0150,0x00000160); // FUNC_MODE
	i2c1_uh2cd_write32(0x0154,0x00000001); // DSI_RX_VC_ENABLE
	i2c1_uh2cd_write32(0x0158,0x000000C8); // IND_TO_COUNT
	i2c1_uh2cd_write32(0x0168,0x0000002A); // DSI_HSYNC_STOP_COUNT
	i2c1_uh2cd_write32(0x0170,0x0000040D); // APF_VDELAYCNT
	i2c1_uh2cd_write32(0x017C,0x00000081); // DSI_TX_MODE
	i2c1_uh2cd_write32(0x018C,0x00000001); // DSI_HSYNC_WIDTH
	i2c1_uh2cd_write32(0x0190,0x00000104); // DSI_HBPR
	i2c1_uh2cd_write32(0x01A4,0x00000000); // DSI_RX_STATE_INT_MASK
	i2c1_uh2cd_write32(0x01C0,0x00000015); // DSI_LPRX_THRESH_COUNT
	i2c1_uh2cd_write32(0x0214,0x00000000); // APP_SIDE_ERR_INT_MASK
	i2c1_uh2cd_write32(0x021C,0x00000080); // DSI_RX_ERR_INT_MASK
	i2c1_uh2cd_write32(0x0224,0x00000000); // DSI_LPTX_INT_MASK
	i2c1_uh2cd_write32(0x0254,0x00000006); // LPTXTIMECNT
	i2c1_uh2cd_write32(0x0258,0x00240204); // TCLK_HEADERCNT
	i2c1_uh2cd_write32(0x025C,0x000D0008); // TCLK_TRAILCNT
	i2c1_uh2cd_write32(0x0260,0x00140006); // THS_HEADERCNT
	i2c1_uh2cd_write32(0x0264,0x00004268); // TWAKEUPCNT
	i2c1_uh2cd_write32(0x0268,0x0000000F); // TCLK_POSTCNT
	i2c1_uh2cd_write32(0x026C,0x000D0008); // THS_TRAILCNT
	i2c1_uh2cd_write32(0x0270,0x00000020); // HSTXVREGCNT
	i2c1_uh2cd_write32(0x0274,0x0000001F); // HSTXVREGEN
	i2c1_uh2cd_write32(0x0278,0x00060005); // BTA_COUNT
	i2c1_uh2cd_write32(0x027C,0x00000002); // DPHY_TX ADJUST
	i2c1_uh2cd_write32(0x011C,0x00000001); // DSITX_START
	// DSI-TX1 Transition Timing
	i2c1_uh2cd_write32(0x0308,0x00000001); // DSI_TX_CLKEN
	i2c1_uh2cd_write32(0x030C,0x00000001); // DSI_TX_CLKSEL
	i2c1_uh2cd_write32(0x04A0,0x00000001); // MIPI_PLL_CONTROL
	i2c1_uh2cd_write32(0x04AC,0x000090CF); // MIPI_PLL_CNF
	HAL_Delay(1);
	i2c1_uh2cd_write32(0x04A0,0x00000003); // MIPI_PLL_CONTROL
	i2c1_uh2cd_write32(0x0318,0x00000014); // LANE_ENABLE
	i2c1_uh2cd_write32(0x0320,0x00001770); // LINE_INIT_COUNT
	i2c1_uh2cd_write32(0x0324,0x00000000); // HSTX_TO_COUNT
	i2c1_uh2cd_write32(0x0328,0x00000101); // FUNC_ENABLE
	i2c1_uh2cd_write32(0x0330,0x00010000); // DSI_TATO_COUNT
	i2c1_uh2cd_write32(0x0334,0x00005000); // DSI_PRESP_BTA_COUNT
	i2c1_uh2cd_write32(0x0338,0x00010000); // DSI_PRESP_LPR_COUNT
	i2c1_uh2cd_write32(0x033C,0x00010000); // DSI_PRESP_LPW_COUNT
	i2c1_uh2cd_write32(0x0340,0x00010000); // DSI_PRESP_HSR_COUNT
	i2c1_uh2cd_write32(0x0344,0x00010000); // DSI_PRESP_HSW_COUNT
	i2c1_uh2cd_write32(0x0348,0x00001000); // DSI_PR_TO_COUNT
	i2c1_uh2cd_write32(0x034C,0x00010000); // DSI_LRX-H_TO_COUNT
	i2c1_uh2cd_write32(0x0350,0x00000160); // FUNC_MODE
	i2c1_uh2cd_write32(0x0354,0x00000001); // DSI_RX_VC_ENABLE
	i2c1_uh2cd_write32(0x0358,0x000000C8); // IND_TO_COUNT
	i2c1_uh2cd_write32(0x0368,0x0000002A); // DSI_HSYNC_STOP_COUNT
	i2c1_uh2cd_write32(0x0370,0x0000040D); // APF_VDELAYCNT
	i2c1_uh2cd_write32(0x037C,0x00000081); // DSI_TX_MODE
	i2c1_uh2cd_write32(0x038C,0x00000001); // DSI_HSYNC_WIDTH
	i2c1_uh2cd_write32(0x0390,0x00000104); // DSI_HBPR
	i2c1_uh2cd_write32(0x03A4,0x00000000); // DSI_RX_STATE_INT_MASK
	i2c1_uh2cd_write32(0x03C0,0x00000015); // DSI_LPRX_THRESH_COUNT
	i2c1_uh2cd_write32(0x0414,0x00000000); // APP_SIDE_ERR_INT_MASK
	i2c1_uh2cd_write32(0x041C,0x00000080); // DSI_RX_ERR_INT_MASK
	i2c1_uh2cd_write32(0x0424,0x00000000); // DSI_LPTX_INT_MASK
	i2c1_uh2cd_write32(0x0454,0x00000006); // LPTXTIMECNT
	i2c1_uh2cd_write32(0x0458,0x00240204); // TCLK_HEADERCNT
	i2c1_uh2cd_write32(0x045C,0x000D0008); // TCLK_TRAILCNT
	i2c1_uh2cd_write32(0x0460,0x00140006); // THS_HEADERCNT
	i2c1_uh2cd_write32(0x0464,0x00004268); // TWAKEUPCNT
	i2c1_uh2cd_write32(0x0468,0x0000000F); // TCLK_POSTCNT
	i2c1_uh2cd_write32(0x046C,0x000D0008); // THS_TRAILCNT
	i2c1_uh2cd_write32(0x0470,0x00000020); // HSTXVREGCNT
	i2c1_uh2cd_write32(0x0474,0x0000001F); // HSTXVREGEN
	i2c1_uh2cd_write32(0x0478,0x00060005); // BTA_COUNT
	i2c1_uh2cd_write32(0x047C,0x00000002); // DPHY_TX ADJUST
	i2c1_uh2cd_write32(0x031C,0x00000001); // DSITX_START
	// Command Transmission Before Video Start
	i2c1_uh2cd_write32(0x0110,0x00000016); // MODE_CONFIG
	i2c1_uh2cd_write32(0x0310,0x00000016); // MODE_CONFIG
	
	printf("HDMI_Startup: MIPI DSI configured\r\n");
	
	// LCD Initialization
	i2c1_uh2cd_write16(0x0500,0x0004); // CMD_SEL
	
	printf("HDMI_Startup: Calling Panel_Init_2...\r\n");
	Panel_Init_2();
	printf("HDMI_Startup: Panel_Init_2 done\r\n");

	// Split Control
	i2c1_uh2cd_write16(0x5000,0x0100); // STX0_CTL
	i2c1_uh2cd_write16(0x500C,0x8000); // STX0_FPX
	i2c1_uh2cd_write16(0x5080,0x0100); // STX1_CTL
	// HDMI PHY
	i2c1_uh2cd_write8(0x8410,0x03); // PHY CTL
	i2c1_uh2cd_write8(0x8413,0x3F); // PHY_ENB
	i2c1_uh2cd_write8(0x84F0,0x31); // APLL_CTL
	i2c1_uh2cd_write8(0x84F4,0x01); // DDCIO_CTL
	// HDMI Clock
	i2c1_uh2cd_write16(0x8540,0x12C0); // SYS_FREQ0_1
	i2c1_uh2cd_write8(0x8630,0x00); // LOCKDET_FREQ0
	i2c1_uh2cd_write16(0x8631,0x0753); // LOCKDET_REF1_2
	i2c1_uh2cd_write8(0x8670,0x02); // NCO_F0_MOD
	i2c1_uh2cd_write16(0x8A0C,0x12C0); // CSC_SCLK0_1
	// HDMI Interrupt Mask, Clear
	i2c1_uh2cd_write8(0x8502,0xFF); // SYS_INT
	i2c1_uh2cd_write8(0x8512,0xFE); // SYS_INTM
	// Interrupt Control (TOP level)
	i2c1_uh2cd_write16(0x0014,0x0FBF); // IntStatus
	i2c1_uh2cd_write16(0x0016,0x0DBF); // IntMask
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
	i2c1_uh2cd_write8(0x8C42,0x38); // EDID_RAM
	i2c1_uh2cd_write8(0x8C43,0x80); // EDID_RAM
	i2c1_uh2cd_write8(0x8C44,0x47); // EDID_RAM
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
	i2c1_uh2cd_write8(0x8C54,0x38); // EDID_RAM
	i2c1_uh2cd_write8(0x8C55,0x80); // EDID_RAM
	i2c1_uh2cd_write8(0x8C56,0x47); // EDID_RAM
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
	i2c1_uh2cd_write8(0x8C71,0x17); // EDID_RAM
	i2c1_uh2cd_write8(0x8C72,0x3D); // EDID_RAM
	i2c1_uh2cd_write8(0x8C73,0x0F); // EDID_RAM
	i2c1_uh2cd_write8(0x8C74,0x8C); // EDID_RAM
	i2c1_uh2cd_write8(0x8C75,0x17); // EDID_RAM
	i2c1_uh2cd_write8(0x8C76,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8C77,0x0A); // EDID_RAM
	i2c1_uh2cd_write8(0x8C78,0x20); // EDID_RAM
	i2c1_uh2cd_write8(0x8C79,0x20); // EDID_RAM
	i2c1_uh2cd_write8(0x8C7A,0x20); // EDID_RAM
	i2c1_uh2cd_write8(0x8C7B,0x20); // EDID_RAM
	i2c1_uh2cd_write8(0x8C7C,0x20); // EDID_RAM
	i2c1_uh2cd_write8(0x8C7D,0x20); // EDID_RAM
	i2c1_uh2cd_write8(0x8C7E,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8C7F,0x87); // EDID_RAM
	i2c1_uh2cd_write8(0x8C80,0x02); // EDID_RAM
	i2c1_uh2cd_write8(0x8C81,0x03); // EDID_RAM
	i2c1_uh2cd_write8(0x8C82,0x17); // EDID_RAM
	i2c1_uh2cd_write8(0x8C83,0x74); // EDID_RAM
	i2c1_uh2cd_write8(0x8C84,0x47); // EDID_RAM
	i2c1_uh2cd_write8(0x8C85,0x04); // EDID_RAM
	i2c1_uh2cd_write8(0x8C86,0x13); // EDID_RAM
	i2c1_uh2cd_write8(0x8C87,0x03); // EDID_RAM
	i2c1_uh2cd_write8(0x8C88,0x02); // EDID_RAM
	i2c1_uh2cd_write8(0x8C89,0x07); // EDID_RAM
	i2c1_uh2cd_write8(0x8C8A,0x06); // EDID_RAM
	i2c1_uh2cd_write8(0x8C8B,0x01); // EDID_RAM
	i2c1_uh2cd_write8(0x8C8C,0x23); // EDID_RAM
	i2c1_uh2cd_write8(0x8C8D,0x09); // EDID_RAM
	i2c1_uh2cd_write8(0x8C8E,0x07); // EDID_RAM
	i2c1_uh2cd_write8(0x8C8F,0x01); // EDID_RAM
	i2c1_uh2cd_write8(0x8C90,0x66); // EDID_RAM
	i2c1_uh2cd_write8(0x8C91,0x03); // EDID_RAM
	i2c1_uh2cd_write8(0x8C92,0x0C); // EDID_RAM
	i2c1_uh2cd_write8(0x8C93,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8C94,0x30); // EDID_RAM
	i2c1_uh2cd_write8(0x8C95,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8C96,0x80); // EDID_RAM
	i2c1_uh2cd_write8(0x8C97,0x8C); // EDID_RAM
	i2c1_uh2cd_write8(0x8C98,0x0A); // EDID_RAM
	i2c1_uh2cd_write8(0x8C99,0xD0); // EDID_RAM
	i2c1_uh2cd_write8(0x8C9A,0x28); // EDID_RAM
	i2c1_uh2cd_write8(0x8C9B,0x3C); // EDID_RAM
	i2c1_uh2cd_write8(0x8C9C,0x38); // EDID_RAM
	i2c1_uh2cd_write8(0x8C9D,0xFA); // EDID_RAM
	i2c1_uh2cd_write8(0x8C9E,0x40); // EDID_RAM
	i2c1_uh2cd_write8(0x8C9F,0x80); // EDID_RAM
	i2c1_uh2cd_write8(0x8CA0,0x0C); // EDID_RAM
	i2c1_uh2cd_write8(0x8CA1,0x70); // EDID_RAM
	i2c1_uh2cd_write8(0x8CA2,0x64); // EDID_RAM
	i2c1_uh2cd_write8(0x8CA3,0x32); // EDID_RAM
	i2c1_uh2cd_write8(0x8CA4,0x44); // EDID_RAM
	i2c1_uh2cd_write8(0x8CA5,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CA6,0x38); // EDID_RAM
	i2c1_uh2cd_write8(0x8CA7,0x80); // EDID_RAM
	i2c1_uh2cd_write8(0x8CA8,0x47); // EDID_RAM
	i2c1_uh2cd_write8(0x8CA9,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CAA,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CAB,0x18); // EDID_RAM
	i2c1_uh2cd_write8(0x8CAC,0x28); // EDID_RAM
	i2c1_uh2cd_write8(0x8CAD,0x3C); // EDID_RAM
	i2c1_uh2cd_write8(0x8CAE,0x38); // EDID_RAM
	i2c1_uh2cd_write8(0x8CAF,0xFA); // EDID_RAM
	i2c1_uh2cd_write8(0x8CB0,0x40); // EDID_RAM
	i2c1_uh2cd_write8(0x8CB1,0x80); // EDID_RAM
	i2c1_uh2cd_write8(0x8CB2,0x0C); // EDID_RAM
	i2c1_uh2cd_write8(0x8CB3,0x70); // EDID_RAM
	i2c1_uh2cd_write8(0x8CB4,0x64); // EDID_RAM
	i2c1_uh2cd_write8(0x8CB5,0x32); // EDID_RAM
	i2c1_uh2cd_write8(0x8CB6,0x44); // EDID_RAM
	i2c1_uh2cd_write8(0x8CB7,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CB8,0x38); // EDID_RAM
	i2c1_uh2cd_write8(0x8CB9,0x80); // EDID_RAM
	i2c1_uh2cd_write8(0x8CBA,0x47); // EDID_RAM
	i2c1_uh2cd_write8(0x8CBB,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CBC,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CBD,0x18); // EDID_RAM
	i2c1_uh2cd_write8(0x8CBE,0x28); // EDID_RAM
	i2c1_uh2cd_write8(0x8CBF,0x3C); // EDID_RAM
	i2c1_uh2cd_write8(0x8CC0,0x38); // EDID_RAM
	i2c1_uh2cd_write8(0x8CC1,0xFA); // EDID_RAM
	i2c1_uh2cd_write8(0x8CC2,0x40); // EDID_RAM
	i2c1_uh2cd_write8(0x8CC3,0x80); // EDID_RAM
	i2c1_uh2cd_write8(0x8CC4,0x0C); // EDID_RAM
	i2c1_uh2cd_write8(0x8CC5,0x70); // EDID_RAM
	i2c1_uh2cd_write8(0x8CC6,0x64); // EDID_RAM
	i2c1_uh2cd_write8(0x8CC7,0x32); // EDID_RAM
	i2c1_uh2cd_write8(0x8CC8,0x44); // EDID_RAM
	i2c1_uh2cd_write8(0x8CC9,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CCA,0x38); // EDID_RAM
	i2c1_uh2cd_write8(0x8CCB,0x80); // EDID_RAM
	i2c1_uh2cd_write8(0x8CCC,0x47); // EDID_RAM
	i2c1_uh2cd_write8(0x8CCD,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CCE,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CCF,0x18); // EDID_RAM
	i2c1_uh2cd_write8(0x8CD0,0x28); // EDID_RAM
	i2c1_uh2cd_write8(0x8CD1,0x3C); // EDID_RAM
	i2c1_uh2cd_write8(0x8CD2,0x38); // EDID_RAM
	i2c1_uh2cd_write8(0x8CD3,0xFA); // EDID_RAM
	i2c1_uh2cd_write8(0x8CD4,0x40); // EDID_RAM
	i2c1_uh2cd_write8(0x8CD5,0x80); // EDID_RAM
	i2c1_uh2cd_write8(0x8CD6,0x0C); // EDID_RAM
	i2c1_uh2cd_write8(0x8CD7,0x70); // EDID_RAM
	i2c1_uh2cd_write8(0x8CD8,0x64); // EDID_RAM
	i2c1_uh2cd_write8(0x8CD9,0x32); // EDID_RAM
	i2c1_uh2cd_write8(0x8CDA,0x44); // EDID_RAM
	i2c1_uh2cd_write8(0x8CDB,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CDC,0x38); // EDID_RAM
	i2c1_uh2cd_write8(0x8CDD,0x80); // EDID_RAM
	i2c1_uh2cd_write8(0x8CDE,0x47); // EDID_RAM
	i2c1_uh2cd_write8(0x8CDF,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CE0,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CE1,0x18); // EDID_RAM
	i2c1_uh2cd_write8(0x8CE2,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CE3,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CE4,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CE5,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CE6,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CE7,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CE8,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CE9,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CEA,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CEB,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CEC,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CED,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CEE,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CEF,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CF0,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CF1,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CF2,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CF3,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CF4,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CF5,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CF6,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CF7,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CF8,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CF9,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CFA,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CFB,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CFC,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CFD,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CFE,0x00); // EDID_RAM
	i2c1_uh2cd_write8(0x8CFF,0x34); // EDID_RAM
	// HDCP Setting
	// Video Color Format Setting
	i2c1_uh2cd_write8(0x8A02,0x42); // VOUT_SYNC0
	// HDMI SYSTEM
	i2c1_uh2cd_write8(0x8543,0x02); // DDC_CTL
	i2c1_uh2cd_write8(0x8544,0x10); // HPD_CTL
	// HDMI Audio Setting
	// Let HDMI Source start access
	i2c1_uh2cd_write8(0x854A,0x01); // INIT_END
	
	printf("HDMI_Startup: Starting video TX...\r\n");
	
	// Start Video TX
	i2c1_uh2cd_write16(0x0004,0x0C17); // ConfCtl0
	i2c1_uh2cd_write16(0x0006,0x0000); // ConfCtl1
	// Command Transmission After Video Start.
	i2c1_uh2cd_write32(0x0110,0x00000006); // MODE_CONFIG
	i2c1_uh2cd_write32(0x0310,0x00000006); // MODE_CONFIG
	HAL_Delay(100);
	i2c1_uh2cd_write16(0x0504,0x0015);
	i2c1_uh2cd_write16(0x0504,0xFF51); // Write Display Brightness (51h, tham số 0xFF) [14]
	i2c1_uh2cd_write16(0x0504,0x0015);
	i2c1_uh2cd_write16(0x0504,0x2C53); // Write Control Display (53h, tham số 0x2C: Bật LED PWM) [14]
	HAL_Delay(100); // Chờ thêm sau khi bật đèn nền.
	// Set Display On
	i2c1_uh2cd_write16(0x0504,0x0005); // DCSCMD_Q
	i2c1_uh2cd_write16(0x0504,0x0029); // DCSCMD_Q
	HAL_Delay(10);
	
	printf("HDMI_Startup: Complete!\r\n");
}

void TC358870_Enable_ColorBar(void) {
    // 1. Cấu hình các thông số khung hình (Timing) cho bộ Color Bar
    // Dựa trên thông số màn hình FHD+ 1080x1920 [8, 9]

    i2c1_uh2cd_write16(0x7010, 1080); // cb_h_act: Số pixel ngang hiển thị [10, 11]
    i2c1_uh2cd_write16(0x7012, 1920); // cb_v_act: Số dòng dọc hiển thị [12, 13]

    i2c1_uh2cd_write16(0x700C, 1176); // cb_h_total: Tổng số pixel ngang (H-Total) [14, 15]
    i2c1_uh2cd_write16(0x700E, 1942); // cb_v_total: Tổng số dòng dọc (V-Total) [11, 16]

    // 2. Cấu hình vị trí bắt đầu và độ rộng xung đồng bộ (điển hình)
    i2c1_uh2cd_write16(0x7014, 40);   // cb_h_start [13, 17]
    i2c1_uh2cd_write16(0x7016, 10);   // cb_v_start [13, 18]
    i2c1_uh2cd_write16(0x7008, 10);   // cb_hs_width [19, 20]
    i2c1_uh2cd_write16(0x700A, 2);    // cb_vs_width [15, 21]

    // 3. Thiết lập chế độ và Kích hoạt
    // Thanh ghi 0x7000 (CB_CTRL0):
    // Bit 0: cb_en = 1 (Bật Color Bar) [20, 22]
    // Bit 2:1: cb_type = 10 (Chế độ Color Bar) [2, 20]
    // Bit 3: cb_clksel = 0 (Sử dụng CSITX0 Byte clock) [2, 20]
    // Giá trị ghi: 0b0101 = 0x0005

    i2c1_uh2cd_write16(0x7000, 0x0005);

    printf("Internal Color Bar Enabled\r\n");
}
