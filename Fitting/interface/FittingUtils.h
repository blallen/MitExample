#ifndef MITEXAMPLE_FITTING_FITTINGUTILS_H
#define MITEXAMPLE_FITTING_FITTINGUTILS_H

class RooDataSet;
class RooArgSet;
class TTree;

namespace mithep {
  
  class FittingUtils {

  public:
    static RooDataSet* createDataSet(TTree*, RooArgSet*, char const* name = "dataset", char const* title = "T&P dataset");
  };

}

#endif
