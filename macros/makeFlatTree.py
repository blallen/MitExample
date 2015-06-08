import ROOT

source = ROOT.TFile.Open('ntuples_noid.root')
tree = source.FindObjectAny('events')

output = ROOT.TFile('ntuples_noid_skim.root', 'recreate')
output.cd()
tree.CloneTree().Write()
output.Write()
output.Close()
source.Close()
