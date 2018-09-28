#include "DataFormats/GEMDigi/interface/GEMAMCStatusDigi.h"

GEMAMCStatusDigi::GEMAMCStatusDigi(gem::AMCdata amc)
{
  m_AMCnum = amc.amcNum();
  m_L1A = amc.l1A();
  m_BX = amc.bx();
  m_Dlength = amc.dlength();
  m_FV = amc.fv();
  m_Rtype = amc.runType();
  m_Param1 = amc.param1();
  m_Param2 = amc.param2();
  m_Param3 = amc.param3();
  m_orbitNum = amc.orbitNum();
  m_BID = amc.boardId();
  m_GEMDAV = amc.gemDAV();
  m_Bstatus = amc.bStatus();
  m_GDcount = amc.gdCount();
  m_ttsState = amc.ttsState();
  m_ChamT = amc.chamberTimeOut();
  m_OOSG = amc.oosGLIB();
};
