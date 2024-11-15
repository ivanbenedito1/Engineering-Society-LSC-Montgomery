/*!
 * @file MAX30102.h
 */

#ifndef _MAX30102_H
#define _MAX30102_H

#include "MAX3010x_multiLed_core.h"

/**
 * MAX30102 Sample Data
 */
struct MAX30102Sample {
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
 * MAX30102 Sensor Driver
 */
class MAX30102 : public MAX3010xMultiLed<MAX30102, MAX30102Sample> {
  friend class MAX3010xMultiLed<MAX30102, MAX30102Sample>; 
  friend class MAX3010x<MAX30102, MAX30102Sample>; 
  
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
   * MAX30102 Interrupts
   */
  static const uint8_t INT_A_FULL = 0;            
  static const uint8_t INT_TEMP_RDY = 1;          
  static const uint8_t INT_PPG_RDY = 2;           
  static const uint8_t INT_ALC_OVF = 3;           
  static const uint8_t INT_PWR_RDY = 4;           
  
  MAX30102(uint8_t addr = MAX3010x_ADDR, TwoWire& wire = Wire);
  
  /**
   * LED Identifiers (unique names)
   */
  enum Led {
    SENSOR_LED_RED = 0,       //!< Red LED
    SENSOR_LED_IR = 1         //!< IR LED
  };
  
  bool setLedCurrent(Led led, uint8_t current);
  
  /**
   * Slot Configuration
   */
  enum SlotConfiguration {
    SLOT_OFF = 0,         
    SLOT_LED_RED = 1,     
    SLOT_LED_IR = 2       
  };
  
  struct MultiLedConfiguration {
    SlotConfiguration slot[4]; 
  };
  
  bool setMultiLedConfiguration(const MultiLedConfiguration& cfg);
};

#endif
