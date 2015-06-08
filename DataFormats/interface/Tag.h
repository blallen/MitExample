#ifndef MITEXAMPLE_DATAFORMATS_TAG_H
#define MITEXAMPLE_DATAFORMATS_TAG_H

#include "Rtypes.h"

namespace mithep {

  class TnPEvent;

  class Tag {
  public:
    Tag(TnPEvent&, UInt_t);

    Float_t& charge;
    Float_t& pt;
    Float_t& eta;
    Float_t& phi;
    Float_t& px;
    Float_t& py;
    Float_t& pz;
    Float_t& energy;

  private:
    Tag(); // hide default constructor
  };

}

#endif
