/* File generated from device description on 30/04/2024, 20:54:39 */

/* AS5600: 12-bit programmable magnetic rotary position sensor */

#ifndef AS5600_REGS
#define AS5600_REGS

#define AS5600_ADDR                                                   (0x36)  /* AS5600 address */

/* AS5600_ZMCO, R: ZMCO shows how many times ZPOS and MPOS have been permanently written */
#define AS5600_ZMCO_ADDR                                              (0x0)  /* (AS5600_ZMCO) ZMCO shows how many times ZPOS and MPOS have been permanently written address */

#define AS5600_ZMCO_ZMCO_Pos                                          (0)  /* (AS5600_ZMCO) Number of times ZPOS and MPOS have been permanently written position */
#define AS5600_ZMCO_ZMCO_Msk                                          ((0x3) << AS5600_ZMCO_ZMCO_Pos)  /* (AS5600_ZMCO) Number of times ZPOS and MPOS have been permanently written mask */
#define AS5600_ZMCO_ZMCO(value)                                       (AS5600_ZMCO_ZMCO_Msk & ((value) << AS5600_ZMCO_ZMCO_Pos))  /* Assignment of ZMCO value in AS5600_ZMCO register */
#define AS5600_ZMCO_ZMCO_Get(reg)                                     (((reg) & AS5600_ZMCO_ZMCO_Msk) >> AS5600_ZMCO_ZMCO_Pos)  /* Assignment of ZMCO value in AS5600_ZMCO register */
#define AS5600_ZMCO_Msk                                               (0x3)  /* (AS5600_ZMCO) Register mask */


/* AS5600_ZPOS, R/W: Start position for a narrower angular range */
#define AS5600_ZPOS_ADDR_0                                            (0x1)  /* (AS5600_ZPOS) Start position for a narrower angular range address */
#define AS5600_ZPOS_ADDR_1                                            (0x2)  /* (AS5600_ZPOS) Start position for a narrower angular range address */

#define AS5600_ZPOS_ZPOS_Pos                                          (0)  /* (AS5600_ZPOS) Start position for a narrower angular range position */
#define AS5600_ZPOS_ZPOS_Msk                                          ((0xfff) << AS5600_ZPOS_ZPOS_Pos)  /* (AS5600_ZPOS) Start position for a narrower angular range mask */
#define AS5600_ZPOS_ZPOS(value)                                       (AS5600_ZPOS_ZPOS_Msk & ((value) << AS5600_ZPOS_ZPOS_Pos))  /* Assignment of ZPOS value in AS5600_ZPOS register */
#define AS5600_ZPOS_ZPOS_Get(reg)                                     (((reg) & AS5600_ZPOS_ZPOS_Msk) >> AS5600_ZPOS_ZPOS_Pos)  /* Assignment of ZPOS value in AS5600_ZPOS register */
#define AS5600_ZPOS_Msk                                               (0xfff)  /* (AS5600_ZPOS) Register mask */


/* AS5600_MPOS, R/W: Stop position for a narrower angular range */
#define AS5600_MPOS_ADDR_0                                            (0x3)  /* (AS5600_MPOS) Stop position for a narrower angular range address */
#define AS5600_MPOS_ADDR_1                                            (0x4)  /* (AS5600_MPOS) Stop position for a narrower angular range address */

#define AS5600_MPOS_MPOS_Pos                                          (0)  /* (AS5600_MPOS) Stop position for a narrower angular range position */
#define AS5600_MPOS_MPOS_Msk                                          ((0xfff) << AS5600_MPOS_MPOS_Pos)  /* (AS5600_MPOS) Stop position for a narrower angular range mask */
#define AS5600_MPOS_MPOS(value)                                       (AS5600_MPOS_MPOS_Msk & ((value) << AS5600_MPOS_MPOS_Pos))  /* Assignment of MPOS value in AS5600_MPOS register */
#define AS5600_MPOS_MPOS_Get(reg)                                     (((reg) & AS5600_MPOS_MPOS_Msk) >> AS5600_MPOS_MPOS_Pos)  /* Assignment of MPOS value in AS5600_MPOS register */
#define AS5600_MPOS_Msk                                               (0xfff)  /* (AS5600_MPOS) Register mask */


/* AS5600_MANG, R/W: Maximum angle for a narrower angular range */
#define AS5600_MANG_ADDR_0                                            (0x5)  /* (AS5600_MANG) Maximum angle for a narrower angular range address */
#define AS5600_MANG_ADDR_1                                            (0x6)  /* (AS5600_MANG) Maximum angle for a narrower angular range address */

#define AS5600_MANG_MANG_Pos                                          (0)  /* (AS5600_MANG) Maximum angle for a narrower angular range position */
#define AS5600_MANG_MANG_Msk                                          ((0xfff) << AS5600_MANG_MANG_Pos)  /* (AS5600_MANG) Maximum angle for a narrower angular range mask */
#define AS5600_MANG_MANG(value)                                       (AS5600_MANG_MANG_Msk & ((value) << AS5600_MANG_MANG_Pos))  /* Assignment of MANG value in AS5600_MANG register */
#define AS5600_MANG_MANG_Get(reg)                                     (((reg) & AS5600_MANG_MANG_Msk) >> AS5600_MANG_MANG_Pos)  /* Assignment of MANG value in AS5600_MANG register */
#define AS5600_MANG_Msk                                               (0xfff)  /* (AS5600_MANG) Register mask */


/* AS5600_CONF, R/W: CONF register supports customizing the AS5600 */
#define AS5600_CONF_ADDR_0                                            (0x7)  /* (AS5600_CONF) CONF register supports customizing the AS5600 address */
#define AS5600_CONF_ADDR_1                                            (0x8)  /* (AS5600_CONF) CONF register supports customizing the AS5600 address */

#define AS5600_CONF_PM_Pos                                            (0)  /* (AS5600_CONF) Power mode position */
#define AS5600_CONF_PM_Msk                                            ((0x3) << AS5600_CONF_PM_Pos)  /* (AS5600_CONF) Power mode mask */
#define AS5600_CONF_PM(value)                                         (AS5600_CONF_PM_Msk & ((value) << AS5600_CONF_PM_Pos))  /* Assignment of PM value in AS5600_CONF register */
#define AS5600_CONF_PM_Get(reg)                                       (((reg) & AS5600_CONF_PM_Msk) >> AS5600_CONF_PM_Pos)  /* Assignment of PM value in AS5600_CONF register */
#define   AS5600_CONF_PM_NOM_Val                                      (0)  /* (AS5600_CONF) Nominal value */
#define   AS5600_CONF_PM_LPM1_Val                                     (1)  /* (AS5600_CONF) Low power mode 1 value */
#define   AS5600_CONF_PM_LPM2_Val                                     (2)  /* (AS5600_CONF) Low power mode 2 value */
#define   AS5600_CONF_PM_LPM3_Val                                     (3)  /* (AS5600_CONF) Low power mode 3 value */
#define AS5600_CONF_PM_NOM                                            (AS5600_CONF_PM_NOM_Val << AS5600_CONF_PM_Pos)  /* (AS5600_CONF) Nominal */
#define AS5600_CONF_PM_LPM1                                           (AS5600_CONF_PM_LPM1_Val << AS5600_CONF_PM_Pos)  /* (AS5600_CONF) Low power mode 1 */
#define AS5600_CONF_PM_LPM2                                           (AS5600_CONF_PM_LPM2_Val << AS5600_CONF_PM_Pos)  /* (AS5600_CONF) Low power mode 2 */
#define AS5600_CONF_PM_LPM3                                           (AS5600_CONF_PM_LPM3_Val << AS5600_CONF_PM_Pos)  /* (AS5600_CONF) Low power mode 3 */
#define AS5600_CONF_HYST_Pos                                          (2)  /* (AS5600_CONF) Hysteresis position */
#define AS5600_CONF_HYST_Msk                                          ((0x3) << AS5600_CONF_HYST_Pos)  /* (AS5600_CONF) Hysteresis mask */
#define AS5600_CONF_HYST(value)                                       (AS5600_CONF_HYST_Msk & ((value) << AS5600_CONF_HYST_Pos))  /* Assignment of HYST value in AS5600_CONF register */
#define AS5600_CONF_HYST_Get(reg)                                     (((reg) & AS5600_CONF_HYST_Msk) >> AS5600_CONF_HYST_Pos)  /* Assignment of HYST value in AS5600_CONF register */
#define   AS5600_CONF_HYST_OFF_Val                                    (0)  /* (AS5600_CONF) Off value */
#define   AS5600_CONF_HYST_1LSB_Val                                   (1)  /* (AS5600_CONF) 1 LSB value */
#define   AS5600_CONF_HYST_2LSB_Val                                   (2)  /* (AS5600_CONF) 2 LSB value */
#define   AS5600_CONF_HYST_3LSB_Val                                   (3)  /* (AS5600_CONF) 3 LSB value */
#define AS5600_CONF_HYST_OFF                                          (AS5600_CONF_HYST_OFF_Val << AS5600_CONF_HYST_Pos)  /* (AS5600_CONF) Off */
#define AS5600_CONF_HYST_1LSB                                         (AS5600_CONF_HYST_1LSB_Val << AS5600_CONF_HYST_Pos)  /* (AS5600_CONF) 1 LSB */
#define AS5600_CONF_HYST_2LSB                                         (AS5600_CONF_HYST_2LSB_Val << AS5600_CONF_HYST_Pos)  /* (AS5600_CONF) 2 LSB */
#define AS5600_CONF_HYST_3LSB                                         (AS5600_CONF_HYST_3LSB_Val << AS5600_CONF_HYST_Pos)  /* (AS5600_CONF) 3 LSB */
#define AS5600_CONF_OUTS_Pos                                          (4)  /* (AS5600_CONF) Output stage position */
#define AS5600_CONF_OUTS_Msk                                          ((0x3) << AS5600_CONF_OUTS_Pos)  /* (AS5600_CONF) Output stage mask */
#define AS5600_CONF_OUTS(value)                                       (AS5600_CONF_OUTS_Msk & ((value) << AS5600_CONF_OUTS_Pos))  /* Assignment of OUTS value in AS5600_CONF register */
#define AS5600_CONF_OUTS_Get(reg)                                     (((reg) & AS5600_CONF_OUTS_Msk) >> AS5600_CONF_OUTS_Pos)  /* Assignment of OUTS value in AS5600_CONF register */
#define   AS5600_CONF_OUTS_A_FULL_Val                                 (0)  /* (AS5600_CONF) Analog, full range from 0% to 100% between GND and VDD value */
#define   AS5600_CONF_OUTS_A_RED_Val                                  (1)  /* (AS5600_CONF) Analog, reduced range from 10% to 90% between GND and VDD value */
#define   AS5600_CONF_OUTS_PWM_Val                                    (2)  /* (AS5600_CONF) Digital PWM value */
#define AS5600_CONF_OUTS_A_FULL                                       (AS5600_CONF_OUTS_A_FULL_Val << AS5600_CONF_OUTS_Pos)  /* (AS5600_CONF) Analog, full range from 0% to 100% between GND and VDD */
#define AS5600_CONF_OUTS_A_RED                                        (AS5600_CONF_OUTS_A_RED_Val << AS5600_CONF_OUTS_Pos)  /* (AS5600_CONF) Analog, reduced range from 10% to 90% between GND and VDD */
#define AS5600_CONF_OUTS_PWM                                          (AS5600_CONF_OUTS_PWM_Val << AS5600_CONF_OUTS_Pos)  /* (AS5600_CONF) Digital PWM */
#define AS5600_CONF_PWMF_Pos                                          (6)  /* (AS5600_CONF) PWM frequency position */
#define AS5600_CONF_PWMF_Msk                                          ((0x3) << AS5600_CONF_PWMF_Pos)  /* (AS5600_CONF) PWM frequency mask */
#define AS5600_CONF_PWMF(value)                                       (AS5600_CONF_PWMF_Msk & ((value) << AS5600_CONF_PWMF_Pos))  /* Assignment of PWMF value in AS5600_CONF register */
#define AS5600_CONF_PWMF_Get(reg)                                     (((reg) & AS5600_CONF_PWMF_Msk) >> AS5600_CONF_PWMF_Pos)  /* Assignment of PWMF value in AS5600_CONF register */
#define   AS5600_CONF_PWMF_115Hz_Val                                  (0)  /* (AS5600_CONF) 115Hz value */
#define   AS5600_CONF_PWMF_230Hz_Val                                  (1)  /* (AS5600_CONF) 230Hz value */
#define   AS5600_CONF_PWMF_460Hz_Val                                  (2)  /* (AS5600_CONF) 460Hz value */
#define   AS5600_CONF_PWMF_920Hz_Val                                  (3)  /* (AS5600_CONF) 920Hz value */
#define AS5600_CONF_PWMF_115Hz                                        (AS5600_CONF_PWMF_115Hz_Val << AS5600_CONF_PWMF_Pos)  /* (AS5600_CONF) 115Hz */
#define AS5600_CONF_PWMF_230Hz                                        (AS5600_CONF_PWMF_230Hz_Val << AS5600_CONF_PWMF_Pos)  /* (AS5600_CONF) 230Hz */
#define AS5600_CONF_PWMF_460Hz                                        (AS5600_CONF_PWMF_460Hz_Val << AS5600_CONF_PWMF_Pos)  /* (AS5600_CONF) 460Hz */
#define AS5600_CONF_PWMF_920Hz                                        (AS5600_CONF_PWMF_920Hz_Val << AS5600_CONF_PWMF_Pos)  /* (AS5600_CONF) 920Hz */
#define AS5600_CONF_SF_Pos                                            (8)  /* (AS5600_CONF) Slow filter position */
#define AS5600_CONF_SF_Msk                                            ((0x3) << AS5600_CONF_SF_Pos)  /* (AS5600_CONF) Slow filter mask */
#define AS5600_CONF_SF(value)                                         (AS5600_CONF_SF_Msk & ((value) << AS5600_CONF_SF_Pos))  /* Assignment of SF value in AS5600_CONF register */
#define AS5600_CONF_SF_Get(reg)                                       (((reg) & AS5600_CONF_SF_Msk) >> AS5600_CONF_SF_Pos)  /* Assignment of SF value in AS5600_CONF register */
#define   AS5600_CONF_SF_16x_Val                                      (0)  /* (AS5600_CONF) 16x, forced in Low Power Mode (LPM) value */
#define   AS5600_CONF_SF_8x_Val                                       (1)  /* (AS5600_CONF) 8x value */
#define   AS5600_CONF_SF_4x_Val                                       (2)  /* (AS5600_CONF) 4x value */
#define   AS5600_CONF_SF_2x_Val                                       (3)  /* (AS5600_CONF) 2x value */
#define AS5600_CONF_SF_16x                                            (AS5600_CONF_SF_16x_Val << AS5600_CONF_SF_Pos)  /* (AS5600_CONF) 16x, forced in Low Power Mode (LPM) */
#define AS5600_CONF_SF_8x                                             (AS5600_CONF_SF_8x_Val << AS5600_CONF_SF_Pos)  /* (AS5600_CONF) 8x */
#define AS5600_CONF_SF_4x                                             (AS5600_CONF_SF_4x_Val << AS5600_CONF_SF_Pos)  /* (AS5600_CONF) 4x */
#define AS5600_CONF_SF_2x                                             (AS5600_CONF_SF_2x_Val << AS5600_CONF_SF_Pos)  /* (AS5600_CONF) 2x */
#define AS5600_CONF_FTH_Pos                                           (10)  /* (AS5600_CONF) Fast filter threshold position */
#define AS5600_CONF_FTH_Msk                                           ((0x7) << AS5600_CONF_FTH_Pos)  /* (AS5600_CONF) Fast filter threshold mask */
#define AS5600_CONF_FTH(value)                                        (AS5600_CONF_FTH_Msk & ((value) << AS5600_CONF_FTH_Pos))  /* Assignment of FTH value in AS5600_CONF register */
#define AS5600_CONF_FTH_Get(reg)                                      (((reg) & AS5600_CONF_FTH_Msk) >> AS5600_CONF_FTH_Pos)  /* Assignment of FTH value in AS5600_CONF register */
#define   AS5600_CONF_FTH_SFO_Val                                     (0)  /* (AS5600_CONF) Slow filter only value */
#define   AS5600_CONF_FTH_6LSBs_Val                                   (1)  /* (AS5600_CONF) 6 LSBs value */
#define   AS5600_CONF_FTH_7LSBs_Val                                   (2)  /* (AS5600_CONF) 7 LSBs value */
#define   AS5600_CONF_FTH_9LSBs_Val                                   (3)  /* (AS5600_CONF) 9 LSBs value */
#define   AS5600_CONF_FTH_18LSBs_Val                                  (4)  /* (AS5600_CONF) 18 LSBs value */
#define   AS5600_CONF_FTH_21LSBs_Val                                  (5)  /* (AS5600_CONF) 21 LSBs value */
#define   AS5600_CONF_FTH_24LSBs_Val                                  (6)  /* (AS5600_CONF) 24 LSBs value */
#define   AS5600_CONF_FTH_10LSBs_Val                                  (7)  /* (AS5600_CONF) 10 LSBs value */
#define AS5600_CONF_FTH_SFO                                           (AS5600_CONF_FTH_SFO_Val << AS5600_CONF_FTH_Pos)  /* (AS5600_CONF) Slow filter only */
#define AS5600_CONF_FTH_6LSBs                                         (AS5600_CONF_FTH_6LSBs_Val << AS5600_CONF_FTH_Pos)  /* (AS5600_CONF) 6 LSBs */
#define AS5600_CONF_FTH_7LSBs                                         (AS5600_CONF_FTH_7LSBs_Val << AS5600_CONF_FTH_Pos)  /* (AS5600_CONF) 7 LSBs */
#define AS5600_CONF_FTH_9LSBs                                         (AS5600_CONF_FTH_9LSBs_Val << AS5600_CONF_FTH_Pos)  /* (AS5600_CONF) 9 LSBs */
#define AS5600_CONF_FTH_18LSBs                                        (AS5600_CONF_FTH_18LSBs_Val << AS5600_CONF_FTH_Pos)  /* (AS5600_CONF) 18 LSBs */
#define AS5600_CONF_FTH_21LSBs                                        (AS5600_CONF_FTH_21LSBs_Val << AS5600_CONF_FTH_Pos)  /* (AS5600_CONF) 21 LSBs */
#define AS5600_CONF_FTH_24LSBs                                        (AS5600_CONF_FTH_24LSBs_Val << AS5600_CONF_FTH_Pos)  /* (AS5600_CONF) 24 LSBs */
#define AS5600_CONF_FTH_10LSBs                                        (AS5600_CONF_FTH_10LSBs_Val << AS5600_CONF_FTH_Pos)  /* (AS5600_CONF) 10 LSBs */
#define AS5600_CONF_WD_Pos                                            (13)  /* (AS5600_CONF) Watchdog position */
#define AS5600_CONF_WD_Msk                                            ((0x1) << AS5600_CONF_WD_Pos)  /* (AS5600_CONF) Watchdog mask */
#define AS5600_CONF_WD(value)                                         (AS5600_CONF_WD_Msk & ((value) << AS5600_CONF_WD_Pos))  /* Assignment of WD value in AS5600_CONF register */
#define AS5600_CONF_WD_Get(reg)                                       (((reg) & AS5600_CONF_WD_Msk) >> AS5600_CONF_WD_Pos)  /* Assignment of WD value in AS5600_CONF register */
#define AS5600_CONF_Msk                                               (0x3fff)  /* (AS5600_CONF) Register mask */


/* AS5600_RAW_ANGLE, R: The RAW ANGLE register contains the unscaled and unmodified angle */
#define AS5600_RAW_ANGLE_ADDR_0                                       (0xc)  /* (AS5600_RAW_ANGLE) The RAW ANGLE register contains the unscaled and unmodified angle address */
#define AS5600_RAW_ANGLE_ADDR_1                                       (0xd)  /* (AS5600_RAW_ANGLE) The RAW ANGLE register contains the unscaled and unmodified angle address */

#define AS5600_RAW_ANGLE_RAW_ANGLE_Pos                                (0)  /* (AS5600_RAW_ANGLE) Unscaled and unmodified angle position */
#define AS5600_RAW_ANGLE_RAW_ANGLE_Msk                                ((0xfff) << AS5600_RAW_ANGLE_RAW_ANGLE_Pos)  /* (AS5600_RAW_ANGLE) Unscaled and unmodified angle mask */
#define AS5600_RAW_ANGLE_RAW_ANGLE(value)                             (AS5600_RAW_ANGLE_RAW_ANGLE_Msk & ((value) << AS5600_RAW_ANGLE_RAW_ANGLE_Pos))  /* Assignment of RAW_ANGLE value in AS5600_RAW_ANGLE register */
#define AS5600_RAW_ANGLE_RAW_ANGLE_Get(reg)                           (((reg) & AS5600_RAW_ANGLE_RAW_ANGLE_Msk) >> AS5600_RAW_ANGLE_RAW_ANGLE_Pos)  /* Assignment of RAW_ANGLE value in AS5600_RAW_ANGLE register */
#define AS5600_RAW_ANGLE_Msk                                          (0xfff)  /* (AS5600_RAW_ANGLE) Register mask */


/* AS5600_ANGLE, R: The ANGLE register contains the scaled output value */
#define AS5600_ANGLE_ADDR_0                                           (0xe)  /* (AS5600_ANGLE) The ANGLE register contains the scaled output value address */
#define AS5600_ANGLE_ADDR_1                                           (0xf)  /* (AS5600_ANGLE) The ANGLE register contains the scaled output value address */

#define AS5600_ANGLE_ANGLE_Pos                                        (0)  /* (AS5600_ANGLE) Scaled output value position */
#define AS5600_ANGLE_ANGLE_Msk                                        ((0xfff) << AS5600_ANGLE_ANGLE_Pos)  /* (AS5600_ANGLE) Scaled output value mask */
#define AS5600_ANGLE_ANGLE(value)                                     (AS5600_ANGLE_ANGLE_Msk & ((value) << AS5600_ANGLE_ANGLE_Pos))  /* Assignment of ANGLE value in AS5600_ANGLE register */
#define AS5600_ANGLE_ANGLE_Get(reg)                                   (((reg) & AS5600_ANGLE_ANGLE_Msk) >> AS5600_ANGLE_ANGLE_Pos)  /* Assignment of ANGLE value in AS5600_ANGLE register */
#define AS5600_ANGLE_Msk                                              (0xfff)  /* (AS5600_ANGLE) Register mask */


/* AS5600_STATUS, R: The STATUS register provides bits that indicate the current state of the AS5600 */
#define AS5600_STATUS_ADDR                                            (0xb)  /* (AS5600_STATUS) The STATUS register provides bits that indicate the current state of the AS5600 address */

#define AS5600_STATUS_MD_Pos                                          (3)  /* (AS5600_STATUS) Magnet was detected position */
#define AS5600_STATUS_MD_Msk                                          ((0x1) << AS5600_STATUS_MD_Pos)  /* (AS5600_STATUS) Magnet was detected mask */
#define AS5600_STATUS_MD(value)                                       (AS5600_STATUS_MD_Msk & ((value) << AS5600_STATUS_MD_Pos))  /* Assignment of MD value in AS5600_STATUS register */
#define AS5600_STATUS_MD_Get(reg)                                     (((reg) & AS5600_STATUS_MD_Msk) >> AS5600_STATUS_MD_Pos)  /* Assignment of MD value in AS5600_STATUS register */
#define AS5600_STATUS_ML_Pos                                          (4)  /* (AS5600_STATUS) AGC maximum gain overflow, magnet too weak position */
#define AS5600_STATUS_ML_Msk                                          ((0x1) << AS5600_STATUS_ML_Pos)  /* (AS5600_STATUS) AGC maximum gain overflow, magnet too weak mask */
#define AS5600_STATUS_ML(value)                                       (AS5600_STATUS_ML_Msk & ((value) << AS5600_STATUS_ML_Pos))  /* Assignment of ML value in AS5600_STATUS register */
#define AS5600_STATUS_ML_Get(reg)                                     (((reg) & AS5600_STATUS_ML_Msk) >> AS5600_STATUS_ML_Pos)  /* Assignment of ML value in AS5600_STATUS register */
#define AS5600_STATUS_MH_Pos                                          (5)  /* (AS5600_STATUS) AGC minimum gain overflow, magnet too strong position */
#define AS5600_STATUS_MH_Msk                                          ((0x1) << AS5600_STATUS_MH_Pos)  /* (AS5600_STATUS) AGC minimum gain overflow, magnet too strong mask */
#define AS5600_STATUS_MH(value)                                       (AS5600_STATUS_MH_Msk & ((value) << AS5600_STATUS_MH_Pos))  /* Assignment of MH value in AS5600_STATUS register */
#define AS5600_STATUS_MH_Get(reg)                                     (((reg) & AS5600_STATUS_MH_Msk) >> AS5600_STATUS_MH_Pos)  /* Assignment of MH value in AS5600_STATUS register */
#define AS5600_STATUS_Msk                                             (0x38)  /* (AS5600_STATUS) Register mask */


/* AS5600_AGC, R: The AS5600 uses Automatic Gain Control in a closed loop to  compensate for variations of the magnetic field strength due  to changes of temperature, airgap between IC and magnet, and  magnet degradation. The AGC register indicates the gain */
#define AS5600_AGC_ADDR                                               (0x1a)  /* (AS5600_AGC) The AS5600 uses Automatic Gain Control in a closed loop to  compensate for variations of the magnetic field strength due  to changes of temperature, airgap between IC and magnet, and  magnet degradation. The AGC register indicates the gain address */

#define AS5600_AGC_AGC_Pos                                            (0)  /* (AS5600_AGC) AGC gain position */
#define AS5600_AGC_AGC_Msk                                            ((0xff) << AS5600_AGC_AGC_Pos)  /* (AS5600_AGC) AGC gain mask */
#define AS5600_AGC_AGC(value)                                         (AS5600_AGC_AGC_Msk & ((value) << AS5600_AGC_AGC_Pos))  /* Assignment of AGC value in AS5600_AGC register */
#define AS5600_AGC_AGC_Get(reg)                                       (((reg) & AS5600_AGC_AGC_Msk) >> AS5600_AGC_AGC_Pos)  /* Assignment of AGC value in AS5600_AGC register */
#define AS5600_AGC_Msk                                                (0xff)  /* (AS5600_AGC) Register mask */


/* AS5600_MAGNITUDE, R: The MAGNITUDE register indicates the magnitude value of the internal CORDIC */
#define AS5600_MAGNITUDE_ADDR_0                                       (0x1b)  /* (AS5600_MAGNITUDE) The MAGNITUDE register indicates the magnitude value of the internal CORDIC address */
#define AS5600_MAGNITUDE_ADDR_1                                       (0x1c)  /* (AS5600_MAGNITUDE) The MAGNITUDE register indicates the magnitude value of the internal CORDIC address */

#define AS5600_MAGNITUDE_MAGNITUDE_Pos                                (0)  /* (AS5600_MAGNITUDE) CORDIC magnitude position */
#define AS5600_MAGNITUDE_MAGNITUDE_Msk                                ((0xfff) << AS5600_MAGNITUDE_MAGNITUDE_Pos)  /* (AS5600_MAGNITUDE) CORDIC magnitude mask */
#define AS5600_MAGNITUDE_MAGNITUDE(value)                             (AS5600_MAGNITUDE_MAGNITUDE_Msk & ((value) << AS5600_MAGNITUDE_MAGNITUDE_Pos))  /* Assignment of MAGNITUDE value in AS5600_MAGNITUDE register */
#define AS5600_MAGNITUDE_MAGNITUDE_Get(reg)                           (((reg) & AS5600_MAGNITUDE_MAGNITUDE_Msk) >> AS5600_MAGNITUDE_MAGNITUDE_Pos)  /* Assignment of MAGNITUDE value in AS5600_MAGNITUDE register */
#define AS5600_MAGNITUDE_Msk                                          (0xfff)  /* (AS5600_MAGNITUDE) Register mask */


/* AS5600_BURN, W: The non-volatile memory is used to permanently program the  configuration.  */
#define AS5600_BURN_ADDR                                              (0xff)  /* (AS5600_BURN) The non-volatile memory is used to permanently program the  configuration.  address */

#define AS5600_BURN_CMD_Pos                                           (0)  /* (AS5600_BURN) Burn command position */
#define AS5600_BURN_CMD_Msk                                           ((0xff) << AS5600_BURN_CMD_Pos)  /* (AS5600_BURN) Burn command mask */
#define AS5600_BURN_CMD(value)                                        (AS5600_BURN_CMD_Msk & ((value) << AS5600_BURN_CMD_Pos))  /* Assignment of CMD value in AS5600_BURN register */
#define AS5600_BURN_CMD_Get(reg)                                      (((reg) & AS5600_BURN_CMD_Msk) >> AS5600_BURN_CMD_Pos)  /* Assignment of CMD value in AS5600_BURN register */
#define   AS5600_BURN_CMD_SETTING_Val                                 (64)  /* (AS5600_BURN) Burn setting command MANG can be written only if ZPOS and MPOS have never been permanently written (ZMCO = 00). The BURN_ SETTING command can be performed only one time value */
#define   AS5600_BURN_CMD_ANGLE_Val                                   (128)  /* (AS5600_BURN) Burn ang The BURN_ANGLE command can  be executed up to 3 times. ZMCO shows how many times ZPOS  and MPOS have been permanently written.le command value */
#define AS5600_BURN_CMD_SETTING                                       (AS5600_BURN_CMD_SETTING_Val << AS5600_BURN_CMD_Pos)  /* (AS5600_BURN) Burn setting command MANG can be written only if ZPOS and MPOS have never been permanently written (ZMCO = 00). The BURN_ SETTING command can be performed only one time */
#define AS5600_BURN_CMD_ANGLE                                         (AS5600_BURN_CMD_ANGLE_Val << AS5600_BURN_CMD_Pos)  /* (AS5600_BURN) Burn ang The BURN_ANGLE command can  be executed up to 3 times. ZMCO shows how many times ZPOS  and MPOS have been permanently written.le command */
#define AS5600_BURN_Msk                                               (0xff)  /* (AS5600_BURN) Register mask */


#endif  // AS5600_REGS
