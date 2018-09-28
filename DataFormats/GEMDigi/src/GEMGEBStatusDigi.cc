#include "DataFormats/GEMDigi/interface/GEMGEBStatusDigi.h"

GEMGEBStatusDigi::GEMGEBStatusDigi(gem::GEBdata geb)
{
  ZeroSupWordsCnt_ = geb.zeroSupWordsCnt();
  Vwh_    = geb.vwh();
  ErrorC_ = geb.errorC();
  OHCRC_  = geb.ohCRC();
  Vwt_    = geb.vwt();
  InFu_   = geb.inFu();
  Stuckd_ = geb.stuckd();
  OHBC_   = geb.ohBC();
  OHEC_   = geb.ohEC();
};
