#ifndef EventFilter_GEMRawToDigi_VFATdata_h
#define EventFilter_GEMRawToDigi_VFATdata_h

#include <stdint.h>

namespace gem {
  class VFATdata 
  {
    /// VFAT data structure - 3 words of 64 bits each
    union {
      uint64_t m_firstWord;
      // v3 dataformat
      struct {
	uint64_t m_msData1 : 16; ///<channels from 65to128
	uint16_t m_BC      : 16; ///<Bunch Crossing number, 12 bits
	uint8_t  m_EC      : 8;  ///<Event Counter, 8 bits
	uint8_t  m_Header  : 8;  ///<normally 0x1E. 0x5E indicates that the VFAT3 internal buffer is half-full, so it's like a warning
	uint8_t  m_CRCcheck: 8;  ///<bits 183:177 are not used, should be 0, bit 176 is 1 if CTP7 detected a CRC mismatch
	uint8_t  m_Pos     : 8;  ///<an 8bit value indicating the VFAT position on this GEB (it can be 0 to 23)
      };      
      // v2 dataformat
      struct {
	uint64_t m_msData1v2 : 16; ///<channels from 65to128 - placeholder since m_msData1 reads same info
	uint16_t m_ChipID  : 12; ///<Chip ID, 12 bits
	uint8_t  m_b1110   : 4;  ///<1110:4 Control bits, shoud be 1110
	uint8_t  m_Flag    : 4;  ///<Control Flags: 4 bits, Hamming Error/AFULL/SEUlogic/SUEI2C
	uint8_t  m_ECv2    : 8;  ///<Event Counter, 8 bits
	uint8_t  m_b1100   : 4;  ///<1100:4, Control bits, shoud be 1100
	uint16_t m_BCv2    : 12; ///<Bunch Crossing number, 12 bits
	uint8_t  m_b1010   : 4;  ///<1010:4 Control bits, shoud be 1010
      };
    };
    union {      
      uint64_t m_secondWord;
      struct {
      	uint64_t m_lsData1 : 16; ///<channels from 1to64 
      	uint64_t m_msData2 : 48; ///<channels from 65to128
      };
    };
    union {      
      uint64_t m_thirdWord;
      struct {
      	uint64_t m_crc     : 16; ///<Check Sum value, 16 bits
      	uint64_t m_lsData2 : 48; ///<channels from 1to64 
      };
    };
    
  public:
        
    VFATdata(){
      m_isBlockGood = true;
    }
    ~VFATdata(){}
    VFATdata(const uint16_t BC,
	     const uint8_t EC,
	     const uint8_t Pos,
	     const uint64_t lsDatas,
	     const uint64_t msDatas)
      {
	// this constructor only used for packing sim digis
	m_Header = 0x1E;
	m_CRCcheck = 0;
	
	m_BC = BC;
	m_EC = EC;
	m_Pos = Pos;
	
	m_lsData1 = lsDatas >> 48;
	m_lsData2 = lsDatas & 0x0000ffffffffffff;
	
	m_msData1 = msDatas >> 48;
	m_msData2 = msDatas & 0x0000ffffffffffff;
	m_ver = 3;
	
	m_crc = this->checkCRC();// crc check not yet implemented for v3
      }

    //!Read first word from the block.
    void read_fw(uint64_t word) { m_firstWord = word;}
    uint64_t get_fw() const { return m_firstWord;}

    //!Read second word from the block.
    void read_sw(uint64_t word) { m_secondWord = word;}
    uint64_t get_sw() const { return m_secondWord;}

    //!Read third word from the block.
    void read_tw(uint64_t word) { m_thirdWord = word;}
    uint64_t get_tw() const { return m_thirdWord;}

    void setPhiPos(int i) {m_phiPos = i;}
    int phiPos() const {return m_phiPos;}
    
    uint64_t  lsData() const {
      return static_cast<uint64_t>(m_lsData1) << 48 | m_lsData2;
    }
    uint64_t  msData() const {
      return static_cast<uint64_t>(m_msData1) << 48 | m_msData2;
    }
    
    uint16_t  bc() const {
      if (m_ver==2) return m_BCv2;
      return m_BC;
    }
    uint8_t   ec() const {
      if (m_ver==2) return m_ECv2;
      return m_EC;
    }

    void setVersion(int i) {m_ver = i;}
    void setIsBlockGood(bool t) { m_isBlockGood = t;}
    bool isBlockGood() const { return m_isBlockGood;}
    
    bool quality() {
      if (m_ver == 2)
	return (m_b1010==10) && (m_b1100==12) && (m_b1110==14) && (m_crc==this->checkCRC());
      // quality test not yet implemented in v3
      return true;
    }

    /// v3
    uint8_t   header     () const {return m_Header;      }
    uint8_t   crcCheck   () const {return m_CRCcheck;    }
    uint8_t   position   () const {return m_Pos;         }

    /// v2
    uint8_t   b1010      () const { return m_b1010;      }
    uint8_t   b1100      () const { return m_b1100;      }
    uint8_t   flag       () const { return m_Flag;       }
    uint8_t   b1110      () const { return m_b1110;      }
    uint16_t  chipID     () const { return m_ChipID;     }
    uint16_t  crc        () const { return m_crc;        }
    
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
      vfatBlockWords[11] = ((0x000f & m_b1010)<<12) | this->bc();
      vfatBlockWords[10] = ((0x000f & m_b1100)<<12) | ((0x00ff & this->ec()) <<4) | (0x000f & m_Flag);
      vfatBlockWords[9]  = ((0x000f & m_b1110)<<12) | m_ChipID;
      vfatBlockWords[8]  = (0xffff000000000000 & this->msData()) >> 48;
      vfatBlockWords[7]  = (0x0000ffff00000000 & this->msData()) >> 32;
      vfatBlockWords[6]  = (0x00000000ffff0000 & this->msData()) >> 16;
      vfatBlockWords[5]  = (0x000000000000ffff & this->msData());
      vfatBlockWords[4]  = (0xffff000000000000 & this->lsData()) >> 48;
      vfatBlockWords[3]  = (0x0000ffff00000000 & this->lsData()) >> 32;
      vfatBlockWords[2]  = (0x00000000ffff0000 & this->lsData()) >> 16;
      vfatBlockWords[1]  = (0x000000000000ffff & this->lsData());

      uint16_t crc_fin = 0xffff;
      for (int i = 11; i >= 1; i--){
	crc_fin = this->crc_cal(crc_fin, vfatBlockWords[i]);
      }
      return(crc_fin);
    }
    
    static const int nChannels = 128;
    static const int sizeChipID = 12;
    
  private:
    
    int      m_ver;         /// vfat version
    int      m_phiPos;      /// phi position of vfat in chamber
    bool     m_isBlockGood; ///<Shows if block is good (control bits, chip ID and CRC checks)
  
  };
}
#endif
