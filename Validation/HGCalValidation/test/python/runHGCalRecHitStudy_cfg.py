import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

from Configuration.Eras.Era_Phase2C11I13M9_cff import Phase2C11I13M9
process = cms.Process('HGCGeomAnalysis',Phase2C11I13M9)
#process.load('Configuration.Geometry.GeometryExtended2026D77_cff')
#process.load('Configuration.Geometry.GeometryExtended2026D77Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2026D83_cff')
process.load('Configuration.Geometry.GeometryExtended2026D83Reco_cff')

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic_T21', '')

process.MessageLogger.cerr.FwkReport.reportEvery = 2

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('file:step3.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.load('Validation.HGCalValidation.hgcalRecHitStudy_cff')

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('hgcRecHitD83tt.root'),
                                   closeFileFast = cms.untracked.bool(True)
                                   )

SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",ignoreTotal = cms.untracked.int32(1) )

process.p = cms.Path(process.hgcalRecHitStudyEE+process.hgcalRecHitStudyFH+process.hgcalRecHitStudyBH)

