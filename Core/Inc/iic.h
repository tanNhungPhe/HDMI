#ifndef _IIC_H
#define _IIC_H

#include "i2cSoft.h"

// Wrapper macros để tương thích với code cũ
// Map các hàm i2c1_uh2cd_write* sang TC358870_i2c_Write
#define i2c1_uh2cd_write8(reg, val)    TC358870_i2c_Write(reg, val, 1)
#define i2c1_uh2cd_write16(reg, val)   TC358870_i2c_Write(reg, val, 2)
#define i2c1_uh2cd_write32(reg, val)   TC358870_i2c_Write(reg, val, 4)

// Map các hàm đọc
#define i2c1_uh2cd_read8(reg)          TC358870_i2c_Read(reg)
#define i2c1_uh2cd_read16(reg)         TC358870_i2c_Read16(reg)
#define i2c1_uh2cd_read32(reg)         ({ uint32_t val; TC358870_i2c_Read32(reg, &val); val; })

#endif /* _IIC_H */
