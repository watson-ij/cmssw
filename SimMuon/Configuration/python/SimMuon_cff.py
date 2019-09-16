import FWCore.ParameterSet.Config as cms

# Muon Digitization (CSC, DT, RPC electronics responce)
# CSC digitizer
#
from SimMuon.CSCDigitizer.muonCSCDigis_cfi import *
from CalibMuon.CSCCalibration.CSCChannelMapper_cfi import *
from CalibMuon.CSCCalibration.CSCIndexer_cfi import *
# DT digitizer
#
from SimMuon.DTDigitizer.muondtdigi_cfi import *
# RPC digitizer
# 
from SimMuon.RPCDigitizer.muonrpcdigi_cfi import *
muonDigiTask = cms.Task(simMuonCSCDigis, simMuonDTDigis, simMuonRPCDigis)
muonDigi = cms.Sequence(muonDigiTask)

from SimMuon.GEMDigitizer.muonGEMDigi_cff import *

_run3_muonDigiTask = muonDigiTask.copy()
_run3_muonDigiTask.add(muonGEMDigiTask)

from Configuration.Eras.Modifier_run2_GEM_2017_cff import run2_GEM_2017
run2_GEM_2017.toReplaceWith( muonDigiTask, _run3_muonDigiTask )
from Configuration.Eras.Modifier_run3_GEM_cff import run3_GEM
run3_GEM.toReplaceWith( muonDigiTask, _run3_muonDigiTask )
