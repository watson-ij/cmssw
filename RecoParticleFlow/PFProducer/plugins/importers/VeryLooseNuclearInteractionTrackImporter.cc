#include "TrackFromParentImporter.h"
#include "DataFormats/ParticleFlowReco/interface/PFDisplacedTrackerVertex.h"

namespace {
  class VeryLooseNuclAdaptor {
  public:
    static bool check_importable(const reco::PFDisplacedTrackerVertexCollection::value_type& t) {
      const auto& vtx = t.displacedVertexRef();
      return ((vtx->isNucl() && vtx->position().rho() > 2.7) || (vtx->isNucl_Loose()) || (vtx->isNucl_Kink()));
    }
    static const reco::PFRecTrackRefVector& get_track_refs(
        const reco::PFDisplacedTrackerVertexCollection::value_type& t) {
      return t.pfRecTracks();
    }
    static void set_element_info(reco::PFBlockElement* elem,
                                 const edm::Ref<reco::PFDisplacedTrackerVertexCollection>& parref) {
      const reco::PFBlockElementTrack* tkelem = static_cast<const reco::PFBlockElementTrack*>(elem);
      const reco::PFRecTrackRef& reftrack = tkelem->trackRefPF();
      reco::PFBlockElement::TrackType tkType = reco::PFBlockElement::DEFAULT;
      if (parref->isIncomingTrack(reftrack))
        tkType = reco::PFBlockElement::T_TO_DISP;
      else if (parref->isOutgoingTrack(reftrack))
        tkType = reco::PFBlockElement::T_FROM_DISP;
      elem->setDisplacedVertexRef(parref, tkType);
    }
  };
}  // namespace

typedef pflow::importers::TrackFromParentImporter<reco::PFDisplacedTrackerVertexCollection, VeryLooseNuclAdaptor>
    VeryLooseNuclearInteractionTrackImporter;

DEFINE_EDM_PLUGIN(BlockElementImporterFactory,
                  VeryLooseNuclearInteractionTrackImporter,
                  "VeryLooseNuclearInteractionTrackImporter");
