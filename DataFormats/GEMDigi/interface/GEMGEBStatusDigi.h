#ifndef DataFormats_GEMDigi_GEMGEBStatusDigi_H
#define DataFormats_GEMDigi_GEMGEBStatusDigi_H

#include <cstdint>
#include "EventFilter/GEMRawToDigi/interface/GEBdata.h"

class GEMGEBStatusDigi {

 public:
  GEMGEBStatusDigi(gem::GEBdata geb);
  GEMGEBStatusDigi(){};
  
  uint32_t getZeroSupWordsCnt() const {return ZeroSupWordsCnt_;}   ///<Returns Zero Suppression flags
  uint16_t getVwh() const     {return Vwh_;}       ///<Returns VFAT word count (size of VFAT payload)
  uint16_t getErrorC() const  {return ErrorC_;}    ///<Returns thirteen flags in GEM Chamber Header
  uint16_t getOHCRC() const   {return OHCRC_;}     ///<Returns OH CRC 
  uint16_t getVwt() const     {return Vwt_;}       ///<Returns VFAT word count
  uint8_t  getInFu() const    {return InFu_;}      ///<Returns InFIFO underflow flag
  uint8_t  getStuckd() const  {return Stuckd_;}    ///<Returns Stuck data flag
  uint16_t getOHBC() const {return OHBC_;} //!OH BC
  uint32_t getOHEC() const {return OHEC_;} //!OH EC
  int nVfats() const {return int(ZeroSupWordsCnt_+Vwh_)/3;} //!OH EC

 private:
  
  uint16_t ZeroSupWordsCnt_;
  uint16_t Vwh_;
  uint16_t ErrorC_;
  uint16_t OHCRC_;     
  uint16_t Vwt_;      
  uint8_t  InFu_;    
  uint8_t  Stuckd_; 
  uint16_t OHBC_;
  uint32_t OHEC_;

};
#endif
