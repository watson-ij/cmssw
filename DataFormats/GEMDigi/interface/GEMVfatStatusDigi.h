#ifndef DataFormats_GEMDigi_GEMVfatStatusDigi_H
#define DataFormats_GEMDigi_GEMVfatStatusDigi_H

#include <cstdint>
#include "EventFilter/GEMRawToDigi/interface/VFATdata.h"

class GEMVfatStatusDigi {

 public:
  GEMVfatStatusDigi(gem::VFATdata vfat); 
  GEMVfatStatusDigi(){} 
  
  bool     isBlockGood() const { return isBlockGood_; } 
  uint8_t  quality() const { return quality_; }
  uint8_t  flag() const { return flag_; }
  int      position() const { return position_; }
  uint16_t bc() const { return bc_; }
  uint8_t  ec() const { return ec_; }
  
 private:

  bool     isBlockGood_;///<Shows if block is good (control bits, chip ID and CRC checks)
  uint8_t  quality_;    /// quality flag - bit: 0 good, 1 crc fail, 2 b1010 fail, 3 b1100 fail, 4 b1110
  uint8_t  flag_;       ///<Control Flags: 4 bits, Hamming Error/AFULL/SEUlogic/SUEI2C
  int      position_;   /// vfat postion in chamber
  uint16_t bc_;
  uint8_t  ec_;
  
};
#endif
