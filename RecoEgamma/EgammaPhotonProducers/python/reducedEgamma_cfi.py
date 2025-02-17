import FWCore.ParameterSet.Config as cms

from RecoEgamma.EgammaIsolationAlgos.interestingEgammaIsoDetIdsSequence_cff import interestingEgammaIsoHCALSel
from RecoJets.Configuration.CaloTowersES_cfi import *

reducedEgamma = cms.EDProducer("ReducedEGProducer",
  keepPfSuperclusterPtMin = cms.double(5.),
  keepPfSuperclusterAbsetaMax = cms.double(2.5),
  relinkSuperclusterPtMin = cms.double(99999.), # no SC linking
  keepPhotons = cms.string("hadTowOverEm()<0.15 && pt>10 && (pt>14 || chargedHadronIso()<10)"), #keep in output
  slimRelinkPhotons = cms.string("hadTowOverEm()<0.15 && pt>10 && (pt>14 || chargedHadronIso()<10)"), #keep only slimmed SuperCluster plus seed cluster
  relinkPhotons = cms.string("(r9()>0.8 || chargedHadronIso()<20 || chargedHadronIso()<0.3*pt())"), #keep all associated clusters/rechits/conversions
  keepOOTPhotons = cms.string("pt>10"), #keep in output
  slimRelinkOOTPhotons = cms.string("pt>10"), #keep only slimmed SuperCluster plus seed cluster
  relinkOOTPhotons = cms.string("(r9()>0.8)"), #keep all associated clusters/rechits/conversions
  keepGsfElectrons = cms.string(""), #keep in output
  slimRelinkGsfElectrons = cms.string(""), #keep only slimmed SuperCluster plus seed cluster
  relinkGsfElectrons = cms.string("pt>5"), #keep all associated clusters/rechits/conversions
  pflowSuperclusters = cms.InputTag("superClusterMerger"),
  photons = cms.InputTag("gedPhotons"),
  ootPhotons = cms.InputTag("ootPhotons"),
  gsfElectrons = cms.InputTag("gedGsfElectrons"),
  conversions = cms.InputTag("allConversions"),
  gsfTracks = cms.InputTag("electronGsfTracks"),
  singleConversions = cms.InputTag("particleFlowEGamma"),
  barrelEcalHits = cms.InputTag("reducedEcalRecHitsEB"),
  endcapEcalHits = cms.InputTag("reducedEcalRecHitsEE"),
  preshowerEcalHits = cms.InputTag("reducedEcalRecHitsES"),
  hbheHits= cms.InputTag("reducedHcalRecHits","hbhereco"),
  photonsPFValMap = cms.InputTag("particleBasedIsolation","gedPhotons"),
  gsfElectronsPFValMap = cms.InputTag("particleBasedIsolation","gedGsfElectrons"),
  photonIDSources = cms.VInputTag(),
  photonIDOutput = cms.vstring(),
  gsfElectronIDSources = cms.VInputTag(),
  gsfElectronIDOutput = cms.vstring(),
  photonFloatValueMapSources = cms.VInputTag(),
  photonFloatValueMapOutput = cms.vstring(),
  ootPhotonFloatValueMapSources = cms.VInputTag(),
  ootPhotonFloatValueMapOutput = cms.vstring(),
  hiPhotonIsolationMapInput = cms.InputTag(""),
  hiPhotonIsolationMapOutput = cms.string(""),
  gsfElectronFloatValueMapSources = cms.VInputTag(),
  gsfElectronFloatValueMapOutput = cms.vstring(),
  applyPhotonCalibOnData = cms.bool(False),
  applyPhotonCalibOnMC = cms.bool(False),
  applyGsfElectronCalibOnData = cms.bool(False),
  applyGsfElectronCalibOnMC = cms.bool(False), 
  photonCalibEnergySource = cms.InputTag(""),
  photonCalibEnergyErrSource = cms.InputTag(""),
  gsfElectronCalibEnergySource = cms.InputTag(""),
  gsfElectronCalibEnergyErrSource = cms.InputTag(""),
  gsfElectronCalibEcalEnergySource = cms.InputTag(""),
  gsfElectronCalibEcalEnergyErrSource = cms.InputTag(""),
  hcalHitSel = interestingEgammaIsoHCALSel
)

superClusterMerger = cms.EDProducer("EgammaSuperClusterMerger",
  src = cms.VInputTag(
    cms.InputTag("particleFlowSuperClusterECAL:particleFlowSuperClusterECALBarrel"),
    cms.InputTag("particleFlowSuperClusterECAL:particleFlowSuperClusterECALEndcapWithPreshower"),
  )
)

from Configuration.Eras.Modifier_phase2_common_cff import phase2_common
phase2_common.toModify(reducedEgamma, 
        preshowerEcalHits = "",
)

from RecoEgamma.EgammaPhotonProducers.reducedEgamma_tools import calibrateReducedEgamma
from Configuration.ProcessModifiers.run2_miniAOD_UL_cff import run2_miniAOD_UL
modifyReducedEGammaRun2MiniAOD = (run2_miniAOD_UL).makeProcessModifier(calibrateReducedEgamma)

from Configuration.ProcessModifiers.pp_on_AA_cff import pp_on_AA

pp_on_AA.toModify(
    reducedEgamma,
    ootPhotons = "",
    keepPhotons = "pt>15 && abs(eta)<2.5",
    slimRelinkPhotons = "pt>15 && abs(eta)<2.5",
    relinkPhotons = "pt>15 && abs(eta)<2.5",
    hiPhotonIsolationMapInput = "photonIsolationHIProducerppGED",
    hiPhotonIsolationMapOutput = "photonIsolationHIProducerppGED"
)

from Configuration.ProcessModifiers.run2_miniAOD_pp_on_AA_103X_cff import run2_miniAOD_pp_on_AA_103X
run2_miniAOD_pp_on_AA_103X.toModify(
    reducedEgamma,
    photonsPFValMap = "pfEGammaToCandidateRemapperCleaned:photons",
    gsfElectronsPFValMap = "pfEGammaToCandidateRemapperCleaned:electrons"
)

from Configuration.ProcessModifiers.egamma_lowPt_exclusive_cff import egamma_lowPt_exclusive
egamma_lowPt_exclusive.toModify(reducedEgamma, keepPfSuperclusterPtMin = 1.0, keepPhotons = "", slimRelinkPhotons = "", relinkPhotons = "", relinkGsfElectrons = "")
