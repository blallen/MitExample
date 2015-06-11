if [ -z $CMSSW_BASE ]
then
  echo ""
  echo " Setting up MitExample failed! (\$CMSSW_BASE = is empty)."
  echo ""
else
  export MIT_EXAMPLE_DIR="$CMSSW_BASE/src/MitExample"
  export PATH="$MIT_EXAMPLE_DIR/bin:${PATH}"
  export PYTHONPATH="$MIT_EXAMPLE_DIR/python:${PYTHONPATH}"
fi
