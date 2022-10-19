import FWCore.ParameterSet.Config as cms
from DQMServices.Core.DQMEDHarvester import DQMEDHarvester

gemTnPEfficiencyClient = DQMEDHarvester("TnPEfficiencyClient",
                                       #Histogram names listed as "passProbeHistoName:failProbeHistoName"
                                       subsystem = cms.untracked.string("GEM"),
                                       histoNames = cms.untracked.vstring("GEM_nPassingProbe_allCh:GEM_nFailingProbe_allCh",
                                                                          "GEM_nPassingProbe_allCh_1D:GEM_nFailingProbe_allCh_1D",
                                                                          "GEM_nPassingProbe_chamber_1D:GEM_nFailingProbe_chamber_1D",
                                                                          "GEM_nPassingProbe_chamber_p1_1D:GEM_nFailingProbe_chamber_p1_1D",
                                                                          "GEM_nPassingProbe_chamber_p2_1D:GEM_nFailingProbe_chamber_p2_1D",
                                                                          "GEM_nPassingProbe_chamber_n1_1D:GEM_nFailingProbe_chamber_n1_1D",
                                                                          "GEM_nPassingProbe_chamber_n2_1D:GEM_nFailingProbe_chamber_n2_1D",
                                                                          "GEM_nPassingProbe_pt_1D:GEM_nFailingProbe_pt_1D",
                                                                          "GEM_nPassingProbe_eta_1D:GEM_nFailingProbe_eta_1D",
                                                                          "GEM_nPassingProbe_phi_1D:GEM_nFailingProbe_phi_1D",
                                                                          "GEM_nPassingProbe_Ch_region:GEM_nFailingProbe_Ch_region",
                                                                          "GEM_nPassingProbe_Ch_region_GE1:GEM_nFailingProbe_Ch_region_GE1",
                                                                          "GEM_nPassingProbe_Ch_region_GE1_NoL:GEM_nFailingProbe_Ch_region_GE1_NoL",
                                                                          "GEM_nPassingProbe_Ch_eta:GEM_nFailingProbe_Ch_eta",
                                                                          "GEM_nPassingProbe_Ch_phi:GEM_nFailingProbe_Ch_phi",
                                                                          "ME0_nPassingProbe_chamber_1D:ME0_nFailingProbe_chamber_1D",
                                                                          "GEM_nPassingProbe_Ch_region_layer_phase2:GEM_nFailingProbe_Ch_region_layer_phase2"),
                                       diagnosticPrescale = cms.untracked.int32(1))
