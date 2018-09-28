#ifndef EventFilter_GEMRawToDigi_VFATdata_h
#define EventFilter_GEMRawToDigi_VFATdata_h

#include <stdint.h>

namespace gem {
  class VFATdata 
  {
  public:
        
    VFATdata(){
      isBlockGood_ = true;
      ver_ = 1;
    }
    
    ~VFATdata(){}
    
    VFATdata(const uint16_t BC,
	     const uint8_t EC,
	     const uint8_t Pos,
	     const uint64_t lsDatas,
	     const uint64_t msDatas)
      {
	// this constructor only used for packing sim digis
	Header_ = 0x1E;
	CRCcheck_ = 0;
	
	BC_ = BC;
	EC_ = EC;
	Pos_ = Pos;
	
	lsData1_ = lsDatas >> 48;
	lsData2_ = lsDatas & 0x0000ffffffffffff;
	
	msData1_ = msDatas >> 48;
	msData2_ = msDatas & 0x0000ffffffffffff;
	ver_ = 3;
	
	crc_ = this->checkCRC();// crc check not yet implemented for v3
      }

    //!Read first word from the block.
    void read_fw(uint64_t word) { firstWord_ = word;}
    uint64_t get_fw() const { return firstWord_;}

    //!Read second word from the block.
    void read_sw(uint64_t word) { secondWord_ = word;}
    uint64_t get_sw() const { return secondWord_;}

    //!Read third word from the block.
    void read_tw(uint64_t word) { thirdWord_ = word;}
    uint64_t get_tw() const { return thirdWord_;}

    void setPhiPos(int i) {phiPos_ = i;}
    int phiPos() const {return phiPos_;}
    
    uint64_t  lsData() const {
      return static_cast<uint64_t>(lsData1_) << 48 | lsData2_;
    }
    uint64_t  msData() const {
      return static_cast<uint64_t>(msData1_) << 48 | msData2_;
    }
    
    uint16_t  bc() const {
      if (ver_==2) return BCv2_;
      return BC_;
    }
    uint8_t   ec() const {
      if (ver_==2) return ECv2_;
      return EC_;
    }

    void setVersion(int i) {ver_ = i;}
    int version() {return ver_;}
    void setIsBlockGood(bool t) { isBlockGood_ = t;}
    bool isBlockGood() const { return isBlockGood_;}
    
    bool quality() const {
      if (ver_ == 2)
	return (b1010_==10) && (b1100_==12) && (b1110_==14);// && (crc_==checkCRC());
      // quality test not yet implemented in v3
      return true;
    }

    /// v3
    uint8_t   header     () const {return Header_;      }
    uint8_t   crcCheck   () const {return CRCcheck_;    }
    uint8_t   position   () const {return Pos_;         }

    /// v2
    uint8_t   b1010      () const { return b1010_;      }
    uint8_t   b1100      () const { return b1100_;      }
    uint8_t   b1110      () const { return b1110_;      }
    uint8_t   flag       () const { return Flag_;       }
    uint16_t  chipID     () const { return ChipID_;     }
    uint16_t  crc        () const { return crc_;        }
    
    uint16_t crc_cal(uint16_t crc_in, uint16_t dato)
    {
      uint16_t v = 0x0001;
      uint16_t mask = 0x0001;
      uint16_t d=0x0000;
      uint16_t crc_temp = crc_in;
      unsigned char datalen = 16;
      for (int i=0; i<datalen; i++){
	if (dato & v) d = 0x0001;
	else d = 0x0000;
	if ((crc_temp & mask)^d) crc_temp = crc_temp>>1 ^ 0x8408;
	else crc_temp = crc_temp>>1;
	v<<=1;
      }
      return(crc_temp);
    }
    
    uint16_t checkCRC()
    {
      uint16_t vfatBlockWords[12];
      vfatBlockWords[11] = ((0x000f & b1010_)<<12) | bc();
      vfatBlockWords[10] = ((0x000f & b1100_)<<12) | ((0x00ff & ec()) <<4) | (0x000f & Flag_);
      vfatBlockWords[9]  = ((0x000f & b1110_)<<12) | ChipID_;
      vfatBlockWords[8]  = (0xffff000000000000 & msData()) >> 48;
      vfatBlockWords[7]  = (0x0000ffff00000000 & msData()) >> 32;
      vfatBlockWords[6]  = (0x00000000ffff0000 & msData()) >> 16;
      vfatBlockWords[5]  = (0x000000000000ffff & msData());
      vfatBlockWords[4]  = (0xffff000000000000 & lsData()) >> 48;
      vfatBlockWords[3]  = (0x0000ffff00000000 & lsData()) >> 32;
      vfatBlockWords[2]  = (0x00000000ffff0000 & lsData()) >> 16;
      vfatBlockWords[1]  = (0x000000000000ffff & lsData());

      uint16_t crc_fin = 0xffff;
      for (int i = 11; i >= 1; i--){
	crc_fin = crc_cal(crc_fin, vfatBlockWords[i]);
      }
      return(crc_fin);
    }
    
    static const int nChannels = 128;
    static const int sizeChipID = 12;
    
  private:
    
    int      ver_;         /// vfat version
    int      phiPos_;      /// phi position of vfat in chamber
    bool     isBlockGood_; ///<Shows if block is good (control bits, chip ID and CRC checks)
    
    /// VFAT data structure - 3 words of 64 bits each
    union {
      uint64_t firstWord_;
      // v3 dataformat
      struct {
	uint64_t msData1_  : 16; ///<channels from 65to128
	uint64_t BC_       : 16; ///<Bunch Crossing number, 16 bits
	uint64_t EC_       : 8;  ///<Event Counter, 8 bits
	uint64_t Header_   : 8;  ///<normally 0x1E. 0x5E indicates that the VFAT3 internal buffer is half-full, so it's like a warning
	uint64_t CRCcheck_ : 8;  ///<bits 183:177 are not used, should be 0, bit 176 is 1 if CTP7 detected a CRC mismatch
	uint64_t Pos_      : 8;  ///<an 8bit value indicating the VFAT position on this GEB (it can be 0 to 23)
      };
      // v2 dataformat
      struct {
	uint64_t msData1v2_: 16; ///<channels from 65to128 - placeholder since msData1 reads same info
	uint64_t ChipID_   : 12; ///<Chip ID, 12 bits
	uint64_t b1110_    : 4;  ///<1110:4 Control bits, shoud be 1110
	uint64_t Flag_     : 4;  ///<Control Flags: 4 bits, Hamming Error/AFULL/SEUlogic/SUEI2C
	uint64_t ECv2_     : 8;  ///<Event Counter, 8 bits
	uint64_t b1100_    : 4;  ///<1100:4, Control bits, shoud be 1100
	uint64_t BCv2_     : 12; ///<Bunch Crossing number, 12 bits
	uint64_t b1010_    : 4;  ///<1010:4 Control bits, shoud be 1010
      };
    };
    union {      
      uint64_t secondWord_;
      struct {
      	uint64_t lsData1_ : 16; ///<channels from 1to64 
      	uint64_t msData2_ : 48; ///<channels from 65to128
      };
    };
    union {      
      uint64_t thirdWord_;
      struct {
      	uint64_t crc_     : 16; ///<Check Sum value, 16 bits
      	uint64_t lsData2_ : 48; ///<channels from 1to64 
      };
    };
      
  };
}
#endif
