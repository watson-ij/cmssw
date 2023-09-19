import FWCore.ParameterSet.Config as cms

from DQM.GEM.GEMDigiSource_cfi import *
from DQM.GEM.GEMRecHitSource_cfi import *
from DQM.GEM.GEMDAQStatusSource_cfi import *
from DQM.GEM.gemEfficiencyAnalyzer_cff import *

from DQMOffline.MuonDPG.gemTnPEfficiencyTask_cfi import *

GEMDigiSource.runType      = "offline"
GEMRecHitSource.runType    = "offline"
GEMDAQStatusSource.runType = "offline"

from Configuration.Eras.Modifier_run3_GEM_cff import run3_GEM
from Configuration.Eras.Modifier_phase2_GEM_cff import phase2_GEM
run3_GEM.toModify(GEMRecHitSource, recHitsInputLabel = cms.InputTag("gemRecHits", "allGemRechits"))
phase2_GEM.toModify(GEMRecHitSource, recHitsInputLabel = cms.InputTag("gemRecHits", ""))

gemSources = cms.Sequence(
    GEMDigiSource *
    GEMRecHitSource *
    GEMDAQStatusSource *
    gemEfficiencyAnalyzerTightGlbSeq *
    gemEfficiencyAnalyzerStaSeq *
    gemTnPEfficiencyMonitor
)
