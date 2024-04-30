/* File generated from device description on 30/04/2024, 21:08:55 */

/* LSM6DSO32: 3-axis gyroscope and accelerometer */

#ifndef LSM6DSO32_REGS
#define LSM6DSO32_REGS

#define LSM6DSO32_ADDR_0                                              (0x6a)  /* LSM6DSO32 address */
#define LSM6DSO32_ADDR_1                                              (0x6b)  /* LSM6DSO32 address */

/* LSM6DSO32_FUNC_CFG_ACCESS, R/W: Enable embedded functions register */
#define LSM6DSO32_FUNC_CFG_ACCESS_ADDR                                (0x1)  /* (LSM6DSO32_FUNC_CFG_ACCESS) Enable embedded functions register address */

#define LSM6DSO32_FUNC_CFG_ACCESS_SHUB_REG_ACCESS_Pos                 (6)  /* (LSM6DSO32_FUNC_CFG_ACCESS) Enable access to the sensor hub (I²C master) registers position */
#define LSM6DSO32_FUNC_CFG_ACCESS_SHUB_REG_ACCESS_Msk                 ((0x1) << LSM6DSO32_FUNC_CFG_ACCESS_SHUB_REG_ACCESS_Pos)  /* (LSM6DSO32_FUNC_CFG_ACCESS) Enable access to the sensor hub (I²C master) registers mask */
#define LSM6DSO32_FUNC_CFG_ACCESS_SHUB_REG_ACCESS(value)              (LSM6DSO32_FUNC_CFG_ACCESS_SHUB_REG_ACCESS_Msk & ((value) << LSM6DSO32_FUNC_CFG_ACCESS_SHUB_REG_ACCESS_Pos))  /* Assignment of SHUB_REG_ACCESS value in LSM6DSO32_FUNC_CFG_ACCESS register */
#define LSM6DSO32_FUNC_CFG_ACCESS_SHUB_REG_ACCESS_Get(reg)            (((reg) & LSM6DSO32_FUNC_CFG_ACCESS_SHUB_REG_ACCESS_Msk) >> LSM6DSO32_FUNC_CFG_ACCESS_SHUB_REG_ACCESS_Pos)  /* Assignment of SHUB_REG_ACCESS value in LSM6DSO32_FUNC_CFG_ACCESS register */
#define LSM6DSO32_FUNC_CFG_ACCESS_FUNC_CFG_ACCESS_Pos                 (7)  /* (LSM6DSO32_FUNC_CFG_ACCESS) Enable access to the embedded functions configuration registers position */
#define LSM6DSO32_FUNC_CFG_ACCESS_FUNC_CFG_ACCESS_Msk                 ((0x1) << LSM6DSO32_FUNC_CFG_ACCESS_FUNC_CFG_ACCESS_Pos)  /* (LSM6DSO32_FUNC_CFG_ACCESS) Enable access to the embedded functions configuration registers mask */
#define LSM6DSO32_FUNC_CFG_ACCESS_FUNC_CFG_ACCESS(value)              (LSM6DSO32_FUNC_CFG_ACCESS_FUNC_CFG_ACCESS_Msk & ((value) << LSM6DSO32_FUNC_CFG_ACCESS_FUNC_CFG_ACCESS_Pos))  /* Assignment of FUNC_CFG_ACCESS value in LSM6DSO32_FUNC_CFG_ACCESS register */
#define LSM6DSO32_FUNC_CFG_ACCESS_FUNC_CFG_ACCESS_Get(reg)            (((reg) & LSM6DSO32_FUNC_CFG_ACCESS_FUNC_CFG_ACCESS_Msk) >> LSM6DSO32_FUNC_CFG_ACCESS_FUNC_CFG_ACCESS_Pos)  /* Assignment of FUNC_CFG_ACCESS value in LSM6DSO32_FUNC_CFG_ACCESS register */
#define LSM6DSO32_FUNC_CFG_ACCESS_Msk                                 (0xc0)  /* (LSM6DSO32_FUNC_CFG_ACCESS) Register mask */


/* LSM6DSO32_PIN_CTRL, R/W: SDO pin pull-up enable/disable register */
#define LSM6DSO32_PIN_CTRL_ADDR                                       (0x2)  /* (LSM6DSO32_PIN_CTRL) SDO pin pull-up enable/disable register address */
#define LSM6DSO32_PIN_CTRL_RESETVALUE                                 (0x3f)  /* (LSM6DSO32_PIN_CTRL) SDO pin pull-up enable/disable register reset value */

#define LSM6DSO32_PIN_CTRL_SDO_PU_EN_Pos                              (6)  /* (LSM6DSO32_PIN_CTRL) Enable pull-up on SDO pin position */
#define LSM6DSO32_PIN_CTRL_SDO_PU_EN_Msk                              ((0x1) << LSM6DSO32_PIN_CTRL_SDO_PU_EN_Pos)  /* (LSM6DSO32_PIN_CTRL) Enable pull-up on SDO pin mask */
#define LSM6DSO32_PIN_CTRL_SDO_PU_EN(value)                           (LSM6DSO32_PIN_CTRL_SDO_PU_EN_Msk & ((value) << LSM6DSO32_PIN_CTRL_SDO_PU_EN_Pos))  /* Assignment of SDO_PU_EN value in LSM6DSO32_PIN_CTRL register */
#define LSM6DSO32_PIN_CTRL_SDO_PU_EN_Get(reg)                         (((reg) & LSM6DSO32_PIN_CTRL_SDO_PU_EN_Msk) >> LSM6DSO32_PIN_CTRL_SDO_PU_EN_Pos)  /* Assignment of SDO_PU_EN value in LSM6DSO32_PIN_CTRL register */
#define LSM6DSO32_PIN_CTRL_Msk                                        (0x40)  /* (LSM6DSO32_PIN_CTRL) Register mask */


/* LSM6DSO32_FIFO_CTRL1, R/W: FIFO control register 1 */
#define LSM6DSO32_FIFO_CTRL1_ADDR                                     (0x7)  /* (LSM6DSO32_FIFO_CTRL1) FIFO control register 1 address */

#define LSM6DSO32_FIFO_CTRL1_WTM_Pos                                  (0)  /* (LSM6DSO32_FIFO_CTRL1) FIFO watermark threshold, in conjunction with WTM8 in FIFO_CTRL2 position */
#define LSM6DSO32_FIFO_CTRL1_WTM_Msk                                  ((0xff) << LSM6DSO32_FIFO_CTRL1_WTM_Pos)  /* (LSM6DSO32_FIFO_CTRL1) FIFO watermark threshold, in conjunction with WTM8 in FIFO_CTRL2 mask */
#define LSM6DSO32_FIFO_CTRL1_WTM(value)                               (LSM6DSO32_FIFO_CTRL1_WTM_Msk & ((value) << LSM6DSO32_FIFO_CTRL1_WTM_Pos))  /* Assignment of WTM value in LSM6DSO32_FIFO_CTRL1 register */
#define LSM6DSO32_FIFO_CTRL1_WTM_Get(reg)                             (((reg) & LSM6DSO32_FIFO_CTRL1_WTM_Msk) >> LSM6DSO32_FIFO_CTRL1_WTM_Pos)  /* Assignment of WTM value in LSM6DSO32_FIFO_CTRL1 register */
#define LSM6DSO32_FIFO_CTRL1_Msk                                      (0xff)  /* (LSM6DSO32_FIFO_CTRL1) Register mask */


/* LSM6DSO32_FIFO_CTRL2, R/W: FIFO control register 2 */
#define LSM6DSO32_FIFO_CTRL2_ADDR                                     (0x8)  /* (LSM6DSO32_FIFO_CTRL2) FIFO control register 2 address */

#define LSM6DSO32_FIFO_CTRL2_WTM_Pos                                  (0)  /* (LSM6DSO32_FIFO_CTRL2) FIFO watermark threshold, in conjunction with WTM_FIFO[7:0] in FIFO_CTRL1 position */
#define LSM6DSO32_FIFO_CTRL2_WTM_Msk                                  ((0x1) << LSM6DSO32_FIFO_CTRL2_WTM_Pos)  /* (LSM6DSO32_FIFO_CTRL2) FIFO watermark threshold, in conjunction with WTM_FIFO[7:0] in FIFO_CTRL1 mask */
#define LSM6DSO32_FIFO_CTRL2_WTM(value)                               (LSM6DSO32_FIFO_CTRL2_WTM_Msk & ((value) << LSM6DSO32_FIFO_CTRL2_WTM_Pos))  /* Assignment of WTM value in LSM6DSO32_FIFO_CTRL2 register */
#define LSM6DSO32_FIFO_CTRL2_WTM_Get(reg)                             (((reg) & LSM6DSO32_FIFO_CTRL2_WTM_Msk) >> LSM6DSO32_FIFO_CTRL2_WTM_Pos)  /* Assignment of WTM value in LSM6DSO32_FIFO_CTRL2 register */
#define LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_Pos                         (1)  /* (LSM6DSO32_FIFO_CTRL2) This field configures the compression algorithm to write non-compressed data at each rate position */
#define LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_Msk                         ((0x3) << LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_Pos)  /* (LSM6DSO32_FIFO_CTRL2) This field configures the compression algorithm to write non-compressed data at each rate mask */
#define LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE(value)                      (LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_Msk & ((value) << LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_Pos))  /* Assignment of UNCOPTR_RATE value in LSM6DSO32_FIFO_CTRL2 register */
#define LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_Get(reg)                    (((reg) & LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_Msk) >> LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_Pos)  /* Assignment of UNCOPTR_RATE value in LSM6DSO32_FIFO_CTRL2 register */
#define   LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_NOF_Val                   (0)  /* (LSM6DSO32_FIFO_CTRL2) Non-compressed data writing is not forced value */
#define   LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_8BDR_Val                  (1)  /* (LSM6DSO32_FIFO_CTRL2) Non-compressed data every 8 batch data rate value */
#define   LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_16BDR_Val                 (2)  /* (LSM6DSO32_FIFO_CTRL2) Non-compressed data every 16 batch data rate value */
#define   LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_32BDR_Val                 (3)  /* (LSM6DSO32_FIFO_CTRL2) Non-compressed data every 32 batch data rate value */
#define LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_NOF                         (LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_NOF_Val << LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_Pos)  /* (LSM6DSO32_FIFO_CTRL2) Non-compressed data writing is not forced */
#define LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_8BDR                        (LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_8BDR_Val << LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_Pos)  /* (LSM6DSO32_FIFO_CTRL2) Non-compressed data every 8 batch data rate */
#define LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_16BDR                       (LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_16BDR_Val << LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_Pos)  /* (LSM6DSO32_FIFO_CTRL2) Non-compressed data every 16 batch data rate */
#define LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_32BDR                       (LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_32BDR_Val << LSM6DSO32_FIFO_CTRL2_UNCOPTR_RATE_Pos)  /* (LSM6DSO32_FIFO_CTRL2) Non-compressed data every 32 batch data rate */
#define LSM6DSO32_FIFO_CTRL2_ODRCHG_EN_Pos                            (4)  /* (LSM6DSO32_FIFO_CTRL2) Enables ODR CHANGE virtual sensor to be batched in FIFO position */
#define LSM6DSO32_FIFO_CTRL2_ODRCHG_EN_Msk                            ((0x1) << LSM6DSO32_FIFO_CTRL2_ODRCHG_EN_Pos)  /* (LSM6DSO32_FIFO_CTRL2) Enables ODR CHANGE virtual sensor to be batched in FIFO mask */
#define LSM6DSO32_FIFO_CTRL2_ODRCHG_EN(value)                         (LSM6DSO32_FIFO_CTRL2_ODRCHG_EN_Msk & ((value) << LSM6DSO32_FIFO_CTRL2_ODRCHG_EN_Pos))  /* Assignment of ODRCHG_EN value in LSM6DSO32_FIFO_CTRL2 register */
#define LSM6DSO32_FIFO_CTRL2_ODRCHG_EN_Get(reg)                       (((reg) & LSM6DSO32_FIFO_CTRL2_ODRCHG_EN_Msk) >> LSM6DSO32_FIFO_CTRL2_ODRCHG_EN_Pos)  /* Assignment of ODRCHG_EN value in LSM6DSO32_FIFO_CTRL2 register */
#define LSM6DSO32_FIFO_CTRL2_FIFO_COMPR_RT_EN_Pos                     (6)  /* (LSM6DSO32_FIFO_CTRL2) Enables/Disables compression algorithm runtime position */
#define LSM6DSO32_FIFO_CTRL2_FIFO_COMPR_RT_EN_Msk                     ((0x1) << LSM6DSO32_FIFO_CTRL2_FIFO_COMPR_RT_EN_Pos)  /* (LSM6DSO32_FIFO_CTRL2) Enables/Disables compression algorithm runtime mask */
#define LSM6DSO32_FIFO_CTRL2_FIFO_COMPR_RT_EN(value)                  (LSM6DSO32_FIFO_CTRL2_FIFO_COMPR_RT_EN_Msk & ((value) << LSM6DSO32_FIFO_CTRL2_FIFO_COMPR_RT_EN_Pos))  /* Assignment of FIFO_COMPR_RT_EN value in LSM6DSO32_FIFO_CTRL2 register */
#define LSM6DSO32_FIFO_CTRL2_FIFO_COMPR_RT_EN_Get(reg)                (((reg) & LSM6DSO32_FIFO_CTRL2_FIFO_COMPR_RT_EN_Msk) >> LSM6DSO32_FIFO_CTRL2_FIFO_COMPR_RT_EN_Pos)  /* Assignment of FIFO_COMPR_RT_EN value in LSM6DSO32_FIFO_CTRL2 register */
#define LSM6DSO32_FIFO_CTRL2_STOP_ON_WTM_Pos                          (7)  /* (LSM6DSO32_FIFO_CTRL2) Sensing chain FIFO stop values memorization at threshold level position */
#define LSM6DSO32_FIFO_CTRL2_STOP_ON_WTM_Msk                          ((0x1) << LSM6DSO32_FIFO_CTRL2_STOP_ON_WTM_Pos)  /* (LSM6DSO32_FIFO_CTRL2) Sensing chain FIFO stop values memorization at threshold level mask */
#define LSM6DSO32_FIFO_CTRL2_STOP_ON_WTM(value)                       (LSM6DSO32_FIFO_CTRL2_STOP_ON_WTM_Msk & ((value) << LSM6DSO32_FIFO_CTRL2_STOP_ON_WTM_Pos))  /* Assignment of STOP_ON_WTM value in LSM6DSO32_FIFO_CTRL2 register */
#define LSM6DSO32_FIFO_CTRL2_STOP_ON_WTM_Get(reg)                     (((reg) & LSM6DSO32_FIFO_CTRL2_STOP_ON_WTM_Msk) >> LSM6DSO32_FIFO_CTRL2_STOP_ON_WTM_Pos)  /* Assignment of STOP_ON_WTM value in LSM6DSO32_FIFO_CTRL2 register */
#define LSM6DSO32_FIFO_CTRL2_Msk                                      (0xd7)  /* (LSM6DSO32_FIFO_CTRL2) Register mask */


/* LSM6DSO32_FIFO_CTRL3, R/W: FIFO control register 3 */
#define LSM6DSO32_FIFO_CTRL3_ADDR                                     (0x9)  /* (LSM6DSO32_FIFO_CTRL3) FIFO control register 3 address */

#define LSM6DSO32_FIFO_CTRL3_BDR_XL_Pos                               (0)  /* (LSM6DSO32_FIFO_CTRL3) Selects Batching Data Rate (writing frequency in FIFO) for accelerometer data position */
#define LSM6DSO32_FIFO_CTRL3_BDR_XL_Msk                               ((0xf) << LSM6DSO32_FIFO_CTRL3_BDR_XL_Pos)  /* (LSM6DSO32_FIFO_CTRL3) Selects Batching Data Rate (writing frequency in FIFO) for accelerometer data mask */
#define LSM6DSO32_FIFO_CTRL3_BDR_XL(value)                            (LSM6DSO32_FIFO_CTRL3_BDR_XL_Msk & ((value) << LSM6DSO32_FIFO_CTRL3_BDR_XL_Pos))  /* Assignment of BDR_XL value in LSM6DSO32_FIFO_CTRL3 register */
#define LSM6DSO32_FIFO_CTRL3_BDR_XL_Get(reg)                          (((reg) & LSM6DSO32_FIFO_CTRL3_BDR_XL_Msk) >> LSM6DSO32_FIFO_CTRL3_BDR_XL_Pos)  /* Assignment of BDR_XL value in LSM6DSO32_FIFO_CTRL3 register */
#define   LSM6DSO32_FIFO_CTRL3_BDR_XL_NB_Val                          (0)  /* (LSM6DSO32_FIFO_CTRL3) Accelerometer not batched in FIFO value */
#define   LSM6DSO32_FIFO_CTRL3_BDR_XL_12_5Hz_Val                      (1)  /* (LSM6DSO32_FIFO_CTRL3) 12.5Hz value */
#define   LSM6DSO32_FIFO_CTRL3_BDR_XL_26Hz_Val                        (2)  /* (LSM6DSO32_FIFO_CTRL3) 26Hz value */
#define   LSM6DSO32_FIFO_CTRL3_BDR_XL_52Hz_Val                        (3)  /* (LSM6DSO32_FIFO_CTRL3) 52Hz value */
#define   LSM6DSO32_FIFO_CTRL3_BDR_XL_104Hz_Val                       (4)  /* (LSM6DSO32_FIFO_CTRL3) 104Hz value */
#define   LSM6DSO32_FIFO_CTRL3_BDR_XL_208Hz_Val                       (5)  /* (LSM6DSO32_FIFO_CTRL3) 208Hz value */
#define   LSM6DSO32_FIFO_CTRL3_BDR_XL_417Hz_Val                       (6)  /* (LSM6DSO32_FIFO_CTRL3) 417Hz value */
#define   LSM6DSO32_FIFO_CTRL3_BDR_XL_833Hz_Val                       (7)  /* (LSM6DSO32_FIFO_CTRL3) 833Hz value */
#define   LSM6DSO32_FIFO_CTRL3_BDR_XL_1667Hz_Val                      (8)  /* (LSM6DSO32_FIFO_CTRL3) 1667Hz value */
#define   LSM6DSO32_FIFO_CTRL3_BDR_XL_3333Hz_Val                      (9)  /* (LSM6DSO32_FIFO_CTRL3) 3333Hz value */
#define   LSM6DSO32_FIFO_CTRL3_BDR_XL_6667Hz_Val                      (10)  /* (LSM6DSO32_FIFO_CTRL3) 6667Hz value */
#define   LSM6DSO32_FIFO_CTRL3_BDR_XL_1_6Hz_Val                       (11)  /* (LSM6DSO32_FIFO_CTRL3) 1.6Hz value */
#define LSM6DSO32_FIFO_CTRL3_BDR_XL_NB                                (LSM6DSO32_FIFO_CTRL3_BDR_XL_NB_Val << LSM6DSO32_FIFO_CTRL3_BDR_XL_Pos)  /* (LSM6DSO32_FIFO_CTRL3) Accelerometer not batched in FIFO */
#define LSM6DSO32_FIFO_CTRL3_BDR_XL_12_5Hz                            (LSM6DSO32_FIFO_CTRL3_BDR_XL_12_5Hz_Val << LSM6DSO32_FIFO_CTRL3_BDR_XL_Pos)  /* (LSM6DSO32_FIFO_CTRL3) 12.5Hz */
#define LSM6DSO32_FIFO_CTRL3_BDR_XL_26Hz                              (LSM6DSO32_FIFO_CTRL3_BDR_XL_26Hz_Val << LSM6DSO32_FIFO_CTRL3_BDR_XL_Pos)  /* (LSM6DSO32_FIFO_CTRL3) 26Hz */
#define LSM6DSO32_FIFO_CTRL3_BDR_XL_52Hz                              (LSM6DSO32_FIFO_CTRL3_BDR_XL_52Hz_Val << LSM6DSO32_FIFO_CTRL3_BDR_XL_Pos)  /* (LSM6DSO32_FIFO_CTRL3) 52Hz */
#define LSM6DSO32_FIFO_CTRL3_BDR_XL_104Hz                             (LSM6DSO32_FIFO_CTRL3_BDR_XL_104Hz_Val << LSM6DSO32_FIFO_CTRL3_BDR_XL_Pos)  /* (LSM6DSO32_FIFO_CTRL3) 104Hz */
#define LSM6DSO32_FIFO_CTRL3_BDR_XL_208Hz                             (LSM6DSO32_FIFO_CTRL3_BDR_XL_208Hz_Val << LSM6DSO32_FIFO_CTRL3_BDR_XL_Pos)  /* (LSM6DSO32_FIFO_CTRL3) 208Hz */
#define LSM6DSO32_FIFO_CTRL3_BDR_XL_417Hz                             (LSM6DSO32_FIFO_CTRL3_BDR_XL_417Hz_Val << LSM6DSO32_FIFO_CTRL3_BDR_XL_Pos)  /* (LSM6DSO32_FIFO_CTRL3) 417Hz */
#define LSM6DSO32_FIFO_CTRL3_BDR_XL_833Hz                             (LSM6DSO32_FIFO_CTRL3_BDR_XL_833Hz_Val << LSM6DSO32_FIFO_CTRL3_BDR_XL_Pos)  /* (LSM6DSO32_FIFO_CTRL3) 833Hz */
#define LSM6DSO32_FIFO_CTRL3_BDR_XL_1667Hz                            (LSM6DSO32_FIFO_CTRL3_BDR_XL_1667Hz_Val << LSM6DSO32_FIFO_CTRL3_BDR_XL_Pos)  /* (LSM6DSO32_FIFO_CTRL3) 1667Hz */
#define LSM6DSO32_FIFO_CTRL3_BDR_XL_3333Hz                            (LSM6DSO32_FIFO_CTRL3_BDR_XL_3333Hz_Val << LSM6DSO32_FIFO_CTRL3_BDR_XL_Pos)  /* (LSM6DSO32_FIFO_CTRL3) 3333Hz */
#define LSM6DSO32_FIFO_CTRL3_BDR_XL_6667Hz                            (LSM6DSO32_FIFO_CTRL3_BDR_XL_6667Hz_Val << LSM6DSO32_FIFO_CTRL3_BDR_XL_Pos)  /* (LSM6DSO32_FIFO_CTRL3) 6667Hz */
#define LSM6DSO32_FIFO_CTRL3_BDR_XL_1_6Hz                             (LSM6DSO32_FIFO_CTRL3_BDR_XL_1_6Hz_Val << LSM6DSO32_FIFO_CTRL3_BDR_XL_Pos)  /* (LSM6DSO32_FIFO_CTRL3) 1.6Hz */
#define LSM6DSO32_FIFO_CTRL3_BDR_GY_Pos                               (4)  /* (LSM6DSO32_FIFO_CTRL3) Selects Batching Data Rate (writing frequency in FIFO) for gyroscope data position */
#define LSM6DSO32_FIFO_CTRL3_BDR_GY_Msk                               ((0xf) << LSM6DSO32_FIFO_CTRL3_BDR_GY_Pos)  /* (LSM6DSO32_FIFO_CTRL3) Selects Batching Data Rate (writing frequency in FIFO) for gyroscope data mask */
#define LSM6DSO32_FIFO_CTRL3_BDR_GY(value)                            (LSM6DSO32_FIFO_CTRL3_BDR_GY_Msk & ((value) << LSM6DSO32_FIFO_CTRL3_BDR_GY_Pos))  /* Assignment of BDR_GY value in LSM6DSO32_FIFO_CTRL3 register */
#define LSM6DSO32_FIFO_CTRL3_BDR_GY_Get(reg)                          (((reg) & LSM6DSO32_FIFO_CTRL3_BDR_GY_Msk) >> LSM6DSO32_FIFO_CTRL3_BDR_GY_Pos)  /* Assignment of BDR_GY value in LSM6DSO32_FIFO_CTRL3 register */
#define   LSM6DSO32_FIFO_CTRL3_BDR_GY_NB_Val                          (0)  /* (LSM6DSO32_FIFO_CTRL3) Gyro not batched in FIFO value */
#define   LSM6DSO32_FIFO_CTRL3_BDR_GY_12_5Hz_Val                      (1)  /* (LSM6DSO32_FIFO_CTRL3) 12.5Hz value */
#define   LSM6DSO32_FIFO_CTRL3_BDR_GY_26Hz_Val                        (2)  /* (LSM6DSO32_FIFO_CTRL3) 26Hz value */
#define   LSM6DSO32_FIFO_CTRL3_BDR_GY_52Hz_Val                        (3)  /* (LSM6DSO32_FIFO_CTRL3) 52Hz value */
#define   LSM6DSO32_FIFO_CTRL3_BDR_GY_104Hz_Val                       (4)  /* (LSM6DSO32_FIFO_CTRL3) 104Hz value */
#define   LSM6DSO32_FIFO_CTRL3_BDR_GY_208Hz_Val                       (5)  /* (LSM6DSO32_FIFO_CTRL3) 208Hz value */
#define   LSM6DSO32_FIFO_CTRL3_BDR_GY_417Hz_Val                       (6)  /* (LSM6DSO32_FIFO_CTRL3) 417Hz value */
#define   LSM6DSO32_FIFO_CTRL3_BDR_GY_833Hz_Val                       (7)  /* (LSM6DSO32_FIFO_CTRL3) 833Hz value */
#define   LSM6DSO32_FIFO_CTRL3_BDR_GY_1667Hz_Val                      (8)  /* (LSM6DSO32_FIFO_CTRL3) 1667Hz value */
#define   LSM6DSO32_FIFO_CTRL3_BDR_GY_3333Hz_Val                      (9)  /* (LSM6DSO32_FIFO_CTRL3) 3333Hz value */
#define   LSM6DSO32_FIFO_CTRL3_BDR_GY_6667Hz_Val                      (10)  /* (LSM6DSO32_FIFO_CTRL3) 6667Hz value */
#define   LSM6DSO32_FIFO_CTRL3_BDR_GY_6_5Hz_Val                       (11)  /* (LSM6DSO32_FIFO_CTRL3) 6.5Hz value */
#define LSM6DSO32_FIFO_CTRL3_BDR_GY_NB                                (LSM6DSO32_FIFO_CTRL3_BDR_GY_NB_Val << LSM6DSO32_FIFO_CTRL3_BDR_GY_Pos)  /* (LSM6DSO32_FIFO_CTRL3) Gyro not batched in FIFO */
#define LSM6DSO32_FIFO_CTRL3_BDR_GY_12_5Hz                            (LSM6DSO32_FIFO_CTRL3_BDR_GY_12_5Hz_Val << LSM6DSO32_FIFO_CTRL3_BDR_GY_Pos)  /* (LSM6DSO32_FIFO_CTRL3) 12.5Hz */
#define LSM6DSO32_FIFO_CTRL3_BDR_GY_26Hz                              (LSM6DSO32_FIFO_CTRL3_BDR_GY_26Hz_Val << LSM6DSO32_FIFO_CTRL3_BDR_GY_Pos)  /* (LSM6DSO32_FIFO_CTRL3) 26Hz */
#define LSM6DSO32_FIFO_CTRL3_BDR_GY_52Hz                              (LSM6DSO32_FIFO_CTRL3_BDR_GY_52Hz_Val << LSM6DSO32_FIFO_CTRL3_BDR_GY_Pos)  /* (LSM6DSO32_FIFO_CTRL3) 52Hz */
#define LSM6DSO32_FIFO_CTRL3_BDR_GY_104Hz                             (LSM6DSO32_FIFO_CTRL3_BDR_GY_104Hz_Val << LSM6DSO32_FIFO_CTRL3_BDR_GY_Pos)  /* (LSM6DSO32_FIFO_CTRL3) 104Hz */
#define LSM6DSO32_FIFO_CTRL3_BDR_GY_208Hz                             (LSM6DSO32_FIFO_CTRL3_BDR_GY_208Hz_Val << LSM6DSO32_FIFO_CTRL3_BDR_GY_Pos)  /* (LSM6DSO32_FIFO_CTRL3) 208Hz */
#define LSM6DSO32_FIFO_CTRL3_BDR_GY_417Hz                             (LSM6DSO32_FIFO_CTRL3_BDR_GY_417Hz_Val << LSM6DSO32_FIFO_CTRL3_BDR_GY_Pos)  /* (LSM6DSO32_FIFO_CTRL3) 417Hz */
#define LSM6DSO32_FIFO_CTRL3_BDR_GY_833Hz                             (LSM6DSO32_FIFO_CTRL3_BDR_GY_833Hz_Val << LSM6DSO32_FIFO_CTRL3_BDR_GY_Pos)  /* (LSM6DSO32_FIFO_CTRL3) 833Hz */
#define LSM6DSO32_FIFO_CTRL3_BDR_GY_1667Hz                            (LSM6DSO32_FIFO_CTRL3_BDR_GY_1667Hz_Val << LSM6DSO32_FIFO_CTRL3_BDR_GY_Pos)  /* (LSM6DSO32_FIFO_CTRL3) 1667Hz */
#define LSM6DSO32_FIFO_CTRL3_BDR_GY_3333Hz                            (LSM6DSO32_FIFO_CTRL3_BDR_GY_3333Hz_Val << LSM6DSO32_FIFO_CTRL3_BDR_GY_Pos)  /* (LSM6DSO32_FIFO_CTRL3) 3333Hz */
#define LSM6DSO32_FIFO_CTRL3_BDR_GY_6667Hz                            (LSM6DSO32_FIFO_CTRL3_BDR_GY_6667Hz_Val << LSM6DSO32_FIFO_CTRL3_BDR_GY_Pos)  /* (LSM6DSO32_FIFO_CTRL3) 6667Hz */
#define LSM6DSO32_FIFO_CTRL3_BDR_GY_6_5Hz                             (LSM6DSO32_FIFO_CTRL3_BDR_GY_6_5Hz_Val << LSM6DSO32_FIFO_CTRL3_BDR_GY_Pos)  /* (LSM6DSO32_FIFO_CTRL3) 6.5Hz */
#define LSM6DSO32_FIFO_CTRL3_Msk                                      (0xff)  /* (LSM6DSO32_FIFO_CTRL3) Register mask */


/* LSM6DSO32_FIFO_CTRL4, R/W: FIFO control register 4 */
#define LSM6DSO32_FIFO_CTRL4_ADDR                                     (0xa)  /* (LSM6DSO32_FIFO_CTRL4) FIFO control register 4 address */

#define LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_Pos                          (0)  /* (LSM6DSO32_FIFO_CTRL4) FIFO mode selection position */
#define LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_Msk                          ((0x7) << LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_Pos)  /* (LSM6DSO32_FIFO_CTRL4) FIFO mode selection mask */
#define LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0(value)                       (LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_Msk & ((value) << LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_Pos))  /* Assignment of FIFO_MODE_0 value in LSM6DSO32_FIFO_CTRL4 register */
#define LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_Get(reg)                     (((reg) & LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_Msk) >> LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_Pos)  /* Assignment of FIFO_MODE_0 value in LSM6DSO32_FIFO_CTRL4 register */
#define   LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_BYPASS_Val                 (0)  /* (LSM6DSO32_FIFO_CTRL4) Bypass mode: FIFO disabled value */
#define   LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_FIFO_Val                   (1)  /* (LSM6DSO32_FIFO_CTRL4) FIFO mode: stops collecting data when FIFO is full value */
#define   LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_CONT_TO_FIFO_Val           (3)  /* (LSM6DSO32_FIFO_CTRL4) Continuous-to-FIFO mode: Continuous mode until trigger is deasserted, then FIFO mode value */
#define   LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_BYPASS_TO_CONT_Val         (4)  /* (LSM6DSO32_FIFO_CTRL4) Bypass-to-Continuous mode: Bypass mode until trigger is deasserted, then Continuous mode value */
#define   LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_CONT_Val                   (6)  /* (LSM6DSO32_FIFO_CTRL4) Continuous mode: if the FIFO is full, the new sample overwrites the older one value */
#define   LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_BYPASS_TO_FIFO_Val         (7)  /* (LSM6DSO32_FIFO_CTRL4) Bypass-to-FIFO mode: Bypass mode until trigger is deasserted, then FIFO mode. value */
#define LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_BYPASS                       (LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_BYPASS_Val << LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_Pos)  /* (LSM6DSO32_FIFO_CTRL4) Bypass mode: FIFO disabled */
#define LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_FIFO                         (LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_FIFO_Val << LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_Pos)  /* (LSM6DSO32_FIFO_CTRL4) FIFO mode: stops collecting data when FIFO is full */
#define LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_CONT_TO_FIFO                 (LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_CONT_TO_FIFO_Val << LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_Pos)  /* (LSM6DSO32_FIFO_CTRL4) Continuous-to-FIFO mode: Continuous mode until trigger is deasserted, then FIFO mode */
#define LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_BYPASS_TO_CONT               (LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_BYPASS_TO_CONT_Val << LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_Pos)  /* (LSM6DSO32_FIFO_CTRL4) Bypass-to-Continuous mode: Bypass mode until trigger is deasserted, then Continuous mode */
#define LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_CONT                         (LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_CONT_Val << LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_Pos)  /* (LSM6DSO32_FIFO_CTRL4) Continuous mode: if the FIFO is full, the new sample overwrites the older one */
#define LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_BYPASS_TO_FIFO               (LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_BYPASS_TO_FIFO_Val << LSM6DSO32_FIFO_CTRL4_FIFO_MODE_0_Pos)  /* (LSM6DSO32_FIFO_CTRL4) Bypass-to-FIFO mode: Bypass mode until trigger is deasserted, then FIFO mode. */
#define LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_Pos                          (4)  /* (LSM6DSO32_FIFO_CTRL4) Selects batching data rate (writing frequency in FIFO) for temperature data position */
#define LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_Msk                          ((0x3) << LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_Pos)  /* (LSM6DSO32_FIFO_CTRL4) Selects batching data rate (writing frequency in FIFO) for temperature data mask */
#define LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH(value)                       (LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_Msk & ((value) << LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_Pos))  /* Assignment of ODR_T_BATCH value in LSM6DSO32_FIFO_CTRL4 register */
#define LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_Get(reg)                     (((reg) & LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_Msk) >> LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_Pos)  /* Assignment of ODR_T_BATCH value in LSM6DSO32_FIFO_CTRL4 register */
#define   LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_NB_Val                     (0)  /* (LSM6DSO32_FIFO_CTRL4) Temperature not batched in FIFO value */
#define   LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_1_6Hz_Val                  (1)  /* (LSM6DSO32_FIFO_CTRL4) 1.6Hz value */
#define   LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_12_5Hz_Val                 (2)  /* (LSM6DSO32_FIFO_CTRL4) 12.5Hz value */
#define   LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_52Hz_Val                   (3)  /* (LSM6DSO32_FIFO_CTRL4) 52Hz value */
#define LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_NB                           (LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_NB_Val << LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_Pos)  /* (LSM6DSO32_FIFO_CTRL4) Temperature not batched in FIFO */
#define LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_1_6Hz                        (LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_1_6Hz_Val << LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_Pos)  /* (LSM6DSO32_FIFO_CTRL4) 1.6Hz */
#define LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_12_5Hz                       (LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_12_5Hz_Val << LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_Pos)  /* (LSM6DSO32_FIFO_CTRL4) 12.5Hz */
#define LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_52Hz                         (LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_52Hz_Val << LSM6DSO32_FIFO_CTRL4_ODR_T_BATCH_Pos)  /* (LSM6DSO32_FIFO_CTRL4) 52Hz */
#define LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_Pos                         (6)  /* (LSM6DSO32_FIFO_CTRL4) Selects decimation for timestamp batching in FIFO. Writing rate will be the maximum rate between XL and GYRO BDR divided by decimation decoder position */
#define LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_Msk                         ((0x3) << LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_Pos)  /* (LSM6DSO32_FIFO_CTRL4) Selects decimation for timestamp batching in FIFO. Writing rate will be the maximum rate between XL and GYRO BDR divided by decimation decoder mask */
#define LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH(value)                      (LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_Msk & ((value) << LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_Pos))  /* Assignment of DEC_TS_BATCH value in LSM6DSO32_FIFO_CTRL4 register */
#define LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_Get(reg)                    (((reg) & LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_Msk) >> LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_Pos)  /* Assignment of DEC_TS_BATCH value in LSM6DSO32_FIFO_CTRL4 register */
#define   LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_NB_Val                    (0)  /* (LSM6DSO32_FIFO_CTRL4) Timestamp not batched in FIFO value */
#define   LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_D1_Val                    (1)  /* (LSM6DSO32_FIFO_CTRL4) Decimation 1: max(BDR_XL[Hz],BDR_GY[Hz]) [Hz] value */
#define   LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_D8_Val                    (2)  /* (LSM6DSO32_FIFO_CTRL4) Decimation 8: max(BDR_XL[Hz],BDR_GY[Hz])/8 [Hz] value */
#define   LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_D16_Val                   (3)  /* (LSM6DSO32_FIFO_CTRL4) Decimation 32: max(BDR_XL[Hz],BDR_GY[Hz])/32 [Hz] value */
#define LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_NB                          (LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_NB_Val << LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_Pos)  /* (LSM6DSO32_FIFO_CTRL4) Timestamp not batched in FIFO */
#define LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_D1                          (LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_D1_Val << LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_Pos)  /* (LSM6DSO32_FIFO_CTRL4) Decimation 1: max(BDR_XL[Hz],BDR_GY[Hz]) [Hz] */
#define LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_D8                          (LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_D8_Val << LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_Pos)  /* (LSM6DSO32_FIFO_CTRL4) Decimation 8: max(BDR_XL[Hz],BDR_GY[Hz])/8 [Hz] */
#define LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_D16                         (LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_D16_Val << LSM6DSO32_FIFO_CTRL4_DEC_TS_BATCH_Pos)  /* (LSM6DSO32_FIFO_CTRL4) Decimation 32: max(BDR_XL[Hz],BDR_GY[Hz])/32 [Hz] */
#define LSM6DSO32_FIFO_CTRL4_Msk                                      (0xf7)  /* (LSM6DSO32_FIFO_CTRL4) Register mask */


/* LSM6DSO32_COUNTER_BDR_REG1, R/W: Counter batch data rate register 1 */
#define LSM6DSO32_COUNTER_BDR_REG1_ADDR                               (0xb)  /* (LSM6DSO32_COUNTER_BDR_REG1) Counter batch data rate register 1 address */

#define LSM6DSO32_COUNTER_BDR_REG1_CNT_BDR_TH_Pos                     (0)  /* (LSM6DSO32_COUNTER_BDR_REG1) In conjunction with CNT_BDR_TH_[7:0] in COUNTER_BDR_REG2 (0Ch), sets the threshold for the internal counter of batching events. When this counter reaches the threshold, the counter is reset and the COUNTER_BDR_IA flag in FIFO_STATUS2 (3Bh) is set to ‘1’ position */
#define LSM6DSO32_COUNTER_BDR_REG1_CNT_BDR_TH_Msk                     ((0x7) << LSM6DSO32_COUNTER_BDR_REG1_CNT_BDR_TH_Pos)  /* (LSM6DSO32_COUNTER_BDR_REG1) In conjunction with CNT_BDR_TH_[7:0] in COUNTER_BDR_REG2 (0Ch), sets the threshold for the internal counter of batching events. When this counter reaches the threshold, the counter is reset and the COUNTER_BDR_IA flag in FIFO_STATUS2 (3Bh) is set to ‘1’ mask */
#define LSM6DSO32_COUNTER_BDR_REG1_CNT_BDR_TH(value)                  (LSM6DSO32_COUNTER_BDR_REG1_CNT_BDR_TH_Msk & ((value) << LSM6DSO32_COUNTER_BDR_REG1_CNT_BDR_TH_Pos))  /* Assignment of CNT_BDR_TH value in LSM6DSO32_COUNTER_BDR_REG1 register */
#define LSM6DSO32_COUNTER_BDR_REG1_CNT_BDR_TH_Get(reg)                (((reg) & LSM6DSO32_COUNTER_BDR_REG1_CNT_BDR_TH_Msk) >> LSM6DSO32_COUNTER_BDR_REG1_CNT_BDR_TH_Pos)  /* Assignment of CNT_BDR_TH value in LSM6DSO32_COUNTER_BDR_REG1 register */
#define LSM6DSO32_COUNTER_BDR_REG1_TRIG_COUNTER_BDR_Pos               (5)  /* (LSM6DSO32_COUNTER_BDR_REG1) Selects the trigger for the internal counter of batching events between XL and gyro position */
#define LSM6DSO32_COUNTER_BDR_REG1_TRIG_COUNTER_BDR_Msk               ((0x1) << LSM6DSO32_COUNTER_BDR_REG1_TRIG_COUNTER_BDR_Pos)  /* (LSM6DSO32_COUNTER_BDR_REG1) Selects the trigger for the internal counter of batching events between XL and gyro mask */
#define LSM6DSO32_COUNTER_BDR_REG1_TRIG_COUNTER_BDR(value)            (LSM6DSO32_COUNTER_BDR_REG1_TRIG_COUNTER_BDR_Msk & ((value) << LSM6DSO32_COUNTER_BDR_REG1_TRIG_COUNTER_BDR_Pos))  /* Assignment of TRIG_COUNTER_BDR value in LSM6DSO32_COUNTER_BDR_REG1 register */
#define LSM6DSO32_COUNTER_BDR_REG1_TRIG_COUNTER_BDR_Get(reg)          (((reg) & LSM6DSO32_COUNTER_BDR_REG1_TRIG_COUNTER_BDR_Msk) >> LSM6DSO32_COUNTER_BDR_REG1_TRIG_COUNTER_BDR_Pos)  /* Assignment of TRIG_COUNTER_BDR value in LSM6DSO32_COUNTER_BDR_REG1 register */
#define   LSM6DSO32_COUNTER_BDR_REG1_TRIG_COUNTER_BDR_XL_Val          (0)  /* (LSM6DSO32_COUNTER_BDR_REG1) XL batching event value */
#define   LSM6DSO32_COUNTER_BDR_REG1_TRIG_COUNTER_BDR_GYRO_Val        (1)  /* (LSM6DSO32_COUNTER_BDR_REG1) GYRO batching event value */
#define LSM6DSO32_COUNTER_BDR_REG1_TRIG_COUNTER_BDR_XL                (LSM6DSO32_COUNTER_BDR_REG1_TRIG_COUNTER_BDR_XL_Val << LSM6DSO32_COUNTER_BDR_REG1_TRIG_COUNTER_BDR_Pos)  /* (LSM6DSO32_COUNTER_BDR_REG1) XL batching event */
#define LSM6DSO32_COUNTER_BDR_REG1_TRIG_COUNTER_BDR_GYRO              (LSM6DSO32_COUNTER_BDR_REG1_TRIG_COUNTER_BDR_GYRO_Val << LSM6DSO32_COUNTER_BDR_REG1_TRIG_COUNTER_BDR_Pos)  /* (LSM6DSO32_COUNTER_BDR_REG1) GYRO batching event */
#define LSM6DSO32_COUNTER_BDR_REG1_RST_COUNTER_BDR_Pos                (6)  /* (LSM6DSO32_COUNTER_BDR_REG1) Resets the internal counter of batching events for a single sensor This bit is automatically reset to zero if it was set to ‘1’ position */
#define LSM6DSO32_COUNTER_BDR_REG1_RST_COUNTER_BDR_Msk                ((0x1) << LSM6DSO32_COUNTER_BDR_REG1_RST_COUNTER_BDR_Pos)  /* (LSM6DSO32_COUNTER_BDR_REG1) Resets the internal counter of batching events for a single sensor This bit is automatically reset to zero if it was set to ‘1’ mask */
#define LSM6DSO32_COUNTER_BDR_REG1_RST_COUNTER_BDR(value)             (LSM6DSO32_COUNTER_BDR_REG1_RST_COUNTER_BDR_Msk & ((value) << LSM6DSO32_COUNTER_BDR_REG1_RST_COUNTER_BDR_Pos))  /* Assignment of RST_COUNTER_BDR value in LSM6DSO32_COUNTER_BDR_REG1 register */
#define LSM6DSO32_COUNTER_BDR_REG1_RST_COUNTER_BDR_Get(reg)           (((reg) & LSM6DSO32_COUNTER_BDR_REG1_RST_COUNTER_BDR_Msk) >> LSM6DSO32_COUNTER_BDR_REG1_RST_COUNTER_BDR_Pos)  /* Assignment of RST_COUNTER_BDR value in LSM6DSO32_COUNTER_BDR_REG1 register */
#define LSM6DSO32_COUNTER_BDR_REG1_DATAREADY_PULSED_Pos               (7)  /* (LSM6DSO32_COUNTER_BDR_REG1) Enables pulsed data-ready mode position */
#define LSM6DSO32_COUNTER_BDR_REG1_DATAREADY_PULSED_Msk               ((0x1) << LSM6DSO32_COUNTER_BDR_REG1_DATAREADY_PULSED_Pos)  /* (LSM6DSO32_COUNTER_BDR_REG1) Enables pulsed data-ready mode mask */
#define LSM6DSO32_COUNTER_BDR_REG1_DATAREADY_PULSED(value)            (LSM6DSO32_COUNTER_BDR_REG1_DATAREADY_PULSED_Msk & ((value) << LSM6DSO32_COUNTER_BDR_REG1_DATAREADY_PULSED_Pos))  /* Assignment of DATAREADY_PULSED value in LSM6DSO32_COUNTER_BDR_REG1 register */
#define LSM6DSO32_COUNTER_BDR_REG1_DATAREADY_PULSED_Get(reg)          (((reg) & LSM6DSO32_COUNTER_BDR_REG1_DATAREADY_PULSED_Msk) >> LSM6DSO32_COUNTER_BDR_REG1_DATAREADY_PULSED_Pos)  /* Assignment of DATAREADY_PULSED value in LSM6DSO32_COUNTER_BDR_REG1 register */
#define   LSM6DSO32_COUNTER_BDR_REG1_DATAREADY_PULSED_LATCH_Val       (0)  /* (LSM6DSO32_COUNTER_BDR_REG1) Data-ready latched mode (returns to 0 only after an interface reading) value */
#define   LSM6DSO32_COUNTER_BDR_REG1_DATAREADY_PULSED_PULSED_Val      (1)  /* (LSM6DSO32_COUNTER_BDR_REG1) Data-ready pulsed mode (the data ready pulses are 75 μs long value */
#define LSM6DSO32_COUNTER_BDR_REG1_DATAREADY_PULSED_LATCH             (LSM6DSO32_COUNTER_BDR_REG1_DATAREADY_PULSED_LATCH_Val << LSM6DSO32_COUNTER_BDR_REG1_DATAREADY_PULSED_Pos)  /* (LSM6DSO32_COUNTER_BDR_REG1) Data-ready latched mode (returns to 0 only after an interface reading) */
#define LSM6DSO32_COUNTER_BDR_REG1_DATAREADY_PULSED_PULSED            (LSM6DSO32_COUNTER_BDR_REG1_DATAREADY_PULSED_PULSED_Val << LSM6DSO32_COUNTER_BDR_REG1_DATAREADY_PULSED_Pos)  /* (LSM6DSO32_COUNTER_BDR_REG1) Data-ready pulsed mode (the data ready pulses are 75 μs long */
#define LSM6DSO32_COUNTER_BDR_REG1_Msk                                (0xe7)  /* (LSM6DSO32_COUNTER_BDR_REG1) Register mask */


/* LSM6DSO32_COUNTER_BDR_REG2, R/W: Counter batch data rate register 2 */
#define LSM6DSO32_COUNTER_BDR_REG2_ADDR                               (0xc)  /* (LSM6DSO32_COUNTER_BDR_REG2) Counter batch data rate register 2 address */

#define LSM6DSO32_COUNTER_BDR_REG2_CNT_BDR_TH_Pos                     (0)  /* (LSM6DSO32_COUNTER_BDR_REG2) In conjunction with CNT_BDR_TH_[10:8] in COUNTER_BDR_REG1 (0Bh), sets the threshold for the internal counter of batching events. When this counter reaches the threshold, the counter is reset and the COUNTER_BDR_IA flag in FIFO_STATUS2 (3Bh) is set to ‘1’ position */
#define LSM6DSO32_COUNTER_BDR_REG2_CNT_BDR_TH_Msk                     ((0xff) << LSM6DSO32_COUNTER_BDR_REG2_CNT_BDR_TH_Pos)  /* (LSM6DSO32_COUNTER_BDR_REG2) In conjunction with CNT_BDR_TH_[10:8] in COUNTER_BDR_REG1 (0Bh), sets the threshold for the internal counter of batching events. When this counter reaches the threshold, the counter is reset and the COUNTER_BDR_IA flag in FIFO_STATUS2 (3Bh) is set to ‘1’ mask */
#define LSM6DSO32_COUNTER_BDR_REG2_CNT_BDR_TH(value)                  (LSM6DSO32_COUNTER_BDR_REG2_CNT_BDR_TH_Msk & ((value) << LSM6DSO32_COUNTER_BDR_REG2_CNT_BDR_TH_Pos))  /* Assignment of CNT_BDR_TH value in LSM6DSO32_COUNTER_BDR_REG2 register */
#define LSM6DSO32_COUNTER_BDR_REG2_CNT_BDR_TH_Get(reg)                (((reg) & LSM6DSO32_COUNTER_BDR_REG2_CNT_BDR_TH_Msk) >> LSM6DSO32_COUNTER_BDR_REG2_CNT_BDR_TH_Pos)  /* Assignment of CNT_BDR_TH value in LSM6DSO32_COUNTER_BDR_REG2 register */
#define LSM6DSO32_COUNTER_BDR_REG2_Msk                                (0xff)  /* (LSM6DSO32_COUNTER_BDR_REG2) Register mask */


/* LSM6DSO32_INT1_CTRL, R/W: INT1 pin control register */
#define LSM6DSO32_INT1_CTRL_ADDR                                      (0xd)  /* (LSM6DSO32_INT1_CTRL) INT1 pin control register address */

#define LSM6DSO32_INT1_CTRL_INT1_DRDY_XL_Pos                          (0)  /* (LSM6DSO32_INT1_CTRL) Enables accelerometer data-ready interrupt on INT1 pin. It can be also used to trigger an IBI when the MIPI I3CSM interface is used position */
#define LSM6DSO32_INT1_CTRL_INT1_DRDY_XL_Msk                          ((0x1) << LSM6DSO32_INT1_CTRL_INT1_DRDY_XL_Pos)  /* (LSM6DSO32_INT1_CTRL) Enables accelerometer data-ready interrupt on INT1 pin. It can be also used to trigger an IBI when the MIPI I3CSM interface is used mask */
#define LSM6DSO32_INT1_CTRL_INT1_DRDY_XL(value)                       (LSM6DSO32_INT1_CTRL_INT1_DRDY_XL_Msk & ((value) << LSM6DSO32_INT1_CTRL_INT1_DRDY_XL_Pos))  /* Assignment of INT1_DRDY_XL value in LSM6DSO32_INT1_CTRL register */
#define LSM6DSO32_INT1_CTRL_INT1_DRDY_XL_Get(reg)                     (((reg) & LSM6DSO32_INT1_CTRL_INT1_DRDY_XL_Msk) >> LSM6DSO32_INT1_CTRL_INT1_DRDY_XL_Pos)  /* Assignment of INT1_DRDY_XL value in LSM6DSO32_INT1_CTRL register */
#define LSM6DSO32_INT1_CTRL_INT1_DRDY_G_Pos                           (1)  /* (LSM6DSO32_INT1_CTRL) Enables gyroscope data-ready interrupt on INT1 pin. It can be also used to trigger an IBI when the MIPI I3CSM interface is used position */
#define LSM6DSO32_INT1_CTRL_INT1_DRDY_G_Msk                           ((0x1) << LSM6DSO32_INT1_CTRL_INT1_DRDY_G_Pos)  /* (LSM6DSO32_INT1_CTRL) Enables gyroscope data-ready interrupt on INT1 pin. It can be also used to trigger an IBI when the MIPI I3CSM interface is used mask */
#define LSM6DSO32_INT1_CTRL_INT1_DRDY_G(value)                        (LSM6DSO32_INT1_CTRL_INT1_DRDY_G_Msk & ((value) << LSM6DSO32_INT1_CTRL_INT1_DRDY_G_Pos))  /* Assignment of INT1_DRDY_G value in LSM6DSO32_INT1_CTRL register */
#define LSM6DSO32_INT1_CTRL_INT1_DRDY_G_Get(reg)                      (((reg) & LSM6DSO32_INT1_CTRL_INT1_DRDY_G_Msk) >> LSM6DSO32_INT1_CTRL_INT1_DRDY_G_Pos)  /* Assignment of INT1_DRDY_G value in LSM6DSO32_INT1_CTRL register */
#define LSM6DSO32_INT1_CTRL_INT1_BOOT_Pos                             (2)  /* (LSM6DSO32_INT1_CTRL) Enables boot status on INT1 pin position */
#define LSM6DSO32_INT1_CTRL_INT1_BOOT_Msk                             ((0x1) << LSM6DSO32_INT1_CTRL_INT1_BOOT_Pos)  /* (LSM6DSO32_INT1_CTRL) Enables boot status on INT1 pin mask */
#define LSM6DSO32_INT1_CTRL_INT1_BOOT(value)                          (LSM6DSO32_INT1_CTRL_INT1_BOOT_Msk & ((value) << LSM6DSO32_INT1_CTRL_INT1_BOOT_Pos))  /* Assignment of INT1_BOOT value in LSM6DSO32_INT1_CTRL register */
#define LSM6DSO32_INT1_CTRL_INT1_BOOT_Get(reg)                        (((reg) & LSM6DSO32_INT1_CTRL_INT1_BOOT_Msk) >> LSM6DSO32_INT1_CTRL_INT1_BOOT_Pos)  /* Assignment of INT1_BOOT value in LSM6DSO32_INT1_CTRL register */
#define LSM6DSO32_INT1_CTRL_INT1_FIFO_TH_Pos                          (3)  /* (LSM6DSO32_INT1_CTRL) Enables FIFO threshold interrupt on INT1 pin. It can be also used to trigger an IBI when the MIPI I3CSM interface is used position */
#define LSM6DSO32_INT1_CTRL_INT1_FIFO_TH_Msk                          ((0x1) << LSM6DSO32_INT1_CTRL_INT1_FIFO_TH_Pos)  /* (LSM6DSO32_INT1_CTRL) Enables FIFO threshold interrupt on INT1 pin. It can be also used to trigger an IBI when the MIPI I3CSM interface is used mask */
#define LSM6DSO32_INT1_CTRL_INT1_FIFO_TH(value)                       (LSM6DSO32_INT1_CTRL_INT1_FIFO_TH_Msk & ((value) << LSM6DSO32_INT1_CTRL_INT1_FIFO_TH_Pos))  /* Assignment of INT1_FIFO_TH value in LSM6DSO32_INT1_CTRL register */
#define LSM6DSO32_INT1_CTRL_INT1_FIFO_TH_Get(reg)                     (((reg) & LSM6DSO32_INT1_CTRL_INT1_FIFO_TH_Msk) >> LSM6DSO32_INT1_CTRL_INT1_FIFO_TH_Pos)  /* Assignment of INT1_FIFO_TH value in LSM6DSO32_INT1_CTRL register */
#define LSM6DSO32_INT1_CTRL_INT1_FIFO_OVR_Pos                         (4)  /* (LSM6DSO32_INT1_CTRL) Enables FIFO overrun interrupt on INT1 pin. It can be also used to trigger an IBI when the MIPI I3CSM interface is used position */
#define LSM6DSO32_INT1_CTRL_INT1_FIFO_OVR_Msk                         ((0x1) << LSM6DSO32_INT1_CTRL_INT1_FIFO_OVR_Pos)  /* (LSM6DSO32_INT1_CTRL) Enables FIFO overrun interrupt on INT1 pin. It can be also used to trigger an IBI when the MIPI I3CSM interface is used mask */
#define LSM6DSO32_INT1_CTRL_INT1_FIFO_OVR(value)                      (LSM6DSO32_INT1_CTRL_INT1_FIFO_OVR_Msk & ((value) << LSM6DSO32_INT1_CTRL_INT1_FIFO_OVR_Pos))  /* Assignment of INT1_FIFO_OVR value in LSM6DSO32_INT1_CTRL register */
#define LSM6DSO32_INT1_CTRL_INT1_FIFO_OVR_Get(reg)                    (((reg) & LSM6DSO32_INT1_CTRL_INT1_FIFO_OVR_Msk) >> LSM6DSO32_INT1_CTRL_INT1_FIFO_OVR_Pos)  /* Assignment of INT1_FIFO_OVR value in LSM6DSO32_INT1_CTRL register */
#define LSM6DSO32_INT1_CTRL_INT1_FIFO_FULL_Pos                        (5)  /* (LSM6DSO32_INT1_CTRL) Enables FIFO full flag interrupt on INT1 pin. It can be also used to trigger an IBI when the MIPI I3CSM interface is used position */
#define LSM6DSO32_INT1_CTRL_INT1_FIFO_FULL_Msk                        ((0x1) << LSM6DSO32_INT1_CTRL_INT1_FIFO_FULL_Pos)  /* (LSM6DSO32_INT1_CTRL) Enables FIFO full flag interrupt on INT1 pin. It can be also used to trigger an IBI when the MIPI I3CSM interface is used mask */
#define LSM6DSO32_INT1_CTRL_INT1_FIFO_FULL(value)                     (LSM6DSO32_INT1_CTRL_INT1_FIFO_FULL_Msk & ((value) << LSM6DSO32_INT1_CTRL_INT1_FIFO_FULL_Pos))  /* Assignment of INT1_FIFO_FULL value in LSM6DSO32_INT1_CTRL register */
#define LSM6DSO32_INT1_CTRL_INT1_FIFO_FULL_Get(reg)                   (((reg) & LSM6DSO32_INT1_CTRL_INT1_FIFO_FULL_Msk) >> LSM6DSO32_INT1_CTRL_INT1_FIFO_FULL_Pos)  /* Assignment of INT1_FIFO_FULL value in LSM6DSO32_INT1_CTRL register */
#define LSM6DSO32_INT1_CTRL_INT1_CNT_BDR_Pos                          (6)  /* (LSM6DSO32_INT1_CTRL) Enables COUNTER_BDR_IA interrupt on INT1 position */
#define LSM6DSO32_INT1_CTRL_INT1_CNT_BDR_Msk                          ((0x1) << LSM6DSO32_INT1_CTRL_INT1_CNT_BDR_Pos)  /* (LSM6DSO32_INT1_CTRL) Enables COUNTER_BDR_IA interrupt on INT1 mask */
#define LSM6DSO32_INT1_CTRL_INT1_CNT_BDR(value)                       (LSM6DSO32_INT1_CTRL_INT1_CNT_BDR_Msk & ((value) << LSM6DSO32_INT1_CTRL_INT1_CNT_BDR_Pos))  /* Assignment of INT1_CNT_BDR value in LSM6DSO32_INT1_CTRL register */
#define LSM6DSO32_INT1_CTRL_INT1_CNT_BDR_Get(reg)                     (((reg) & LSM6DSO32_INT1_CTRL_INT1_CNT_BDR_Msk) >> LSM6DSO32_INT1_CTRL_INT1_CNT_BDR_Pos)  /* Assignment of INT1_CNT_BDR value in LSM6DSO32_INT1_CTRL register */
#define LSM6DSO32_INT1_CTRL_DEN_DRDY_FLAG_Pos                         (7)  /* (LSM6DSO32_INT1_CTRL) Sends DEN_DRDY (DEN stamped on Sensor Data flag) to INT1 pin position */
#define LSM6DSO32_INT1_CTRL_DEN_DRDY_FLAG_Msk                         ((0x1) << LSM6DSO32_INT1_CTRL_DEN_DRDY_FLAG_Pos)  /* (LSM6DSO32_INT1_CTRL) Sends DEN_DRDY (DEN stamped on Sensor Data flag) to INT1 pin mask */
#define LSM6DSO32_INT1_CTRL_DEN_DRDY_FLAG(value)                      (LSM6DSO32_INT1_CTRL_DEN_DRDY_FLAG_Msk & ((value) << LSM6DSO32_INT1_CTRL_DEN_DRDY_FLAG_Pos))  /* Assignment of DEN_DRDY_FLAG value in LSM6DSO32_INT1_CTRL register */
#define LSM6DSO32_INT1_CTRL_DEN_DRDY_FLAG_Get(reg)                    (((reg) & LSM6DSO32_INT1_CTRL_DEN_DRDY_FLAG_Msk) >> LSM6DSO32_INT1_CTRL_DEN_DRDY_FLAG_Pos)  /* Assignment of DEN_DRDY_FLAG value in LSM6DSO32_INT1_CTRL register */
#define LSM6DSO32_INT1_CTRL_Msk                                       (0xff)  /* (LSM6DSO32_INT1_CTRL) Register mask */


/* LSM6DSO32_INT2_CTRL, R/W: INT2 pin control register */
#define LSM6DSO32_INT2_CTRL_ADDR                                      (0xe)  /* (LSM6DSO32_INT2_CTRL) INT2 pin control register address */

#define LSM6DSO32_INT2_CTRL_INT2_DRDY_XL_Pos                          (0)  /* (LSM6DSO32_INT2_CTRL) Enables accelerometer data-ready interrupt on INT2 pin. It can be also used to trigger an IBI when the MIPI I3CSM interface is used position */
#define LSM6DSO32_INT2_CTRL_INT2_DRDY_XL_Msk                          ((0x1) << LSM6DSO32_INT2_CTRL_INT2_DRDY_XL_Pos)  /* (LSM6DSO32_INT2_CTRL) Enables accelerometer data-ready interrupt on INT2 pin. It can be also used to trigger an IBI when the MIPI I3CSM interface is used mask */
#define LSM6DSO32_INT2_CTRL_INT2_DRDY_XL(value)                       (LSM6DSO32_INT2_CTRL_INT2_DRDY_XL_Msk & ((value) << LSM6DSO32_INT2_CTRL_INT2_DRDY_XL_Pos))  /* Assignment of INT2_DRDY_XL value in LSM6DSO32_INT2_CTRL register */
#define LSM6DSO32_INT2_CTRL_INT2_DRDY_XL_Get(reg)                     (((reg) & LSM6DSO32_INT2_CTRL_INT2_DRDY_XL_Msk) >> LSM6DSO32_INT2_CTRL_INT2_DRDY_XL_Pos)  /* Assignment of INT2_DRDY_XL value in LSM6DSO32_INT2_CTRL register */
#define LSM6DSO32_INT2_CTRL_INT2_DRDY_G_Pos                           (1)  /* (LSM6DSO32_INT2_CTRL) Enables gyroscope data-ready interrupt on INT2 pin. It can be also used to trigger an IBI when the MIPI I3CSM interface is used position */
#define LSM6DSO32_INT2_CTRL_INT2_DRDY_G_Msk                           ((0x1) << LSM6DSO32_INT2_CTRL_INT2_DRDY_G_Pos)  /* (LSM6DSO32_INT2_CTRL) Enables gyroscope data-ready interrupt on INT2 pin. It can be also used to trigger an IBI when the MIPI I3CSM interface is used mask */
#define LSM6DSO32_INT2_CTRL_INT2_DRDY_G(value)                        (LSM6DSO32_INT2_CTRL_INT2_DRDY_G_Msk & ((value) << LSM6DSO32_INT2_CTRL_INT2_DRDY_G_Pos))  /* Assignment of INT2_DRDY_G value in LSM6DSO32_INT2_CTRL register */
#define LSM6DSO32_INT2_CTRL_INT2_DRDY_G_Get(reg)                      (((reg) & LSM6DSO32_INT2_CTRL_INT2_DRDY_G_Msk) >> LSM6DSO32_INT2_CTRL_INT2_DRDY_G_Pos)  /* Assignment of INT2_DRDY_G value in LSM6DSO32_INT2_CTRL register */
#define LSM6DSO32_INT2_CTRL_INT2_BOOT_Pos                             (2)  /* (LSM6DSO32_INT2_CTRL) Enables boot status on INT2 pin position */
#define LSM6DSO32_INT2_CTRL_INT2_BOOT_Msk                             ((0x1) << LSM6DSO32_INT2_CTRL_INT2_BOOT_Pos)  /* (LSM6DSO32_INT2_CTRL) Enables boot status on INT2 pin mask */
#define LSM6DSO32_INT2_CTRL_INT2_BOOT(value)                          (LSM6DSO32_INT2_CTRL_INT2_BOOT_Msk & ((value) << LSM6DSO32_INT2_CTRL_INT2_BOOT_Pos))  /* Assignment of INT2_BOOT value in LSM6DSO32_INT2_CTRL register */
#define LSM6DSO32_INT2_CTRL_INT2_BOOT_Get(reg)                        (((reg) & LSM6DSO32_INT2_CTRL_INT2_BOOT_Msk) >> LSM6DSO32_INT2_CTRL_INT2_BOOT_Pos)  /* Assignment of INT2_BOOT value in LSM6DSO32_INT2_CTRL register */
#define LSM6DSO32_INT2_CTRL_INT2_FIFO_TH_Pos                          (3)  /* (LSM6DSO32_INT2_CTRL) Enables FIFO threshold interrupt on INT2 pin. It can be also used to trigger an IBI when the MIPI I3CSM interface is used position */
#define LSM6DSO32_INT2_CTRL_INT2_FIFO_TH_Msk                          ((0x1) << LSM6DSO32_INT2_CTRL_INT2_FIFO_TH_Pos)  /* (LSM6DSO32_INT2_CTRL) Enables FIFO threshold interrupt on INT2 pin. It can be also used to trigger an IBI when the MIPI I3CSM interface is used mask */
#define LSM6DSO32_INT2_CTRL_INT2_FIFO_TH(value)                       (LSM6DSO32_INT2_CTRL_INT2_FIFO_TH_Msk & ((value) << LSM6DSO32_INT2_CTRL_INT2_FIFO_TH_Pos))  /* Assignment of INT2_FIFO_TH value in LSM6DSO32_INT2_CTRL register */
#define LSM6DSO32_INT2_CTRL_INT2_FIFO_TH_Get(reg)                     (((reg) & LSM6DSO32_INT2_CTRL_INT2_FIFO_TH_Msk) >> LSM6DSO32_INT2_CTRL_INT2_FIFO_TH_Pos)  /* Assignment of INT2_FIFO_TH value in LSM6DSO32_INT2_CTRL register */
#define LSM6DSO32_INT2_CTRL_INT2_FIFO_OVR_Pos                         (4)  /* (LSM6DSO32_INT2_CTRL) Enables FIFO overrun interrupt on INT2 pin. It can be also used to trigger an IBI when the MIPI I3CSM interface is used position */
#define LSM6DSO32_INT2_CTRL_INT2_FIFO_OVR_Msk                         ((0x1) << LSM6DSO32_INT2_CTRL_INT2_FIFO_OVR_Pos)  /* (LSM6DSO32_INT2_CTRL) Enables FIFO overrun interrupt on INT2 pin. It can be also used to trigger an IBI when the MIPI I3CSM interface is used mask */
#define LSM6DSO32_INT2_CTRL_INT2_FIFO_OVR(value)                      (LSM6DSO32_INT2_CTRL_INT2_FIFO_OVR_Msk & ((value) << LSM6DSO32_INT2_CTRL_INT2_FIFO_OVR_Pos))  /* Assignment of INT2_FIFO_OVR value in LSM6DSO32_INT2_CTRL register */
#define LSM6DSO32_INT2_CTRL_INT2_FIFO_OVR_Get(reg)                    (((reg) & LSM6DSO32_INT2_CTRL_INT2_FIFO_OVR_Msk) >> LSM6DSO32_INT2_CTRL_INT2_FIFO_OVR_Pos)  /* Assignment of INT2_FIFO_OVR value in LSM6DSO32_INT2_CTRL register */
#define LSM6DSO32_INT2_CTRL_INT2_FIFO_FULL_Pos                        (5)  /* (LSM6DSO32_INT2_CTRL) Enables FIFO full flag interrupt on INT2 pin. It can be also used to trigger an IBI when the MIPI I3CSM interface is used position */
#define LSM6DSO32_INT2_CTRL_INT2_FIFO_FULL_Msk                        ((0x1) << LSM6DSO32_INT2_CTRL_INT2_FIFO_FULL_Pos)  /* (LSM6DSO32_INT2_CTRL) Enables FIFO full flag interrupt on INT2 pin. It can be also used to trigger an IBI when the MIPI I3CSM interface is used mask */
#define LSM6DSO32_INT2_CTRL_INT2_FIFO_FULL(value)                     (LSM6DSO32_INT2_CTRL_INT2_FIFO_FULL_Msk & ((value) << LSM6DSO32_INT2_CTRL_INT2_FIFO_FULL_Pos))  /* Assignment of INT2_FIFO_FULL value in LSM6DSO32_INT2_CTRL register */
#define LSM6DSO32_INT2_CTRL_INT2_FIFO_FULL_Get(reg)                   (((reg) & LSM6DSO32_INT2_CTRL_INT2_FIFO_FULL_Msk) >> LSM6DSO32_INT2_CTRL_INT2_FIFO_FULL_Pos)  /* Assignment of INT2_FIFO_FULL value in LSM6DSO32_INT2_CTRL register */
#define LSM6DSO32_INT2_CTRL_INT2_CNT_BDR_Pos                          (6)  /* (LSM6DSO32_INT2_CTRL) Enables COUNTER_BDR_IA interrupt on INT2 position */
#define LSM6DSO32_INT2_CTRL_INT2_CNT_BDR_Msk                          ((0x1) << LSM6DSO32_INT2_CTRL_INT2_CNT_BDR_Pos)  /* (LSM6DSO32_INT2_CTRL) Enables COUNTER_BDR_IA interrupt on INT2 mask */
#define LSM6DSO32_INT2_CTRL_INT2_CNT_BDR(value)                       (LSM6DSO32_INT2_CTRL_INT2_CNT_BDR_Msk & ((value) << LSM6DSO32_INT2_CTRL_INT2_CNT_BDR_Pos))  /* Assignment of INT2_CNT_BDR value in LSM6DSO32_INT2_CTRL register */
#define LSM6DSO32_INT2_CTRL_INT2_CNT_BDR_Get(reg)                     (((reg) & LSM6DSO32_INT2_CTRL_INT2_CNT_BDR_Msk) >> LSM6DSO32_INT2_CTRL_INT2_CNT_BDR_Pos)  /* Assignment of INT2_CNT_BDR value in LSM6DSO32_INT2_CTRL register */
#define LSM6DSO32_INT2_CTRL_Msk                                       (0x7f)  /* (LSM6DSO32_INT2_CTRL) Register mask */


/* LSM6DSO32_WHO_AM_I, R: WHO_AM_I register. This is a read-only register. Its value is fixed at 6Ch */
#define LSM6DSO32_WHO_AM_I_ADDR                                       (0xf)  /* (LSM6DSO32_WHO_AM_I) WHO_AM_I register. This is a read-only register. Its value is fixed at 6Ch address */
#define LSM6DSO32_WHO_AM_I_RESETVALUE                                 (0x6c)  /* (LSM6DSO32_WHO_AM_I) WHO_AM_I register. This is a read-only register. Its value is fixed at 6Ch reset value */

#define LSM6DSO32_WHO_AM_I_Msk                                        (0xff)  /* (LSM6DSO32_WHO_AM_I) Register mask */


/* LSM6DSO32_CTRL1_XL, R/W: Acceleration control 1 */
#define LSM6DSO32_CTRL1_XL_ADDR                                       (0x10)  /* (LSM6DSO32_CTRL1_XL) Acceleration control 1 address */

#define LSM6DSO32_CTRL1_XL_LPF2_XL_EN_Pos                             (1)  /* (LSM6DSO32_CTRL1_XL) Accelerometer high-resolution selection position */
#define LSM6DSO32_CTRL1_XL_LPF2_XL_EN_Msk                             ((0x1) << LSM6DSO32_CTRL1_XL_LPF2_XL_EN_Pos)  /* (LSM6DSO32_CTRL1_XL) Accelerometer high-resolution selection mask */
#define LSM6DSO32_CTRL1_XL_LPF2_XL_EN(value)                          (LSM6DSO32_CTRL1_XL_LPF2_XL_EN_Msk & ((value) << LSM6DSO32_CTRL1_XL_LPF2_XL_EN_Pos))  /* Assignment of LPF2_XL_EN value in LSM6DSO32_CTRL1_XL register */
#define LSM6DSO32_CTRL1_XL_LPF2_XL_EN_Get(reg)                        (((reg) & LSM6DSO32_CTRL1_XL_LPF2_XL_EN_Msk) >> LSM6DSO32_CTRL1_XL_LPF2_XL_EN_Pos)  /* Assignment of LPF2_XL_EN value in LSM6DSO32_CTRL1_XL register */
#define LSM6DSO32_CTRL1_XL_FS0_XL_Pos                                 (2)  /* (LSM6DSO32_CTRL1_XL) Accelerometer full-scale selection position */
#define LSM6DSO32_CTRL1_XL_FS0_XL_Msk                                 ((0x7) << LSM6DSO32_CTRL1_XL_FS0_XL_Pos)  /* (LSM6DSO32_CTRL1_XL) Accelerometer full-scale selection mask */
#define LSM6DSO32_CTRL1_XL_FS0_XL(value)                              (LSM6DSO32_CTRL1_XL_FS0_XL_Msk & ((value) << LSM6DSO32_CTRL1_XL_FS0_XL_Pos))  /* Assignment of FS0_XL value in LSM6DSO32_CTRL1_XL register */
#define LSM6DSO32_CTRL1_XL_FS0_XL_Get(reg)                            (((reg) & LSM6DSO32_CTRL1_XL_FS0_XL_Msk) >> LSM6DSO32_CTRL1_XL_FS0_XL_Pos)  /* Assignment of FS0_XL value in LSM6DSO32_CTRL1_XL register */
#define   LSM6DSO32_CTRL1_XL_FS0_XL_4G_Val                            (0)  /* (LSM6DSO32_CTRL1_XL) +/- 4g value */
#define   LSM6DSO32_CTRL1_XL_FS0_XL_32G_Val                           (1)  /* (LSM6DSO32_CTRL1_XL) +/- 32g value */
#define   LSM6DSO32_CTRL1_XL_FS0_XL_8G_Val                            (2)  /* (LSM6DSO32_CTRL1_XL) +/- 8g value */
#define   LSM6DSO32_CTRL1_XL_FS0_XL_16G_Val                           (3)  /* (LSM6DSO32_CTRL1_XL) +/- 16g value */
#define LSM6DSO32_CTRL1_XL_FS0_XL_4G                                  (LSM6DSO32_CTRL1_XL_FS0_XL_4G_Val << LSM6DSO32_CTRL1_XL_FS0_XL_Pos)  /* (LSM6DSO32_CTRL1_XL) +/- 4g */
#define LSM6DSO32_CTRL1_XL_FS0_XL_32G                                 (LSM6DSO32_CTRL1_XL_FS0_XL_32G_Val << LSM6DSO32_CTRL1_XL_FS0_XL_Pos)  /* (LSM6DSO32_CTRL1_XL) +/- 32g */
#define LSM6DSO32_CTRL1_XL_FS0_XL_8G                                  (LSM6DSO32_CTRL1_XL_FS0_XL_8G_Val << LSM6DSO32_CTRL1_XL_FS0_XL_Pos)  /* (LSM6DSO32_CTRL1_XL) +/- 8g */
#define LSM6DSO32_CTRL1_XL_FS0_XL_16G                                 (LSM6DSO32_CTRL1_XL_FS0_XL_16G_Val << LSM6DSO32_CTRL1_XL_FS0_XL_Pos)  /* (LSM6DSO32_CTRL1_XL) +/- 16g */
#define LSM6DSO32_CTRL1_XL_ODR_XL_Pos                                 (4)  /* (LSM6DSO32_CTRL1_XL) Accelerometer ODR selection position */
#define LSM6DSO32_CTRL1_XL_ODR_XL_Msk                                 ((0xf) << LSM6DSO32_CTRL1_XL_ODR_XL_Pos)  /* (LSM6DSO32_CTRL1_XL) Accelerometer ODR selection mask */
#define LSM6DSO32_CTRL1_XL_ODR_XL(value)                              (LSM6DSO32_CTRL1_XL_ODR_XL_Msk & ((value) << LSM6DSO32_CTRL1_XL_ODR_XL_Pos))  /* Assignment of ODR_XL value in LSM6DSO32_CTRL1_XL register */
#define LSM6DSO32_CTRL1_XL_ODR_XL_Get(reg)                            (((reg) & LSM6DSO32_CTRL1_XL_ODR_XL_Msk) >> LSM6DSO32_CTRL1_XL_ODR_XL_Pos)  /* Assignment of ODR_XL value in LSM6DSO32_CTRL1_XL register */
#define   LSM6DSO32_CTRL1_XL_ODR_XL_POWER_DOWN_Val                    (0)  /* (LSM6DSO32_CTRL1_XL) Power down value */
#define   LSM6DSO32_CTRL1_XL_ODR_XL_12_5Hz_Val                        (1)  /* (LSM6DSO32_CTRL1_XL) 12.5Hz value */
#define   LSM6DSO32_CTRL1_XL_ODR_XL_26Hz_Val                          (2)  /* (LSM6DSO32_CTRL1_XL) 26Hz value */
#define   LSM6DSO32_CTRL1_XL_ODR_XL_52Hz_Val                          (3)  /* (LSM6DSO32_CTRL1_XL) 52Hz value */
#define   LSM6DSO32_CTRL1_XL_ODR_XL_104Hz_Val                         (4)  /* (LSM6DSO32_CTRL1_XL) 104Hz value */
#define   LSM6DSO32_CTRL1_XL_ODR_XL_208Hz_Val                         (5)  /* (LSM6DSO32_CTRL1_XL) 208Hz value */
#define   LSM6DSO32_CTRL1_XL_ODR_XL_417Hz_Val                         (6)  /* (LSM6DSO32_CTRL1_XL) 417Hz value */
#define   LSM6DSO32_CTRL1_XL_ODR_XL_833Hz_Val                         (7)  /* (LSM6DSO32_CTRL1_XL) 833Hz value */
#define   LSM6DSO32_CTRL1_XL_ODR_XL_1667Hz_Val                        (8)  /* (LSM6DSO32_CTRL1_XL) 1667Hz value */
#define   LSM6DSO32_CTRL1_XL_ODR_XL_3333Hz_Val                        (9)  /* (LSM6DSO32_CTRL1_XL) 3333Hz value */
#define   LSM6DSO32_CTRL1_XL_ODR_XL_6667Hz_Val                        (10)  /* (LSM6DSO32_CTRL1_XL) 6667Hz value */
#define   LSM6DSO32_CTRL1_XL_ODR_XL_1_6Hz_Val                         (11)  /* (LSM6DSO32_CTRL1_XL) 1.6Hz value */
#define LSM6DSO32_CTRL1_XL_ODR_XL_POWER_DOWN                          (LSM6DSO32_CTRL1_XL_ODR_XL_POWER_DOWN_Val << LSM6DSO32_CTRL1_XL_ODR_XL_Pos)  /* (LSM6DSO32_CTRL1_XL) Power down */
#define LSM6DSO32_CTRL1_XL_ODR_XL_12_5Hz                              (LSM6DSO32_CTRL1_XL_ODR_XL_12_5Hz_Val << LSM6DSO32_CTRL1_XL_ODR_XL_Pos)  /* (LSM6DSO32_CTRL1_XL) 12.5Hz */
#define LSM6DSO32_CTRL1_XL_ODR_XL_26Hz                                (LSM6DSO32_CTRL1_XL_ODR_XL_26Hz_Val << LSM6DSO32_CTRL1_XL_ODR_XL_Pos)  /* (LSM6DSO32_CTRL1_XL) 26Hz */
#define LSM6DSO32_CTRL1_XL_ODR_XL_52Hz                                (LSM6DSO32_CTRL1_XL_ODR_XL_52Hz_Val << LSM6DSO32_CTRL1_XL_ODR_XL_Pos)  /* (LSM6DSO32_CTRL1_XL) 52Hz */
#define LSM6DSO32_CTRL1_XL_ODR_XL_104Hz                               (LSM6DSO32_CTRL1_XL_ODR_XL_104Hz_Val << LSM6DSO32_CTRL1_XL_ODR_XL_Pos)  /* (LSM6DSO32_CTRL1_XL) 104Hz */
#define LSM6DSO32_CTRL1_XL_ODR_XL_208Hz                               (LSM6DSO32_CTRL1_XL_ODR_XL_208Hz_Val << LSM6DSO32_CTRL1_XL_ODR_XL_Pos)  /* (LSM6DSO32_CTRL1_XL) 208Hz */
#define LSM6DSO32_CTRL1_XL_ODR_XL_417Hz                               (LSM6DSO32_CTRL1_XL_ODR_XL_417Hz_Val << LSM6DSO32_CTRL1_XL_ODR_XL_Pos)  /* (LSM6DSO32_CTRL1_XL) 417Hz */
#define LSM6DSO32_CTRL1_XL_ODR_XL_833Hz                               (LSM6DSO32_CTRL1_XL_ODR_XL_833Hz_Val << LSM6DSO32_CTRL1_XL_ODR_XL_Pos)  /* (LSM6DSO32_CTRL1_XL) 833Hz */
#define LSM6DSO32_CTRL1_XL_ODR_XL_1667Hz                              (LSM6DSO32_CTRL1_XL_ODR_XL_1667Hz_Val << LSM6DSO32_CTRL1_XL_ODR_XL_Pos)  /* (LSM6DSO32_CTRL1_XL) 1667Hz */
#define LSM6DSO32_CTRL1_XL_ODR_XL_3333Hz                              (LSM6DSO32_CTRL1_XL_ODR_XL_3333Hz_Val << LSM6DSO32_CTRL1_XL_ODR_XL_Pos)  /* (LSM6DSO32_CTRL1_XL) 3333Hz */
#define LSM6DSO32_CTRL1_XL_ODR_XL_6667Hz                              (LSM6DSO32_CTRL1_XL_ODR_XL_6667Hz_Val << LSM6DSO32_CTRL1_XL_ODR_XL_Pos)  /* (LSM6DSO32_CTRL1_XL) 6667Hz */
#define LSM6DSO32_CTRL1_XL_ODR_XL_1_6Hz                               (LSM6DSO32_CTRL1_XL_ODR_XL_1_6Hz_Val << LSM6DSO32_CTRL1_XL_ODR_XL_Pos)  /* (LSM6DSO32_CTRL1_XL) 1.6Hz */
#define LSM6DSO32_CTRL1_XL_Msk                                        (0xfe)  /* (LSM6DSO32_CTRL1_XL) Register mask */


/* LSM6DSO32_CTRL2_G, R/W: Gyroscope control register 2 */
#define LSM6DSO32_CTRL2_G_ADDR                                        (0x11)  /* (LSM6DSO32_CTRL2_G) Gyroscope control register 2 address */

#define LSM6DSO32_CTRL2_G_FS_125_Pos                                  (1)  /* (LSM6DSO32_CTRL2_G) Selects gyro chain full-scale ±125 dps position */
#define LSM6DSO32_CTRL2_G_FS_125_Msk                                  ((0x1) << LSM6DSO32_CTRL2_G_FS_125_Pos)  /* (LSM6DSO32_CTRL2_G) Selects gyro chain full-scale ±125 dps mask */
#define LSM6DSO32_CTRL2_G_FS_125(value)                               (LSM6DSO32_CTRL2_G_FS_125_Msk & ((value) << LSM6DSO32_CTRL2_G_FS_125_Pos))  /* Assignment of FS_125 value in LSM6DSO32_CTRL2_G register */
#define LSM6DSO32_CTRL2_G_FS_125_Get(reg)                             (((reg) & LSM6DSO32_CTRL2_G_FS_125_Msk) >> LSM6DSO32_CTRL2_G_FS_125_Pos)  /* Assignment of FS_125 value in LSM6DSO32_CTRL2_G register */
#define LSM6DSO32_CTRL2_G_FS_G_Pos                                    (2)
#define LSM6DSO32_CTRL2_G_FS_G_Msk                                    ((0x3) << LSM6DSO32_CTRL2_G_FS_G_Pos)
#define LSM6DSO32_CTRL2_G_FS_G(value)                                 (LSM6DSO32_CTRL2_G_FS_G_Msk & ((value) << LSM6DSO32_CTRL2_G_FS_G_Pos))
#define LSM6DSO32_CTRL2_G_FS_G_Get(reg)                               (((reg) & LSM6DSO32_CTRL2_G_FS_G_Msk) >> LSM6DSO32_CTRL2_G_FS_G_Pos)
#define   LSM6DSO32_CTRL2_G_FS_G_250DPS_Val                           (0)  /* (LSM6DSO32_CTRL2_G) +/- 250dps value */
#define   LSM6DSO32_CTRL2_G_FS_G_500DPS_Val                           (1)  /* (LSM6DSO32_CTRL2_G) +/- 500dps value */
#define   LSM6DSO32_CTRL2_G_FS_G_1000DPS_Val                          (2)  /* (LSM6DSO32_CTRL2_G) +/- 1000dps value */
#define   LSM6DSO32_CTRL2_G_FS_G_2000DPS_Val                          (3)  /* (LSM6DSO32_CTRL2_G) +/- 2000dps value */
#define LSM6DSO32_CTRL2_G_FS_G_250DPS                                 (LSM6DSO32_CTRL2_G_FS_G_250DPS_Val << LSM6DSO32_CTRL2_G_FS_G_Pos)  /* (LSM6DSO32_CTRL2_G) +/- 250dps */
#define LSM6DSO32_CTRL2_G_FS_G_500DPS                                 (LSM6DSO32_CTRL2_G_FS_G_500DPS_Val << LSM6DSO32_CTRL2_G_FS_G_Pos)  /* (LSM6DSO32_CTRL2_G) +/- 500dps */
#define LSM6DSO32_CTRL2_G_FS_G_1000DPS                                (LSM6DSO32_CTRL2_G_FS_G_1000DPS_Val << LSM6DSO32_CTRL2_G_FS_G_Pos)  /* (LSM6DSO32_CTRL2_G) +/- 1000dps */
#define LSM6DSO32_CTRL2_G_FS_G_2000DPS                                (LSM6DSO32_CTRL2_G_FS_G_2000DPS_Val << LSM6DSO32_CTRL2_G_FS_G_Pos)  /* (LSM6DSO32_CTRL2_G) +/- 2000dps */
#define LSM6DSO32_CTRL2_G_ODR_G_Pos                                   (4)
#define LSM6DSO32_CTRL2_G_ODR_G_Msk                                   ((0xf) << LSM6DSO32_CTRL2_G_ODR_G_Pos)
#define LSM6DSO32_CTRL2_G_ODR_G(value)                                (LSM6DSO32_CTRL2_G_ODR_G_Msk & ((value) << LSM6DSO32_CTRL2_G_ODR_G_Pos))
#define LSM6DSO32_CTRL2_G_ODR_G_Get(reg)                              (((reg) & LSM6DSO32_CTRL2_G_ODR_G_Msk) >> LSM6DSO32_CTRL2_G_ODR_G_Pos)
#define   LSM6DSO32_CTRL2_G_ODR_G_POWER_DOWN_Val                      (0)  /* (LSM6DSO32_CTRL2_G) Power down value */
#define   LSM6DSO32_CTRL2_G_ODR_G_12_5Hz_Val                          (1)  /* (LSM6DSO32_CTRL2_G) 12.5Hz value */
#define   LSM6DSO32_CTRL2_G_ODR_G_26Hz_Val                            (2)  /* (LSM6DSO32_CTRL2_G) 26Hz value */
#define   LSM6DSO32_CTRL2_G_ODR_G_52Hz_Val                            (3)  /* (LSM6DSO32_CTRL2_G) 52Hz value */
#define   LSM6DSO32_CTRL2_G_ODR_G_104Hz_Val                           (4)  /* (LSM6DSO32_CTRL2_G) 104Hz value */
#define   LSM6DSO32_CTRL2_G_ODR_G_208Hz_Val                           (5)  /* (LSM6DSO32_CTRL2_G) 208Hz value */
#define   LSM6DSO32_CTRL2_G_ODR_G_417Hz_Val                           (6)  /* (LSM6DSO32_CTRL2_G) 417Hz value */
#define   LSM6DSO32_CTRL2_G_ODR_G_833Hz_Val                           (7)  /* (LSM6DSO32_CTRL2_G) 833Hz value */
#define   LSM6DSO32_CTRL2_G_ODR_G_1667Hz_Val                          (8)  /* (LSM6DSO32_CTRL2_G) 1667Hz value */
#define   LSM6DSO32_CTRL2_G_ODR_G_3333Hz_Val                          (9)  /* (LSM6DSO32_CTRL2_G) 3333Hz value */
#define   LSM6DSO32_CTRL2_G_ODR_G_6667Hz_Val                          (10)  /* (LSM6DSO32_CTRL2_G) 6667Hz value */
#define LSM6DSO32_CTRL2_G_ODR_G_POWER_DOWN                            (LSM6DSO32_CTRL2_G_ODR_G_POWER_DOWN_Val << LSM6DSO32_CTRL2_G_ODR_G_Pos)  /* (LSM6DSO32_CTRL2_G) Power down */
#define LSM6DSO32_CTRL2_G_ODR_G_12_5Hz                                (LSM6DSO32_CTRL2_G_ODR_G_12_5Hz_Val << LSM6DSO32_CTRL2_G_ODR_G_Pos)  /* (LSM6DSO32_CTRL2_G) 12.5Hz */
#define LSM6DSO32_CTRL2_G_ODR_G_26Hz                                  (LSM6DSO32_CTRL2_G_ODR_G_26Hz_Val << LSM6DSO32_CTRL2_G_ODR_G_Pos)  /* (LSM6DSO32_CTRL2_G) 26Hz */
#define LSM6DSO32_CTRL2_G_ODR_G_52Hz                                  (LSM6DSO32_CTRL2_G_ODR_G_52Hz_Val << LSM6DSO32_CTRL2_G_ODR_G_Pos)  /* (LSM6DSO32_CTRL2_G) 52Hz */
#define LSM6DSO32_CTRL2_G_ODR_G_104Hz                                 (LSM6DSO32_CTRL2_G_ODR_G_104Hz_Val << LSM6DSO32_CTRL2_G_ODR_G_Pos)  /* (LSM6DSO32_CTRL2_G) 104Hz */
#define LSM6DSO32_CTRL2_G_ODR_G_208Hz                                 (LSM6DSO32_CTRL2_G_ODR_G_208Hz_Val << LSM6DSO32_CTRL2_G_ODR_G_Pos)  /* (LSM6DSO32_CTRL2_G) 208Hz */
#define LSM6DSO32_CTRL2_G_ODR_G_417Hz                                 (LSM6DSO32_CTRL2_G_ODR_G_417Hz_Val << LSM6DSO32_CTRL2_G_ODR_G_Pos)  /* (LSM6DSO32_CTRL2_G) 417Hz */
#define LSM6DSO32_CTRL2_G_ODR_G_833Hz                                 (LSM6DSO32_CTRL2_G_ODR_G_833Hz_Val << LSM6DSO32_CTRL2_G_ODR_G_Pos)  /* (LSM6DSO32_CTRL2_G) 833Hz */
#define LSM6DSO32_CTRL2_G_ODR_G_1667Hz                                (LSM6DSO32_CTRL2_G_ODR_G_1667Hz_Val << LSM6DSO32_CTRL2_G_ODR_G_Pos)  /* (LSM6DSO32_CTRL2_G) 1667Hz */
#define LSM6DSO32_CTRL2_G_ODR_G_3333Hz                                (LSM6DSO32_CTRL2_G_ODR_G_3333Hz_Val << LSM6DSO32_CTRL2_G_ODR_G_Pos)  /* (LSM6DSO32_CTRL2_G) 3333Hz */
#define LSM6DSO32_CTRL2_G_ODR_G_6667Hz                                (LSM6DSO32_CTRL2_G_ODR_G_6667Hz_Val << LSM6DSO32_CTRL2_G_ODR_G_Pos)  /* (LSM6DSO32_CTRL2_G) 6667Hz */
#define LSM6DSO32_CTRL2_G_Msk                                         (0xfe)  /* (LSM6DSO32_CTRL2_G) Register mask */


/* LSM6DSO32_CTRL3_C, R/W: Control register 3 */
#define LSM6DSO32_CTRL3_C_ADDR                                        (0x12)  /* (LSM6DSO32_CTRL3_C) Control register 3 address */
#define LSM6DSO32_CTRL3_C_RESETVALUE                                  (0x4)  /* (LSM6DSO32_CTRL3_C) Control register 3 reset value */

#define LSM6DSO32_CTRL3_C_SW_RESET_Pos                                (0)  /* (LSM6DSO32_CTRL3_C) Software reset position */
#define LSM6DSO32_CTRL3_C_SW_RESET_Msk                                ((0x1) << LSM6DSO32_CTRL3_C_SW_RESET_Pos)  /* (LSM6DSO32_CTRL3_C) Software reset mask */
#define LSM6DSO32_CTRL3_C_SW_RESET(value)                             (LSM6DSO32_CTRL3_C_SW_RESET_Msk & ((value) << LSM6DSO32_CTRL3_C_SW_RESET_Pos))  /* Assignment of SW_RESET value in LSM6DSO32_CTRL3_C register */
#define LSM6DSO32_CTRL3_C_SW_RESET_Get(reg)                           (((reg) & LSM6DSO32_CTRL3_C_SW_RESET_Msk) >> LSM6DSO32_CTRL3_C_SW_RESET_Pos)  /* Assignment of SW_RESET value in LSM6DSO32_CTRL3_C register */
#define LSM6DSO32_CTRL3_C_IF_INC_Pos                                  (2)  /* (LSM6DSO32_CTRL3_C) Register address automatically incremented during a multiple byte access with a serial interface (I²C or SPI) position */
#define LSM6DSO32_CTRL3_C_IF_INC_Msk                                  ((0x1) << LSM6DSO32_CTRL3_C_IF_INC_Pos)  /* (LSM6DSO32_CTRL3_C) Register address automatically incremented during a multiple byte access with a serial interface (I²C or SPI) mask */
#define LSM6DSO32_CTRL3_C_IF_INC(value)                               (LSM6DSO32_CTRL3_C_IF_INC_Msk & ((value) << LSM6DSO32_CTRL3_C_IF_INC_Pos))  /* Assignment of IF_INC value in LSM6DSO32_CTRL3_C register */
#define LSM6DSO32_CTRL3_C_IF_INC_Get(reg)                             (((reg) & LSM6DSO32_CTRL3_C_IF_INC_Msk) >> LSM6DSO32_CTRL3_C_IF_INC_Pos)  /* Assignment of IF_INC value in LSM6DSO32_CTRL3_C register */
#define LSM6DSO32_CTRL3_C_SIM_Pos                                     (3)  /* (LSM6DSO32_CTRL3_C) SPI Serial Interface Mode selection position */
#define LSM6DSO32_CTRL3_C_SIM_Msk                                     ((0x1) << LSM6DSO32_CTRL3_C_SIM_Pos)  /* (LSM6DSO32_CTRL3_C) SPI Serial Interface Mode selection mask */
#define LSM6DSO32_CTRL3_C_SIM(value)                                  (LSM6DSO32_CTRL3_C_SIM_Msk & ((value) << LSM6DSO32_CTRL3_C_SIM_Pos))  /* Assignment of SIM value in LSM6DSO32_CTRL3_C register */
#define LSM6DSO32_CTRL3_C_SIM_Get(reg)                                (((reg) & LSM6DSO32_CTRL3_C_SIM_Msk) >> LSM6DSO32_CTRL3_C_SIM_Pos)  /* Assignment of SIM value in LSM6DSO32_CTRL3_C register */
#define   LSM6DSO32_CTRL3_C_SIM_4_WIRE_Val                            (0)  /* (LSM6DSO32_CTRL3_C) 4-wire interface value */
#define   LSM6DSO32_CTRL3_C_SIM_3_WIRE_Val                            (1)  /* (LSM6DSO32_CTRL3_C) 3-wire interface value */
#define LSM6DSO32_CTRL3_C_SIM_4_WIRE                                  (LSM6DSO32_CTRL3_C_SIM_4_WIRE_Val << LSM6DSO32_CTRL3_C_SIM_Pos)  /* (LSM6DSO32_CTRL3_C) 4-wire interface */
#define LSM6DSO32_CTRL3_C_SIM_3_WIRE                                  (LSM6DSO32_CTRL3_C_SIM_3_WIRE_Val << LSM6DSO32_CTRL3_C_SIM_Pos)  /* (LSM6DSO32_CTRL3_C) 3-wire interface */
#define LSM6DSO32_CTRL3_C_PP_OD_Pos                                   (4)  /* (LSM6DSO32_CTRL3_C) Push-pull/open-drain selection on INT1 and INT2 pins position */
#define LSM6DSO32_CTRL3_C_PP_OD_Msk                                   ((0x1) << LSM6DSO32_CTRL3_C_PP_OD_Pos)  /* (LSM6DSO32_CTRL3_C) Push-pull/open-drain selection on INT1 and INT2 pins mask */
#define LSM6DSO32_CTRL3_C_PP_OD(value)                                (LSM6DSO32_CTRL3_C_PP_OD_Msk & ((value) << LSM6DSO32_CTRL3_C_PP_OD_Pos))  /* Assignment of PP_OD value in LSM6DSO32_CTRL3_C register */
#define LSM6DSO32_CTRL3_C_PP_OD_Get(reg)                              (((reg) & LSM6DSO32_CTRL3_C_PP_OD_Msk) >> LSM6DSO32_CTRL3_C_PP_OD_Pos)  /* Assignment of PP_OD value in LSM6DSO32_CTRL3_C register */
#define   LSM6DSO32_CTRL3_C_PP_OD_PP_Val                              (0)  /* (LSM6DSO32_CTRL3_C) Push-pull mode value */
#define   LSM6DSO32_CTRL3_C_PP_OD_OD_Val                              (1)  /* (LSM6DSO32_CTRL3_C) Open-drain mode value */
#define LSM6DSO32_CTRL3_C_PP_OD_PP                                    (LSM6DSO32_CTRL3_C_PP_OD_PP_Val << LSM6DSO32_CTRL3_C_PP_OD_Pos)  /* (LSM6DSO32_CTRL3_C) Push-pull mode */
#define LSM6DSO32_CTRL3_C_PP_OD_OD                                    (LSM6DSO32_CTRL3_C_PP_OD_OD_Val << LSM6DSO32_CTRL3_C_PP_OD_Pos)  /* (LSM6DSO32_CTRL3_C) Open-drain mode */
#define LSM6DSO32_CTRL3_C_H_LACTIVE_Pos                               (5)  /* (LSM6DSO32_CTRL3_C) Interrupt activation level position */
#define LSM6DSO32_CTRL3_C_H_LACTIVE_Msk                               ((0x1) << LSM6DSO32_CTRL3_C_H_LACTIVE_Pos)  /* (LSM6DSO32_CTRL3_C) Interrupt activation level mask */
#define LSM6DSO32_CTRL3_C_H_LACTIVE(value)                            (LSM6DSO32_CTRL3_C_H_LACTIVE_Msk & ((value) << LSM6DSO32_CTRL3_C_H_LACTIVE_Pos))  /* Assignment of H_LACTIVE value in LSM6DSO32_CTRL3_C register */
#define LSM6DSO32_CTRL3_C_H_LACTIVE_Get(reg)                          (((reg) & LSM6DSO32_CTRL3_C_H_LACTIVE_Msk) >> LSM6DSO32_CTRL3_C_H_LACTIVE_Pos)  /* Assignment of H_LACTIVE value in LSM6DSO32_CTRL3_C register */
#define   LSM6DSO32_CTRL3_C_H_LACTIVE_H_Val                           (0)  /* (LSM6DSO32_CTRL3_C) Interrupt output pins active high value */
#define   LSM6DSO32_CTRL3_C_H_LACTIVE_L_Val                           (1)  /* (LSM6DSO32_CTRL3_C) Interrupt output pins active low value */
#define LSM6DSO32_CTRL3_C_H_LACTIVE_H                                 (LSM6DSO32_CTRL3_C_H_LACTIVE_H_Val << LSM6DSO32_CTRL3_C_H_LACTIVE_Pos)  /* (LSM6DSO32_CTRL3_C) Interrupt output pins active high */
#define LSM6DSO32_CTRL3_C_H_LACTIVE_L                                 (LSM6DSO32_CTRL3_C_H_LACTIVE_L_Val << LSM6DSO32_CTRL3_C_H_LACTIVE_Pos)  /* (LSM6DSO32_CTRL3_C) Interrupt output pins active low */
#define LSM6DSO32_CTRL3_C_BDU_Pos                                     (6)  /* (LSM6DSO32_CTRL3_C) Block data update position */
#define LSM6DSO32_CTRL3_C_BDU_Msk                                     ((0x1) << LSM6DSO32_CTRL3_C_BDU_Pos)  /* (LSM6DSO32_CTRL3_C) Block data update mask */
#define LSM6DSO32_CTRL3_C_BDU(value)                                  (LSM6DSO32_CTRL3_C_BDU_Msk & ((value) << LSM6DSO32_CTRL3_C_BDU_Pos))  /* Assignment of BDU value in LSM6DSO32_CTRL3_C register */
#define LSM6DSO32_CTRL3_C_BDU_Get(reg)                                (((reg) & LSM6DSO32_CTRL3_C_BDU_Msk) >> LSM6DSO32_CTRL3_C_BDU_Pos)  /* Assignment of BDU value in LSM6DSO32_CTRL3_C register */
#define   LSM6DSO32_CTRL3_C_BDU_CONT_Val                              (0)  /* (LSM6DSO32_CTRL3_C) Continuous update value */
#define   LSM6DSO32_CTRL3_C_BDU_READ_Val                              (1)  /* (LSM6DSO32_CTRL3_C) Output registers are not updated until MSB and LSB have been read value */
#define LSM6DSO32_CTRL3_C_BDU_CONT                                    (LSM6DSO32_CTRL3_C_BDU_CONT_Val << LSM6DSO32_CTRL3_C_BDU_Pos)  /* (LSM6DSO32_CTRL3_C) Continuous update */
#define LSM6DSO32_CTRL3_C_BDU_READ                                    (LSM6DSO32_CTRL3_C_BDU_READ_Val << LSM6DSO32_CTRL3_C_BDU_Pos)  /* (LSM6DSO32_CTRL3_C) Output registers are not updated until MSB and LSB have been read */
#define LSM6DSO32_CTRL3_C_BOOT_Pos                                    (7)  /* (LSM6DSO32_CTRL3_C) Reboots memory content position */
#define LSM6DSO32_CTRL3_C_BOOT_Msk                                    ((0x1) << LSM6DSO32_CTRL3_C_BOOT_Pos)  /* (LSM6DSO32_CTRL3_C) Reboots memory content mask */
#define LSM6DSO32_CTRL3_C_BOOT(value)                                 (LSM6DSO32_CTRL3_C_BOOT_Msk & ((value) << LSM6DSO32_CTRL3_C_BOOT_Pos))  /* Assignment of BOOT value in LSM6DSO32_CTRL3_C register */
#define LSM6DSO32_CTRL3_C_BOOT_Get(reg)                               (((reg) & LSM6DSO32_CTRL3_C_BOOT_Msk) >> LSM6DSO32_CTRL3_C_BOOT_Pos)  /* Assignment of BOOT value in LSM6DSO32_CTRL3_C register */
#define LSM6DSO32_CTRL3_C_Msk                                         (0xfd)  /* (LSM6DSO32_CTRL3_C) Register mask */


/* LSM6DSO32_CTRL4_C, R/W: Control register 4 */
#define LSM6DSO32_CTRL4_C_ADDR                                        (0x13)  /* (LSM6DSO32_CTRL4_C) Control register 4 address */

#define LSM6DSO32_CTRL4_C_LPF1_SEL_G_Pos                              (1)  /* (LSM6DSO32_CTRL4_C) Enables gyroscope digital LPF1; the bandwidth can be selected through FTYPE[2:0] in CTRL6_C (15h) position */
#define LSM6DSO32_CTRL4_C_LPF1_SEL_G_Msk                              ((0x1) << LSM6DSO32_CTRL4_C_LPF1_SEL_G_Pos)  /* (LSM6DSO32_CTRL4_C) Enables gyroscope digital LPF1; the bandwidth can be selected through FTYPE[2:0] in CTRL6_C (15h) mask */
#define LSM6DSO32_CTRL4_C_LPF1_SEL_G(value)                           (LSM6DSO32_CTRL4_C_LPF1_SEL_G_Msk & ((value) << LSM6DSO32_CTRL4_C_LPF1_SEL_G_Pos))  /* Assignment of LPF1_SEL_G value in LSM6DSO32_CTRL4_C register */
#define LSM6DSO32_CTRL4_C_LPF1_SEL_G_Get(reg)                         (((reg) & LSM6DSO32_CTRL4_C_LPF1_SEL_G_Msk) >> LSM6DSO32_CTRL4_C_LPF1_SEL_G_Pos)  /* Assignment of LPF1_SEL_G value in LSM6DSO32_CTRL4_C register */
#define LSM6DSO32_CTRL4_C_I2C_DISABLE_Pos                             (2)  /* (LSM6DSO32_CTRL4_C) Disables I²C interface position */
#define LSM6DSO32_CTRL4_C_I2C_DISABLE_Msk                             ((0x1) << LSM6DSO32_CTRL4_C_I2C_DISABLE_Pos)  /* (LSM6DSO32_CTRL4_C) Disables I²C interface mask */
#define LSM6DSO32_CTRL4_C_I2C_DISABLE(value)                          (LSM6DSO32_CTRL4_C_I2C_DISABLE_Msk & ((value) << LSM6DSO32_CTRL4_C_I2C_DISABLE_Pos))  /* Assignment of I2C_DISABLE value in LSM6DSO32_CTRL4_C register */
#define LSM6DSO32_CTRL4_C_I2C_DISABLE_Get(reg)                        (((reg) & LSM6DSO32_CTRL4_C_I2C_DISABLE_Msk) >> LSM6DSO32_CTRL4_C_I2C_DISABLE_Pos)  /* Assignment of I2C_DISABLE value in LSM6DSO32_CTRL4_C register */
#define LSM6DSO32_CTRL4_C_DRDY_MASK_Pos                               (3)  /* (LSM6DSO32_CTRL4_C) Enables data available position */
#define LSM6DSO32_CTRL4_C_DRDY_MASK_Msk                               ((0x1) << LSM6DSO32_CTRL4_C_DRDY_MASK_Pos)  /* (LSM6DSO32_CTRL4_C) Enables data available mask */
#define LSM6DSO32_CTRL4_C_DRDY_MASK(value)                            (LSM6DSO32_CTRL4_C_DRDY_MASK_Msk & ((value) << LSM6DSO32_CTRL4_C_DRDY_MASK_Pos))  /* Assignment of DRDY_MASK value in LSM6DSO32_CTRL4_C register */
#define LSM6DSO32_CTRL4_C_DRDY_MASK_Get(reg)                          (((reg) & LSM6DSO32_CTRL4_C_DRDY_MASK_Msk) >> LSM6DSO32_CTRL4_C_DRDY_MASK_Pos)  /* Assignment of DRDY_MASK value in LSM6DSO32_CTRL4_C register */
#define LSM6DSO32_CTRL4_C_INT2_ON_INT1_Pos                            (5)  /* (LSM6DSO32_CTRL4_C) All interrupt signals available on INT1 pin enable position */
#define LSM6DSO32_CTRL4_C_INT2_ON_INT1_Msk                            ((0x1) << LSM6DSO32_CTRL4_C_INT2_ON_INT1_Pos)  /* (LSM6DSO32_CTRL4_C) All interrupt signals available on INT1 pin enable mask */
#define LSM6DSO32_CTRL4_C_INT2_ON_INT1(value)                         (LSM6DSO32_CTRL4_C_INT2_ON_INT1_Msk & ((value) << LSM6DSO32_CTRL4_C_INT2_ON_INT1_Pos))  /* Assignment of INT2_ON_INT1 value in LSM6DSO32_CTRL4_C register */
#define LSM6DSO32_CTRL4_C_INT2_ON_INT1_Get(reg)                       (((reg) & LSM6DSO32_CTRL4_C_INT2_ON_INT1_Msk) >> LSM6DSO32_CTRL4_C_INT2_ON_INT1_Pos)  /* Assignment of INT2_ON_INT1 value in LSM6DSO32_CTRL4_C register */
#define LSM6DSO32_CTRL4_C_SLEEP_G_Pos                                 (6)  /* (LSM6DSO32_CTRL4_C) Enables gyroscope sleep mode position */
#define LSM6DSO32_CTRL4_C_SLEEP_G_Msk                                 ((0x1) << LSM6DSO32_CTRL4_C_SLEEP_G_Pos)  /* (LSM6DSO32_CTRL4_C) Enables gyroscope sleep mode mask */
#define LSM6DSO32_CTRL4_C_SLEEP_G(value)                              (LSM6DSO32_CTRL4_C_SLEEP_G_Msk & ((value) << LSM6DSO32_CTRL4_C_SLEEP_G_Pos))  /* Assignment of SLEEP_G value in LSM6DSO32_CTRL4_C register */
#define LSM6DSO32_CTRL4_C_SLEEP_G_Get(reg)                            (((reg) & LSM6DSO32_CTRL4_C_SLEEP_G_Msk) >> LSM6DSO32_CTRL4_C_SLEEP_G_Pos)  /* Assignment of SLEEP_G value in LSM6DSO32_CTRL4_C register */
#define LSM6DSO32_CTRL4_C_Msk                                         (0x6e)  /* (LSM6DSO32_CTRL4_C) Register mask */


/* LSM6DSO32_CTRL5_C, R/W: Control register 5 */
#define LSM6DSO32_CTRL5_C_ADDR                                        (0x14)  /* (LSM6DSO32_CTRL5_C) Control register 5 address */

#define LSM6DSO32_CTRL5_C_ST_XL_Pos                                   (0)  /* (LSM6DSO32_CTRL5_C) Linear acceleration sensor self-test enable position */
#define LSM6DSO32_CTRL5_C_ST_XL_Msk                                   ((0x3) << LSM6DSO32_CTRL5_C_ST_XL_Pos)  /* (LSM6DSO32_CTRL5_C) Linear acceleration sensor self-test enable mask */
#define LSM6DSO32_CTRL5_C_ST_XL(value)                                (LSM6DSO32_CTRL5_C_ST_XL_Msk & ((value) << LSM6DSO32_CTRL5_C_ST_XL_Pos))  /* Assignment of ST_XL value in LSM6DSO32_CTRL5_C register */
#define LSM6DSO32_CTRL5_C_ST_XL_Get(reg)                              (((reg) & LSM6DSO32_CTRL5_C_ST_XL_Msk) >> LSM6DSO32_CTRL5_C_ST_XL_Pos)  /* Assignment of ST_XL value in LSM6DSO32_CTRL5_C register */
#define   LSM6DSO32_CTRL5_C_ST_XL_NORM_Val                            (0)  /* (LSM6DSO32_CTRL5_C) Normal mode value */
#define   LSM6DSO32_CTRL5_C_ST_XL_POS_Val                             (1)  /* (LSM6DSO32_CTRL5_C) Positive sign self-test value */
#define   LSM6DSO32_CTRL5_C_ST_XL_NEG_Val                             (3)  /* (LSM6DSO32_CTRL5_C) Negative sign self-test value */
#define LSM6DSO32_CTRL5_C_ST_XL_NORM                                  (LSM6DSO32_CTRL5_C_ST_XL_NORM_Val << LSM6DSO32_CTRL5_C_ST_XL_Pos)  /* (LSM6DSO32_CTRL5_C) Normal mode */
#define LSM6DSO32_CTRL5_C_ST_XL_POS                                   (LSM6DSO32_CTRL5_C_ST_XL_POS_Val << LSM6DSO32_CTRL5_C_ST_XL_Pos)  /* (LSM6DSO32_CTRL5_C) Positive sign self-test */
#define LSM6DSO32_CTRL5_C_ST_XL_NEG                                   (LSM6DSO32_CTRL5_C_ST_XL_NEG_Val << LSM6DSO32_CTRL5_C_ST_XL_Pos)  /* (LSM6DSO32_CTRL5_C) Negative sign self-test */
#define LSM6DSO32_CTRL5_C_ST_G_Pos                                    (2)  /* (LSM6DSO32_CTRL5_C) Angular rate sensor self-test enable position */
#define LSM6DSO32_CTRL5_C_ST_G_Msk                                    ((0x3) << LSM6DSO32_CTRL5_C_ST_G_Pos)  /* (LSM6DSO32_CTRL5_C) Angular rate sensor self-test enable mask */
#define LSM6DSO32_CTRL5_C_ST_G(value)                                 (LSM6DSO32_CTRL5_C_ST_G_Msk & ((value) << LSM6DSO32_CTRL5_C_ST_G_Pos))  /* Assignment of ST_G value in LSM6DSO32_CTRL5_C register */
#define LSM6DSO32_CTRL5_C_ST_G_Get(reg)                               (((reg) & LSM6DSO32_CTRL5_C_ST_G_Msk) >> LSM6DSO32_CTRL5_C_ST_G_Pos)  /* Assignment of ST_G value in LSM6DSO32_CTRL5_C register */
#define   LSM6DSO32_CTRL5_C_ST_G_NORM_Val                             (0)  /* (LSM6DSO32_CTRL5_C) Normal mode value */
#define   LSM6DSO32_CTRL5_C_ST_G_POS_Val                              (1)  /* (LSM6DSO32_CTRL5_C) Positive sign self-test value */
#define   LSM6DSO32_CTRL5_C_ST_G_NEG_Val                              (2)  /* (LSM6DSO32_CTRL5_C) Negative sign self-test value */
#define LSM6DSO32_CTRL5_C_ST_G_NORM                                   (LSM6DSO32_CTRL5_C_ST_G_NORM_Val << LSM6DSO32_CTRL5_C_ST_G_Pos)  /* (LSM6DSO32_CTRL5_C) Normal mode */
#define LSM6DSO32_CTRL5_C_ST_G_POS                                    (LSM6DSO32_CTRL5_C_ST_G_POS_Val << LSM6DSO32_CTRL5_C_ST_G_Pos)  /* (LSM6DSO32_CTRL5_C) Positive sign self-test */
#define LSM6DSO32_CTRL5_C_ST_G_NEG                                    (LSM6DSO32_CTRL5_C_ST_G_NEG_Val << LSM6DSO32_CTRL5_C_ST_G_Pos)  /* (LSM6DSO32_CTRL5_C) Negative sign self-test */
#define LSM6DSO32_CTRL5_C_ROUNDING_Pos                                (5)  /* (LSM6DSO32_CTRL5_C) Circular burst-mode (rounding) read from the output registers position */
#define LSM6DSO32_CTRL5_C_ROUNDING_Msk                                ((0x3) << LSM6DSO32_CTRL5_C_ROUNDING_Pos)  /* (LSM6DSO32_CTRL5_C) Circular burst-mode (rounding) read from the output registers mask */
#define LSM6DSO32_CTRL5_C_ROUNDING(value)                             (LSM6DSO32_CTRL5_C_ROUNDING_Msk & ((value) << LSM6DSO32_CTRL5_C_ROUNDING_Pos))  /* Assignment of ROUNDING value in LSM6DSO32_CTRL5_C register */
#define LSM6DSO32_CTRL5_C_ROUNDING_Get(reg)                           (((reg) & LSM6DSO32_CTRL5_C_ROUNDING_Msk) >> LSM6DSO32_CTRL5_C_ROUNDING_Pos)  /* Assignment of ROUNDING value in LSM6DSO32_CTRL5_C register */
#define   LSM6DSO32_CTRL5_C_ROUNDING_OFF_Val                          (0)  /* (LSM6DSO32_CTRL5_C) No rounding value */
#define   LSM6DSO32_CTRL5_C_ROUNDING_XL_Val                           (1)  /* (LSM6DSO32_CTRL5_C) Accelerometer only value */
#define   LSM6DSO32_CTRL5_C_ROUNDING_G_Val                            (2)  /* (LSM6DSO32_CTRL5_C) Gyroscope only value */
#define   LSM6DSO32_CTRL5_C_ROUNDING_G_XL_Val                         (3)  /* (LSM6DSO32_CTRL5_C) Gyroscope + accelerometer value */
#define LSM6DSO32_CTRL5_C_ROUNDING_OFF                                (LSM6DSO32_CTRL5_C_ROUNDING_OFF_Val << LSM6DSO32_CTRL5_C_ROUNDING_Pos)  /* (LSM6DSO32_CTRL5_C) No rounding */
#define LSM6DSO32_CTRL5_C_ROUNDING_XL                                 (LSM6DSO32_CTRL5_C_ROUNDING_XL_Val << LSM6DSO32_CTRL5_C_ROUNDING_Pos)  /* (LSM6DSO32_CTRL5_C) Accelerometer only */
#define LSM6DSO32_CTRL5_C_ROUNDING_G                                  (LSM6DSO32_CTRL5_C_ROUNDING_G_Val << LSM6DSO32_CTRL5_C_ROUNDING_Pos)  /* (LSM6DSO32_CTRL5_C) Gyroscope only */
#define LSM6DSO32_CTRL5_C_ROUNDING_G_XL                               (LSM6DSO32_CTRL5_C_ROUNDING_G_XL_Val << LSM6DSO32_CTRL5_C_ROUNDING_Pos)  /* (LSM6DSO32_CTRL5_C) Gyroscope + accelerometer */
#define LSM6DSO32_CTRL5_C_X_ULP_EN_Pos                                (7)  /* (LSM6DSO32_CTRL5_C) Accelerometer ultra-low-power mode enable position */
#define LSM6DSO32_CTRL5_C_X_ULP_EN_Msk                                ((0x1) << LSM6DSO32_CTRL5_C_X_ULP_EN_Pos)  /* (LSM6DSO32_CTRL5_C) Accelerometer ultra-low-power mode enable mask */
#define LSM6DSO32_CTRL5_C_X_ULP_EN(value)                             (LSM6DSO32_CTRL5_C_X_ULP_EN_Msk & ((value) << LSM6DSO32_CTRL5_C_X_ULP_EN_Pos))  /* Assignment of X_ULP_EN value in LSM6DSO32_CTRL5_C register */
#define LSM6DSO32_CTRL5_C_X_ULP_EN_Get(reg)                           (((reg) & LSM6DSO32_CTRL5_C_X_ULP_EN_Msk) >> LSM6DSO32_CTRL5_C_X_ULP_EN_Pos)  /* Assignment of X_ULP_EN value in LSM6DSO32_CTRL5_C register */
#define LSM6DSO32_CTRL5_C_Msk                                         (0xef)  /* (LSM6DSO32_CTRL5_C) Register mask */


/* LSM6DSO32_CTRL6_C, R/W: Control register 6 */
#define LSM6DSO32_CTRL6_C_ADDR                                        (0x15)  /* (LSM6DSO32_CTRL6_C) Control register 6 address */

#define LSM6DSO32_CTRL6_C_FTYPE_Pos                                   (0)  /* (LSM6DSO32_CTRL6_C) Gyroscope's low-pass filter (LPF1) bandwidth selection position */
#define LSM6DSO32_CTRL6_C_FTYPE_Msk                                   ((0xf) << LSM6DSO32_CTRL6_C_FTYPE_Pos)  /* (LSM6DSO32_CTRL6_C) Gyroscope's low-pass filter (LPF1) bandwidth selection mask */
#define LSM6DSO32_CTRL6_C_FTYPE(value)                                (LSM6DSO32_CTRL6_C_FTYPE_Msk & ((value) << LSM6DSO32_CTRL6_C_FTYPE_Pos))  /* Assignment of FTYPE value in LSM6DSO32_CTRL6_C register */
#define LSM6DSO32_CTRL6_C_FTYPE_Get(reg)                              (((reg) & LSM6DSO32_CTRL6_C_FTYPE_Msk) >> LSM6DSO32_CTRL6_C_FTYPE_Pos)  /* Assignment of FTYPE value in LSM6DSO32_CTRL6_C register */
#define LSM6DSO32_CTRL6_C_USR_OFF_W_Pos                               (3)  /* (LSM6DSO32_CTRL6_C) Weight of XL user offset bits of registers X_OFS_USR (73h), Y_OFS_USR (74h), Z_OFS_USR (75h) position */
#define LSM6DSO32_CTRL6_C_USR_OFF_W_Msk                               ((0x1) << LSM6DSO32_CTRL6_C_USR_OFF_W_Pos)  /* (LSM6DSO32_CTRL6_C) Weight of XL user offset bits of registers X_OFS_USR (73h), Y_OFS_USR (74h), Z_OFS_USR (75h) mask */
#define LSM6DSO32_CTRL6_C_USR_OFF_W(value)                            (LSM6DSO32_CTRL6_C_USR_OFF_W_Msk & ((value) << LSM6DSO32_CTRL6_C_USR_OFF_W_Pos))  /* Assignment of USR_OFF_W value in LSM6DSO32_CTRL6_C register */
#define LSM6DSO32_CTRL6_C_USR_OFF_W_Get(reg)                          (((reg) & LSM6DSO32_CTRL6_C_USR_OFF_W_Msk) >> LSM6DSO32_CTRL6_C_USR_OFF_W_Pos)  /* Assignment of USR_OFF_W value in LSM6DSO32_CTRL6_C register */
#define   LSM6DSO32_CTRL6_C_USR_OFF_W_10G_Val                         (0)  /* (LSM6DSO32_CTRL6_C) 2^-10 g/LSB value */
#define   LSM6DSO32_CTRL6_C_USR_OFF_W_6G_Val                          (1)  /* (LSM6DSO32_CTRL6_C) 2^-6 g/LSB value */
#define LSM6DSO32_CTRL6_C_USR_OFF_W_10G                               (LSM6DSO32_CTRL6_C_USR_OFF_W_10G_Val << LSM6DSO32_CTRL6_C_USR_OFF_W_Pos)  /* (LSM6DSO32_CTRL6_C) 2^-10 g/LSB */
#define LSM6DSO32_CTRL6_C_USR_OFF_W_6G                                (LSM6DSO32_CTRL6_C_USR_OFF_W_6G_Val << LSM6DSO32_CTRL6_C_USR_OFF_W_Pos)  /* (LSM6DSO32_CTRL6_C) 2^-6 g/LSB */
#define LSM6DSO32_CTRL6_C_XL_HM_MODE_Pos                              (4)  /* (LSM6DSO32_CTRL6_C) High-performance operating mode disable for acceleromete position */
#define LSM6DSO32_CTRL6_C_XL_HM_MODE_Msk                              ((0x1) << LSM6DSO32_CTRL6_C_XL_HM_MODE_Pos)  /* (LSM6DSO32_CTRL6_C) High-performance operating mode disable for acceleromete mask */
#define LSM6DSO32_CTRL6_C_XL_HM_MODE(value)                           (LSM6DSO32_CTRL6_C_XL_HM_MODE_Msk & ((value) << LSM6DSO32_CTRL6_C_XL_HM_MODE_Pos))  /* Assignment of XL_HM_MODE value in LSM6DSO32_CTRL6_C register */
#define LSM6DSO32_CTRL6_C_XL_HM_MODE_Get(reg)                         (((reg) & LSM6DSO32_CTRL6_C_XL_HM_MODE_Msk) >> LSM6DSO32_CTRL6_C_XL_HM_MODE_Pos)  /* Assignment of XL_HM_MODE value in LSM6DSO32_CTRL6_C register */
#define LSM6DSO32_CTRL6_C_LVL2_EN_Pos                                 (5)  /* (LSM6DSO32_CTRL6_C) DEN level-sensitive latched enable position */
#define LSM6DSO32_CTRL6_C_LVL2_EN_Msk                                 ((0x1) << LSM6DSO32_CTRL6_C_LVL2_EN_Pos)  /* (LSM6DSO32_CTRL6_C) DEN level-sensitive latched enable mask */
#define LSM6DSO32_CTRL6_C_LVL2_EN(value)                              (LSM6DSO32_CTRL6_C_LVL2_EN_Msk & ((value) << LSM6DSO32_CTRL6_C_LVL2_EN_Pos))  /* Assignment of LVL2_EN value in LSM6DSO32_CTRL6_C register */
#define LSM6DSO32_CTRL6_C_LVL2_EN_Get(reg)                            (((reg) & LSM6DSO32_CTRL6_C_LVL2_EN_Msk) >> LSM6DSO32_CTRL6_C_LVL2_EN_Pos)  /* Assignment of LVL2_EN value in LSM6DSO32_CTRL6_C register */
#define   LSM6DSO32_CTRL6_C_LVL2_EN_L_TRIG_Val                        (2)  /* (LSM6DSO32_CTRL6_C) Level-sensitive trigger mode is selected value */
#define   LSM6DSO32_CTRL6_C_LVL2_EN_L_LATCH_Val                       (3)  /* (LSM6DSO32_CTRL6_C) Level-sensitive latched mode is selected value */
#define   LSM6DSO32_CTRL6_C_LVL2_EN_EDGE_Val                          (4)  /* (LSM6DSO32_CTRL6_C) Edge-sensitive trigger mode is selected value */
#define   LSM6DSO32_CTRL6_C_LVL2_EN_L_FIFO_Val                        (6)  /* (LSM6DSO32_CTRL6_C) Level-sensitive FIFO enable mode is selected value */
#define LSM6DSO32_CTRL6_C_LVL2_EN_L_TRIG                              (LSM6DSO32_CTRL6_C_LVL2_EN_L_TRIG_Val << LSM6DSO32_CTRL6_C_LVL2_EN_Pos)  /* (LSM6DSO32_CTRL6_C) Level-sensitive trigger mode is selected */
#define LSM6DSO32_CTRL6_C_LVL2_EN_L_LATCH                             (LSM6DSO32_CTRL6_C_LVL2_EN_L_LATCH_Val << LSM6DSO32_CTRL6_C_LVL2_EN_Pos)  /* (LSM6DSO32_CTRL6_C) Level-sensitive latched mode is selected */
#define LSM6DSO32_CTRL6_C_LVL2_EN_EDGE                                (LSM6DSO32_CTRL6_C_LVL2_EN_EDGE_Val << LSM6DSO32_CTRL6_C_LVL2_EN_Pos)  /* (LSM6DSO32_CTRL6_C) Edge-sensitive trigger mode is selected */
#define LSM6DSO32_CTRL6_C_LVL2_EN_L_FIFO                              (LSM6DSO32_CTRL6_C_LVL2_EN_L_FIFO_Val << LSM6DSO32_CTRL6_C_LVL2_EN_Pos)  /* (LSM6DSO32_CTRL6_C) Level-sensitive FIFO enable mode is selected */
#define LSM6DSO32_CTRL6_C_LVL1_EN_Pos                                 (6)  /* (LSM6DSO32_CTRL6_C) DEN level-sensitive latched enable position */
#define LSM6DSO32_CTRL6_C_LVL1_EN_Msk                                 ((0x1) << LSM6DSO32_CTRL6_C_LVL1_EN_Pos)  /* (LSM6DSO32_CTRL6_C) DEN level-sensitive latched enable mask */
#define LSM6DSO32_CTRL6_C_LVL1_EN(value)                              (LSM6DSO32_CTRL6_C_LVL1_EN_Msk & ((value) << LSM6DSO32_CTRL6_C_LVL1_EN_Pos))  /* Assignment of LVL1_EN value in LSM6DSO32_CTRL6_C register */
#define LSM6DSO32_CTRL6_C_LVL1_EN_Get(reg)                            (((reg) & LSM6DSO32_CTRL6_C_LVL1_EN_Msk) >> LSM6DSO32_CTRL6_C_LVL1_EN_Pos)  /* Assignment of LVL1_EN value in LSM6DSO32_CTRL6_C register */
#define   LSM6DSO32_CTRL6_C_LVL1_EN_L_TRIG_Val                        (2)  /* (LSM6DSO32_CTRL6_C) Level-sensitive trigger mode is selected value */
#define   LSM6DSO32_CTRL6_C_LVL1_EN_L_LATCH_Val                       (3)  /* (LSM6DSO32_CTRL6_C) Level-sensitive latched mode is selected value */
#define   LSM6DSO32_CTRL6_C_LVL1_EN_EDGE_Val                          (4)  /* (LSM6DSO32_CTRL6_C) Edge-sensitive trigger mode is selected value */
#define   LSM6DSO32_CTRL6_C_LVL1_EN_L_FIFO_Val                        (6)  /* (LSM6DSO32_CTRL6_C) Level-sensitive FIFO enable mode is selected value */
#define LSM6DSO32_CTRL6_C_LVL1_EN_L_TRIG                              (LSM6DSO32_CTRL6_C_LVL1_EN_L_TRIG_Val << LSM6DSO32_CTRL6_C_LVL1_EN_Pos)  /* (LSM6DSO32_CTRL6_C) Level-sensitive trigger mode is selected */
#define LSM6DSO32_CTRL6_C_LVL1_EN_L_LATCH                             (LSM6DSO32_CTRL6_C_LVL1_EN_L_LATCH_Val << LSM6DSO32_CTRL6_C_LVL1_EN_Pos)  /* (LSM6DSO32_CTRL6_C) Level-sensitive latched mode is selected */
#define LSM6DSO32_CTRL6_C_LVL1_EN_EDGE                                (LSM6DSO32_CTRL6_C_LVL1_EN_EDGE_Val << LSM6DSO32_CTRL6_C_LVL1_EN_Pos)  /* (LSM6DSO32_CTRL6_C) Edge-sensitive trigger mode is selected */
#define LSM6DSO32_CTRL6_C_LVL1_EN_L_FIFO                              (LSM6DSO32_CTRL6_C_LVL1_EN_L_FIFO_Val << LSM6DSO32_CTRL6_C_LVL1_EN_Pos)  /* (LSM6DSO32_CTRL6_C) Level-sensitive FIFO enable mode is selected */
#define LSM6DSO32_CTRL6_C_TRIG_EN_Pos                                 (7)  /* (LSM6DSO32_CTRL6_C) DEN data edge-sensitive trigger enable position */
#define LSM6DSO32_CTRL6_C_TRIG_EN_Msk                                 ((0x1) << LSM6DSO32_CTRL6_C_TRIG_EN_Pos)  /* (LSM6DSO32_CTRL6_C) DEN data edge-sensitive trigger enable mask */
#define LSM6DSO32_CTRL6_C_TRIG_EN(value)                              (LSM6DSO32_CTRL6_C_TRIG_EN_Msk & ((value) << LSM6DSO32_CTRL6_C_TRIG_EN_Pos))  /* Assignment of TRIG_EN value in LSM6DSO32_CTRL6_C register */
#define LSM6DSO32_CTRL6_C_TRIG_EN_Get(reg)                            (((reg) & LSM6DSO32_CTRL6_C_TRIG_EN_Msk) >> LSM6DSO32_CTRL6_C_TRIG_EN_Pos)  /* Assignment of TRIG_EN value in LSM6DSO32_CTRL6_C register */
#define   LSM6DSO32_CTRL6_C_TRIG_EN_L_TRIG_Val                        (2)  /* (LSM6DSO32_CTRL6_C) Level-sensitive trigger mode is selected value */
#define   LSM6DSO32_CTRL6_C_TRIG_EN_L_LATCH_Val                       (3)  /* (LSM6DSO32_CTRL6_C) Level-sensitive latched mode is selected value */
#define   LSM6DSO32_CTRL6_C_TRIG_EN_EDGE_Val                          (4)  /* (LSM6DSO32_CTRL6_C) Edge-sensitive trigger mode is selected value */
#define   LSM6DSO32_CTRL6_C_TRIG_EN_L_FIFO_Val                        (6)  /* (LSM6DSO32_CTRL6_C) Level-sensitive FIFO enable mode is selected value */
#define LSM6DSO32_CTRL6_C_TRIG_EN_L_TRIG                              (LSM6DSO32_CTRL6_C_TRIG_EN_L_TRIG_Val << LSM6DSO32_CTRL6_C_TRIG_EN_Pos)  /* (LSM6DSO32_CTRL6_C) Level-sensitive trigger mode is selected */
#define LSM6DSO32_CTRL6_C_TRIG_EN_L_LATCH                             (LSM6DSO32_CTRL6_C_TRIG_EN_L_LATCH_Val << LSM6DSO32_CTRL6_C_TRIG_EN_Pos)  /* (LSM6DSO32_CTRL6_C) Level-sensitive latched mode is selected */
#define LSM6DSO32_CTRL6_C_TRIG_EN_EDGE                                (LSM6DSO32_CTRL6_C_TRIG_EN_EDGE_Val << LSM6DSO32_CTRL6_C_TRIG_EN_Pos)  /* (LSM6DSO32_CTRL6_C) Edge-sensitive trigger mode is selected */
#define LSM6DSO32_CTRL6_C_TRIG_EN_L_FIFO                              (LSM6DSO32_CTRL6_C_TRIG_EN_L_FIFO_Val << LSM6DSO32_CTRL6_C_TRIG_EN_Pos)  /* (LSM6DSO32_CTRL6_C) Level-sensitive FIFO enable mode is selected */
#define LSM6DSO32_CTRL6_C_Msk                                         (0xff)  /* (LSM6DSO32_CTRL6_C) Register mask */


/* LSM6DSO32_CTRL7_C, R/W: Control register 7 */
#define LSM6DSO32_CTRL7_C_ADDR                                        (0x16)  /* (LSM6DSO32_CTRL7_C) Control register 7 address */

#define LSM6DSO32_CTRL7_C_USR_OFF_ON_OUT_Pos                          (1)  /* (LSM6DSO32_CTRL7_C) Enables accelerometer user offset correction block; it's valid for the low-pass path position */
#define LSM6DSO32_CTRL7_C_USR_OFF_ON_OUT_Msk                          ((0x1) << LSM6DSO32_CTRL7_C_USR_OFF_ON_OUT_Pos)  /* (LSM6DSO32_CTRL7_C) Enables accelerometer user offset correction block; it's valid for the low-pass path mask */
#define LSM6DSO32_CTRL7_C_USR_OFF_ON_OUT(value)                       (LSM6DSO32_CTRL7_C_USR_OFF_ON_OUT_Msk & ((value) << LSM6DSO32_CTRL7_C_USR_OFF_ON_OUT_Pos))  /* Assignment of USR_OFF_ON_OUT value in LSM6DSO32_CTRL7_C register */
#define LSM6DSO32_CTRL7_C_USR_OFF_ON_OUT_Get(reg)                     (((reg) & LSM6DSO32_CTRL7_C_USR_OFF_ON_OUT_Msk) >> LSM6DSO32_CTRL7_C_USR_OFF_ON_OUT_Pos)  /* Assignment of USR_OFF_ON_OUT value in LSM6DSO32_CTRL7_C register */
#define LSM6DSO32_CTRL7_C_HPM_G_Pos                                   (4)  /* (LSM6DSO32_CTRL7_C) Gyroscope digital HP filter cutoff selection position */
#define LSM6DSO32_CTRL7_C_HPM_G_Msk                                   ((0x3) << LSM6DSO32_CTRL7_C_HPM_G_Pos)  /* (LSM6DSO32_CTRL7_C) Gyroscope digital HP filter cutoff selection mask */
#define LSM6DSO32_CTRL7_C_HPM_G(value)                                (LSM6DSO32_CTRL7_C_HPM_G_Msk & ((value) << LSM6DSO32_CTRL7_C_HPM_G_Pos))  /* Assignment of HPM_G value in LSM6DSO32_CTRL7_C register */
#define LSM6DSO32_CTRL7_C_HPM_G_Get(reg)                              (((reg) & LSM6DSO32_CTRL7_C_HPM_G_Msk) >> LSM6DSO32_CTRL7_C_HPM_G_Pos)  /* Assignment of HPM_G value in LSM6DSO32_CTRL7_C register */
#define   LSM6DSO32_CTRL7_C_HPM_G_16mHz_Val                           (0)  /* (LSM6DSO32_CTRL7_C) 16mHz value */
#define   LSM6DSO32_CTRL7_C_HPM_G_65mHz_Val                           (1)  /* (LSM6DSO32_CTRL7_C) 65mHz value */
#define   LSM6DSO32_CTRL7_C_HPM_G_260mHz_Val                          (2)  /* (LSM6DSO32_CTRL7_C) 260mHz value */
#define   LSM6DSO32_CTRL7_C_HPM_G_1Hz_Val                             (3)  /* (LSM6DSO32_CTRL7_C) 1.04Hz value */
#define LSM6DSO32_CTRL7_C_HPM_G_16mHz                                 (LSM6DSO32_CTRL7_C_HPM_G_16mHz_Val << LSM6DSO32_CTRL7_C_HPM_G_Pos)  /* (LSM6DSO32_CTRL7_C) 16mHz */
#define LSM6DSO32_CTRL7_C_HPM_G_65mHz                                 (LSM6DSO32_CTRL7_C_HPM_G_65mHz_Val << LSM6DSO32_CTRL7_C_HPM_G_Pos)  /* (LSM6DSO32_CTRL7_C) 65mHz */
#define LSM6DSO32_CTRL7_C_HPM_G_260mHz                                (LSM6DSO32_CTRL7_C_HPM_G_260mHz_Val << LSM6DSO32_CTRL7_C_HPM_G_Pos)  /* (LSM6DSO32_CTRL7_C) 260mHz */
#define LSM6DSO32_CTRL7_C_HPM_G_1Hz                                   (LSM6DSO32_CTRL7_C_HPM_G_1Hz_Val << LSM6DSO32_CTRL7_C_HPM_G_Pos)  /* (LSM6DSO32_CTRL7_C) 1.04Hz */
#define LSM6DSO32_CTRL7_C_HP_EN_G_Pos                                 (6)  /* (LSM6DSO32_CTRL7_C) Enables gyroscope digital high-pass filter. The filter is enabled only if the gyro is in HP mode position */
#define LSM6DSO32_CTRL7_C_HP_EN_G_Msk                                 ((0x1) << LSM6DSO32_CTRL7_C_HP_EN_G_Pos)  /* (LSM6DSO32_CTRL7_C) Enables gyroscope digital high-pass filter. The filter is enabled only if the gyro is in HP mode mask */
#define LSM6DSO32_CTRL7_C_HP_EN_G(value)                              (LSM6DSO32_CTRL7_C_HP_EN_G_Msk & ((value) << LSM6DSO32_CTRL7_C_HP_EN_G_Pos))  /* Assignment of HP_EN_G value in LSM6DSO32_CTRL7_C register */
#define LSM6DSO32_CTRL7_C_HP_EN_G_Get(reg)                            (((reg) & LSM6DSO32_CTRL7_C_HP_EN_G_Msk) >> LSM6DSO32_CTRL7_C_HP_EN_G_Pos)  /* Assignment of HP_EN_G value in LSM6DSO32_CTRL7_C register */
#define LSM6DSO32_CTRL7_C_G_HM_MODE_Pos                               (7)  /* (LSM6DSO32_CTRL7_C) Disables high-performance operating mode for gyroscope position */
#define LSM6DSO32_CTRL7_C_G_HM_MODE_Msk                               ((0x1) << LSM6DSO32_CTRL7_C_G_HM_MODE_Pos)  /* (LSM6DSO32_CTRL7_C) Disables high-performance operating mode for gyroscope mask */
#define LSM6DSO32_CTRL7_C_G_HM_MODE(value)                            (LSM6DSO32_CTRL7_C_G_HM_MODE_Msk & ((value) << LSM6DSO32_CTRL7_C_G_HM_MODE_Pos))  /* Assignment of G_HM_MODE value in LSM6DSO32_CTRL7_C register */
#define LSM6DSO32_CTRL7_C_G_HM_MODE_Get(reg)                          (((reg) & LSM6DSO32_CTRL7_C_G_HM_MODE_Msk) >> LSM6DSO32_CTRL7_C_G_HM_MODE_Pos)  /* Assignment of G_HM_MODE value in LSM6DSO32_CTRL7_C register */
#define LSM6DSO32_CTRL7_C_Msk                                         (0xf2)  /* (LSM6DSO32_CTRL7_C) Register mask */


/* LSM6DSO32_CTRL8_C, R/W: Control register 8 */
#define LSM6DSO32_CTRL8_C_ADDR                                        (0x17)  /* (LSM6DSO32_CTRL8_C) Control register 8 address */

#define LSM6DSO32_CTRL8_C_LOW_PASS_ON_6D_Pos                          (0)  /* (LSM6DSO32_CTRL8_C) LPF2 on 6D function selection position */
#define LSM6DSO32_CTRL8_C_LOW_PASS_ON_6D_Msk                          ((0x1) << LSM6DSO32_CTRL8_C_LOW_PASS_ON_6D_Pos)  /* (LSM6DSO32_CTRL8_C) LPF2 on 6D function selection mask */
#define LSM6DSO32_CTRL8_C_LOW_PASS_ON_6D(value)                       (LSM6DSO32_CTRL8_C_LOW_PASS_ON_6D_Msk & ((value) << LSM6DSO32_CTRL8_C_LOW_PASS_ON_6D_Pos))  /* Assignment of LOW_PASS_ON_6D value in LSM6DSO32_CTRL8_C register */
#define LSM6DSO32_CTRL8_C_LOW_PASS_ON_6D_Get(reg)                     (((reg) & LSM6DSO32_CTRL8_C_LOW_PASS_ON_6D_Msk) >> LSM6DSO32_CTRL8_C_LOW_PASS_ON_6D_Pos)  /* Assignment of LOW_PASS_ON_6D value in LSM6DSO32_CTRL8_C register */
#define LSM6DSO32_CTRL8_C_HP_SLOPE_XL_EN_Pos                          (2)  /* (LSM6DSO32_CTRL8_C) Accelerometer slope filter / high-pass filter selection position */
#define LSM6DSO32_CTRL8_C_HP_SLOPE_XL_EN_Msk                          ((0x1) << LSM6DSO32_CTRL8_C_HP_SLOPE_XL_EN_Pos)  /* (LSM6DSO32_CTRL8_C) Accelerometer slope filter / high-pass filter selection mask */
#define LSM6DSO32_CTRL8_C_HP_SLOPE_XL_EN(value)                       (LSM6DSO32_CTRL8_C_HP_SLOPE_XL_EN_Msk & ((value) << LSM6DSO32_CTRL8_C_HP_SLOPE_XL_EN_Pos))  /* Assignment of HP_SLOPE_XL_EN value in LSM6DSO32_CTRL8_C register */
#define LSM6DSO32_CTRL8_C_HP_SLOPE_XL_EN_Get(reg)                     (((reg) & LSM6DSO32_CTRL8_C_HP_SLOPE_XL_EN_Msk) >> LSM6DSO32_CTRL8_C_HP_SLOPE_XL_EN_Pos)  /* Assignment of HP_SLOPE_XL_EN value in LSM6DSO32_CTRL8_C register */
#define LSM6DSO32_CTRL8_C_FASTSETTL_MODE_XL_Pos                       (3)  /* (LSM6DSO32_CTRL8_C) Enables accelerometer LPF2 and HPF fast-settling mode. The filter sets the second samples after writing this bit. Active only during device exit from power- down mode position */
#define LSM6DSO32_CTRL8_C_FASTSETTL_MODE_XL_Msk                       ((0x1) << LSM6DSO32_CTRL8_C_FASTSETTL_MODE_XL_Pos)  /* (LSM6DSO32_CTRL8_C) Enables accelerometer LPF2 and HPF fast-settling mode. The filter sets the second samples after writing this bit. Active only during device exit from power- down mode mask */
#define LSM6DSO32_CTRL8_C_FASTSETTL_MODE_XL(value)                    (LSM6DSO32_CTRL8_C_FASTSETTL_MODE_XL_Msk & ((value) << LSM6DSO32_CTRL8_C_FASTSETTL_MODE_XL_Pos))  /* Assignment of FASTSETTL_MODE_XL value in LSM6DSO32_CTRL8_C register */
#define LSM6DSO32_CTRL8_C_FASTSETTL_MODE_XL_Get(reg)                  (((reg) & LSM6DSO32_CTRL8_C_FASTSETTL_MODE_XL_Msk) >> LSM6DSO32_CTRL8_C_FASTSETTL_MODE_XL_Pos)  /* Assignment of FASTSETTL_MODE_XL value in LSM6DSO32_CTRL8_C register */
#define LSM6DSO32_CTRL8_C_HP_REF_MODE_XL_Pos                          (4)  /* (LSM6DSO32_CTRL8_C) Enables accelerometer high-pass filter reference mode (valid for high-pass path - HP_SLOPE_XL_EN bit must be ‘1’) position */
#define LSM6DSO32_CTRL8_C_HP_REF_MODE_XL_Msk                          ((0x1) << LSM6DSO32_CTRL8_C_HP_REF_MODE_XL_Pos)  /* (LSM6DSO32_CTRL8_C) Enables accelerometer high-pass filter reference mode (valid for high-pass path - HP_SLOPE_XL_EN bit must be ‘1’) mask */
#define LSM6DSO32_CTRL8_C_HP_REF_MODE_XL(value)                       (LSM6DSO32_CTRL8_C_HP_REF_MODE_XL_Msk & ((value) << LSM6DSO32_CTRL8_C_HP_REF_MODE_XL_Pos))  /* Assignment of HP_REF_MODE_XL value in LSM6DSO32_CTRL8_C register */
#define LSM6DSO32_CTRL8_C_HP_REF_MODE_XL_Get(reg)                     (((reg) & LSM6DSO32_CTRL8_C_HP_REF_MODE_XL_Msk) >> LSM6DSO32_CTRL8_C_HP_REF_MODE_XL_Pos)  /* Assignment of HP_REF_MODE_XL value in LSM6DSO32_CTRL8_C register */
#define LSM6DSO32_CTRL8_C_HPCF_XL_Pos                                 (5)  /* (LSM6DSO32_CTRL8_C) Accelerometer LPF2 and HP filter configuration and cutoff setting position */
#define LSM6DSO32_CTRL8_C_HPCF_XL_Msk                                 ((0x7) << LSM6DSO32_CTRL8_C_HPCF_XL_Pos)  /* (LSM6DSO32_CTRL8_C) Accelerometer LPF2 and HP filter configuration and cutoff setting mask */
#define LSM6DSO32_CTRL8_C_HPCF_XL(value)                              (LSM6DSO32_CTRL8_C_HPCF_XL_Msk & ((value) << LSM6DSO32_CTRL8_C_HPCF_XL_Pos))  /* Assignment of HPCF_XL value in LSM6DSO32_CTRL8_C register */
#define LSM6DSO32_CTRL8_C_HPCF_XL_Get(reg)                            (((reg) & LSM6DSO32_CTRL8_C_HPCF_XL_Msk) >> LSM6DSO32_CTRL8_C_HPCF_XL_Pos)  /* Assignment of HPCF_XL value in LSM6DSO32_CTRL8_C register */
#define LSM6DSO32_CTRL8_C_Msk                                         (0xfd)  /* (LSM6DSO32_CTRL8_C) Register mask */


/* LSM6DSO32_CTRL9_XL, R/W: Control register 9 */
#define LSM6DSO32_CTRL9_XL_ADDR                                       (0x18)  /* (LSM6DSO32_CTRL9_XL) Control register 9 address */

#define LSM6DSO32_CTRL9_XL_I3C_DISABLE_Pos                            (1)  /* (LSM6DSO32_CTRL9_XL) Disables MIPI I3CSM communication protocol position */
#define LSM6DSO32_CTRL9_XL_I3C_DISABLE_Msk                            ((0x1) << LSM6DSO32_CTRL9_XL_I3C_DISABLE_Pos)  /* (LSM6DSO32_CTRL9_XL) Disables MIPI I3CSM communication protocol mask */
#define LSM6DSO32_CTRL9_XL_I3C_DISABLE(value)                         (LSM6DSO32_CTRL9_XL_I3C_DISABLE_Msk & ((value) << LSM6DSO32_CTRL9_XL_I3C_DISABLE_Pos))  /* Assignment of I3C_DISABLE value in LSM6DSO32_CTRL9_XL register */
#define LSM6DSO32_CTRL9_XL_I3C_DISABLE_Get(reg)                       (((reg) & LSM6DSO32_CTRL9_XL_I3C_DISABLE_Msk) >> LSM6DSO32_CTRL9_XL_I3C_DISABLE_Pos)  /* Assignment of I3C_DISABLE value in LSM6DSO32_CTRL9_XL register */
#define LSM6DSO32_CTRL9_XL_DEN_LH_Pos                                 (2)  /* (LSM6DSO32_CTRL9_XL) DEN active level configuration position */
#define LSM6DSO32_CTRL9_XL_DEN_LH_Msk                                 ((0x1) << LSM6DSO32_CTRL9_XL_DEN_LH_Pos)  /* (LSM6DSO32_CTRL9_XL) DEN active level configuration mask */
#define LSM6DSO32_CTRL9_XL_DEN_LH(value)                              (LSM6DSO32_CTRL9_XL_DEN_LH_Msk & ((value) << LSM6DSO32_CTRL9_XL_DEN_LH_Pos))  /* Assignment of DEN_LH value in LSM6DSO32_CTRL9_XL register */
#define LSM6DSO32_CTRL9_XL_DEN_LH_Get(reg)                            (((reg) & LSM6DSO32_CTRL9_XL_DEN_LH_Msk) >> LSM6DSO32_CTRL9_XL_DEN_LH_Pos)  /* Assignment of DEN_LH value in LSM6DSO32_CTRL9_XL register */
#define   LSM6DSO32_CTRL9_XL_DEN_LH_L_Val                             (0)  /* (LSM6DSO32_CTRL9_XL) Active low value */
#define   LSM6DSO32_CTRL9_XL_DEN_LH_H_Val                             (1)  /* (LSM6DSO32_CTRL9_XL) Active high value */
#define LSM6DSO32_CTRL9_XL_DEN_LH_L                                   (LSM6DSO32_CTRL9_XL_DEN_LH_L_Val << LSM6DSO32_CTRL9_XL_DEN_LH_Pos)  /* (LSM6DSO32_CTRL9_XL) Active low */
#define LSM6DSO32_CTRL9_XL_DEN_LH_H                                   (LSM6DSO32_CTRL9_XL_DEN_LH_H_Val << LSM6DSO32_CTRL9_XL_DEN_LH_Pos)  /* (LSM6DSO32_CTRL9_XL) Active high */
#define LSM6DSO32_CTRL9_XL_DEN_XL_EN_Pos                              (3)  /* (LSM6DSO32_CTRL9_XL) Extends DEN functionality to accelerometer sensor position */
#define LSM6DSO32_CTRL9_XL_DEN_XL_EN_Msk                              ((0x1) << LSM6DSO32_CTRL9_XL_DEN_XL_EN_Pos)  /* (LSM6DSO32_CTRL9_XL) Extends DEN functionality to accelerometer sensor mask */
#define LSM6DSO32_CTRL9_XL_DEN_XL_EN(value)                           (LSM6DSO32_CTRL9_XL_DEN_XL_EN_Msk & ((value) << LSM6DSO32_CTRL9_XL_DEN_XL_EN_Pos))  /* Assignment of DEN_XL_EN value in LSM6DSO32_CTRL9_XL register */
#define LSM6DSO32_CTRL9_XL_DEN_XL_EN_Get(reg)                         (((reg) & LSM6DSO32_CTRL9_XL_DEN_XL_EN_Msk) >> LSM6DSO32_CTRL9_XL_DEN_XL_EN_Pos)  /* Assignment of DEN_XL_EN value in LSM6DSO32_CTRL9_XL register */
#define LSM6DSO32_CTRL9_XL_DEN_XL_G_Pos                               (4)  /* (LSM6DSO32_CTRL9_XL) DEN stamping sensor selection position */
#define LSM6DSO32_CTRL9_XL_DEN_XL_G_Msk                               ((0x1) << LSM6DSO32_CTRL9_XL_DEN_XL_G_Pos)  /* (LSM6DSO32_CTRL9_XL) DEN stamping sensor selection mask */
#define LSM6DSO32_CTRL9_XL_DEN_XL_G(value)                            (LSM6DSO32_CTRL9_XL_DEN_XL_G_Msk & ((value) << LSM6DSO32_CTRL9_XL_DEN_XL_G_Pos))  /* Assignment of DEN_XL_G value in LSM6DSO32_CTRL9_XL register */
#define LSM6DSO32_CTRL9_XL_DEN_XL_G_Get(reg)                          (((reg) & LSM6DSO32_CTRL9_XL_DEN_XL_G_Msk) >> LSM6DSO32_CTRL9_XL_DEN_XL_G_Pos)  /* Assignment of DEN_XL_G value in LSM6DSO32_CTRL9_XL register */
#define   LSM6DSO32_CTRL9_XL_DEN_XL_G_G_Val                           (0)  /* (LSM6DSO32_CTRL9_XL) DEN pin info stamped in the gyroscope axis selected by bits [7:5] value */
#define   LSM6DSO32_CTRL9_XL_DEN_XL_G_XL_Val                          (1)  /* (LSM6DSO32_CTRL9_XL) DEN pin info stamped in the accelerometer axis selected by bits [7:5] value */
#define LSM6DSO32_CTRL9_XL_DEN_XL_G_G                                 (LSM6DSO32_CTRL9_XL_DEN_XL_G_G_Val << LSM6DSO32_CTRL9_XL_DEN_XL_G_Pos)  /* (LSM6DSO32_CTRL9_XL) DEN pin info stamped in the gyroscope axis selected by bits [7:5] */
#define LSM6DSO32_CTRL9_XL_DEN_XL_G_XL                                (LSM6DSO32_CTRL9_XL_DEN_XL_G_XL_Val << LSM6DSO32_CTRL9_XL_DEN_XL_G_Pos)  /* (LSM6DSO32_CTRL9_XL) DEN pin info stamped in the accelerometer axis selected by bits [7:5] */
#define LSM6DSO32_CTRL9_XL_DEN_Z_Pos                                  (5)  /* (LSM6DSO32_CTRL9_XL) DEN value stored in LSB of Z-axis position */
#define LSM6DSO32_CTRL9_XL_DEN_Z_Msk                                  ((0x1) << LSM6DSO32_CTRL9_XL_DEN_Z_Pos)  /* (LSM6DSO32_CTRL9_XL) DEN value stored in LSB of Z-axis mask */
#define LSM6DSO32_CTRL9_XL_DEN_Z(value)                               (LSM6DSO32_CTRL9_XL_DEN_Z_Msk & ((value) << LSM6DSO32_CTRL9_XL_DEN_Z_Pos))  /* Assignment of DEN_Z value in LSM6DSO32_CTRL9_XL register */
#define LSM6DSO32_CTRL9_XL_DEN_Z_Get(reg)                             (((reg) & LSM6DSO32_CTRL9_XL_DEN_Z_Msk) >> LSM6DSO32_CTRL9_XL_DEN_Z_Pos)  /* Assignment of DEN_Z value in LSM6DSO32_CTRL9_XL register */
#define LSM6DSO32_CTRL9_XL_DEN_Y_Pos                                  (6)  /* (LSM6DSO32_CTRL9_XL) DEN value stored in LSB of Y-axis position */
#define LSM6DSO32_CTRL9_XL_DEN_Y_Msk                                  ((0x1) << LSM6DSO32_CTRL9_XL_DEN_Y_Pos)  /* (LSM6DSO32_CTRL9_XL) DEN value stored in LSB of Y-axis mask */
#define LSM6DSO32_CTRL9_XL_DEN_Y(value)                               (LSM6DSO32_CTRL9_XL_DEN_Y_Msk & ((value) << LSM6DSO32_CTRL9_XL_DEN_Y_Pos))  /* Assignment of DEN_Y value in LSM6DSO32_CTRL9_XL register */
#define LSM6DSO32_CTRL9_XL_DEN_Y_Get(reg)                             (((reg) & LSM6DSO32_CTRL9_XL_DEN_Y_Msk) >> LSM6DSO32_CTRL9_XL_DEN_Y_Pos)  /* Assignment of DEN_Y value in LSM6DSO32_CTRL9_XL register */
#define LSM6DSO32_CTRL9_XL_DEN_X_Pos                                  (7)  /* (LSM6DSO32_CTRL9_XL) DEN value stored in LSB of X-axis position */
#define LSM6DSO32_CTRL9_XL_DEN_X_Msk                                  ((0x1) << LSM6DSO32_CTRL9_XL_DEN_X_Pos)  /* (LSM6DSO32_CTRL9_XL) DEN value stored in LSB of X-axis mask */
#define LSM6DSO32_CTRL9_XL_DEN_X(value)                               (LSM6DSO32_CTRL9_XL_DEN_X_Msk & ((value) << LSM6DSO32_CTRL9_XL_DEN_X_Pos))  /* Assignment of DEN_X value in LSM6DSO32_CTRL9_XL register */
#define LSM6DSO32_CTRL9_XL_DEN_X_Get(reg)                             (((reg) & LSM6DSO32_CTRL9_XL_DEN_X_Msk) >> LSM6DSO32_CTRL9_XL_DEN_X_Pos)  /* Assignment of DEN_X value in LSM6DSO32_CTRL9_XL register */
#define LSM6DSO32_CTRL9_XL_Msk                                        (0xfe)  /* (LSM6DSO32_CTRL9_XL) Register mask */


/* LSM6DSO32_CTRL10_C, R/W: Control register 10 */
#define LSM6DSO32_CTRL10_C_ADDR                                       (0x19)  /* (LSM6DSO32_CTRL10_C) Control register 10 address */

#define LSM6DSO32_CTRL10_C_TIMESTAMP_EN_Pos                           (5)  /* (LSM6DSO32_CTRL10_C) Enables timestamp counter position */
#define LSM6DSO32_CTRL10_C_TIMESTAMP_EN_Msk                           ((0x1) << LSM6DSO32_CTRL10_C_TIMESTAMP_EN_Pos)  /* (LSM6DSO32_CTRL10_C) Enables timestamp counter mask */
#define LSM6DSO32_CTRL10_C_TIMESTAMP_EN(value)                        (LSM6DSO32_CTRL10_C_TIMESTAMP_EN_Msk & ((value) << LSM6DSO32_CTRL10_C_TIMESTAMP_EN_Pos))  /* Assignment of TIMESTAMP_EN value in LSM6DSO32_CTRL10_C register */
#define LSM6DSO32_CTRL10_C_TIMESTAMP_EN_Get(reg)                      (((reg) & LSM6DSO32_CTRL10_C_TIMESTAMP_EN_Msk) >> LSM6DSO32_CTRL10_C_TIMESTAMP_EN_Pos)  /* Assignment of TIMESTAMP_EN value in LSM6DSO32_CTRL10_C register */
#define LSM6DSO32_CTRL10_C_Msk                                        (0x20)  /* (LSM6DSO32_CTRL10_C) Register mask */


/* LSM6DSO32_ALL_INT_SRC, R: Source register for all interrupts */
#define LSM6DSO32_ALL_INT_SRC_ADDR                                    (0x1a)  /* (LSM6DSO32_ALL_INT_SRC) Source register for all interrupts address */

#define LSM6DSO32_ALL_INT_SRC_FF_IA_Pos                               (0)  /* (LSM6DSO32_ALL_INT_SRC) Free-fall event status position */
#define LSM6DSO32_ALL_INT_SRC_FF_IA_Msk                               ((0x1) << LSM6DSO32_ALL_INT_SRC_FF_IA_Pos)  /* (LSM6DSO32_ALL_INT_SRC) Free-fall event status mask */
#define LSM6DSO32_ALL_INT_SRC_FF_IA(value)                            (LSM6DSO32_ALL_INT_SRC_FF_IA_Msk & ((value) << LSM6DSO32_ALL_INT_SRC_FF_IA_Pos))  /* Assignment of FF_IA value in LSM6DSO32_ALL_INT_SRC register */
#define LSM6DSO32_ALL_INT_SRC_FF_IA_Get(reg)                          (((reg) & LSM6DSO32_ALL_INT_SRC_FF_IA_Msk) >> LSM6DSO32_ALL_INT_SRC_FF_IA_Pos)  /* Assignment of FF_IA value in LSM6DSO32_ALL_INT_SRC register */
#define LSM6DSO32_ALL_INT_SRC_WU_IA_Pos                               (1)  /* (LSM6DSO32_ALL_INT_SRC) Wake-up event status position */
#define LSM6DSO32_ALL_INT_SRC_WU_IA_Msk                               ((0x1) << LSM6DSO32_ALL_INT_SRC_WU_IA_Pos)  /* (LSM6DSO32_ALL_INT_SRC) Wake-up event status mask */
#define LSM6DSO32_ALL_INT_SRC_WU_IA(value)                            (LSM6DSO32_ALL_INT_SRC_WU_IA_Msk & ((value) << LSM6DSO32_ALL_INT_SRC_WU_IA_Pos))  /* Assignment of WU_IA value in LSM6DSO32_ALL_INT_SRC register */
#define LSM6DSO32_ALL_INT_SRC_WU_IA_Get(reg)                          (((reg) & LSM6DSO32_ALL_INT_SRC_WU_IA_Msk) >> LSM6DSO32_ALL_INT_SRC_WU_IA_Pos)  /* Assignment of WU_IA value in LSM6DSO32_ALL_INT_SRC register */
#define LSM6DSO32_ALL_INT_SRC_SINGLE_TAP_Pos                          (2)  /* (LSM6DSO32_ALL_INT_SRC) Single-tap event status position */
#define LSM6DSO32_ALL_INT_SRC_SINGLE_TAP_Msk                          ((0x1) << LSM6DSO32_ALL_INT_SRC_SINGLE_TAP_Pos)  /* (LSM6DSO32_ALL_INT_SRC) Single-tap event status mask */
#define LSM6DSO32_ALL_INT_SRC_SINGLE_TAP(value)                       (LSM6DSO32_ALL_INT_SRC_SINGLE_TAP_Msk & ((value) << LSM6DSO32_ALL_INT_SRC_SINGLE_TAP_Pos))  /* Assignment of SINGLE_TAP value in LSM6DSO32_ALL_INT_SRC register */
#define LSM6DSO32_ALL_INT_SRC_SINGLE_TAP_Get(reg)                     (((reg) & LSM6DSO32_ALL_INT_SRC_SINGLE_TAP_Msk) >> LSM6DSO32_ALL_INT_SRC_SINGLE_TAP_Pos)  /* Assignment of SINGLE_TAP value in LSM6DSO32_ALL_INT_SRC register */
#define LSM6DSO32_ALL_INT_SRC_DOUBLE_TAP_Pos                          (3)  /* (LSM6DSO32_ALL_INT_SRC) Double-tap event status position */
#define LSM6DSO32_ALL_INT_SRC_DOUBLE_TAP_Msk                          ((0x1) << LSM6DSO32_ALL_INT_SRC_DOUBLE_TAP_Pos)  /* (LSM6DSO32_ALL_INT_SRC) Double-tap event status mask */
#define LSM6DSO32_ALL_INT_SRC_DOUBLE_TAP(value)                       (LSM6DSO32_ALL_INT_SRC_DOUBLE_TAP_Msk & ((value) << LSM6DSO32_ALL_INT_SRC_DOUBLE_TAP_Pos))  /* Assignment of DOUBLE_TAP value in LSM6DSO32_ALL_INT_SRC register */
#define LSM6DSO32_ALL_INT_SRC_DOUBLE_TAP_Get(reg)                     (((reg) & LSM6DSO32_ALL_INT_SRC_DOUBLE_TAP_Msk) >> LSM6DSO32_ALL_INT_SRC_DOUBLE_TAP_Pos)  /* Assignment of DOUBLE_TAP value in LSM6DSO32_ALL_INT_SRC register */
#define LSM6DSO32_ALL_INT_SRC_D6D_IA_Pos                              (4)  /* (LSM6DSO32_ALL_INT_SRC) Interrupt active for change in position of portrait, landscape, face-up, face-down position */
#define LSM6DSO32_ALL_INT_SRC_D6D_IA_Msk                              ((0x1) << LSM6DSO32_ALL_INT_SRC_D6D_IA_Pos)  /* (LSM6DSO32_ALL_INT_SRC) Interrupt active for change in position of portrait, landscape, face-up, face-down mask */
#define LSM6DSO32_ALL_INT_SRC_D6D_IA(value)                           (LSM6DSO32_ALL_INT_SRC_D6D_IA_Msk & ((value) << LSM6DSO32_ALL_INT_SRC_D6D_IA_Pos))  /* Assignment of D6D_IA value in LSM6DSO32_ALL_INT_SRC register */
#define LSM6DSO32_ALL_INT_SRC_D6D_IA_Get(reg)                         (((reg) & LSM6DSO32_ALL_INT_SRC_D6D_IA_Msk) >> LSM6DSO32_ALL_INT_SRC_D6D_IA_Pos)  /* Assignment of D6D_IA value in LSM6DSO32_ALL_INT_SRC register */
#define LSM6DSO32_ALL_INT_SRC_SLEEP_CHANGE_IA_Pos                     (5)  /* (LSM6DSO32_ALL_INT_SRC) Detects change event in activity/inactivity status position */
#define LSM6DSO32_ALL_INT_SRC_SLEEP_CHANGE_IA_Msk                     ((0x1) << LSM6DSO32_ALL_INT_SRC_SLEEP_CHANGE_IA_Pos)  /* (LSM6DSO32_ALL_INT_SRC) Detects change event in activity/inactivity status mask */
#define LSM6DSO32_ALL_INT_SRC_SLEEP_CHANGE_IA(value)                  (LSM6DSO32_ALL_INT_SRC_SLEEP_CHANGE_IA_Msk & ((value) << LSM6DSO32_ALL_INT_SRC_SLEEP_CHANGE_IA_Pos))  /* Assignment of SLEEP_CHANGE_IA value in LSM6DSO32_ALL_INT_SRC register */
#define LSM6DSO32_ALL_INT_SRC_SLEEP_CHANGE_IA_Get(reg)                (((reg) & LSM6DSO32_ALL_INT_SRC_SLEEP_CHANGE_IA_Msk) >> LSM6DSO32_ALL_INT_SRC_SLEEP_CHANGE_IA_Pos)  /* Assignment of SLEEP_CHANGE_IA value in LSM6DSO32_ALL_INT_SRC register */
#define LSM6DSO32_ALL_INT_SRC_TIMESTAMP_ENDCOUNT_Pos                  (7)  /* (LSM6DSO32_ALL_INT_SRC) Alerts timestamp overflow within 6.4ms position */
#define LSM6DSO32_ALL_INT_SRC_TIMESTAMP_ENDCOUNT_Msk                  ((0x1) << LSM6DSO32_ALL_INT_SRC_TIMESTAMP_ENDCOUNT_Pos)  /* (LSM6DSO32_ALL_INT_SRC) Alerts timestamp overflow within 6.4ms mask */
#define LSM6DSO32_ALL_INT_SRC_TIMESTAMP_ENDCOUNT(value)               (LSM6DSO32_ALL_INT_SRC_TIMESTAMP_ENDCOUNT_Msk & ((value) << LSM6DSO32_ALL_INT_SRC_TIMESTAMP_ENDCOUNT_Pos))  /* Assignment of TIMESTAMP_ENDCOUNT value in LSM6DSO32_ALL_INT_SRC register */
#define LSM6DSO32_ALL_INT_SRC_TIMESTAMP_ENDCOUNT_Get(reg)             (((reg) & LSM6DSO32_ALL_INT_SRC_TIMESTAMP_ENDCOUNT_Msk) >> LSM6DSO32_ALL_INT_SRC_TIMESTAMP_ENDCOUNT_Pos)  /* Assignment of TIMESTAMP_ENDCOUNT value in LSM6DSO32_ALL_INT_SRC register */
#define LSM6DSO32_ALL_INT_SRC_Msk                                     (0xbf)  /* (LSM6DSO32_ALL_INT_SRC) Register mask */


/* LSM6DSO32_WAKE_UP_SRC, R: Wake-up interrupt source register */
#define LSM6DSO32_WAKE_UP_SRC_ADDR                                    (0x1b)  /* (LSM6DSO32_WAKE_UP_SRC) Wake-up interrupt source register address */

#define LSM6DSO32_WAKE_UP_SRC_Z_WU_Pos                                (0)  /* (LSM6DSO32_WAKE_UP_SRC) Wakeup event detection status on Z-axis position */
#define LSM6DSO32_WAKE_UP_SRC_Z_WU_Msk                                ((0x1) << LSM6DSO32_WAKE_UP_SRC_Z_WU_Pos)  /* (LSM6DSO32_WAKE_UP_SRC) Wakeup event detection status on Z-axis mask */
#define LSM6DSO32_WAKE_UP_SRC_Z_WU(value)                             (LSM6DSO32_WAKE_UP_SRC_Z_WU_Msk & ((value) << LSM6DSO32_WAKE_UP_SRC_Z_WU_Pos))  /* Assignment of Z_WU value in LSM6DSO32_WAKE_UP_SRC register */
#define LSM6DSO32_WAKE_UP_SRC_Z_WU_Get(reg)                           (((reg) & LSM6DSO32_WAKE_UP_SRC_Z_WU_Msk) >> LSM6DSO32_WAKE_UP_SRC_Z_WU_Pos)  /* Assignment of Z_WU value in LSM6DSO32_WAKE_UP_SRC register */
#define LSM6DSO32_WAKE_UP_SRC_Y_WU_Pos                                (1)  /* (LSM6DSO32_WAKE_UP_SRC) Wakeup event detection status on X-axis position */
#define LSM6DSO32_WAKE_UP_SRC_Y_WU_Msk                                ((0x1) << LSM6DSO32_WAKE_UP_SRC_Y_WU_Pos)  /* (LSM6DSO32_WAKE_UP_SRC) Wakeup event detection status on X-axis mask */
#define LSM6DSO32_WAKE_UP_SRC_Y_WU(value)                             (LSM6DSO32_WAKE_UP_SRC_Y_WU_Msk & ((value) << LSM6DSO32_WAKE_UP_SRC_Y_WU_Pos))  /* Assignment of Y_WU value in LSM6DSO32_WAKE_UP_SRC register */
#define LSM6DSO32_WAKE_UP_SRC_Y_WU_Get(reg)                           (((reg) & LSM6DSO32_WAKE_UP_SRC_Y_WU_Msk) >> LSM6DSO32_WAKE_UP_SRC_Y_WU_Pos)  /* Assignment of Y_WU value in LSM6DSO32_WAKE_UP_SRC register */
#define LSM6DSO32_WAKE_UP_SRC_X_WU_Pos                                (2)  /* (LSM6DSO32_WAKE_UP_SRC) Wakeup event detection status on Y-axis position */
#define LSM6DSO32_WAKE_UP_SRC_X_WU_Msk                                ((0x1) << LSM6DSO32_WAKE_UP_SRC_X_WU_Pos)  /* (LSM6DSO32_WAKE_UP_SRC) Wakeup event detection status on Y-axis mask */
#define LSM6DSO32_WAKE_UP_SRC_X_WU(value)                             (LSM6DSO32_WAKE_UP_SRC_X_WU_Msk & ((value) << LSM6DSO32_WAKE_UP_SRC_X_WU_Pos))  /* Assignment of X_WU value in LSM6DSO32_WAKE_UP_SRC register */
#define LSM6DSO32_WAKE_UP_SRC_X_WU_Get(reg)                           (((reg) & LSM6DSO32_WAKE_UP_SRC_X_WU_Msk) >> LSM6DSO32_WAKE_UP_SRC_X_WU_Pos)  /* Assignment of X_WU value in LSM6DSO32_WAKE_UP_SRC register */
#define LSM6DSO32_WAKE_UP_SRC_WU_IA_Pos                               (3)  /* (LSM6DSO32_WAKE_UP_SRC) Wakeup event detection status position */
#define LSM6DSO32_WAKE_UP_SRC_WU_IA_Msk                               ((0x1) << LSM6DSO32_WAKE_UP_SRC_WU_IA_Pos)  /* (LSM6DSO32_WAKE_UP_SRC) Wakeup event detection status mask */
#define LSM6DSO32_WAKE_UP_SRC_WU_IA(value)                            (LSM6DSO32_WAKE_UP_SRC_WU_IA_Msk & ((value) << LSM6DSO32_WAKE_UP_SRC_WU_IA_Pos))  /* Assignment of WU_IA value in LSM6DSO32_WAKE_UP_SRC register */
#define LSM6DSO32_WAKE_UP_SRC_WU_IA_Get(reg)                          (((reg) & LSM6DSO32_WAKE_UP_SRC_WU_IA_Msk) >> LSM6DSO32_WAKE_UP_SRC_WU_IA_Pos)  /* Assignment of WU_IA value in LSM6DSO32_WAKE_UP_SRC register */
#define LSM6DSO32_WAKE_UP_SRC_SLEEP_STATE_Pos                         (4)  /* (LSM6DSO32_WAKE_UP_SRC) Sleep status bit position */
#define LSM6DSO32_WAKE_UP_SRC_SLEEP_STATE_Msk                         ((0x1) << LSM6DSO32_WAKE_UP_SRC_SLEEP_STATE_Pos)  /* (LSM6DSO32_WAKE_UP_SRC) Sleep status bit mask */
#define LSM6DSO32_WAKE_UP_SRC_SLEEP_STATE(value)                      (LSM6DSO32_WAKE_UP_SRC_SLEEP_STATE_Msk & ((value) << LSM6DSO32_WAKE_UP_SRC_SLEEP_STATE_Pos))  /* Assignment of SLEEP_STATE value in LSM6DSO32_WAKE_UP_SRC register */
#define LSM6DSO32_WAKE_UP_SRC_SLEEP_STATE_Get(reg)                    (((reg) & LSM6DSO32_WAKE_UP_SRC_SLEEP_STATE_Msk) >> LSM6DSO32_WAKE_UP_SRC_SLEEP_STATE_Pos)  /* Assignment of SLEEP_STATE value in LSM6DSO32_WAKE_UP_SRC register */
#define LSM6DSO32_WAKE_UP_SRC_FF_IA_Pos                               (5)  /* (LSM6DSO32_WAKE_UP_SRC) Free-fall event detection status position */
#define LSM6DSO32_WAKE_UP_SRC_FF_IA_Msk                               ((0x1) << LSM6DSO32_WAKE_UP_SRC_FF_IA_Pos)  /* (LSM6DSO32_WAKE_UP_SRC) Free-fall event detection status mask */
#define LSM6DSO32_WAKE_UP_SRC_FF_IA(value)                            (LSM6DSO32_WAKE_UP_SRC_FF_IA_Msk & ((value) << LSM6DSO32_WAKE_UP_SRC_FF_IA_Pos))  /* Assignment of FF_IA value in LSM6DSO32_WAKE_UP_SRC register */
#define LSM6DSO32_WAKE_UP_SRC_FF_IA_Get(reg)                          (((reg) & LSM6DSO32_WAKE_UP_SRC_FF_IA_Msk) >> LSM6DSO32_WAKE_UP_SRC_FF_IA_Pos)  /* Assignment of FF_IA value in LSM6DSO32_WAKE_UP_SRC register */
#define LSM6DSO32_WAKE_UP_SRC_SLEEP_CHANGE_IA_Pos                     (6)  /* (LSM6DSO32_WAKE_UP_SRC) Detects change event in activity/inactivity status position */
#define LSM6DSO32_WAKE_UP_SRC_SLEEP_CHANGE_IA_Msk                     ((0x1) << LSM6DSO32_WAKE_UP_SRC_SLEEP_CHANGE_IA_Pos)  /* (LSM6DSO32_WAKE_UP_SRC) Detects change event in activity/inactivity status mask */
#define LSM6DSO32_WAKE_UP_SRC_SLEEP_CHANGE_IA(value)                  (LSM6DSO32_WAKE_UP_SRC_SLEEP_CHANGE_IA_Msk & ((value) << LSM6DSO32_WAKE_UP_SRC_SLEEP_CHANGE_IA_Pos))  /* Assignment of SLEEP_CHANGE_IA value in LSM6DSO32_WAKE_UP_SRC register */
#define LSM6DSO32_WAKE_UP_SRC_SLEEP_CHANGE_IA_Get(reg)                (((reg) & LSM6DSO32_WAKE_UP_SRC_SLEEP_CHANGE_IA_Msk) >> LSM6DSO32_WAKE_UP_SRC_SLEEP_CHANGE_IA_Pos)  /* Assignment of SLEEP_CHANGE_IA value in LSM6DSO32_WAKE_UP_SRC register */
#define LSM6DSO32_WAKE_UP_SRC_Msk                                     (0x7f)  /* (LSM6DSO32_WAKE_UP_SRC) Register mask */


/* LSM6DSO32_TAP_SRC, R: Tap source register */
#define LSM6DSO32_TAP_SRC_ADDR                                        (0x1c)  /* (LSM6DSO32_TAP_SRC) Tap source register address */

#define LSM6DSO32_TAP_SRC_Z_TAP_Pos                                   (0)  /* (LSM6DSO32_TAP_SRC) Tap event detection status on Z-axis position */
#define LSM6DSO32_TAP_SRC_Z_TAP_Msk                                   ((0x1) << LSM6DSO32_TAP_SRC_Z_TAP_Pos)  /* (LSM6DSO32_TAP_SRC) Tap event detection status on Z-axis mask */
#define LSM6DSO32_TAP_SRC_Z_TAP(value)                                (LSM6DSO32_TAP_SRC_Z_TAP_Msk & ((value) << LSM6DSO32_TAP_SRC_Z_TAP_Pos))  /* Assignment of Z_TAP value in LSM6DSO32_TAP_SRC register */
#define LSM6DSO32_TAP_SRC_Z_TAP_Get(reg)                              (((reg) & LSM6DSO32_TAP_SRC_Z_TAP_Msk) >> LSM6DSO32_TAP_SRC_Z_TAP_Pos)  /* Assignment of Z_TAP value in LSM6DSO32_TAP_SRC register */
#define LSM6DSO32_TAP_SRC_Y_TAP_Pos                                   (1)  /* (LSM6DSO32_TAP_SRC) Tap event detection status on Y-axis position */
#define LSM6DSO32_TAP_SRC_Y_TAP_Msk                                   ((0x1) << LSM6DSO32_TAP_SRC_Y_TAP_Pos)  /* (LSM6DSO32_TAP_SRC) Tap event detection status on Y-axis mask */
#define LSM6DSO32_TAP_SRC_Y_TAP(value)                                (LSM6DSO32_TAP_SRC_Y_TAP_Msk & ((value) << LSM6DSO32_TAP_SRC_Y_TAP_Pos))  /* Assignment of Y_TAP value in LSM6DSO32_TAP_SRC register */
#define LSM6DSO32_TAP_SRC_Y_TAP_Get(reg)                              (((reg) & LSM6DSO32_TAP_SRC_Y_TAP_Msk) >> LSM6DSO32_TAP_SRC_Y_TAP_Pos)  /* Assignment of Y_TAP value in LSM6DSO32_TAP_SRC register */
#define LSM6DSO32_TAP_SRC_X_TAP_Pos                                   (2)  /* (LSM6DSO32_TAP_SRC) Tap event detection status on X-axis position */
#define LSM6DSO32_TAP_SRC_X_TAP_Msk                                   ((0x1) << LSM6DSO32_TAP_SRC_X_TAP_Pos)  /* (LSM6DSO32_TAP_SRC) Tap event detection status on X-axis mask */
#define LSM6DSO32_TAP_SRC_X_TAP(value)                                (LSM6DSO32_TAP_SRC_X_TAP_Msk & ((value) << LSM6DSO32_TAP_SRC_X_TAP_Pos))  /* Assignment of X_TAP value in LSM6DSO32_TAP_SRC register */
#define LSM6DSO32_TAP_SRC_X_TAP_Get(reg)                              (((reg) & LSM6DSO32_TAP_SRC_X_TAP_Msk) >> LSM6DSO32_TAP_SRC_X_TAP_Pos)  /* Assignment of X_TAP value in LSM6DSO32_TAP_SRC register */
#define LSM6DSO32_TAP_SRC_TAP_SIGN_Pos                                (3)  /* (LSM6DSO32_TAP_SRC) Sign of acceleration detected by tap event position */
#define LSM6DSO32_TAP_SRC_TAP_SIGN_Msk                                ((0x1) << LSM6DSO32_TAP_SRC_TAP_SIGN_Pos)  /* (LSM6DSO32_TAP_SRC) Sign of acceleration detected by tap event mask */
#define LSM6DSO32_TAP_SRC_TAP_SIGN(value)                             (LSM6DSO32_TAP_SRC_TAP_SIGN_Msk & ((value) << LSM6DSO32_TAP_SRC_TAP_SIGN_Pos))  /* Assignment of TAP_SIGN value in LSM6DSO32_TAP_SRC register */
#define LSM6DSO32_TAP_SRC_TAP_SIGN_Get(reg)                           (((reg) & LSM6DSO32_TAP_SRC_TAP_SIGN_Msk) >> LSM6DSO32_TAP_SRC_TAP_SIGN_Pos)  /* Assignment of TAP_SIGN value in LSM6DSO32_TAP_SRC register */
#define LSM6DSO32_TAP_SRC_DOUBLE_TAP_Pos                              (4)  /* (LSM6DSO32_TAP_SRC) Double-tap event detection status position */
#define LSM6DSO32_TAP_SRC_DOUBLE_TAP_Msk                              ((0x1) << LSM6DSO32_TAP_SRC_DOUBLE_TAP_Pos)  /* (LSM6DSO32_TAP_SRC) Double-tap event detection status mask */
#define LSM6DSO32_TAP_SRC_DOUBLE_TAP(value)                           (LSM6DSO32_TAP_SRC_DOUBLE_TAP_Msk & ((value) << LSM6DSO32_TAP_SRC_DOUBLE_TAP_Pos))  /* Assignment of DOUBLE_TAP value in LSM6DSO32_TAP_SRC register */
#define LSM6DSO32_TAP_SRC_DOUBLE_TAP_Get(reg)                         (((reg) & LSM6DSO32_TAP_SRC_DOUBLE_TAP_Msk) >> LSM6DSO32_TAP_SRC_DOUBLE_TAP_Pos)  /* Assignment of DOUBLE_TAP value in LSM6DSO32_TAP_SRC register */
#define LSM6DSO32_TAP_SRC_SINGLE_TAP_Pos                              (5)  /* (LSM6DSO32_TAP_SRC) Single-tap event status position */
#define LSM6DSO32_TAP_SRC_SINGLE_TAP_Msk                              ((0x1) << LSM6DSO32_TAP_SRC_SINGLE_TAP_Pos)  /* (LSM6DSO32_TAP_SRC) Single-tap event status mask */
#define LSM6DSO32_TAP_SRC_SINGLE_TAP(value)                           (LSM6DSO32_TAP_SRC_SINGLE_TAP_Msk & ((value) << LSM6DSO32_TAP_SRC_SINGLE_TAP_Pos))  /* Assignment of SINGLE_TAP value in LSM6DSO32_TAP_SRC register */
#define LSM6DSO32_TAP_SRC_SINGLE_TAP_Get(reg)                         (((reg) & LSM6DSO32_TAP_SRC_SINGLE_TAP_Msk) >> LSM6DSO32_TAP_SRC_SINGLE_TAP_Pos)  /* Assignment of SINGLE_TAP value in LSM6DSO32_TAP_SRC register */
#define LSM6DSO32_TAP_SRC_TAP_IA_Pos                                  (6)  /* (LSM6DSO32_TAP_SRC) Tap event detection status position */
#define LSM6DSO32_TAP_SRC_TAP_IA_Msk                                  ((0x1) << LSM6DSO32_TAP_SRC_TAP_IA_Pos)  /* (LSM6DSO32_TAP_SRC) Tap event detection status mask */
#define LSM6DSO32_TAP_SRC_TAP_IA(value)                               (LSM6DSO32_TAP_SRC_TAP_IA_Msk & ((value) << LSM6DSO32_TAP_SRC_TAP_IA_Pos))  /* Assignment of TAP_IA value in LSM6DSO32_TAP_SRC register */
#define LSM6DSO32_TAP_SRC_TAP_IA_Get(reg)                             (((reg) & LSM6DSO32_TAP_SRC_TAP_IA_Msk) >> LSM6DSO32_TAP_SRC_TAP_IA_Pos)  /* Assignment of TAP_IA value in LSM6DSO32_TAP_SRC register */
#define LSM6DSO32_TAP_SRC_Msk                                         (0x7f)  /* (LSM6DSO32_TAP_SRC) Register mask */


/* LSM6DSO32_D6D_SRC, R: Portrait, landscape, face-up and face-down source register */
#define LSM6DSO32_D6D_SRC_ADDR                                        (0x1d)  /* (LSM6DSO32_D6D_SRC) Portrait, landscape, face-up and face-down source register address */

#define LSM6DSO32_D6D_SRC_XL_Pos                                      (0)  /* (LSM6DSO32_D6D_SRC) X-axis low event position */
#define LSM6DSO32_D6D_SRC_XL_Msk                                      ((0x1) << LSM6DSO32_D6D_SRC_XL_Pos)  /* (LSM6DSO32_D6D_SRC) X-axis low event mask */
#define LSM6DSO32_D6D_SRC_XL(value)                                   (LSM6DSO32_D6D_SRC_XL_Msk & ((value) << LSM6DSO32_D6D_SRC_XL_Pos))  /* Assignment of XL value in LSM6DSO32_D6D_SRC register */
#define LSM6DSO32_D6D_SRC_XL_Get(reg)                                 (((reg) & LSM6DSO32_D6D_SRC_XL_Msk) >> LSM6DSO32_D6D_SRC_XL_Pos)  /* Assignment of XL value in LSM6DSO32_D6D_SRC register */
#define LSM6DSO32_D6D_SRC_XH_Pos                                      (1)  /* (LSM6DSO32_D6D_SRC) X-axis high event position */
#define LSM6DSO32_D6D_SRC_XH_Msk                                      ((0x1) << LSM6DSO32_D6D_SRC_XH_Pos)  /* (LSM6DSO32_D6D_SRC) X-axis high event mask */
#define LSM6DSO32_D6D_SRC_XH(value)                                   (LSM6DSO32_D6D_SRC_XH_Msk & ((value) << LSM6DSO32_D6D_SRC_XH_Pos))  /* Assignment of XH value in LSM6DSO32_D6D_SRC register */
#define LSM6DSO32_D6D_SRC_XH_Get(reg)                                 (((reg) & LSM6DSO32_D6D_SRC_XH_Msk) >> LSM6DSO32_D6D_SRC_XH_Pos)  /* Assignment of XH value in LSM6DSO32_D6D_SRC register */
#define LSM6DSO32_D6D_SRC_YL_Pos                                      (2)  /* (LSM6DSO32_D6D_SRC) Y-axis low event position */
#define LSM6DSO32_D6D_SRC_YL_Msk                                      ((0x1) << LSM6DSO32_D6D_SRC_YL_Pos)  /* (LSM6DSO32_D6D_SRC) Y-axis low event mask */
#define LSM6DSO32_D6D_SRC_YL(value)                                   (LSM6DSO32_D6D_SRC_YL_Msk & ((value) << LSM6DSO32_D6D_SRC_YL_Pos))  /* Assignment of YL value in LSM6DSO32_D6D_SRC register */
#define LSM6DSO32_D6D_SRC_YL_Get(reg)                                 (((reg) & LSM6DSO32_D6D_SRC_YL_Msk) >> LSM6DSO32_D6D_SRC_YL_Pos)  /* Assignment of YL value in LSM6DSO32_D6D_SRC register */
#define LSM6DSO32_D6D_SRC_YH_Pos                                      (3)  /* (LSM6DSO32_D6D_SRC) Y-axis high event position */
#define LSM6DSO32_D6D_SRC_YH_Msk                                      ((0x1) << LSM6DSO32_D6D_SRC_YH_Pos)  /* (LSM6DSO32_D6D_SRC) Y-axis high event mask */
#define LSM6DSO32_D6D_SRC_YH(value)                                   (LSM6DSO32_D6D_SRC_YH_Msk & ((value) << LSM6DSO32_D6D_SRC_YH_Pos))  /* Assignment of YH value in LSM6DSO32_D6D_SRC register */
#define LSM6DSO32_D6D_SRC_YH_Get(reg)                                 (((reg) & LSM6DSO32_D6D_SRC_YH_Msk) >> LSM6DSO32_D6D_SRC_YH_Pos)  /* Assignment of YH value in LSM6DSO32_D6D_SRC register */
#define LSM6DSO32_D6D_SRC_ZL_Pos                                      (4)  /* (LSM6DSO32_D6D_SRC) Z-axis low event position */
#define LSM6DSO32_D6D_SRC_ZL_Msk                                      ((0x1) << LSM6DSO32_D6D_SRC_ZL_Pos)  /* (LSM6DSO32_D6D_SRC) Z-axis low event mask */
#define LSM6DSO32_D6D_SRC_ZL(value)                                   (LSM6DSO32_D6D_SRC_ZL_Msk & ((value) << LSM6DSO32_D6D_SRC_ZL_Pos))  /* Assignment of ZL value in LSM6DSO32_D6D_SRC register */
#define LSM6DSO32_D6D_SRC_ZL_Get(reg)                                 (((reg) & LSM6DSO32_D6D_SRC_ZL_Msk) >> LSM6DSO32_D6D_SRC_ZL_Pos)  /* Assignment of ZL value in LSM6DSO32_D6D_SRC register */
#define LSM6DSO32_D6D_SRC_ZH_Pos                                      (5)  /* (LSM6DSO32_D6D_SRC) Z-axis high event position */
#define LSM6DSO32_D6D_SRC_ZH_Msk                                      ((0x1) << LSM6DSO32_D6D_SRC_ZH_Pos)  /* (LSM6DSO32_D6D_SRC) Z-axis high event mask */
#define LSM6DSO32_D6D_SRC_ZH(value)                                   (LSM6DSO32_D6D_SRC_ZH_Msk & ((value) << LSM6DSO32_D6D_SRC_ZH_Pos))  /* Assignment of ZH value in LSM6DSO32_D6D_SRC register */
#define LSM6DSO32_D6D_SRC_ZH_Get(reg)                                 (((reg) & LSM6DSO32_D6D_SRC_ZH_Msk) >> LSM6DSO32_D6D_SRC_ZH_Pos)  /* Assignment of ZH value in LSM6DSO32_D6D_SRC register */
#define LSM6DSO32_D6D_SRC_D6D_IA_Pos                                  (6)  /* (LSM6DSO32_D6D_SRC) Interrupt active for change position portrait, landscape, face-up, face-down position */
#define LSM6DSO32_D6D_SRC_D6D_IA_Msk                                  ((0x1) << LSM6DSO32_D6D_SRC_D6D_IA_Pos)  /* (LSM6DSO32_D6D_SRC) Interrupt active for change position portrait, landscape, face-up, face-down mask */
#define LSM6DSO32_D6D_SRC_D6D_IA(value)                               (LSM6DSO32_D6D_SRC_D6D_IA_Msk & ((value) << LSM6DSO32_D6D_SRC_D6D_IA_Pos))  /* Assignment of D6D_IA value in LSM6DSO32_D6D_SRC register */
#define LSM6DSO32_D6D_SRC_D6D_IA_Get(reg)                             (((reg) & LSM6DSO32_D6D_SRC_D6D_IA_Msk) >> LSM6DSO32_D6D_SRC_D6D_IA_Pos)  /* Assignment of D6D_IA value in LSM6DSO32_D6D_SRC register */
#define LSM6DSO32_D6D_SRC_DEN_DRDY_Pos                                (7)  /* (LSM6DSO32_D6D_SRC) DEN data-ready signal. It is set high when data output is related to the data coming from a DEN active condition position */
#define LSM6DSO32_D6D_SRC_DEN_DRDY_Msk                                ((0x1) << LSM6DSO32_D6D_SRC_DEN_DRDY_Pos)  /* (LSM6DSO32_D6D_SRC) DEN data-ready signal. It is set high when data output is related to the data coming from a DEN active condition mask */
#define LSM6DSO32_D6D_SRC_DEN_DRDY(value)                             (LSM6DSO32_D6D_SRC_DEN_DRDY_Msk & ((value) << LSM6DSO32_D6D_SRC_DEN_DRDY_Pos))  /* Assignment of DEN_DRDY value in LSM6DSO32_D6D_SRC register */
#define LSM6DSO32_D6D_SRC_DEN_DRDY_Get(reg)                           (((reg) & LSM6DSO32_D6D_SRC_DEN_DRDY_Msk) >> LSM6DSO32_D6D_SRC_DEN_DRDY_Pos)  /* Assignment of DEN_DRDY value in LSM6DSO32_D6D_SRC register */
#define LSM6DSO32_D6D_SRC_Msk                                         (0xff)  /* (LSM6DSO32_D6D_SRC) Register mask */


/* LSM6DSO32_STATUS_REG, R: Status register */
#define LSM6DSO32_STATUS_REG_ADDR                                     (0x1e)  /* (LSM6DSO32_STATUS_REG) Status register address */

#define LSM6DSO32_STATUS_REG_XLDA_Pos                                 (0)  /* (LSM6DSO32_STATUS_REG) Accelerometer new data available position */
#define LSM6DSO32_STATUS_REG_XLDA_Msk                                 ((0x1) << LSM6DSO32_STATUS_REG_XLDA_Pos)  /* (LSM6DSO32_STATUS_REG) Accelerometer new data available mask */
#define LSM6DSO32_STATUS_REG_XLDA(value)                              (LSM6DSO32_STATUS_REG_XLDA_Msk & ((value) << LSM6DSO32_STATUS_REG_XLDA_Pos))  /* Assignment of XLDA value in LSM6DSO32_STATUS_REG register */
#define LSM6DSO32_STATUS_REG_XLDA_Get(reg)                            (((reg) & LSM6DSO32_STATUS_REG_XLDA_Msk) >> LSM6DSO32_STATUS_REG_XLDA_Pos)  /* Assignment of XLDA value in LSM6DSO32_STATUS_REG register */
#define LSM6DSO32_STATUS_REG_GDA_Pos                                  (1)  /* (LSM6DSO32_STATUS_REG) Gyroscope new data available position */
#define LSM6DSO32_STATUS_REG_GDA_Msk                                  ((0x1) << LSM6DSO32_STATUS_REG_GDA_Pos)  /* (LSM6DSO32_STATUS_REG) Gyroscope new data available mask */
#define LSM6DSO32_STATUS_REG_GDA(value)                               (LSM6DSO32_STATUS_REG_GDA_Msk & ((value) << LSM6DSO32_STATUS_REG_GDA_Pos))  /* Assignment of GDA value in LSM6DSO32_STATUS_REG register */
#define LSM6DSO32_STATUS_REG_GDA_Get(reg)                             (((reg) & LSM6DSO32_STATUS_REG_GDA_Msk) >> LSM6DSO32_STATUS_REG_GDA_Pos)  /* Assignment of GDA value in LSM6DSO32_STATUS_REG register */
#define LSM6DSO32_STATUS_REG_TDA_Pos                                  (2)  /* (LSM6DSO32_STATUS_REG) Temperature new data available position */
#define LSM6DSO32_STATUS_REG_TDA_Msk                                  ((0x1) << LSM6DSO32_STATUS_REG_TDA_Pos)  /* (LSM6DSO32_STATUS_REG) Temperature new data available mask */
#define LSM6DSO32_STATUS_REG_TDA(value)                               (LSM6DSO32_STATUS_REG_TDA_Msk & ((value) << LSM6DSO32_STATUS_REG_TDA_Pos))  /* Assignment of TDA value in LSM6DSO32_STATUS_REG register */
#define LSM6DSO32_STATUS_REG_TDA_Get(reg)                             (((reg) & LSM6DSO32_STATUS_REG_TDA_Msk) >> LSM6DSO32_STATUS_REG_TDA_Pos)  /* Assignment of TDA value in LSM6DSO32_STATUS_REG register */
#define LSM6DSO32_STATUS_REG_Msk                                      (0x7)  /* (LSM6DSO32_STATUS_REG) Register mask */


/* LSM6DSO32_OUT_TEMP_L, R: Temperature data output register. L and H registers together express a 16-bit word in two’s complement */
#define LSM6DSO32_OUT_TEMP_L_ADDR                                     (0x20)  /* (LSM6DSO32_OUT_TEMP_L) Temperature data output register. L and H registers together express a 16-bit word in two’s complement address */

#define LSM6DSO32_OUT_TEMP_L_Msk                                      (0xff)  /* (LSM6DSO32_OUT_TEMP_L) Register mask */


/* LSM6DSO32_OUT_TEMP_H, R: Temperature data output register. L and H registers together express a 16-bit word in two’s complement */
#define LSM6DSO32_OUT_TEMP_H_ADDR                                     (0x21)  /* (LSM6DSO32_OUT_TEMP_H) Temperature data output register. L and H registers together express a 16-bit word in two’s complement address */

#define LSM6DSO32_OUT_TEMP_H_Msk                                      (0xff)  /* (LSM6DSO32_OUT_TEMP_H) Register mask */


/* LSM6DSO32_OUTX_L_G, R: Angular rate sensor pitch axis (X) angular rate output register. The value is expressed as a 16-bit word in two’s complement */
#define LSM6DSO32_OUTX_L_G_ADDR                                       (0x22)  /* (LSM6DSO32_OUTX_L_G) Angular rate sensor pitch axis (X) angular rate output register. The value is expressed as a 16-bit word in two’s complement address */

#define LSM6DSO32_OUTX_L_G_Msk                                        (0xff)  /* (LSM6DSO32_OUTX_L_G) Register mask */


/* LSM6DSO32_OUTX_H_G, R: Angular rate sensor pitch axis (X) angular rate output register. The value is expressed as a 16-bit word in two’s complement */
#define LSM6DSO32_OUTX_H_G_ADDR                                       (0x23)  /* (LSM6DSO32_OUTX_H_G) Angular rate sensor pitch axis (X) angular rate output register. The value is expressed as a 16-bit word in two’s complement address */

#define LSM6DSO32_OUTX_H_G_Msk                                        (0xff)  /* (LSM6DSO32_OUTX_H_G) Register mask */


/* LSM6DSO32_OUTY_L_G, R: Angular rate sensor pitch axis (Y) angular rate output register. The value is expressed as a 16-bit word in two’s complement */
#define LSM6DSO32_OUTY_L_G_ADDR                                       (0x24)  /* (LSM6DSO32_OUTY_L_G) Angular rate sensor pitch axis (Y) angular rate output register. The value is expressed as a 16-bit word in two’s complement address */

#define LSM6DSO32_OUTY_L_G_Msk                                        (0xff)  /* (LSM6DSO32_OUTY_L_G) Register mask */


/* LSM6DSO32_OUTY_H_G, R: Angular rate sensor pitch axis (Y) angular rate output register. The value is expressed as a 16-bit word in two’s complement */
#define LSM6DSO32_OUTY_H_G_ADDR                                       (0x25)  /* (LSM6DSO32_OUTY_H_G) Angular rate sensor pitch axis (Y) angular rate output register. The value is expressed as a 16-bit word in two’s complement address */

#define LSM6DSO32_OUTY_H_G_Msk                                        (0xff)  /* (LSM6DSO32_OUTY_H_G) Register mask */


/* LSM6DSO32_OUTZ_L_G, R: Angular rate sensor pitch axis (Z) angular rate output register. The value is expressed as a 16-bit word in two’s complement */
#define LSM6DSO32_OUTZ_L_G_ADDR                                       (0x26)  /* (LSM6DSO32_OUTZ_L_G) Angular rate sensor pitch axis (Z) angular rate output register. The value is expressed as a 16-bit word in two’s complement address */

#define LSM6DSO32_OUTZ_L_G_Msk                                        (0xff)  /* (LSM6DSO32_OUTZ_L_G) Register mask */


/* LSM6DSO32_OUTZ_H_G, R: Angular rate sensor pitch axis (Z) angular rate output register. The value is expressed as a 16-bit word in two’s complement */
#define LSM6DSO32_OUTZ_H_G_ADDR                                       (0x27)  /* (LSM6DSO32_OUTZ_H_G) Angular rate sensor pitch axis (Z) angular rate output register. The value is expressed as a 16-bit word in two’s complement address */

#define LSM6DSO32_OUTZ_H_G_Msk                                        (0xff)  /* (LSM6DSO32_OUTZ_H_G) Register mask */


/* LSM6DSO32_OUTX_L_A, R: Linear acceleration sensor X-axis output register. The value is expressed as a 16-bit word in two’s complement */
#define LSM6DSO32_OUTX_L_A_ADDR                                       (0x28)  /* (LSM6DSO32_OUTX_L_A) Linear acceleration sensor X-axis output register. The value is expressed as a 16-bit word in two’s complement address */

#define LSM6DSO32_OUTX_L_A_Msk                                        (0xff)  /* (LSM6DSO32_OUTX_L_A) Register mask */


/* LSM6DSO32_OUTX_H_A, R: Linear acceleration sensor X-axis output register. The value is expressed as a 16-bit word in two’s complement */
#define LSM6DSO32_OUTX_H_A_ADDR                                       (0x29)  /* (LSM6DSO32_OUTX_H_A) Linear acceleration sensor X-axis output register. The value is expressed as a 16-bit word in two’s complement address */

#define LSM6DSO32_OUTX_H_A_Msk                                        (0xff)  /* (LSM6DSO32_OUTX_H_A) Register mask */


/* LSM6DSO32_OUTY_L_A, R: Linear acceleration sensor Y-axis output register. The value is expressed as a 16-bit word in two’s complement */
#define LSM6DSO32_OUTY_L_A_ADDR                                       (0x2a)  /* (LSM6DSO32_OUTY_L_A) Linear acceleration sensor Y-axis output register. The value is expressed as a 16-bit word in two’s complement address */

#define LSM6DSO32_OUTY_L_A_Msk                                        (0xff)  /* (LSM6DSO32_OUTY_L_A) Register mask */


/* LSM6DSO32_OUTY_H_A, R: Linear acceleration sensor Y-axis output register. The value is expressed as a 16-bit word in two’s complement */
#define LSM6DSO32_OUTY_H_A_ADDR                                       (0x2b)  /* (LSM6DSO32_OUTY_H_A) Linear acceleration sensor Y-axis output register. The value is expressed as a 16-bit word in two’s complement address */

#define LSM6DSO32_OUTY_H_A_Msk                                        (0xff)  /* (LSM6DSO32_OUTY_H_A) Register mask */


/* LSM6DSO32_OUTZ_L_A, R: Linear acceleration sensor Z-axis output register. The value is expressed as a 16-bit word in two’s complement */
#define LSM6DSO32_OUTZ_L_A_ADDR                                       (0x2c)  /* (LSM6DSO32_OUTZ_L_A) Linear acceleration sensor Z-axis output register. The value is expressed as a 16-bit word in two’s complement address */

#define LSM6DSO32_OUTZ_L_A_Msk                                        (0xff)  /* (LSM6DSO32_OUTZ_L_A) Register mask */


/* LSM6DSO32_OUTZ_H_A, R: Linear acceleration sensor Z-axis output register. The value is expressed as a 16-bit word in two’s complement */
#define LSM6DSO32_OUTZ_H_A_ADDR                                       (0x2d)  /* (LSM6DSO32_OUTZ_H_A) Linear acceleration sensor Z-axis output register. The value is expressed as a 16-bit word in two’s complement address */

#define LSM6DSO32_OUTZ_H_A_Msk                                        (0xff)  /* (LSM6DSO32_OUTZ_H_A) Register mask */


/* LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE, R: Embedded functions status register */
#define LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_ADDR                       (0x35)  /* (LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE) Embedded functions status register address */

#define LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_STEP_DET_Pos            (3)  /* (LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE) Interrupt status bit for step detection position */
#define LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_STEP_DET_Msk            ((0x1) << LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_STEP_DET_Pos)  /* (LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE) Interrupt status bit for step detection mask */
#define LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_STEP_DET(value)         (LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_STEP_DET_Msk & ((value) << LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_STEP_DET_Pos))  /* Assignment of IS_STEP_DET value in LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE register */
#define LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_STEP_DET_Get(reg)       (((reg) & LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_STEP_DET_Msk) >> LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_STEP_DET_Pos)  /* Assignment of IS_STEP_DET value in LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE register */
#define LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_TILT_Pos                (4)  /* (LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE) Interrupt status bit for tilt detection position */
#define LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_TILT_Msk                ((0x1) << LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_TILT_Pos)  /* (LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE) Interrupt status bit for tilt detection mask */
#define LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_TILT(value)             (LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_TILT_Msk & ((value) << LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_TILT_Pos))  /* Assignment of IS_TILT value in LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE register */
#define LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_TILT_Get(reg)           (((reg) & LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_TILT_Msk) >> LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_TILT_Pos)  /* Assignment of IS_TILT value in LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE register */
#define LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_SIGMOT_Pos              (5)  /* (LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE) Interrupt status bit for significant motion detection position */
#define LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_SIGMOT_Msk              ((0x1) << LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_SIGMOT_Pos)  /* (LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE) Interrupt status bit for significant motion detection mask */
#define LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_SIGMOT(value)           (LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_SIGMOT_Msk & ((value) << LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_SIGMOT_Pos))  /* Assignment of IS_SIGMOT value in LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE register */
#define LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_SIGMOT_Get(reg)         (((reg) & LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_SIGMOT_Msk) >> LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_SIGMOT_Pos)  /* Assignment of IS_SIGMOT value in LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE register */
#define LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_FSM_LC_Pos              (7)
#define LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_FSM_LC_Msk              ((0x1) << LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_FSM_LC_Pos)
#define LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_FSM_LC(value)           (LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_FSM_LC_Msk & ((value) << LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_FSM_LC_Pos))
#define LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_FSM_LC_Get(reg)         (((reg) & LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_FSM_LC_Msk) >> LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_IS_FSM_LC_Pos)
#define LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE_Msk                        (0xb8)  /* (LSM6DSO32_EMB_FUNC_STATUS_MAINPAGE) Register mask */


/* LSM6DSO32_FSM_STATUS_A_MAINPAGE, R: Finite state machine status register */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_ADDR                          (0x36)  /* (LSM6DSO32_FSM_STATUS_A_MAINPAGE) Finite state machine status register address */

#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM1_Pos                   (0)  /* (LSM6DSO32_FSM_STATUS_A_MAINPAGE) Interrupt status for FSM1 interrupt event position */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM1_Msk                   ((0x1) << LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM1_Pos)  /* (LSM6DSO32_FSM_STATUS_A_MAINPAGE) Interrupt status for FSM1 interrupt event mask */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM1(value)                (LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM1_Msk & ((value) << LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM1_Pos))  /* Assignment of IS_FSM1 value in LSM6DSO32_FSM_STATUS_A_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM1_Get(reg)              (((reg) & LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM1_Msk) >> LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM1_Pos)  /* Assignment of IS_FSM1 value in LSM6DSO32_FSM_STATUS_A_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM2_Pos                   (1)  /* (LSM6DSO32_FSM_STATUS_A_MAINPAGE) Interrupt status for FSM2 interrupt event position */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM2_Msk                   ((0x1) << LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM2_Pos)  /* (LSM6DSO32_FSM_STATUS_A_MAINPAGE) Interrupt status for FSM2 interrupt event mask */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM2(value)                (LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM2_Msk & ((value) << LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM2_Pos))  /* Assignment of IS_FSM2 value in LSM6DSO32_FSM_STATUS_A_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM2_Get(reg)              (((reg) & LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM2_Msk) >> LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM2_Pos)  /* Assignment of IS_FSM2 value in LSM6DSO32_FSM_STATUS_A_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM3_Pos                   (2)  /* (LSM6DSO32_FSM_STATUS_A_MAINPAGE) Interrupt status for FSM3 interrupt event position */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM3_Msk                   ((0x1) << LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM3_Pos)  /* (LSM6DSO32_FSM_STATUS_A_MAINPAGE) Interrupt status for FSM3 interrupt event mask */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM3(value)                (LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM3_Msk & ((value) << LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM3_Pos))  /* Assignment of IS_FSM3 value in LSM6DSO32_FSM_STATUS_A_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM3_Get(reg)              (((reg) & LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM3_Msk) >> LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM3_Pos)  /* Assignment of IS_FSM3 value in LSM6DSO32_FSM_STATUS_A_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM4_Pos                   (3)  /* (LSM6DSO32_FSM_STATUS_A_MAINPAGE) Interrupt status for FSM4 interrupt event position */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM4_Msk                   ((0x1) << LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM4_Pos)  /* (LSM6DSO32_FSM_STATUS_A_MAINPAGE) Interrupt status for FSM4 interrupt event mask */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM4(value)                (LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM4_Msk & ((value) << LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM4_Pos))  /* Assignment of IS_FSM4 value in LSM6DSO32_FSM_STATUS_A_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM4_Get(reg)              (((reg) & LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM4_Msk) >> LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM4_Pos)  /* Assignment of IS_FSM4 value in LSM6DSO32_FSM_STATUS_A_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM5_Pos                   (4)  /* (LSM6DSO32_FSM_STATUS_A_MAINPAGE) Interrupt status for FSM5 interrupt event position */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM5_Msk                   ((0x1) << LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM5_Pos)  /* (LSM6DSO32_FSM_STATUS_A_MAINPAGE) Interrupt status for FSM5 interrupt event mask */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM5(value)                (LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM5_Msk & ((value) << LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM5_Pos))  /* Assignment of IS_FSM5 value in LSM6DSO32_FSM_STATUS_A_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM5_Get(reg)              (((reg) & LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM5_Msk) >> LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM5_Pos)  /* Assignment of IS_FSM5 value in LSM6DSO32_FSM_STATUS_A_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM6_Pos                   (5)  /* (LSM6DSO32_FSM_STATUS_A_MAINPAGE) Interrupt status for FSM6 interrupt event position */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM6_Msk                   ((0x1) << LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM6_Pos)  /* (LSM6DSO32_FSM_STATUS_A_MAINPAGE) Interrupt status for FSM6 interrupt event mask */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM6(value)                (LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM6_Msk & ((value) << LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM6_Pos))  /* Assignment of IS_FSM6 value in LSM6DSO32_FSM_STATUS_A_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM6_Get(reg)              (((reg) & LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM6_Msk) >> LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM6_Pos)  /* Assignment of IS_FSM6 value in LSM6DSO32_FSM_STATUS_A_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM7_Pos                   (6)  /* (LSM6DSO32_FSM_STATUS_A_MAINPAGE) Interrupt status for FSM7 interrupt event position */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM7_Msk                   ((0x1) << LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM7_Pos)  /* (LSM6DSO32_FSM_STATUS_A_MAINPAGE) Interrupt status for FSM7 interrupt event mask */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM7(value)                (LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM7_Msk & ((value) << LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM7_Pos))  /* Assignment of IS_FSM7 value in LSM6DSO32_FSM_STATUS_A_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM7_Get(reg)              (((reg) & LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM7_Msk) >> LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM7_Pos)  /* Assignment of IS_FSM7 value in LSM6DSO32_FSM_STATUS_A_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM8_Pos                   (7)  /* (LSM6DSO32_FSM_STATUS_A_MAINPAGE) Interrupt status for FSM8 interrupt event position */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM8_Msk                   ((0x1) << LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM8_Pos)  /* (LSM6DSO32_FSM_STATUS_A_MAINPAGE) Interrupt status for FSM8 interrupt event mask */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM8(value)                (LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM8_Msk & ((value) << LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM8_Pos))  /* Assignment of IS_FSM8 value in LSM6DSO32_FSM_STATUS_A_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM8_Get(reg)              (((reg) & LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM8_Msk) >> LSM6DSO32_FSM_STATUS_A_MAINPAGE_IS_FSM8_Pos)  /* Assignment of IS_FSM8 value in LSM6DSO32_FSM_STATUS_A_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_A_MAINPAGE_Msk                           (0xff)  /* (LSM6DSO32_FSM_STATUS_A_MAINPAGE) Register mask */


/* LSM6DSO32_FSM_STATUS_B_MAINPAGE, R: Finite state machine status register */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_ADDR                          (0x37)  /* (LSM6DSO32_FSM_STATUS_B_MAINPAGE) Finite state machine status register address */

#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM9_Pos                   (0)  /* (LSM6DSO32_FSM_STATUS_B_MAINPAGE) Interrupt status for FSM9 interrupt event position */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM9_Msk                   ((0x1) << LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM9_Pos)  /* (LSM6DSO32_FSM_STATUS_B_MAINPAGE) Interrupt status for FSM9 interrupt event mask */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM9(value)                (LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM9_Msk & ((value) << LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM9_Pos))  /* Assignment of IS_FSM9 value in LSM6DSO32_FSM_STATUS_B_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM9_Get(reg)              (((reg) & LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM9_Msk) >> LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM9_Pos)  /* Assignment of IS_FSM9 value in LSM6DSO32_FSM_STATUS_B_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM10_Pos                  (1)  /* (LSM6DSO32_FSM_STATUS_B_MAINPAGE) Interrupt status for FSM10 interrupt event position */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM10_Msk                  ((0x1) << LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM10_Pos)  /* (LSM6DSO32_FSM_STATUS_B_MAINPAGE) Interrupt status for FSM10 interrupt event mask */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM10(value)               (LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM10_Msk & ((value) << LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM10_Pos))  /* Assignment of IS_FSM10 value in LSM6DSO32_FSM_STATUS_B_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM10_Get(reg)             (((reg) & LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM10_Msk) >> LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM10_Pos)  /* Assignment of IS_FSM10 value in LSM6DSO32_FSM_STATUS_B_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM11_Pos                  (2)  /* (LSM6DSO32_FSM_STATUS_B_MAINPAGE) Interrupt status for FSM11 interrupt event position */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM11_Msk                  ((0x1) << LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM11_Pos)  /* (LSM6DSO32_FSM_STATUS_B_MAINPAGE) Interrupt status for FSM11 interrupt event mask */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM11(value)               (LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM11_Msk & ((value) << LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM11_Pos))  /* Assignment of IS_FSM11 value in LSM6DSO32_FSM_STATUS_B_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM11_Get(reg)             (((reg) & LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM11_Msk) >> LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM11_Pos)  /* Assignment of IS_FSM11 value in LSM6DSO32_FSM_STATUS_B_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM12_Pos                  (3)  /* (LSM6DSO32_FSM_STATUS_B_MAINPAGE) Interrupt status for FSM12 interrupt event position */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM12_Msk                  ((0x1) << LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM12_Pos)  /* (LSM6DSO32_FSM_STATUS_B_MAINPAGE) Interrupt status for FSM12 interrupt event mask */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM12(value)               (LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM12_Msk & ((value) << LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM12_Pos))  /* Assignment of IS_FSM12 value in LSM6DSO32_FSM_STATUS_B_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM12_Get(reg)             (((reg) & LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM12_Msk) >> LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM12_Pos)  /* Assignment of IS_FSM12 value in LSM6DSO32_FSM_STATUS_B_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM13_Pos                  (4)  /* (LSM6DSO32_FSM_STATUS_B_MAINPAGE) Interrupt status for FSM13 interrupt event position */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM13_Msk                  ((0x1) << LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM13_Pos)  /* (LSM6DSO32_FSM_STATUS_B_MAINPAGE) Interrupt status for FSM13 interrupt event mask */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM13(value)               (LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM13_Msk & ((value) << LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM13_Pos))  /* Assignment of IS_FSM13 value in LSM6DSO32_FSM_STATUS_B_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM13_Get(reg)             (((reg) & LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM13_Msk) >> LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM13_Pos)  /* Assignment of IS_FSM13 value in LSM6DSO32_FSM_STATUS_B_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM14_Pos                  (5)  /* (LSM6DSO32_FSM_STATUS_B_MAINPAGE) Interrupt status for FSM14 interrupt event position */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM14_Msk                  ((0x1) << LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM14_Pos)  /* (LSM6DSO32_FSM_STATUS_B_MAINPAGE) Interrupt status for FSM14 interrupt event mask */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM14(value)               (LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM14_Msk & ((value) << LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM14_Pos))  /* Assignment of IS_FSM14 value in LSM6DSO32_FSM_STATUS_B_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM14_Get(reg)             (((reg) & LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM14_Msk) >> LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM14_Pos)  /* Assignment of IS_FSM14 value in LSM6DSO32_FSM_STATUS_B_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM15_Pos                  (6)  /* (LSM6DSO32_FSM_STATUS_B_MAINPAGE) Interrupt status for FSM15 interrupt event position */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM15_Msk                  ((0x1) << LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM15_Pos)  /* (LSM6DSO32_FSM_STATUS_B_MAINPAGE) Interrupt status for FSM15 interrupt event mask */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM15(value)               (LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM15_Msk & ((value) << LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM15_Pos))  /* Assignment of IS_FSM15 value in LSM6DSO32_FSM_STATUS_B_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM15_Get(reg)             (((reg) & LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM15_Msk) >> LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM15_Pos)  /* Assignment of IS_FSM15 value in LSM6DSO32_FSM_STATUS_B_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM16_Pos                  (7)  /* (LSM6DSO32_FSM_STATUS_B_MAINPAGE) Interrupt status for FSM16 interrupt event position */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM16_Msk                  ((0x1) << LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM16_Pos)  /* (LSM6DSO32_FSM_STATUS_B_MAINPAGE) Interrupt status for FSM16 interrupt event mask */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM16(value)               (LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM16_Msk & ((value) << LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM16_Pos))  /* Assignment of IS_FSM16 value in LSM6DSO32_FSM_STATUS_B_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM16_Get(reg)             (((reg) & LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM16_Msk) >> LSM6DSO32_FSM_STATUS_B_MAINPAGE_IS_FSM16_Pos)  /* Assignment of IS_FSM16 value in LSM6DSO32_FSM_STATUS_B_MAINPAGE register */
#define LSM6DSO32_FSM_STATUS_B_MAINPAGE_Msk                           (0xff)  /* (LSM6DSO32_FSM_STATUS_B_MAINPAGE) Register mask */


/* LSM6DSO32_STATUS_MASTER_MAINPAGE, R: Sensor hub source register */
#define LSM6DSO32_STATUS_MASTER_MAINPAGE_ADDR                         (0x39)  /* (LSM6DSO32_STATUS_MASTER_MAINPAGE) Sensor hub source register address */

#define LSM6DSO32_STATUS_MASTER_MAINPAGE_SENS_HUB_ENDOP_Pos           (0)  /* (LSM6DSO32_STATUS_MASTER_MAINPAGE) Sensor hub communication status position */
#define LSM6DSO32_STATUS_MASTER_MAINPAGE_SENS_HUB_ENDOP_Msk           ((0x1) << LSM6DSO32_STATUS_MASTER_MAINPAGE_SENS_HUB_ENDOP_Pos)  /* (LSM6DSO32_STATUS_MASTER_MAINPAGE) Sensor hub communication status mask */
#define LSM6DSO32_STATUS_MASTER_MAINPAGE_SENS_HUB_ENDOP(value)        (LSM6DSO32_STATUS_MASTER_MAINPAGE_SENS_HUB_ENDOP_Msk & ((value) << LSM6DSO32_STATUS_MASTER_MAINPAGE_SENS_HUB_ENDOP_Pos))  /* Assignment of SENS_HUB_ENDOP value in LSM6DSO32_STATUS_MASTER_MAINPAGE register */
#define LSM6DSO32_STATUS_MASTER_MAINPAGE_SENS_HUB_ENDOP_Get(reg)      (((reg) & LSM6DSO32_STATUS_MASTER_MAINPAGE_SENS_HUB_ENDOP_Msk) >> LSM6DSO32_STATUS_MASTER_MAINPAGE_SENS_HUB_ENDOP_Pos)  /* Assignment of SENS_HUB_ENDOP value in LSM6DSO32_STATUS_MASTER_MAINPAGE register */
#define LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE0_NACK_Pos              (3)  /* (LSM6DSO32_STATUS_MASTER_MAINPAGE) This bit is set to 1 if Not acknowledge occurs on slave 0 communication position */
#define LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE0_NACK_Msk              ((0x1) << LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE0_NACK_Pos)  /* (LSM6DSO32_STATUS_MASTER_MAINPAGE) This bit is set to 1 if Not acknowledge occurs on slave 0 communication mask */
#define LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE0_NACK(value)           (LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE0_NACK_Msk & ((value) << LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE0_NACK_Pos))  /* Assignment of SLAVE0_NACK value in LSM6DSO32_STATUS_MASTER_MAINPAGE register */
#define LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE0_NACK_Get(reg)         (((reg) & LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE0_NACK_Msk) >> LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE0_NACK_Pos)  /* Assignment of SLAVE0_NACK value in LSM6DSO32_STATUS_MASTER_MAINPAGE register */
#define LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE1_NACK_Pos              (4)  /* (LSM6DSO32_STATUS_MASTER_MAINPAGE) This bit is set to 1 if Not acknowledge occurs on slave 1 communication position */
#define LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE1_NACK_Msk              ((0x1) << LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE1_NACK_Pos)  /* (LSM6DSO32_STATUS_MASTER_MAINPAGE) This bit is set to 1 if Not acknowledge occurs on slave 1 communication mask */
#define LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE1_NACK(value)           (LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE1_NACK_Msk & ((value) << LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE1_NACK_Pos))  /* Assignment of SLAVE1_NACK value in LSM6DSO32_STATUS_MASTER_MAINPAGE register */
#define LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE1_NACK_Get(reg)         (((reg) & LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE1_NACK_Msk) >> LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE1_NACK_Pos)  /* Assignment of SLAVE1_NACK value in LSM6DSO32_STATUS_MASTER_MAINPAGE register */
#define LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE2_NACK_Pos              (5)  /* (LSM6DSO32_STATUS_MASTER_MAINPAGE) This bit is set to 1 if Not acknowledge occurs on slave 2 communication position */
#define LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE2_NACK_Msk              ((0x1) << LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE2_NACK_Pos)  /* (LSM6DSO32_STATUS_MASTER_MAINPAGE) This bit is set to 1 if Not acknowledge occurs on slave 2 communication mask */
#define LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE2_NACK(value)           (LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE2_NACK_Msk & ((value) << LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE2_NACK_Pos))  /* Assignment of SLAVE2_NACK value in LSM6DSO32_STATUS_MASTER_MAINPAGE register */
#define LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE2_NACK_Get(reg)         (((reg) & LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE2_NACK_Msk) >> LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE2_NACK_Pos)  /* Assignment of SLAVE2_NACK value in LSM6DSO32_STATUS_MASTER_MAINPAGE register */
#define LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE3_NACK_Pos              (6)  /* (LSM6DSO32_STATUS_MASTER_MAINPAGE) This bit is set to 1 if Not acknowledge occurs on slave 3 communication position */
#define LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE3_NACK_Msk              ((0x1) << LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE3_NACK_Pos)  /* (LSM6DSO32_STATUS_MASTER_MAINPAGE) This bit is set to 1 if Not acknowledge occurs on slave 3 communication mask */
#define LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE3_NACK(value)           (LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE3_NACK_Msk & ((value) << LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE3_NACK_Pos))  /* Assignment of SLAVE3_NACK value in LSM6DSO32_STATUS_MASTER_MAINPAGE register */
#define LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE3_NACK_Get(reg)         (((reg) & LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE3_NACK_Msk) >> LSM6DSO32_STATUS_MASTER_MAINPAGE_SLAVE3_NACK_Pos)  /* Assignment of SLAVE3_NACK value in LSM6DSO32_STATUS_MASTER_MAINPAGE register */
#define LSM6DSO32_STATUS_MASTER_MAINPAGE_Msk                          (0x79)  /* (LSM6DSO32_STATUS_MASTER_MAINPAGE) Register mask */


/* LSM6DSO32_FIFO_STATUS_1, R: FIFO status register */
#define LSM6DSO32_FIFO_STATUS_1_ADDR                                  (0x3a)  /* (LSM6DSO32_FIFO_STATUS_1) FIFO status register address */

#define LSM6DSO32_FIFO_STATUS_1_DIFF_FIFO_Pos                         (0)  /* (LSM6DSO32_FIFO_STATUS_1) Number of unread sensor data (TAG + 6 bytes) stored in FIFO. In conjunction with DIFF_FIFO[9:8] in FIFO_STATUS2 position */
#define LSM6DSO32_FIFO_STATUS_1_DIFF_FIFO_Msk                         ((0xff) << LSM6DSO32_FIFO_STATUS_1_DIFF_FIFO_Pos)  /* (LSM6DSO32_FIFO_STATUS_1) Number of unread sensor data (TAG + 6 bytes) stored in FIFO. In conjunction with DIFF_FIFO[9:8] in FIFO_STATUS2 mask */
#define LSM6DSO32_FIFO_STATUS_1_DIFF_FIFO(value)                      (LSM6DSO32_FIFO_STATUS_1_DIFF_FIFO_Msk & ((value) << LSM6DSO32_FIFO_STATUS_1_DIFF_FIFO_Pos))  /* Assignment of DIFF_FIFO value in LSM6DSO32_FIFO_STATUS_1 register */
#define LSM6DSO32_FIFO_STATUS_1_DIFF_FIFO_Get(reg)                    (((reg) & LSM6DSO32_FIFO_STATUS_1_DIFF_FIFO_Msk) >> LSM6DSO32_FIFO_STATUS_1_DIFF_FIFO_Pos)  /* Assignment of DIFF_FIFO value in LSM6DSO32_FIFO_STATUS_1 register */
#define LSM6DSO32_FIFO_STATUS_1_Msk                                   (0xff)  /* (LSM6DSO32_FIFO_STATUS_1) Register mask */


/* LSM6DSO32_FIFO_STATUS_2, R: FIFO status register */
#define LSM6DSO32_FIFO_STATUS_2_ADDR                                  (0x3b)  /* (LSM6DSO32_FIFO_STATUS_2) FIFO status register address */

#define LSM6DSO32_FIFO_STATUS_2_DIFF_FIFO_Pos                         (0)  /* (LSM6DSO32_FIFO_STATUS_2) Number of unread sensor data (TAG + 6 bytes) stored in FIFO. In conjunction with DIFF_FIFO[7:0] in FIFO_STATUS1 position */
#define LSM6DSO32_FIFO_STATUS_2_DIFF_FIFO_Msk                         ((0x3) << LSM6DSO32_FIFO_STATUS_2_DIFF_FIFO_Pos)  /* (LSM6DSO32_FIFO_STATUS_2) Number of unread sensor data (TAG + 6 bytes) stored in FIFO. In conjunction with DIFF_FIFO[7:0] in FIFO_STATUS1 mask */
#define LSM6DSO32_FIFO_STATUS_2_DIFF_FIFO(value)                      (LSM6DSO32_FIFO_STATUS_2_DIFF_FIFO_Msk & ((value) << LSM6DSO32_FIFO_STATUS_2_DIFF_FIFO_Pos))  /* Assignment of DIFF_FIFO value in LSM6DSO32_FIFO_STATUS_2 register */
#define LSM6DSO32_FIFO_STATUS_2_DIFF_FIFO_Get(reg)                    (((reg) & LSM6DSO32_FIFO_STATUS_2_DIFF_FIFO_Msk) >> LSM6DSO32_FIFO_STATUS_2_DIFF_FIFO_Pos)  /* Assignment of DIFF_FIFO value in LSM6DSO32_FIFO_STATUS_2 register */
#define LSM6DSO32_FIFO_STATUS_2_FIFO_OVR_LATCHED_Pos                  (3)  /* (LSM6DSO32_FIFO_STATUS_2) Latched FIFO overrun status position */
#define LSM6DSO32_FIFO_STATUS_2_FIFO_OVR_LATCHED_Msk                  ((0x1) << LSM6DSO32_FIFO_STATUS_2_FIFO_OVR_LATCHED_Pos)  /* (LSM6DSO32_FIFO_STATUS_2) Latched FIFO overrun status mask */
#define LSM6DSO32_FIFO_STATUS_2_FIFO_OVR_LATCHED(value)               (LSM6DSO32_FIFO_STATUS_2_FIFO_OVR_LATCHED_Msk & ((value) << LSM6DSO32_FIFO_STATUS_2_FIFO_OVR_LATCHED_Pos))  /* Assignment of FIFO_OVR_LATCHED value in LSM6DSO32_FIFO_STATUS_2 register */
#define LSM6DSO32_FIFO_STATUS_2_FIFO_OVR_LATCHED_Get(reg)             (((reg) & LSM6DSO32_FIFO_STATUS_2_FIFO_OVR_LATCHED_Msk) >> LSM6DSO32_FIFO_STATUS_2_FIFO_OVR_LATCHED_Pos)  /* Assignment of FIFO_OVR_LATCHED value in LSM6DSO32_FIFO_STATUS_2 register */
#define LSM6DSO32_FIFO_STATUS_2_COUNTER_BDR_IA_Pos                    (4)  /* (LSM6DSO32_FIFO_STATUS_2) Counter BDR reaches the CNT_BDR_TH_[10:0] threshold set in COUNTER_BDR_REG1 (0Bh) and COUNTER_BDR_REG2 (0Ch) position */
#define LSM6DSO32_FIFO_STATUS_2_COUNTER_BDR_IA_Msk                    ((0x1) << LSM6DSO32_FIFO_STATUS_2_COUNTER_BDR_IA_Pos)  /* (LSM6DSO32_FIFO_STATUS_2) Counter BDR reaches the CNT_BDR_TH_[10:0] threshold set in COUNTER_BDR_REG1 (0Bh) and COUNTER_BDR_REG2 (0Ch) mask */
#define LSM6DSO32_FIFO_STATUS_2_COUNTER_BDR_IA(value)                 (LSM6DSO32_FIFO_STATUS_2_COUNTER_BDR_IA_Msk & ((value) << LSM6DSO32_FIFO_STATUS_2_COUNTER_BDR_IA_Pos))  /* Assignment of COUNTER_BDR_IA value in LSM6DSO32_FIFO_STATUS_2 register */
#define LSM6DSO32_FIFO_STATUS_2_COUNTER_BDR_IA_Get(reg)               (((reg) & LSM6DSO32_FIFO_STATUS_2_COUNTER_BDR_IA_Msk) >> LSM6DSO32_FIFO_STATUS_2_COUNTER_BDR_IA_Pos)  /* Assignment of COUNTER_BDR_IA value in LSM6DSO32_FIFO_STATUS_2 register */
#define LSM6DSO32_FIFO_STATUS_2_FIFO_FULL_IA_Pos                      (5)  /* (LSM6DSO32_FIFO_STATUS_2) Smart FIFO full status position */
#define LSM6DSO32_FIFO_STATUS_2_FIFO_FULL_IA_Msk                      ((0x1) << LSM6DSO32_FIFO_STATUS_2_FIFO_FULL_IA_Pos)  /* (LSM6DSO32_FIFO_STATUS_2) Smart FIFO full status mask */
#define LSM6DSO32_FIFO_STATUS_2_FIFO_FULL_IA(value)                   (LSM6DSO32_FIFO_STATUS_2_FIFO_FULL_IA_Msk & ((value) << LSM6DSO32_FIFO_STATUS_2_FIFO_FULL_IA_Pos))  /* Assignment of FIFO_FULL_IA value in LSM6DSO32_FIFO_STATUS_2 register */
#define LSM6DSO32_FIFO_STATUS_2_FIFO_FULL_IA_Get(reg)                 (((reg) & LSM6DSO32_FIFO_STATUS_2_FIFO_FULL_IA_Msk) >> LSM6DSO32_FIFO_STATUS_2_FIFO_FULL_IA_Pos)  /* Assignment of FIFO_FULL_IA value in LSM6DSO32_FIFO_STATUS_2 register */
#define LSM6DSO32_FIFO_STATUS_2_FIFO_OVR_IA_Pos                       (6)  /* (LSM6DSO32_FIFO_STATUS_2) FIFO overrun status position */
#define LSM6DSO32_FIFO_STATUS_2_FIFO_OVR_IA_Msk                       ((0x1) << LSM6DSO32_FIFO_STATUS_2_FIFO_OVR_IA_Pos)  /* (LSM6DSO32_FIFO_STATUS_2) FIFO overrun status mask */
#define LSM6DSO32_FIFO_STATUS_2_FIFO_OVR_IA(value)                    (LSM6DSO32_FIFO_STATUS_2_FIFO_OVR_IA_Msk & ((value) << LSM6DSO32_FIFO_STATUS_2_FIFO_OVR_IA_Pos))  /* Assignment of FIFO_OVR_IA value in LSM6DSO32_FIFO_STATUS_2 register */
#define LSM6DSO32_FIFO_STATUS_2_FIFO_OVR_IA_Get(reg)                  (((reg) & LSM6DSO32_FIFO_STATUS_2_FIFO_OVR_IA_Msk) >> LSM6DSO32_FIFO_STATUS_2_FIFO_OVR_IA_Pos)  /* Assignment of FIFO_OVR_IA value in LSM6DSO32_FIFO_STATUS_2 register */
#define LSM6DSO32_FIFO_STATUS_2_FIFO_WTM_IA_Pos                       (7)  /* (LSM6DSO32_FIFO_STATUS_2) FIFO watermark status position */
#define LSM6DSO32_FIFO_STATUS_2_FIFO_WTM_IA_Msk                       ((0x1) << LSM6DSO32_FIFO_STATUS_2_FIFO_WTM_IA_Pos)  /* (LSM6DSO32_FIFO_STATUS_2) FIFO watermark status mask */
#define LSM6DSO32_FIFO_STATUS_2_FIFO_WTM_IA(value)                    (LSM6DSO32_FIFO_STATUS_2_FIFO_WTM_IA_Msk & ((value) << LSM6DSO32_FIFO_STATUS_2_FIFO_WTM_IA_Pos))  /* Assignment of FIFO_WTM_IA value in LSM6DSO32_FIFO_STATUS_2 register */
#define LSM6DSO32_FIFO_STATUS_2_FIFO_WTM_IA_Get(reg)                  (((reg) & LSM6DSO32_FIFO_STATUS_2_FIFO_WTM_IA_Msk) >> LSM6DSO32_FIFO_STATUS_2_FIFO_WTM_IA_Pos)  /* Assignment of FIFO_WTM_IA value in LSM6DSO32_FIFO_STATUS_2 register */
#define LSM6DSO32_FIFO_STATUS_2_Msk                                   (0xfb)  /* (LSM6DSO32_FIFO_STATUS_2) Register mask */


/* LSM6DSO32_TIMESTAMP0, R: Timestamp first data output register. The value is expressed as a 32-bit word and the bit resolution is 25 μs */
#define LSM6DSO32_TIMESTAMP0_ADDR                                     (0x40)  /* (LSM6DSO32_TIMESTAMP0) Timestamp first data output register. The value is expressed as a 32-bit word and the bit resolution is 25 μs address */

#define LSM6DSO32_TIMESTAMP0_Msk                                      (0xff)  /* (LSM6DSO32_TIMESTAMP0) Register mask */


/* LSM6DSO32_TIMESTAMP1, R: Timestamp first data output register. The value is expressed as a 32-bit word and the bit resolution is 25 μs */
#define LSM6DSO32_TIMESTAMP1_ADDR                                     (0x41)  /* (LSM6DSO32_TIMESTAMP1) Timestamp first data output register. The value is expressed as a 32-bit word and the bit resolution is 25 μs address */

#define LSM6DSO32_TIMESTAMP1_Msk                                      (0xff)  /* (LSM6DSO32_TIMESTAMP1) Register mask */


/* LSM6DSO32_TIMESTAMP2, R: Timestamp first data output register. The value is expressed as a 32-bit word and the bit resolution is 25 μs */
#define LSM6DSO32_TIMESTAMP2_ADDR                                     (0x42)  /* (LSM6DSO32_TIMESTAMP2) Timestamp first data output register. The value is expressed as a 32-bit word and the bit resolution is 25 μs address */

#define LSM6DSO32_TIMESTAMP2_Msk                                      (0xff)  /* (LSM6DSO32_TIMESTAMP2) Register mask */


/* LSM6DSO32_TIMESTAMP3, R: Timestamp first data output register. The value is expressed as a 32-bit word and the bit resolution is 25 μs */
#define LSM6DSO32_TIMESTAMP3_ADDR                                     (0x43)  /* (LSM6DSO32_TIMESTAMP3) Timestamp first data output register. The value is expressed as a 32-bit word and the bit resolution is 25 μs address */

#define LSM6DSO32_TIMESTAMP3_Msk                                      (0xff)  /* (LSM6DSO32_TIMESTAMP3) Register mask */


/* LSM6DSO32_TAP_CFG0, R/W: Activity/inactivity functions, configuration of filtering and tap recognition functions */
#define LSM6DSO32_TAP_CFG0_ADDR                                       (0x56)  /* (LSM6DSO32_TAP_CFG0) Activity/inactivity functions, configuration of filtering and tap recognition functions address */

#define LSM6DSO32_TAP_CFG0_LIR_Pos                                    (0)  /* (LSM6DSO32_TAP_CFG0) Latched interrupt position */
#define LSM6DSO32_TAP_CFG0_LIR_Msk                                    ((0x1) << LSM6DSO32_TAP_CFG0_LIR_Pos)  /* (LSM6DSO32_TAP_CFG0) Latched interrupt mask */
#define LSM6DSO32_TAP_CFG0_LIR(value)                                 (LSM6DSO32_TAP_CFG0_LIR_Msk & ((value) << LSM6DSO32_TAP_CFG0_LIR_Pos))  /* Assignment of LIR value in LSM6DSO32_TAP_CFG0 register */
#define LSM6DSO32_TAP_CFG0_LIR_Get(reg)                               (((reg) & LSM6DSO32_TAP_CFG0_LIR_Msk) >> LSM6DSO32_TAP_CFG0_LIR_Pos)  /* Assignment of LIR value in LSM6DSO32_TAP_CFG0 register */
#define LSM6DSO32_TAP_CFG0_TAP_Z_EN_Pos                               (1)  /* (LSM6DSO32_TAP_CFG0) Enable Z direction in tap recognition position */
#define LSM6DSO32_TAP_CFG0_TAP_Z_EN_Msk                               ((0x1) << LSM6DSO32_TAP_CFG0_TAP_Z_EN_Pos)  /* (LSM6DSO32_TAP_CFG0) Enable Z direction in tap recognition mask */
#define LSM6DSO32_TAP_CFG0_TAP_Z_EN(value)                            (LSM6DSO32_TAP_CFG0_TAP_Z_EN_Msk & ((value) << LSM6DSO32_TAP_CFG0_TAP_Z_EN_Pos))  /* Assignment of TAP_Z_EN value in LSM6DSO32_TAP_CFG0 register */
#define LSM6DSO32_TAP_CFG0_TAP_Z_EN_Get(reg)                          (((reg) & LSM6DSO32_TAP_CFG0_TAP_Z_EN_Msk) >> LSM6DSO32_TAP_CFG0_TAP_Z_EN_Pos)  /* Assignment of TAP_Z_EN value in LSM6DSO32_TAP_CFG0 register */
#define LSM6DSO32_TAP_CFG0_TAP_Y_EN_Pos                               (2)  /* (LSM6DSO32_TAP_CFG0) Enable Y direction in tap recognition position */
#define LSM6DSO32_TAP_CFG0_TAP_Y_EN_Msk                               ((0x1) << LSM6DSO32_TAP_CFG0_TAP_Y_EN_Pos)  /* (LSM6DSO32_TAP_CFG0) Enable Y direction in tap recognition mask */
#define LSM6DSO32_TAP_CFG0_TAP_Y_EN(value)                            (LSM6DSO32_TAP_CFG0_TAP_Y_EN_Msk & ((value) << LSM6DSO32_TAP_CFG0_TAP_Y_EN_Pos))  /* Assignment of TAP_Y_EN value in LSM6DSO32_TAP_CFG0 register */
#define LSM6DSO32_TAP_CFG0_TAP_Y_EN_Get(reg)                          (((reg) & LSM6DSO32_TAP_CFG0_TAP_Y_EN_Msk) >> LSM6DSO32_TAP_CFG0_TAP_Y_EN_Pos)  /* Assignment of TAP_Y_EN value in LSM6DSO32_TAP_CFG0 register */
#define LSM6DSO32_TAP_CFG0_TAP_X_EN_Pos                               (3)  /* (LSM6DSO32_TAP_CFG0) Enable X direction in tap recognition position */
#define LSM6DSO32_TAP_CFG0_TAP_X_EN_Msk                               ((0x1) << LSM6DSO32_TAP_CFG0_TAP_X_EN_Pos)  /* (LSM6DSO32_TAP_CFG0) Enable X direction in tap recognition mask */
#define LSM6DSO32_TAP_CFG0_TAP_X_EN(value)                            (LSM6DSO32_TAP_CFG0_TAP_X_EN_Msk & ((value) << LSM6DSO32_TAP_CFG0_TAP_X_EN_Pos))  /* Assignment of TAP_X_EN value in LSM6DSO32_TAP_CFG0 register */
#define LSM6DSO32_TAP_CFG0_TAP_X_EN_Get(reg)                          (((reg) & LSM6DSO32_TAP_CFG0_TAP_X_EN_Msk) >> LSM6DSO32_TAP_CFG0_TAP_X_EN_Pos)  /* Assignment of TAP_X_EN value in LSM6DSO32_TAP_CFG0 register */
#define LSM6DSO32_TAP_CFG0_SLOPE_FDS_Pos                              (4)  /* (LSM6DSO32_TAP_CFG0) HPF or SLOPE filter selection on wake-up and Activity/Inactivity functions position */
#define LSM6DSO32_TAP_CFG0_SLOPE_FDS_Msk                              ((0x1) << LSM6DSO32_TAP_CFG0_SLOPE_FDS_Pos)  /* (LSM6DSO32_TAP_CFG0) HPF or SLOPE filter selection on wake-up and Activity/Inactivity functions mask */
#define LSM6DSO32_TAP_CFG0_SLOPE_FDS(value)                           (LSM6DSO32_TAP_CFG0_SLOPE_FDS_Msk & ((value) << LSM6DSO32_TAP_CFG0_SLOPE_FDS_Pos))  /* Assignment of SLOPE_FDS value in LSM6DSO32_TAP_CFG0 register */
#define LSM6DSO32_TAP_CFG0_SLOPE_FDS_Get(reg)                         (((reg) & LSM6DSO32_TAP_CFG0_SLOPE_FDS_Msk) >> LSM6DSO32_TAP_CFG0_SLOPE_FDS_Pos)  /* Assignment of SLOPE_FDS value in LSM6DSO32_TAP_CFG0 register */
#define LSM6DSO32_TAP_CFG0_SLEEP_STATUS_ON_INT_Pos                    (5)  /* (LSM6DSO32_TAP_CFG0) Activity/inactivity interrupt mode configuration If INT1_SLEEP_CHANGE or INT2_SLEEP_CHANGE bits are enabled, drives the sleep status or sleep change on the INT pins position */
#define LSM6DSO32_TAP_CFG0_SLEEP_STATUS_ON_INT_Msk                    ((0x1) << LSM6DSO32_TAP_CFG0_SLEEP_STATUS_ON_INT_Pos)  /* (LSM6DSO32_TAP_CFG0) Activity/inactivity interrupt mode configuration If INT1_SLEEP_CHANGE or INT2_SLEEP_CHANGE bits are enabled, drives the sleep status or sleep change on the INT pins mask */
#define LSM6DSO32_TAP_CFG0_SLEEP_STATUS_ON_INT(value)                 (LSM6DSO32_TAP_CFG0_SLEEP_STATUS_ON_INT_Msk & ((value) << LSM6DSO32_TAP_CFG0_SLEEP_STATUS_ON_INT_Pos))  /* Assignment of SLEEP_STATUS_ON_INT value in LSM6DSO32_TAP_CFG0 register */
#define LSM6DSO32_TAP_CFG0_SLEEP_STATUS_ON_INT_Get(reg)               (((reg) & LSM6DSO32_TAP_CFG0_SLEEP_STATUS_ON_INT_Msk) >> LSM6DSO32_TAP_CFG0_SLEEP_STATUS_ON_INT_Pos)  /* Assignment of SLEEP_STATUS_ON_INT value in LSM6DSO32_TAP_CFG0 register */
#define LSM6DSO32_TAP_CFG0_INT_CLR_ON_READ_Pos                        (6)  /* (LSM6DSO32_TAP_CFG0) This bit allows immediately clearing the latched interrupts of an event detection upon the read of the corresponding status register. It must be set to 1 together with LIR position */
#define LSM6DSO32_TAP_CFG0_INT_CLR_ON_READ_Msk                        ((0x1) << LSM6DSO32_TAP_CFG0_INT_CLR_ON_READ_Pos)  /* (LSM6DSO32_TAP_CFG0) This bit allows immediately clearing the latched interrupts of an event detection upon the read of the corresponding status register. It must be set to 1 together with LIR mask */
#define LSM6DSO32_TAP_CFG0_INT_CLR_ON_READ(value)                     (LSM6DSO32_TAP_CFG0_INT_CLR_ON_READ_Msk & ((value) << LSM6DSO32_TAP_CFG0_INT_CLR_ON_READ_Pos))  /* Assignment of INT_CLR_ON_READ value in LSM6DSO32_TAP_CFG0 register */
#define LSM6DSO32_TAP_CFG0_INT_CLR_ON_READ_Get(reg)                   (((reg) & LSM6DSO32_TAP_CFG0_INT_CLR_ON_READ_Msk) >> LSM6DSO32_TAP_CFG0_INT_CLR_ON_READ_Pos)  /* Assignment of INT_CLR_ON_READ value in LSM6DSO32_TAP_CFG0 register */
#define LSM6DSO32_TAP_CFG0_Msk                                        (0x7f)  /* (LSM6DSO32_TAP_CFG0) Register mask */


/* LSM6DSO32_TAP_CFG1, R/W: Tap configuration register */
#define LSM6DSO32_TAP_CFG1_ADDR                                       (0x57)  /* (LSM6DSO32_TAP_CFG1) Tap configuration register address */

#define LSM6DSO32_TAP_CFG1_TAP_THS_X_Pos                              (0)  /* (LSM6DSO32_TAP_CFG1) X-axis tap recognition threshold. 1 LSB = FS_XL / (2^5) position */
#define LSM6DSO32_TAP_CFG1_TAP_THS_X_Msk                              ((0x1f) << LSM6DSO32_TAP_CFG1_TAP_THS_X_Pos)  /* (LSM6DSO32_TAP_CFG1) X-axis tap recognition threshold. 1 LSB = FS_XL / (2^5) mask */
#define LSM6DSO32_TAP_CFG1_TAP_THS_X(value)                           (LSM6DSO32_TAP_CFG1_TAP_THS_X_Msk & ((value) << LSM6DSO32_TAP_CFG1_TAP_THS_X_Pos))  /* Assignment of TAP_THS_X value in LSM6DSO32_TAP_CFG1 register */
#define LSM6DSO32_TAP_CFG1_TAP_THS_X_Get(reg)                         (((reg) & LSM6DSO32_TAP_CFG1_TAP_THS_X_Msk) >> LSM6DSO32_TAP_CFG1_TAP_THS_X_Pos)  /* Assignment of TAP_THS_X value in LSM6DSO32_TAP_CFG1 register */
#define LSM6DSO32_TAP_CFG1_TAP_PRIORITY_Pos                           (5)  /* (LSM6DSO32_TAP_CFG1) Selection of axis priority for TAP detection (highest to lowest) position */
#define LSM6DSO32_TAP_CFG1_TAP_PRIORITY_Msk                           ((0x7) << LSM6DSO32_TAP_CFG1_TAP_PRIORITY_Pos)  /* (LSM6DSO32_TAP_CFG1) Selection of axis priority for TAP detection (highest to lowest) mask */
#define LSM6DSO32_TAP_CFG1_TAP_PRIORITY(value)                        (LSM6DSO32_TAP_CFG1_TAP_PRIORITY_Msk & ((value) << LSM6DSO32_TAP_CFG1_TAP_PRIORITY_Pos))  /* Assignment of TAP_PRIORITY value in LSM6DSO32_TAP_CFG1 register */
#define LSM6DSO32_TAP_CFG1_TAP_PRIORITY_Get(reg)                      (((reg) & LSM6DSO32_TAP_CFG1_TAP_PRIORITY_Msk) >> LSM6DSO32_TAP_CFG1_TAP_PRIORITY_Pos)  /* Assignment of TAP_PRIORITY value in LSM6DSO32_TAP_CFG1 register */
#define   LSM6DSO32_TAP_CFG1_TAP_PRIORITY_XYZ_Val                     (0)  /* (LSM6DSO32_TAP_CFG1) XYZ value */
#define   LSM6DSO32_TAP_CFG1_TAP_PRIORITY_YXZ_Val                     (1)  /* (LSM6DSO32_TAP_CFG1) YXZ value */
#define   LSM6DSO32_TAP_CFG1_TAP_PRIORITY_XZY_Val                     (2)  /* (LSM6DSO32_TAP_CFG1) XZY value */
#define   LSM6DSO32_TAP_CFG1_TAP_PRIORITY_ZYX_Val                     (3)  /* (LSM6DSO32_TAP_CFG1) ZYX value */
#define   LSM6DSO32_TAP_CFG1_TAP_PRIORITY_YZX_Val                     (5)  /* (LSM6DSO32_TAP_CFG1) YZX value */
#define   LSM6DSO32_TAP_CFG1_TAP_PRIORITY_ZXY_Val                     (6)  /* (LSM6DSO32_TAP_CFG1) ZXY value */
#define LSM6DSO32_TAP_CFG1_TAP_PRIORITY_XYZ                           (LSM6DSO32_TAP_CFG1_TAP_PRIORITY_XYZ_Val << LSM6DSO32_TAP_CFG1_TAP_PRIORITY_Pos)  /* (LSM6DSO32_TAP_CFG1) XYZ */
#define LSM6DSO32_TAP_CFG1_TAP_PRIORITY_YXZ                           (LSM6DSO32_TAP_CFG1_TAP_PRIORITY_YXZ_Val << LSM6DSO32_TAP_CFG1_TAP_PRIORITY_Pos)  /* (LSM6DSO32_TAP_CFG1) YXZ */
#define LSM6DSO32_TAP_CFG1_TAP_PRIORITY_XZY                           (LSM6DSO32_TAP_CFG1_TAP_PRIORITY_XZY_Val << LSM6DSO32_TAP_CFG1_TAP_PRIORITY_Pos)  /* (LSM6DSO32_TAP_CFG1) XZY */
#define LSM6DSO32_TAP_CFG1_TAP_PRIORITY_ZYX                           (LSM6DSO32_TAP_CFG1_TAP_PRIORITY_ZYX_Val << LSM6DSO32_TAP_CFG1_TAP_PRIORITY_Pos)  /* (LSM6DSO32_TAP_CFG1) ZYX */
#define LSM6DSO32_TAP_CFG1_TAP_PRIORITY_YZX                           (LSM6DSO32_TAP_CFG1_TAP_PRIORITY_YZX_Val << LSM6DSO32_TAP_CFG1_TAP_PRIORITY_Pos)  /* (LSM6DSO32_TAP_CFG1) YZX */
#define LSM6DSO32_TAP_CFG1_TAP_PRIORITY_ZXY                           (LSM6DSO32_TAP_CFG1_TAP_PRIORITY_ZXY_Val << LSM6DSO32_TAP_CFG1_TAP_PRIORITY_Pos)  /* (LSM6DSO32_TAP_CFG1) ZXY */
#define LSM6DSO32_TAP_CFG1_Msk                                        (0xff)  /* (LSM6DSO32_TAP_CFG1) Register mask */


/* LSM6DSO32_TAP_CFG2, R/W: Enables interrupt and inactivity functions, and tap recognition functions */
#define LSM6DSO32_TAP_CFG2_ADDR                                       (0x58)  /* (LSM6DSO32_TAP_CFG2) Enables interrupt and inactivity functions, and tap recognition functions address */

#define LSM6DSO32_TAP_CFG2_TAP_THS_Y_Pos                              (0)  /* (LSM6DSO32_TAP_CFG2) Y-axis tap recognition threshold. 1 LSB = FS_XL / (2^5) position */
#define LSM6DSO32_TAP_CFG2_TAP_THS_Y_Msk                              ((0x1f) << LSM6DSO32_TAP_CFG2_TAP_THS_Y_Pos)  /* (LSM6DSO32_TAP_CFG2) Y-axis tap recognition threshold. 1 LSB = FS_XL / (2^5) mask */
#define LSM6DSO32_TAP_CFG2_TAP_THS_Y(value)                           (LSM6DSO32_TAP_CFG2_TAP_THS_Y_Msk & ((value) << LSM6DSO32_TAP_CFG2_TAP_THS_Y_Pos))  /* Assignment of TAP_THS_Y value in LSM6DSO32_TAP_CFG2 register */
#define LSM6DSO32_TAP_CFG2_TAP_THS_Y_Get(reg)                         (((reg) & LSM6DSO32_TAP_CFG2_TAP_THS_Y_Msk) >> LSM6DSO32_TAP_CFG2_TAP_THS_Y_Pos)  /* Assignment of TAP_THS_Y value in LSM6DSO32_TAP_CFG2 register */
#define LSM6DSO32_TAP_CFG2_INACT_EN_Pos                               (5)  /* (LSM6DSO32_TAP_CFG2) Enable activity/inactivity (sleep) function position */
#define LSM6DSO32_TAP_CFG2_INACT_EN_Msk                               ((0x3) << LSM6DSO32_TAP_CFG2_INACT_EN_Pos)  /* (LSM6DSO32_TAP_CFG2) Enable activity/inactivity (sleep) function mask */
#define LSM6DSO32_TAP_CFG2_INACT_EN(value)                            (LSM6DSO32_TAP_CFG2_INACT_EN_Msk & ((value) << LSM6DSO32_TAP_CFG2_INACT_EN_Pos))  /* Assignment of INACT_EN value in LSM6DSO32_TAP_CFG2 register */
#define LSM6DSO32_TAP_CFG2_INACT_EN_Get(reg)                          (((reg) & LSM6DSO32_TAP_CFG2_INACT_EN_Msk) >> LSM6DSO32_TAP_CFG2_INACT_EN_Pos)  /* Assignment of INACT_EN value in LSM6DSO32_TAP_CFG2 register */
#define   LSM6DSO32_TAP_CFG2_INACT_EN_OFF_Val                         (0)  /* (LSM6DSO32_TAP_CFG2) Stationary/motion-only interrupts generated, XL and gyro do not change value */
#define   LSM6DSO32_TAP_CFG2_INACT_EN_G_ON_Val                        (1)  /* (LSM6DSO32_TAP_CFG2) Sets accelerometer ODR to 12.5 Hz (low-power mode), gyro does not change value */
#define   LSM6DSO32_TAP_CFG2_INACT_EN_G_SLEEP_Val                     (2)  /* (LSM6DSO32_TAP_CFG2) Sets accelerometer ODR to 12.5 Hz (low-power mode), gyro to sleep mode value */
#define   LSM6DSO32_TAP_CFG2_INACT_EN_G_OFF_Val                       (3)  /* (LSM6DSO32_TAP_CFG2) sets accelerometer ODR to 12.5 Hz (low-power mode), gyro to power-down mode value */
#define LSM6DSO32_TAP_CFG2_INACT_EN_OFF                               (LSM6DSO32_TAP_CFG2_INACT_EN_OFF_Val << LSM6DSO32_TAP_CFG2_INACT_EN_Pos)  /* (LSM6DSO32_TAP_CFG2) Stationary/motion-only interrupts generated, XL and gyro do not change */
#define LSM6DSO32_TAP_CFG2_INACT_EN_G_ON                              (LSM6DSO32_TAP_CFG2_INACT_EN_G_ON_Val << LSM6DSO32_TAP_CFG2_INACT_EN_Pos)  /* (LSM6DSO32_TAP_CFG2) Sets accelerometer ODR to 12.5 Hz (low-power mode), gyro does not change */
#define LSM6DSO32_TAP_CFG2_INACT_EN_G_SLEEP                           (LSM6DSO32_TAP_CFG2_INACT_EN_G_SLEEP_Val << LSM6DSO32_TAP_CFG2_INACT_EN_Pos)  /* (LSM6DSO32_TAP_CFG2) Sets accelerometer ODR to 12.5 Hz (low-power mode), gyro to sleep mode */
#define LSM6DSO32_TAP_CFG2_INACT_EN_G_OFF                             (LSM6DSO32_TAP_CFG2_INACT_EN_G_OFF_Val << LSM6DSO32_TAP_CFG2_INACT_EN_Pos)  /* (LSM6DSO32_TAP_CFG2) sets accelerometer ODR to 12.5 Hz (low-power mode), gyro to power-down mode */
#define LSM6DSO32_TAP_CFG2_INT_ENABLE_Pos                             (7)  /* (LSM6DSO32_TAP_CFG2) Enable basic interrupts (6D/4D, free-fall, wake-up, tap, inactivity position */
#define LSM6DSO32_TAP_CFG2_INT_ENABLE_Msk                             ((0x1) << LSM6DSO32_TAP_CFG2_INT_ENABLE_Pos)  /* (LSM6DSO32_TAP_CFG2) Enable basic interrupts (6D/4D, free-fall, wake-up, tap, inactivity mask */
#define LSM6DSO32_TAP_CFG2_INT_ENABLE(value)                          (LSM6DSO32_TAP_CFG2_INT_ENABLE_Msk & ((value) << LSM6DSO32_TAP_CFG2_INT_ENABLE_Pos))  /* Assignment of INT_ENABLE value in LSM6DSO32_TAP_CFG2 register */
#define LSM6DSO32_TAP_CFG2_INT_ENABLE_Get(reg)                        (((reg) & LSM6DSO32_TAP_CFG2_INT_ENABLE_Msk) >> LSM6DSO32_TAP_CFG2_INT_ENABLE_Pos)  /* Assignment of INT_ENABLE value in LSM6DSO32_TAP_CFG2 register */
#define LSM6DSO32_TAP_CFG2_Msk                                        (0xff)  /* (LSM6DSO32_TAP_CFG2) Register mask */


/* LSM6DSO32_TAP_THS_6D, R/W: Portrait/landscape position and tap function threshold register */
#define LSM6DSO32_TAP_THS_6D_ADDR                                     (0x59)  /* (LSM6DSO32_TAP_THS_6D) Portrait/landscape position and tap function threshold register address */

#define LSM6DSO32_TAP_THS_6D_TAP_THS_Z_Pos                            (0)  /* (LSM6DSO32_TAP_THS_6D) Z-axis tap recognition threshold. 1 LSB = FS_XL / (2^5) position */
#define LSM6DSO32_TAP_THS_6D_TAP_THS_Z_Msk                            ((0x1f) << LSM6DSO32_TAP_THS_6D_TAP_THS_Z_Pos)  /* (LSM6DSO32_TAP_THS_6D) Z-axis tap recognition threshold. 1 LSB = FS_XL / (2^5) mask */
#define LSM6DSO32_TAP_THS_6D_TAP_THS_Z(value)                         (LSM6DSO32_TAP_THS_6D_TAP_THS_Z_Msk & ((value) << LSM6DSO32_TAP_THS_6D_TAP_THS_Z_Pos))  /* Assignment of TAP_THS_Z value in LSM6DSO32_TAP_THS_6D register */
#define LSM6DSO32_TAP_THS_6D_TAP_THS_Z_Get(reg)                       (((reg) & LSM6DSO32_TAP_THS_6D_TAP_THS_Z_Msk) >> LSM6DSO32_TAP_THS_6D_TAP_THS_Z_Pos)  /* Assignment of TAP_THS_Z value in LSM6DSO32_TAP_THS_6D register */
#define LSM6DSO32_TAP_THS_6D_SIXD_THS_Pos                             (5)  /* (LSM6DSO32_TAP_THS_6D) Threshold for 4D/6D function position */
#define LSM6DSO32_TAP_THS_6D_SIXD_THS_Msk                             ((0x3) << LSM6DSO32_TAP_THS_6D_SIXD_THS_Pos)  /* (LSM6DSO32_TAP_THS_6D) Threshold for 4D/6D function mask */
#define LSM6DSO32_TAP_THS_6D_SIXD_THS(value)                          (LSM6DSO32_TAP_THS_6D_SIXD_THS_Msk & ((value) << LSM6DSO32_TAP_THS_6D_SIXD_THS_Pos))  /* Assignment of SIXD_THS value in LSM6DSO32_TAP_THS_6D register */
#define LSM6DSO32_TAP_THS_6D_SIXD_THS_Get(reg)                        (((reg) & LSM6DSO32_TAP_THS_6D_SIXD_THS_Msk) >> LSM6DSO32_TAP_THS_6D_SIXD_THS_Pos)  /* Assignment of SIXD_THS value in LSM6DSO32_TAP_THS_6D register */
#define   LSM6DSO32_TAP_THS_6D_SIXD_THS_68deg_Val                     (0)  /* (LSM6DSO32_TAP_THS_6D) 68deg value */
#define   LSM6DSO32_TAP_THS_6D_SIXD_THS_47deg_Val                     (1)  /* (LSM6DSO32_TAP_THS_6D) 47deg value */
#define LSM6DSO32_TAP_THS_6D_SIXD_THS_68deg                           (LSM6DSO32_TAP_THS_6D_SIXD_THS_68deg_Val << LSM6DSO32_TAP_THS_6D_SIXD_THS_Pos)  /* (LSM6DSO32_TAP_THS_6D) 68deg */
#define LSM6DSO32_TAP_THS_6D_SIXD_THS_47deg                           (LSM6DSO32_TAP_THS_6D_SIXD_THS_47deg_Val << LSM6DSO32_TAP_THS_6D_SIXD_THS_Pos)  /* (LSM6DSO32_TAP_THS_6D) 47deg */
#define LSM6DSO32_TAP_THS_6D_D4D_EN_Pos                               (7)  /* (LSM6DSO32_TAP_THS_6D) 4D orientation detection enable. Z-axis position detection is disabled position */
#define LSM6DSO32_TAP_THS_6D_D4D_EN_Msk                               ((0x1) << LSM6DSO32_TAP_THS_6D_D4D_EN_Pos)  /* (LSM6DSO32_TAP_THS_6D) 4D orientation detection enable. Z-axis position detection is disabled mask */
#define LSM6DSO32_TAP_THS_6D_D4D_EN(value)                            (LSM6DSO32_TAP_THS_6D_D4D_EN_Msk & ((value) << LSM6DSO32_TAP_THS_6D_D4D_EN_Pos))  /* Assignment of D4D_EN value in LSM6DSO32_TAP_THS_6D register */
#define LSM6DSO32_TAP_THS_6D_D4D_EN_Get(reg)                          (((reg) & LSM6DSO32_TAP_THS_6D_D4D_EN_Msk) >> LSM6DSO32_TAP_THS_6D_D4D_EN_Pos)  /* Assignment of D4D_EN value in LSM6DSO32_TAP_THS_6D register */
#define LSM6DSO32_TAP_THS_6D_Msk                                      (0xff)  /* (LSM6DSO32_TAP_THS_6D) Register mask */


/* LSM6DSO32_INT_DUR2, R/W: Tap recognition function setting register */
#define LSM6DSO32_INT_DUR2_ADDR                                       (0x5a)  /* (LSM6DSO32_INT_DUR2) Tap recognition function setting register address */

#define LSM6DSO32_INT_DUR2_SHOCK_Pos                                  (0)  /* (LSM6DSO32_INT_DUR2) Maximum duration of overthreshold event position */
#define LSM6DSO32_INT_DUR2_SHOCK_Msk                                  ((0x3) << LSM6DSO32_INT_DUR2_SHOCK_Pos)  /* (LSM6DSO32_INT_DUR2) Maximum duration of overthreshold event mask */
#define LSM6DSO32_INT_DUR2_SHOCK(value)                               (LSM6DSO32_INT_DUR2_SHOCK_Msk & ((value) << LSM6DSO32_INT_DUR2_SHOCK_Pos))  /* Assignment of SHOCK value in LSM6DSO32_INT_DUR2 register */
#define LSM6DSO32_INT_DUR2_SHOCK_Get(reg)                             (((reg) & LSM6DSO32_INT_DUR2_SHOCK_Msk) >> LSM6DSO32_INT_DUR2_SHOCK_Pos)  /* Assignment of SHOCK value in LSM6DSO32_INT_DUR2 register */
#define LSM6DSO32_INT_DUR2_QUIET_Pos                                  (2)  /* (LSM6DSO32_INT_DUR2) Expected quiet time after a tap detection position */
#define LSM6DSO32_INT_DUR2_QUIET_Msk                                  ((0x3) << LSM6DSO32_INT_DUR2_QUIET_Pos)  /* (LSM6DSO32_INT_DUR2) Expected quiet time after a tap detection mask */
#define LSM6DSO32_INT_DUR2_QUIET(value)                               (LSM6DSO32_INT_DUR2_QUIET_Msk & ((value) << LSM6DSO32_INT_DUR2_QUIET_Pos))  /* Assignment of QUIET value in LSM6DSO32_INT_DUR2 register */
#define LSM6DSO32_INT_DUR2_QUIET_Get(reg)                             (((reg) & LSM6DSO32_INT_DUR2_QUIET_Msk) >> LSM6DSO32_INT_DUR2_QUIET_Pos)  /* Assignment of QUIET value in LSM6DSO32_INT_DUR2 register */
#define LSM6DSO32_INT_DUR2_DUR_Pos                                    (4)  /* (LSM6DSO32_INT_DUR2) Duration of maximum time gap for double tap recognition position */
#define LSM6DSO32_INT_DUR2_DUR_Msk                                    ((0xf) << LSM6DSO32_INT_DUR2_DUR_Pos)  /* (LSM6DSO32_INT_DUR2) Duration of maximum time gap for double tap recognition mask */
#define LSM6DSO32_INT_DUR2_DUR(value)                                 (LSM6DSO32_INT_DUR2_DUR_Msk & ((value) << LSM6DSO32_INT_DUR2_DUR_Pos))  /* Assignment of DUR value in LSM6DSO32_INT_DUR2 register */
#define LSM6DSO32_INT_DUR2_DUR_Get(reg)                               (((reg) & LSM6DSO32_INT_DUR2_DUR_Msk) >> LSM6DSO32_INT_DUR2_DUR_Pos)  /* Assignment of DUR value in LSM6DSO32_INT_DUR2 register */
#define LSM6DSO32_INT_DUR2_Msk                                        (0xff)  /* (LSM6DSO32_INT_DUR2) Register mask */


/* LSM6DSO32_WAKE_UP_THS, R/W: Single/double-tap selection and wake-up configuration */
#define LSM6DSO32_WAKE_UP_THS_ADDR                                    (0x5b)  /* (LSM6DSO32_WAKE_UP_THS) Single/double-tap selection and wake-up configuration address */

#define LSM6DSO32_WAKE_UP_THS_WK_THS_Pos                              (0)  /* (LSM6DSO32_WAKE_UP_THS) Threshold for wakeup. LSB weight depends on WAKE_THS_W in WAKE_UP_DUR (5Ch) position */
#define LSM6DSO32_WAKE_UP_THS_WK_THS_Msk                              ((0x3f) << LSM6DSO32_WAKE_UP_THS_WK_THS_Pos)  /* (LSM6DSO32_WAKE_UP_THS) Threshold for wakeup. LSB weight depends on WAKE_THS_W in WAKE_UP_DUR (5Ch) mask */
#define LSM6DSO32_WAKE_UP_THS_WK_THS(value)                           (LSM6DSO32_WAKE_UP_THS_WK_THS_Msk & ((value) << LSM6DSO32_WAKE_UP_THS_WK_THS_Pos))  /* Assignment of WK_THS value in LSM6DSO32_WAKE_UP_THS register */
#define LSM6DSO32_WAKE_UP_THS_WK_THS_Get(reg)                         (((reg) & LSM6DSO32_WAKE_UP_THS_WK_THS_Msk) >> LSM6DSO32_WAKE_UP_THS_WK_THS_Pos)  /* Assignment of WK_THS value in LSM6DSO32_WAKE_UP_THS register */
#define LSM6DSO32_WAKE_UP_THS_USR_OFF_ON_WU_Pos                       (6)  /* (LSM6DSO32_WAKE_UP_THS) Drives the low-pass filtered data with user offset correction (instead of high-pass filtered data) to the wakeup function position */
#define LSM6DSO32_WAKE_UP_THS_USR_OFF_ON_WU_Msk                       ((0x1) << LSM6DSO32_WAKE_UP_THS_USR_OFF_ON_WU_Pos)  /* (LSM6DSO32_WAKE_UP_THS) Drives the low-pass filtered data with user offset correction (instead of high-pass filtered data) to the wakeup function mask */
#define LSM6DSO32_WAKE_UP_THS_USR_OFF_ON_WU(value)                    (LSM6DSO32_WAKE_UP_THS_USR_OFF_ON_WU_Msk & ((value) << LSM6DSO32_WAKE_UP_THS_USR_OFF_ON_WU_Pos))  /* Assignment of USR_OFF_ON_WU value in LSM6DSO32_WAKE_UP_THS register */
#define LSM6DSO32_WAKE_UP_THS_USR_OFF_ON_WU_Get(reg)                  (((reg) & LSM6DSO32_WAKE_UP_THS_USR_OFF_ON_WU_Msk) >> LSM6DSO32_WAKE_UP_THS_USR_OFF_ON_WU_Pos)  /* Assignment of USR_OFF_ON_WU value in LSM6DSO32_WAKE_UP_THS register */
#define LSM6DSO32_WAKE_UP_THS_SINGLE_DOUBLE_TAP_Pos                   (7)  /* (LSM6DSO32_WAKE_UP_THS) Single/double tap event enable position */
#define LSM6DSO32_WAKE_UP_THS_SINGLE_DOUBLE_TAP_Msk                   ((0x1) << LSM6DSO32_WAKE_UP_THS_SINGLE_DOUBLE_TAP_Pos)  /* (LSM6DSO32_WAKE_UP_THS) Single/double tap event enable mask */
#define LSM6DSO32_WAKE_UP_THS_SINGLE_DOUBLE_TAP(value)                (LSM6DSO32_WAKE_UP_THS_SINGLE_DOUBLE_TAP_Msk & ((value) << LSM6DSO32_WAKE_UP_THS_SINGLE_DOUBLE_TAP_Pos))  /* Assignment of SINGLE_DOUBLE_TAP value in LSM6DSO32_WAKE_UP_THS register */
#define LSM6DSO32_WAKE_UP_THS_SINGLE_DOUBLE_TAP_Get(reg)              (((reg) & LSM6DSO32_WAKE_UP_THS_SINGLE_DOUBLE_TAP_Msk) >> LSM6DSO32_WAKE_UP_THS_SINGLE_DOUBLE_TAP_Pos)  /* Assignment of SINGLE_DOUBLE_TAP value in LSM6DSO32_WAKE_UP_THS register */
#define LSM6DSO32_WAKE_UP_THS_Msk                                     (0xff)  /* (LSM6DSO32_WAKE_UP_THS) Register mask */


/* LSM6DSO32_WAKE_UP_DUR, R/W: Free-fall, wakeup and sleep mode functions duration setting register */
#define LSM6DSO32_WAKE_UP_DUR_ADDR                                    (0x5c)  /* (LSM6DSO32_WAKE_UP_DUR) Free-fall, wakeup and sleep mode functions duration setting register address */

#define LSM6DSO32_WAKE_UP_DUR_SLEEP_DUR_Pos                           (0)  /* (LSM6DSO32_WAKE_UP_DUR) Duration to go in sleep mode. Default value: 0000 (this corresponds to 16 ODR). 1 LSB = 512 ODR position */
#define LSM6DSO32_WAKE_UP_DUR_SLEEP_DUR_Msk                           ((0xf) << LSM6DSO32_WAKE_UP_DUR_SLEEP_DUR_Pos)  /* (LSM6DSO32_WAKE_UP_DUR) Duration to go in sleep mode. Default value: 0000 (this corresponds to 16 ODR). 1 LSB = 512 ODR mask */
#define LSM6DSO32_WAKE_UP_DUR_SLEEP_DUR(value)                        (LSM6DSO32_WAKE_UP_DUR_SLEEP_DUR_Msk & ((value) << LSM6DSO32_WAKE_UP_DUR_SLEEP_DUR_Pos))  /* Assignment of SLEEP_DUR value in LSM6DSO32_WAKE_UP_DUR register */
#define LSM6DSO32_WAKE_UP_DUR_SLEEP_DUR_Get(reg)                      (((reg) & LSM6DSO32_WAKE_UP_DUR_SLEEP_DUR_Msk) >> LSM6DSO32_WAKE_UP_DUR_SLEEP_DUR_Pos)  /* Assignment of SLEEP_DUR value in LSM6DSO32_WAKE_UP_DUR register */
#define LSM6DSO32_WAKE_UP_DUR_WAKE_UP_THS_W_Pos                       (4)  /* (LSM6DSO32_WAKE_UP_DUR) Weight of 1 LSB of wakeup threshold position */
#define LSM6DSO32_WAKE_UP_DUR_WAKE_UP_THS_W_Msk                       ((0x1) << LSM6DSO32_WAKE_UP_DUR_WAKE_UP_THS_W_Pos)  /* (LSM6DSO32_WAKE_UP_DUR) Weight of 1 LSB of wakeup threshold mask */
#define LSM6DSO32_WAKE_UP_DUR_WAKE_UP_THS_W(value)                    (LSM6DSO32_WAKE_UP_DUR_WAKE_UP_THS_W_Msk & ((value) << LSM6DSO32_WAKE_UP_DUR_WAKE_UP_THS_W_Pos))  /* Assignment of WAKE_UP_THS_W value in LSM6DSO32_WAKE_UP_DUR register */
#define LSM6DSO32_WAKE_UP_DUR_WAKE_UP_THS_W_Get(reg)                  (((reg) & LSM6DSO32_WAKE_UP_DUR_WAKE_UP_THS_W_Msk) >> LSM6DSO32_WAKE_UP_DUR_WAKE_UP_THS_W_Pos)  /* Assignment of WAKE_UP_THS_W value in LSM6DSO32_WAKE_UP_DUR register */
#define   LSM6DSO32_WAKE_UP_DUR_WAKE_UP_THS_W_6_Val                   (0)  /* (LSM6DSO32_WAKE_UP_DUR) 1 LSB = FS_XL / (2^6) value */
#define   LSM6DSO32_WAKE_UP_DUR_WAKE_UP_THS_W_8_Val                   (1)  /* (LSM6DSO32_WAKE_UP_DUR) 1 LSB = FS_XL / (2^8) value */
#define LSM6DSO32_WAKE_UP_DUR_WAKE_UP_THS_W_6                         (LSM6DSO32_WAKE_UP_DUR_WAKE_UP_THS_W_6_Val << LSM6DSO32_WAKE_UP_DUR_WAKE_UP_THS_W_Pos)  /* (LSM6DSO32_WAKE_UP_DUR) 1 LSB = FS_XL / (2^6) */
#define LSM6DSO32_WAKE_UP_DUR_WAKE_UP_THS_W_8                         (LSM6DSO32_WAKE_UP_DUR_WAKE_UP_THS_W_8_Val << LSM6DSO32_WAKE_UP_DUR_WAKE_UP_THS_W_Pos)  /* (LSM6DSO32_WAKE_UP_DUR) 1 LSB = FS_XL / (2^8) */
#define LSM6DSO32_WAKE_UP_DUR_WAKE_DUR_Pos                            (5)  /* (LSM6DSO32_WAKE_UP_DUR) Wake up duration event. 1LSB = 1 ODR_time position */
#define LSM6DSO32_WAKE_UP_DUR_WAKE_DUR_Msk                            ((0x3) << LSM6DSO32_WAKE_UP_DUR_WAKE_DUR_Pos)  /* (LSM6DSO32_WAKE_UP_DUR) Wake up duration event. 1LSB = 1 ODR_time mask */
#define LSM6DSO32_WAKE_UP_DUR_WAKE_DUR(value)                         (LSM6DSO32_WAKE_UP_DUR_WAKE_DUR_Msk & ((value) << LSM6DSO32_WAKE_UP_DUR_WAKE_DUR_Pos))  /* Assignment of WAKE_DUR value in LSM6DSO32_WAKE_UP_DUR register */
#define LSM6DSO32_WAKE_UP_DUR_WAKE_DUR_Get(reg)                       (((reg) & LSM6DSO32_WAKE_UP_DUR_WAKE_DUR_Msk) >> LSM6DSO32_WAKE_UP_DUR_WAKE_DUR_Pos)  /* Assignment of WAKE_DUR value in LSM6DSO32_WAKE_UP_DUR register */
#define LSM6DSO32_WAKE_UP_DUR_FF_DUR5_Pos                             (7)  /* (LSM6DSO32_WAKE_UP_DUR) Free fall duration event For the complete configuration of the free-fall duration, refer to FF_DUR[4:0] in FREE_FALL (5Dh) configuration 1 LSB = 1 ODR_time position */
#define LSM6DSO32_WAKE_UP_DUR_FF_DUR5_Msk                             ((0x1) << LSM6DSO32_WAKE_UP_DUR_FF_DUR5_Pos)  /* (LSM6DSO32_WAKE_UP_DUR) Free fall duration event For the complete configuration of the free-fall duration, refer to FF_DUR[4:0] in FREE_FALL (5Dh) configuration 1 LSB = 1 ODR_time mask */
#define LSM6DSO32_WAKE_UP_DUR_FF_DUR5(value)                          (LSM6DSO32_WAKE_UP_DUR_FF_DUR5_Msk & ((value) << LSM6DSO32_WAKE_UP_DUR_FF_DUR5_Pos))  /* Assignment of FF_DUR5 value in LSM6DSO32_WAKE_UP_DUR register */
#define LSM6DSO32_WAKE_UP_DUR_FF_DUR5_Get(reg)                        (((reg) & LSM6DSO32_WAKE_UP_DUR_FF_DUR5_Msk) >> LSM6DSO32_WAKE_UP_DUR_FF_DUR5_Pos)  /* Assignment of FF_DUR5 value in LSM6DSO32_WAKE_UP_DUR register */
#define LSM6DSO32_WAKE_UP_DUR_Msk                                     (0xff)  /* (LSM6DSO32_WAKE_UP_DUR) Register mask */


/* LSM6DSO32_FREE_FALL, R/W: Free-fall function duration setting register */
#define LSM6DSO32_FREE_FALL_ADDR                                      (0x5d)  /* (LSM6DSO32_FREE_FALL) Free-fall function duration setting register address */

#define LSM6DSO32_FREE_FALL_FF_THS_Pos                                (0)  /* (LSM6DSO32_FREE_FALL) Free fall threshold setting position */
#define LSM6DSO32_FREE_FALL_FF_THS_Msk                                ((0x7) << LSM6DSO32_FREE_FALL_FF_THS_Pos)  /* (LSM6DSO32_FREE_FALL) Free fall threshold setting mask */
#define LSM6DSO32_FREE_FALL_FF_THS(value)                             (LSM6DSO32_FREE_FALL_FF_THS_Msk & ((value) << LSM6DSO32_FREE_FALL_FF_THS_Pos))  /* Assignment of FF_THS value in LSM6DSO32_FREE_FALL register */
#define LSM6DSO32_FREE_FALL_FF_THS_Get(reg)                           (((reg) & LSM6DSO32_FREE_FALL_FF_THS_Msk) >> LSM6DSO32_FREE_FALL_FF_THS_Pos)  /* Assignment of FF_THS value in LSM6DSO32_FREE_FALL register */
#define   LSM6DSO32_FREE_FALL_FF_THS_312mg_Val                        (0)  /* (LSM6DSO32_FREE_FALL) 312mg value */
#define   LSM6DSO32_FREE_FALL_FF_THS_438mg_Val                        (1)  /* (LSM6DSO32_FREE_FALL) 438mg value */
#define   LSM6DSO32_FREE_FALL_FF_THS_500mg_Val                        (2)  /* (LSM6DSO32_FREE_FALL) 500mg value */
#define LSM6DSO32_FREE_FALL_FF_THS_312mg                              (LSM6DSO32_FREE_FALL_FF_THS_312mg_Val << LSM6DSO32_FREE_FALL_FF_THS_Pos)  /* (LSM6DSO32_FREE_FALL) 312mg */
#define LSM6DSO32_FREE_FALL_FF_THS_438mg                              (LSM6DSO32_FREE_FALL_FF_THS_438mg_Val << LSM6DSO32_FREE_FALL_FF_THS_Pos)  /* (LSM6DSO32_FREE_FALL) 438mg */
#define LSM6DSO32_FREE_FALL_FF_THS_500mg                              (LSM6DSO32_FREE_FALL_FF_THS_500mg_Val << LSM6DSO32_FREE_FALL_FF_THS_Pos)  /* (LSM6DSO32_FREE_FALL) 500mg */
#define LSM6DSO32_FREE_FALL_FF_DUR_Pos                                (3)  /* (LSM6DSO32_FREE_FALL) Free fall duration event position */
#define LSM6DSO32_FREE_FALL_FF_DUR_Msk                                ((0x1f) << LSM6DSO32_FREE_FALL_FF_DUR_Pos)  /* (LSM6DSO32_FREE_FALL) Free fall duration event mask */
#define LSM6DSO32_FREE_FALL_FF_DUR(value)                             (LSM6DSO32_FREE_FALL_FF_DUR_Msk & ((value) << LSM6DSO32_FREE_FALL_FF_DUR_Pos))  /* Assignment of FF_DUR value in LSM6DSO32_FREE_FALL register */
#define LSM6DSO32_FREE_FALL_FF_DUR_Get(reg)                           (((reg) & LSM6DSO32_FREE_FALL_FF_DUR_Msk) >> LSM6DSO32_FREE_FALL_FF_DUR_Pos)  /* Assignment of FF_DUR value in LSM6DSO32_FREE_FALL register */
#define LSM6DSO32_FREE_FALL_Msk                                       (0xff)  /* (LSM6DSO32_FREE_FALL) Register mask */


/* LSM6DSO32_MD1_CFG, R/W: Functions routing on INT1 register */
#define LSM6DSO32_MD1_CFG_ADDR                                        (0x5e)  /* (LSM6DSO32_MD1_CFG) Functions routing on INT1 register address */

#define LSM6DSO32_MD1_CFG_INT1_SHUB_Pos                               (0)  /* (LSM6DSO32_MD1_CFG) Routing of sensor hub communication concluded event on INT1 position */
#define LSM6DSO32_MD1_CFG_INT1_SHUB_Msk                               ((0x1) << LSM6DSO32_MD1_CFG_INT1_SHUB_Pos)  /* (LSM6DSO32_MD1_CFG) Routing of sensor hub communication concluded event on INT1 mask */
#define LSM6DSO32_MD1_CFG_INT1_SHUB(value)                            (LSM6DSO32_MD1_CFG_INT1_SHUB_Msk & ((value) << LSM6DSO32_MD1_CFG_INT1_SHUB_Pos))  /* Assignment of INT1_SHUB value in LSM6DSO32_MD1_CFG register */
#define LSM6DSO32_MD1_CFG_INT1_SHUB_Get(reg)                          (((reg) & LSM6DSO32_MD1_CFG_INT1_SHUB_Msk) >> LSM6DSO32_MD1_CFG_INT1_SHUB_Pos)  /* Assignment of INT1_SHUB value in LSM6DSO32_MD1_CFG register */
#define LSM6DSO32_MD1_CFG_INT1_EMB_FUNC_Pos                           (1)  /* (LSM6DSO32_MD1_CFG) Routing of embedded functions event on INT1 position */
#define LSM6DSO32_MD1_CFG_INT1_EMB_FUNC_Msk                           ((0x1) << LSM6DSO32_MD1_CFG_INT1_EMB_FUNC_Pos)  /* (LSM6DSO32_MD1_CFG) Routing of embedded functions event on INT1 mask */
#define LSM6DSO32_MD1_CFG_INT1_EMB_FUNC(value)                        (LSM6DSO32_MD1_CFG_INT1_EMB_FUNC_Msk & ((value) << LSM6DSO32_MD1_CFG_INT1_EMB_FUNC_Pos))  /* Assignment of INT1_EMB_FUNC value in LSM6DSO32_MD1_CFG register */
#define LSM6DSO32_MD1_CFG_INT1_EMB_FUNC_Get(reg)                      (((reg) & LSM6DSO32_MD1_CFG_INT1_EMB_FUNC_Msk) >> LSM6DSO32_MD1_CFG_INT1_EMB_FUNC_Pos)  /* Assignment of INT1_EMB_FUNC value in LSM6DSO32_MD1_CFG register */
#define LSM6DSO32_MD1_CFG_INT1_6D_Pos                                 (2)  /* (LSM6DSO32_MD1_CFG) Routing of 6D event on INT1 position */
#define LSM6DSO32_MD1_CFG_INT1_6D_Msk                                 ((0x1) << LSM6DSO32_MD1_CFG_INT1_6D_Pos)  /* (LSM6DSO32_MD1_CFG) Routing of 6D event on INT1 mask */
#define LSM6DSO32_MD1_CFG_INT1_6D(value)                              (LSM6DSO32_MD1_CFG_INT1_6D_Msk & ((value) << LSM6DSO32_MD1_CFG_INT1_6D_Pos))  /* Assignment of INT1_6D value in LSM6DSO32_MD1_CFG register */
#define LSM6DSO32_MD1_CFG_INT1_6D_Get(reg)                            (((reg) & LSM6DSO32_MD1_CFG_INT1_6D_Msk) >> LSM6DSO32_MD1_CFG_INT1_6D_Pos)  /* Assignment of INT1_6D value in LSM6DSO32_MD1_CFG register */
#define LSM6DSO32_MD1_CFG_INT1_DOUBLE_TAP_Pos                         (3)  /* (LSM6DSO32_MD1_CFG) Routing of tap event on INT1 position */
#define LSM6DSO32_MD1_CFG_INT1_DOUBLE_TAP_Msk                         ((0x1) << LSM6DSO32_MD1_CFG_INT1_DOUBLE_TAP_Pos)  /* (LSM6DSO32_MD1_CFG) Routing of tap event on INT1 mask */
#define LSM6DSO32_MD1_CFG_INT1_DOUBLE_TAP(value)                      (LSM6DSO32_MD1_CFG_INT1_DOUBLE_TAP_Msk & ((value) << LSM6DSO32_MD1_CFG_INT1_DOUBLE_TAP_Pos))  /* Assignment of INT1_DOUBLE_TAP value in LSM6DSO32_MD1_CFG register */
#define LSM6DSO32_MD1_CFG_INT1_DOUBLE_TAP_Get(reg)                    (((reg) & LSM6DSO32_MD1_CFG_INT1_DOUBLE_TAP_Msk) >> LSM6DSO32_MD1_CFG_INT1_DOUBLE_TAP_Pos)  /* Assignment of INT1_DOUBLE_TAP value in LSM6DSO32_MD1_CFG register */
#define LSM6DSO32_MD1_CFG_INT1_FF_Pos                                 (4)  /* (LSM6DSO32_MD1_CFG) Routing of free-fall event on INT1 position */
#define LSM6DSO32_MD1_CFG_INT1_FF_Msk                                 ((0x1) << LSM6DSO32_MD1_CFG_INT1_FF_Pos)  /* (LSM6DSO32_MD1_CFG) Routing of free-fall event on INT1 mask */
#define LSM6DSO32_MD1_CFG_INT1_FF(value)                              (LSM6DSO32_MD1_CFG_INT1_FF_Msk & ((value) << LSM6DSO32_MD1_CFG_INT1_FF_Pos))  /* Assignment of INT1_FF value in LSM6DSO32_MD1_CFG register */
#define LSM6DSO32_MD1_CFG_INT1_FF_Get(reg)                            (((reg) & LSM6DSO32_MD1_CFG_INT1_FF_Msk) >> LSM6DSO32_MD1_CFG_INT1_FF_Pos)  /* Assignment of INT1_FF value in LSM6DSO32_MD1_CFG register */
#define LSM6DSO32_MD1_CFG_INT1_WU_Pos                                 (5)  /* (LSM6DSO32_MD1_CFG) Routing of wakeup event on INT1 position */
#define LSM6DSO32_MD1_CFG_INT1_WU_Msk                                 ((0x1) << LSM6DSO32_MD1_CFG_INT1_WU_Pos)  /* (LSM6DSO32_MD1_CFG) Routing of wakeup event on INT1 mask */
#define LSM6DSO32_MD1_CFG_INT1_WU(value)                              (LSM6DSO32_MD1_CFG_INT1_WU_Msk & ((value) << LSM6DSO32_MD1_CFG_INT1_WU_Pos))  /* Assignment of INT1_WU value in LSM6DSO32_MD1_CFG register */
#define LSM6DSO32_MD1_CFG_INT1_WU_Get(reg)                            (((reg) & LSM6DSO32_MD1_CFG_INT1_WU_Msk) >> LSM6DSO32_MD1_CFG_INT1_WU_Pos)  /* Assignment of INT1_WU value in LSM6DSO32_MD1_CFG register */
#define LSM6DSO32_MD1_CFG_INT1_SINGLE_TAP_Pos                         (6)  /* (LSM6DSO32_MD1_CFG) Routing of single-tap recognition event on INT1 position */
#define LSM6DSO32_MD1_CFG_INT1_SINGLE_TAP_Msk                         ((0x1) << LSM6DSO32_MD1_CFG_INT1_SINGLE_TAP_Pos)  /* (LSM6DSO32_MD1_CFG) Routing of single-tap recognition event on INT1 mask */
#define LSM6DSO32_MD1_CFG_INT1_SINGLE_TAP(value)                      (LSM6DSO32_MD1_CFG_INT1_SINGLE_TAP_Msk & ((value) << LSM6DSO32_MD1_CFG_INT1_SINGLE_TAP_Pos))  /* Assignment of INT1_SINGLE_TAP value in LSM6DSO32_MD1_CFG register */
#define LSM6DSO32_MD1_CFG_INT1_SINGLE_TAP_Get(reg)                    (((reg) & LSM6DSO32_MD1_CFG_INT1_SINGLE_TAP_Msk) >> LSM6DSO32_MD1_CFG_INT1_SINGLE_TAP_Pos)  /* Assignment of INT1_SINGLE_TAP value in LSM6DSO32_MD1_CFG register */
#define LSM6DSO32_MD1_CFG_INT1_SLEEP_CHANGE_Pos                       (7)  /* (LSM6DSO32_MD1_CFG) Routing of activity/inactivity recognition event on INT1 position */
#define LSM6DSO32_MD1_CFG_INT1_SLEEP_CHANGE_Msk                       ((0x1) << LSM6DSO32_MD1_CFG_INT1_SLEEP_CHANGE_Pos)  /* (LSM6DSO32_MD1_CFG) Routing of activity/inactivity recognition event on INT1 mask */
#define LSM6DSO32_MD1_CFG_INT1_SLEEP_CHANGE(value)                    (LSM6DSO32_MD1_CFG_INT1_SLEEP_CHANGE_Msk & ((value) << LSM6DSO32_MD1_CFG_INT1_SLEEP_CHANGE_Pos))  /* Assignment of INT1_SLEEP_CHANGE value in LSM6DSO32_MD1_CFG register */
#define LSM6DSO32_MD1_CFG_INT1_SLEEP_CHANGE_Get(reg)                  (((reg) & LSM6DSO32_MD1_CFG_INT1_SLEEP_CHANGE_Msk) >> LSM6DSO32_MD1_CFG_INT1_SLEEP_CHANGE_Pos)  /* Assignment of INT1_SLEEP_CHANGE value in LSM6DSO32_MD1_CFG register */
#define LSM6DSO32_MD1_CFG_Msk                                         (0xff)  /* (LSM6DSO32_MD1_CFG) Register mask */


/* LSM6DSO32_MD2_CFG, R/W: Functions routing on INT2 register */
#define LSM6DSO32_MD2_CFG_ADDR                                        (0x5f)  /* (LSM6DSO32_MD2_CFG) Functions routing on INT2 register address */

#define LSM6DSO32_MD2_CFG_INT2_SHUB_Pos                               (0)  /* (LSM6DSO32_MD2_CFG) Routing of sensor hub communication concluded event on INT2 position */
#define LSM6DSO32_MD2_CFG_INT2_SHUB_Msk                               ((0x1) << LSM6DSO32_MD2_CFG_INT2_SHUB_Pos)  /* (LSM6DSO32_MD2_CFG) Routing of sensor hub communication concluded event on INT2 mask */
#define LSM6DSO32_MD2_CFG_INT2_SHUB(value)                            (LSM6DSO32_MD2_CFG_INT2_SHUB_Msk & ((value) << LSM6DSO32_MD2_CFG_INT2_SHUB_Pos))  /* Assignment of INT2_SHUB value in LSM6DSO32_MD2_CFG register */
#define LSM6DSO32_MD2_CFG_INT2_SHUB_Get(reg)                          (((reg) & LSM6DSO32_MD2_CFG_INT2_SHUB_Msk) >> LSM6DSO32_MD2_CFG_INT2_SHUB_Pos)  /* Assignment of INT2_SHUB value in LSM6DSO32_MD2_CFG register */
#define LSM6DSO32_MD2_CFG_INT2_EMB_FUNC_Pos                           (1)  /* (LSM6DSO32_MD2_CFG) Routing of embedded functions event on INT2 position */
#define LSM6DSO32_MD2_CFG_INT2_EMB_FUNC_Msk                           ((0x1) << LSM6DSO32_MD2_CFG_INT2_EMB_FUNC_Pos)  /* (LSM6DSO32_MD2_CFG) Routing of embedded functions event on INT2 mask */
#define LSM6DSO32_MD2_CFG_INT2_EMB_FUNC(value)                        (LSM6DSO32_MD2_CFG_INT2_EMB_FUNC_Msk & ((value) << LSM6DSO32_MD2_CFG_INT2_EMB_FUNC_Pos))  /* Assignment of INT2_EMB_FUNC value in LSM6DSO32_MD2_CFG register */
#define LSM6DSO32_MD2_CFG_INT2_EMB_FUNC_Get(reg)                      (((reg) & LSM6DSO32_MD2_CFG_INT2_EMB_FUNC_Msk) >> LSM6DSO32_MD2_CFG_INT2_EMB_FUNC_Pos)  /* Assignment of INT2_EMB_FUNC value in LSM6DSO32_MD2_CFG register */
#define LSM6DSO32_MD2_CFG_INT2_6D_Pos                                 (2)  /* (LSM6DSO32_MD2_CFG) Routing of 6D event on INT2 position */
#define LSM6DSO32_MD2_CFG_INT2_6D_Msk                                 ((0x1) << LSM6DSO32_MD2_CFG_INT2_6D_Pos)  /* (LSM6DSO32_MD2_CFG) Routing of 6D event on INT2 mask */
#define LSM6DSO32_MD2_CFG_INT2_6D(value)                              (LSM6DSO32_MD2_CFG_INT2_6D_Msk & ((value) << LSM6DSO32_MD2_CFG_INT2_6D_Pos))  /* Assignment of INT2_6D value in LSM6DSO32_MD2_CFG register */
#define LSM6DSO32_MD2_CFG_INT2_6D_Get(reg)                            (((reg) & LSM6DSO32_MD2_CFG_INT2_6D_Msk) >> LSM6DSO32_MD2_CFG_INT2_6D_Pos)  /* Assignment of INT2_6D value in LSM6DSO32_MD2_CFG register */
#define LSM6DSO32_MD2_CFG_INT2_DOUBLE_TAP_Pos                         (3)  /* (LSM6DSO32_MD2_CFG) Routing of tap event on INT2 position */
#define LSM6DSO32_MD2_CFG_INT2_DOUBLE_TAP_Msk                         ((0x1) << LSM6DSO32_MD2_CFG_INT2_DOUBLE_TAP_Pos)  /* (LSM6DSO32_MD2_CFG) Routing of tap event on INT2 mask */
#define LSM6DSO32_MD2_CFG_INT2_DOUBLE_TAP(value)                      (LSM6DSO32_MD2_CFG_INT2_DOUBLE_TAP_Msk & ((value) << LSM6DSO32_MD2_CFG_INT2_DOUBLE_TAP_Pos))  /* Assignment of INT2_DOUBLE_TAP value in LSM6DSO32_MD2_CFG register */
#define LSM6DSO32_MD2_CFG_INT2_DOUBLE_TAP_Get(reg)                    (((reg) & LSM6DSO32_MD2_CFG_INT2_DOUBLE_TAP_Msk) >> LSM6DSO32_MD2_CFG_INT2_DOUBLE_TAP_Pos)  /* Assignment of INT2_DOUBLE_TAP value in LSM6DSO32_MD2_CFG register */
#define LSM6DSO32_MD2_CFG_INT2_FF_Pos                                 (4)  /* (LSM6DSO32_MD2_CFG) Routing of free-fall event on INT2 position */
#define LSM6DSO32_MD2_CFG_INT2_FF_Msk                                 ((0x1) << LSM6DSO32_MD2_CFG_INT2_FF_Pos)  /* (LSM6DSO32_MD2_CFG) Routing of free-fall event on INT2 mask */
#define LSM6DSO32_MD2_CFG_INT2_FF(value)                              (LSM6DSO32_MD2_CFG_INT2_FF_Msk & ((value) << LSM6DSO32_MD2_CFG_INT2_FF_Pos))  /* Assignment of INT2_FF value in LSM6DSO32_MD2_CFG register */
#define LSM6DSO32_MD2_CFG_INT2_FF_Get(reg)                            (((reg) & LSM6DSO32_MD2_CFG_INT2_FF_Msk) >> LSM6DSO32_MD2_CFG_INT2_FF_Pos)  /* Assignment of INT2_FF value in LSM6DSO32_MD2_CFG register */
#define LSM6DSO32_MD2_CFG_INT2_WU_Pos                                 (5)  /* (LSM6DSO32_MD2_CFG) Routing of wakeup event on INT2 position */
#define LSM6DSO32_MD2_CFG_INT2_WU_Msk                                 ((0x1) << LSM6DSO32_MD2_CFG_INT2_WU_Pos)  /* (LSM6DSO32_MD2_CFG) Routing of wakeup event on INT2 mask */
#define LSM6DSO32_MD2_CFG_INT2_WU(value)                              (LSM6DSO32_MD2_CFG_INT2_WU_Msk & ((value) << LSM6DSO32_MD2_CFG_INT2_WU_Pos))  /* Assignment of INT2_WU value in LSM6DSO32_MD2_CFG register */
#define LSM6DSO32_MD2_CFG_INT2_WU_Get(reg)                            (((reg) & LSM6DSO32_MD2_CFG_INT2_WU_Msk) >> LSM6DSO32_MD2_CFG_INT2_WU_Pos)  /* Assignment of INT2_WU value in LSM6DSO32_MD2_CFG register */
#define LSM6DSO32_MD2_CFG_INT2_SINGLE_TAP_Pos                         (6)  /* (LSM6DSO32_MD2_CFG) Routing of single-tap recognition event on INT2 position */
#define LSM6DSO32_MD2_CFG_INT2_SINGLE_TAP_Msk                         ((0x1) << LSM6DSO32_MD2_CFG_INT2_SINGLE_TAP_Pos)  /* (LSM6DSO32_MD2_CFG) Routing of single-tap recognition event on INT2 mask */
#define LSM6DSO32_MD2_CFG_INT2_SINGLE_TAP(value)                      (LSM6DSO32_MD2_CFG_INT2_SINGLE_TAP_Msk & ((value) << LSM6DSO32_MD2_CFG_INT2_SINGLE_TAP_Pos))  /* Assignment of INT2_SINGLE_TAP value in LSM6DSO32_MD2_CFG register */
#define LSM6DSO32_MD2_CFG_INT2_SINGLE_TAP_Get(reg)                    (((reg) & LSM6DSO32_MD2_CFG_INT2_SINGLE_TAP_Msk) >> LSM6DSO32_MD2_CFG_INT2_SINGLE_TAP_Pos)  /* Assignment of INT2_SINGLE_TAP value in LSM6DSO32_MD2_CFG register */
#define LSM6DSO32_MD2_CFG_INT2_SLEEP_CHANGE_Pos                       (7)  /* (LSM6DSO32_MD2_CFG) Routing of activity/inactivity recognition event on INT2 position */
#define LSM6DSO32_MD2_CFG_INT2_SLEEP_CHANGE_Msk                       ((0x1) << LSM6DSO32_MD2_CFG_INT2_SLEEP_CHANGE_Pos)  /* (LSM6DSO32_MD2_CFG) Routing of activity/inactivity recognition event on INT2 mask */
#define LSM6DSO32_MD2_CFG_INT2_SLEEP_CHANGE(value)                    (LSM6DSO32_MD2_CFG_INT2_SLEEP_CHANGE_Msk & ((value) << LSM6DSO32_MD2_CFG_INT2_SLEEP_CHANGE_Pos))  /* Assignment of INT2_SLEEP_CHANGE value in LSM6DSO32_MD2_CFG register */
#define LSM6DSO32_MD2_CFG_INT2_SLEEP_CHANGE_Get(reg)                  (((reg) & LSM6DSO32_MD2_CFG_INT2_SLEEP_CHANGE_Msk) >> LSM6DSO32_MD2_CFG_INT2_SLEEP_CHANGE_Pos)  /* Assignment of INT2_SLEEP_CHANGE value in LSM6DSO32_MD2_CFG register */
#define LSM6DSO32_MD2_CFG_Msk                                         (0xff)  /* (LSM6DSO32_MD2_CFG) Register mask */


/* LSM6DSO32_I3C_BUS_AVB, R/W: I3C_BUS_AVB register */
#define LSM6DSO32_I3C_BUS_AVB_ADDR                                    (0x62)  /* (LSM6DSO32_I3C_BUS_AVB) I3C_BUS_AVB register address */

#define LSM6DSO32_I3C_BUS_AVB_PD_DIS_INT1_Pos                         (0)  /* (LSM6DSO32_I3C_BUS_AVB) This bit allows disabling the INT1 pull-down position */
#define LSM6DSO32_I3C_BUS_AVB_PD_DIS_INT1_Msk                         ((0x1) << LSM6DSO32_I3C_BUS_AVB_PD_DIS_INT1_Pos)  /* (LSM6DSO32_I3C_BUS_AVB) This bit allows disabling the INT1 pull-down mask */
#define LSM6DSO32_I3C_BUS_AVB_PD_DIS_INT1(value)                      (LSM6DSO32_I3C_BUS_AVB_PD_DIS_INT1_Msk & ((value) << LSM6DSO32_I3C_BUS_AVB_PD_DIS_INT1_Pos))  /* Assignment of PD_DIS_INT1 value in LSM6DSO32_I3C_BUS_AVB register */
#define LSM6DSO32_I3C_BUS_AVB_PD_DIS_INT1_Get(reg)                    (((reg) & LSM6DSO32_I3C_BUS_AVB_PD_DIS_INT1_Msk) >> LSM6DSO32_I3C_BUS_AVB_PD_DIS_INT1_Pos)  /* Assignment of PD_DIS_INT1 value in LSM6DSO32_I3C_BUS_AVB register */
#define LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_Pos                     (3)  /* (LSM6DSO32_I3C_BUS_AVB) These bits are used to select the bus available time when I3C IBI is used position */
#define LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_Msk                     ((0x3) << LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_Pos)  /* (LSM6DSO32_I3C_BUS_AVB) These bits are used to select the bus available time when I3C IBI is used mask */
#define LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL(value)                  (LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_Msk & ((value) << LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_Pos))  /* Assignment of I3C_BUS_AVB_SEL value in LSM6DSO32_I3C_BUS_AVB register */
#define LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_Get(reg)                (((reg) & LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_Msk) >> LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_Pos)  /* Assignment of I3C_BUS_AVB_SEL value in LSM6DSO32_I3C_BUS_AVB register */
#define   LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_50u_Val               (0)  /* (LSM6DSO32_I3C_BUS_AVB) 50usec value */
#define   LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_2u_Val                (1)  /* (LSM6DSO32_I3C_BUS_AVB) 2usec value */
#define   LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_1m_Val                (2)  /* (LSM6DSO32_I3C_BUS_AVB) 1msec value */
#define   LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_25m_Val               (3)  /* (LSM6DSO32_I3C_BUS_AVB) 25msec value */
#define LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_50u                     (LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_50u_Val << LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_Pos)  /* (LSM6DSO32_I3C_BUS_AVB) 50usec */
#define LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_2u                      (LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_2u_Val << LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_Pos)  /* (LSM6DSO32_I3C_BUS_AVB) 2usec */
#define LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_1m                      (LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_1m_Val << LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_Pos)  /* (LSM6DSO32_I3C_BUS_AVB) 1msec */
#define LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_25m                     (LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_25m_Val << LSM6DSO32_I3C_BUS_AVB_I3C_BUS_AVB_SEL_Pos)  /* (LSM6DSO32_I3C_BUS_AVB) 25msec */
#define LSM6DSO32_I3C_BUS_AVB_Msk                                     (0x19)  /* (LSM6DSO32_I3C_BUS_AVB) Register mask */


/* LSM6DSO32_INTERNAL_FREQ_FINE, R: Internal frequency register */
#define LSM6DSO32_INTERNAL_FREQ_FINE_ADDR                             (0x63)  /* (LSM6DSO32_INTERNAL_FREQ_FINE) Internal frequency register address */

#define LSM6DSO32_INTERNAL_FREQ_FINE_FREQ_FINE_Pos                    (0)  /* (LSM6DSO32_INTERNAL_FREQ_FINE) Difference in percentage of the effective ODR (and Timestamp Rate) with respect to the typical. Step: 0.15%. 8-bit format, 2's complement. position */
#define LSM6DSO32_INTERNAL_FREQ_FINE_FREQ_FINE_Msk                    ((0xff) << LSM6DSO32_INTERNAL_FREQ_FINE_FREQ_FINE_Pos)  /* (LSM6DSO32_INTERNAL_FREQ_FINE) Difference in percentage of the effective ODR (and Timestamp Rate) with respect to the typical. Step: 0.15%. 8-bit format, 2's complement. mask */
#define LSM6DSO32_INTERNAL_FREQ_FINE_FREQ_FINE(value)                 (LSM6DSO32_INTERNAL_FREQ_FINE_FREQ_FINE_Msk & ((value) << LSM6DSO32_INTERNAL_FREQ_FINE_FREQ_FINE_Pos))  /* Assignment of FREQ_FINE value in LSM6DSO32_INTERNAL_FREQ_FINE register */
#define LSM6DSO32_INTERNAL_FREQ_FINE_FREQ_FINE_Get(reg)               (((reg) & LSM6DSO32_INTERNAL_FREQ_FINE_FREQ_FINE_Msk) >> LSM6DSO32_INTERNAL_FREQ_FINE_FREQ_FINE_Pos)  /* Assignment of FREQ_FINE value in LSM6DSO32_INTERNAL_FREQ_FINE register */
#define LSM6DSO32_INTERNAL_FREQ_FINE_Msk                              (0xff)  /* (LSM6DSO32_INTERNAL_FREQ_FINE) Register mask */


/* LSM6DSO32_X_OFS_USR, R/W: Accelerometer X-axis user offset correction. The offset value set in the X_OFS_USR offset register is internally subtracted from the acceleration value measured on the X-axis */
#define LSM6DSO32_X_OFS_USR_ADDR                                      (0x73)  /* (LSM6DSO32_X_OFS_USR) Accelerometer X-axis user offset correction. The offset value set in the X_OFS_USR offset register is internally subtracted from the acceleration value measured on the X-axis address */

#define LSM6DSO32_X_OFS_USR_X_OFS_USR_Pos                             (0)  /* (LSM6DSO32_X_OFS_USR) Accelerometer X-axis user offset correction expressed in two’s complement, weight depends on USR_OFF_W in CTRL6_C (15h). The value must be in the range [-127, +127] position */
#define LSM6DSO32_X_OFS_USR_X_OFS_USR_Msk                             ((0xff) << LSM6DSO32_X_OFS_USR_X_OFS_USR_Pos)  /* (LSM6DSO32_X_OFS_USR) Accelerometer X-axis user offset correction expressed in two’s complement, weight depends on USR_OFF_W in CTRL6_C (15h). The value must be in the range [-127, +127] mask */
#define LSM6DSO32_X_OFS_USR_X_OFS_USR(value)                          (LSM6DSO32_X_OFS_USR_X_OFS_USR_Msk & ((value) << LSM6DSO32_X_OFS_USR_X_OFS_USR_Pos))  /* Assignment of X_OFS_USR value in LSM6DSO32_X_OFS_USR register */
#define LSM6DSO32_X_OFS_USR_X_OFS_USR_Get(reg)                        (((reg) & LSM6DSO32_X_OFS_USR_X_OFS_USR_Msk) >> LSM6DSO32_X_OFS_USR_X_OFS_USR_Pos)  /* Assignment of X_OFS_USR value in LSM6DSO32_X_OFS_USR register */
#define LSM6DSO32_X_OFS_USR_Msk                                       (0xff)  /* (LSM6DSO32_X_OFS_USR) Register mask */


/* LSM6DSO32_Y_OFS_USR, R/W: Accelerometer Y-axis user offset correction. The offset value set in the Y_OFS_USR offset register is internally subtracted from the acceleration value measured on the Y-axis */
#define LSM6DSO32_Y_OFS_USR_ADDR                                      (0x74)  /* (LSM6DSO32_Y_OFS_USR) Accelerometer Y-axis user offset correction. The offset value set in the Y_OFS_USR offset register is internally subtracted from the acceleration value measured on the Y-axis address */

#define LSM6DSO32_Y_OFS_USR_Y_OFS_USR_Pos                             (0)  /* (LSM6DSO32_Y_OFS_USR) Accelerometer Y-axis user offset correction expressed in two’s complement, weight depends on USR_OFF_W in CTRL6_C (15h). The value must be in the range [-127, +127] position */
#define LSM6DSO32_Y_OFS_USR_Y_OFS_USR_Msk                             ((0xff) << LSM6DSO32_Y_OFS_USR_Y_OFS_USR_Pos)  /* (LSM6DSO32_Y_OFS_USR) Accelerometer Y-axis user offset correction expressed in two’s complement, weight depends on USR_OFF_W in CTRL6_C (15h). The value must be in the range [-127, +127] mask */
#define LSM6DSO32_Y_OFS_USR_Y_OFS_USR(value)                          (LSM6DSO32_Y_OFS_USR_Y_OFS_USR_Msk & ((value) << LSM6DSO32_Y_OFS_USR_Y_OFS_USR_Pos))  /* Assignment of Y_OFS_USR value in LSM6DSO32_Y_OFS_USR register */
#define LSM6DSO32_Y_OFS_USR_Y_OFS_USR_Get(reg)                        (((reg) & LSM6DSO32_Y_OFS_USR_Y_OFS_USR_Msk) >> LSM6DSO32_Y_OFS_USR_Y_OFS_USR_Pos)  /* Assignment of Y_OFS_USR value in LSM6DSO32_Y_OFS_USR register */
#define LSM6DSO32_Y_OFS_USR_Msk                                       (0xff)  /* (LSM6DSO32_Y_OFS_USR) Register mask */


/* LSM6DSO32_Z_OFS_USR, R/W: Accelerometer Z-axis user offset correction. The offset value set in the Z_OFS_USR offset register is internally subtracted from the acceleration value measured on the Z-axis */
#define LSM6DSO32_Z_OFS_USR_ADDR                                      (0x75)  /* (LSM6DSO32_Z_OFS_USR) Accelerometer Z-axis user offset correction. The offset value set in the Z_OFS_USR offset register is internally subtracted from the acceleration value measured on the Z-axis address */

#define LSM6DSO32_Z_OFS_USR_Z_OFS_USR_Pos                             (0)  /* (LSM6DSO32_Z_OFS_USR) Accelerometer Z-axis user offset correction expressed in two’s complement, weight depends on USR_OFF_W in CTRL6_C (15h). The value must be in the range [-127, +127] position */
#define LSM6DSO32_Z_OFS_USR_Z_OFS_USR_Msk                             ((0xff) << LSM6DSO32_Z_OFS_USR_Z_OFS_USR_Pos)  /* (LSM6DSO32_Z_OFS_USR) Accelerometer Z-axis user offset correction expressed in two’s complement, weight depends on USR_OFF_W in CTRL6_C (15h). The value must be in the range [-127, +127] mask */
#define LSM6DSO32_Z_OFS_USR_Z_OFS_USR(value)                          (LSM6DSO32_Z_OFS_USR_Z_OFS_USR_Msk & ((value) << LSM6DSO32_Z_OFS_USR_Z_OFS_USR_Pos))  /* Assignment of Z_OFS_USR value in LSM6DSO32_Z_OFS_USR register */
#define LSM6DSO32_Z_OFS_USR_Z_OFS_USR_Get(reg)                        (((reg) & LSM6DSO32_Z_OFS_USR_Z_OFS_USR_Msk) >> LSM6DSO32_Z_OFS_USR_Z_OFS_USR_Pos)  /* Assignment of Z_OFS_USR value in LSM6DSO32_Z_OFS_USR register */
#define LSM6DSO32_Z_OFS_USR_Msk                                       (0xff)  /* (LSM6DSO32_Z_OFS_USR) Register mask */


/* LSM6DSO32_FIFO_DATA_OUT_TAG, R: FIFO tag register */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_ADDR                              (0x78)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) FIFO tag register address */

#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_PARITY_Pos                    (0)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Parity check of TAG content position */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_PARITY_Msk                    ((0x1) << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_PARITY_Pos)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Parity check of TAG content mask */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_PARITY(value)                 (LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_PARITY_Msk & ((value) << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_PARITY_Pos))  /* Assignment of TAG_PARITY value in LSM6DSO32_FIFO_DATA_OUT_TAG register */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_PARITY_Get(reg)               (((reg) & LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_PARITY_Msk) >> LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_PARITY_Pos)  /* Assignment of TAG_PARITY value in LSM6DSO32_FIFO_DATA_OUT_TAG register */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_CNT_Pos                       (1)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) 2-bit counter which identifies sensor time slot position */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_CNT_Msk                       ((0x3) << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_CNT_Pos)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) 2-bit counter which identifies sensor time slot mask */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_CNT(value)                    (LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_CNT_Msk & ((value) << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_CNT_Pos))  /* Assignment of TAG_CNT value in LSM6DSO32_FIFO_DATA_OUT_TAG register */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_CNT_Get(reg)                  (((reg) & LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_CNT_Msk) >> LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_CNT_Pos)  /* Assignment of TAG_CNT value in LSM6DSO32_FIFO_DATA_OUT_TAG register */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos                    (3)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) FIFO tag: identifies the sensor position */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Msk                    ((0x1f) << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) FIFO tag: identifies the sensor mask */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR(value)                 (LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Msk & ((value) << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos))  /* Assignment of TAG_SENSOR value in LSM6DSO32_FIFO_DATA_OUT_TAG register */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Get(reg)               (((reg) & LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Msk) >> LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos)  /* Assignment of TAG_SENSOR value in LSM6DSO32_FIFO_DATA_OUT_TAG register */
#define   LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_G_NC_Val             (1)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Gyroscope NC value */
#define   LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_XL_NC_Val            (2)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Accelerometer NC value */
#define   LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_TEMP_Val             (3)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Temperature value */
#define   LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_TIME_Val             (4)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Timestamp value */
#define   LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_CFG_C_Val            (5)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) CFG_Change value */
#define   LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_XL_NC_T_2_Val        (6)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Accelerometer NC_T_2 value */
#define   LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_XL_NC_T_1_Val        (7)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Accelerometer NC_T_1 value */
#define   LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_XL_2xC_Val           (8)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Accelerometer 2xC value */
#define   LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_XL_3xC_Val           (9)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Accelerometer 3xC value */
#define   LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_G_NC_T_2_Val         (10)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Gyroscope NC_T_2 value */
#define   LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_G_NC_T_1_Val         (11)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Gyroscope NC_T_1 value */
#define   LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_G_2xC_Val            (12)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Gyroscope 2xC value */
#define   LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_G_3xC_Val            (13)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Gyroscope 3xC value */
#define   LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_SHS0_Val             (14)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Sensor Hub Slave 0 value */
#define   LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_SHS1_Val             (15)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Sensor Hub Slave 1 value */
#define   LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_SHS2_Val             (16)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Sensor Hub Slave 2 value */
#define   LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_SHS3_Val             (17)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Sensor Hub Slave 3 value */
#define   LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_STEP_Val             (18)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Step counter value */
#define   LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_SH_NACK_Val          (25)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Sensor Hub NACK value */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_G_NC                   (LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_G_NC_Val << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Gyroscope NC */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_XL_NC                  (LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_XL_NC_Val << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Accelerometer NC */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_TEMP                   (LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_TEMP_Val << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Temperature */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_TIME                   (LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_TIME_Val << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Timestamp */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_CFG_C                  (LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_CFG_C_Val << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) CFG_Change */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_XL_NC_T_2              (LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_XL_NC_T_2_Val << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Accelerometer NC_T_2 */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_XL_NC_T_1              (LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_XL_NC_T_1_Val << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Accelerometer NC_T_1 */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_XL_2xC                 (LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_XL_2xC_Val << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Accelerometer 2xC */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_XL_3xC                 (LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_XL_3xC_Val << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Accelerometer 3xC */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_G_NC_T_2               (LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_G_NC_T_2_Val << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Gyroscope NC_T_2 */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_G_NC_T_1               (LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_G_NC_T_1_Val << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Gyroscope NC_T_1 */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_G_2xC                  (LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_G_2xC_Val << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Gyroscope 2xC */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_G_3xC                  (LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_G_3xC_Val << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Gyroscope 3xC */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_SHS0                   (LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_SHS0_Val << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Sensor Hub Slave 0 */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_SHS1                   (LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_SHS1_Val << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Sensor Hub Slave 1 */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_SHS2                   (LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_SHS2_Val << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Sensor Hub Slave 2 */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_SHS3                   (LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_SHS3_Val << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Sensor Hub Slave 3 */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_STEP                   (LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_STEP_Val << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Step counter */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_SH_NACK                (LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_SH_NACK_Val << LSM6DSO32_FIFO_DATA_OUT_TAG_TAG_SENSOR_Pos)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Sensor Hub NACK */
#define LSM6DSO32_FIFO_DATA_OUT_TAG_Msk                               (0xff)  /* (LSM6DSO32_FIFO_DATA_OUT_TAG) Register mask */


/* LSM6DSO32_FIFO_DATA_OUT_X_L, R: FIFO data output X */
#define LSM6DSO32_FIFO_DATA_OUT_X_L_ADDR                              (0x79)  /* (LSM6DSO32_FIFO_DATA_OUT_X_L) FIFO data output X address */

#define LSM6DSO32_FIFO_DATA_OUT_X_L_Msk                               (0xff)  /* (LSM6DSO32_FIFO_DATA_OUT_X_L) Register mask */


/* LSM6DSO32_FIFO_DATA_OUT_X_H, R: FIFO data output X */
#define LSM6DSO32_FIFO_DATA_OUT_X_H_ADDR                              (0x7a)  /* (LSM6DSO32_FIFO_DATA_OUT_X_H) FIFO data output X address */

#define LSM6DSO32_FIFO_DATA_OUT_X_H_Msk                               (0xff)  /* (LSM6DSO32_FIFO_DATA_OUT_X_H) Register mask */


/* LSM6DSO32_FIFO_DATA_OUT_Y_L, R: FIFO data output Y */
#define LSM6DSO32_FIFO_DATA_OUT_Y_L_ADDR                              (0x7b)  /* (LSM6DSO32_FIFO_DATA_OUT_Y_L) FIFO data output Y address */

#define LSM6DSO32_FIFO_DATA_OUT_Y_L_Msk                               (0xff)  /* (LSM6DSO32_FIFO_DATA_OUT_Y_L) Register mask */


/* LSM6DSO32_FIFO_DATA_OUT_Y_H, R: FIFO data output Y */
#define LSM6DSO32_FIFO_DATA_OUT_Y_H_ADDR                              (0x7c)  /* (LSM6DSO32_FIFO_DATA_OUT_Y_H) FIFO data output Y address */

#define LSM6DSO32_FIFO_DATA_OUT_Y_H_Msk                               (0xff)  /* (LSM6DSO32_FIFO_DATA_OUT_Y_H) Register mask */


/* LSM6DSO32_FIFO_DATA_OUT_Z_L, R: FIFO data output Z */
#define LSM6DSO32_FIFO_DATA_OUT_Z_L_ADDR                              (0x7d)  /* (LSM6DSO32_FIFO_DATA_OUT_Z_L) FIFO data output Z address */

#define LSM6DSO32_FIFO_DATA_OUT_Z_L_Msk                               (0xff)  /* (LSM6DSO32_FIFO_DATA_OUT_Z_L) Register mask */


/* LSM6DSO32_FIFO_DATA_OUT_Z_H, R: FIFO data output Z */
#define LSM6DSO32_FIFO_DATA_OUT_Z_H_ADDR                              (0x7e)  /* (LSM6DSO32_FIFO_DATA_OUT_Z_H) FIFO data output Z address */

#define LSM6DSO32_FIFO_DATA_OUT_Z_H_Msk                               (0xff)  /* (LSM6DSO32_FIFO_DATA_OUT_Z_H) Register mask */


#endif  // LSM6DSO32_REGS
