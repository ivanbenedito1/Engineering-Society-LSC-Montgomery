/*!
 * @file MAX30101.h
 */

#ifndef _MAX30101_H
#define _MAX30101_H

#include "MAX3010x_multiLed_core.h"

/**
 * MAX30101 Sample Data
 */
struct MAX30101Sample {
  union {
    struct {
      uint32_t red;     //!< Measurement Value for the Red LED
      uint32_t ir;      //!< Measurement Value for the IR LED
    };                  
    uint32_t slot[4];   
  };                    
  bool valid;           
};

/**
 * MAX30101 Sensor Driver
 */
class MAX30101 : public MAX3010xMultiLed<MAX30101, MAX30101Sample> {
  friend class MAX3010xMultiLed<MAX30101, MAX30101Sample>; 
  friend class MAX3010x<MAX30101, MAX30101Sample>; 
  
  static const uint8_t FIFO_SIZE = 32;            
  static const uint8_t MAX_ACTIVE_LEDS = 4;       
  
  static const uint8_t INT_CNT = 5;
  static const uint8_t INT_CFG_REG[INT_CNT];     
  static const uint8_t INT_CFG_BIT[INT_CNT];      
  static const uint8_t INT_ST_REG[INT_CNT];      
  static const uint8_t INT_ST_BIT[INT_CNT];      

  bool setDefaultConfiguration();
public:
  /*
   * MAX30101 Interrupts
   */
  static const uint8_t INT_A_FULL = 0;            
  static const uint8_t INT_TEMP_RDY = 1;          
  static const uint8_t INT_PPG_RDY = 2;           
  static const uint8_t INT_ALC_OVF = 3;           
  static const uint8_t INT_PWR_RDY = 4;           
  
  MAX30101(uint8_t addr = MAX3010x_ADDR, TwoWire& wire = Wire);
  
  /**
   * LED (IR or red)
   */
  enum Led {
    SENSOR_LED_RED = 0,       //!< Red LED
    SENSOR_LED_IR = 1,        //!< IR LED
    SENSOR_LED_GREEN_CH1 = 2, //!< Green LED (Channel 1)
    SENSOR_LED_GREEN_CH2 = 3  //!< Green LED (Channel 2)
  };
  
  bool setLedCurrent(Led led, uint8_t current);
  
  /**
   * Slot Configuration
   */
  enum SlotConfiguration {
    SLOT_OFF = 0,         
    SLOT_LED_RED = 1,     
    SLOT_LED_IR = 2,      
    SLOT_LED_GREEN = 3    
  };
  
  struct MultiLedConfiguration {
    SlotConfiguration slot[4]; 
  };
  
  bool setMultiLedConfiguration(const MultiLedConfiguration& cfg);
};

#endif
