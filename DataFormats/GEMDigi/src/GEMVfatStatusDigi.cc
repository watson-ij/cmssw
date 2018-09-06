#include "DataFormats/GEMDigi/interface/GEMVfatStatusDigi.h"

GEMVfatStatusDigi::GEMVfatStatusDigi(gem::VFATdata vfat)
{
  quality_ = 0;
  if (vfat.crc() != vfat.checkCRC()) quality_ |= 1UL << 1;
  if (vfat.b1010() != 10) quality_ |= 1UL << 2;
  if (vfat.b1100() != 12) quality_ |= 1UL << 3;
  if (vfat.b1110() != 14) quality_ |= 1UL << 4;
    
  flag_        = vfat.flag();
  isBlockGood_ = vfat.isBlockGood();
  position_    = vfat.phiPos();
};
