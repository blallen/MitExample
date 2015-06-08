import sys
import ROOT

ROOT.gSystem.Load('libRooFit.so')
ROOT.gSystem.Load('libMitExampleDataFormats.so')
ROOT.gSystem.Load('libMitExampleFitting.so')

RooFit = ROOT.RooFit
mithep = ROOT.mithep

source = ROOT.TFile.Open('ntuples_noid_skim.root')
tree = source.Get('events')

'''
output = ROOT.TFile.Open('TnPlots_noid.root', 'recreate')



hDiEleMass = ROOT.TH1F('hDiEleMass', 'M_{ee}', 1000, 0., 200.)

for event in tree :
    for iP in xrange(event.nPairs) :
        tag = mithep.Tag(pair.first)
        probe = mithep.DataFormats.TnPEvent.Probe(pair.second)

        if tag.charge == probe.charge : continue

        pTag = ROOT.TLorentzVector()
        pProbe = ROOT.TLorentzVector()
        pTag.SetPtEtaPhiE(tag.pt, tag.eta, tag.phi, tag.energy)
        pProbe.SetPtEtaPhiE(tag.pt, tag.eta, tag.phi, tag.energy)

        mass = (pTag + pProbe).M()
        hDiEleMass.Fill(mass)
        
output.cd()
hDiEleMass.Write()
output.Write()
output.Close()
source.Close()
'''
        
# Yutaro code
workspace = ROOT.RooWorkspace('workspace')
mass = workspace.factory('mass[0.,200.]')
argset = ROOT.RooArgSet(mass)

dataset = mithep.FittingUtils.createDataSet(tree, argset)

sig = workspace.factory('BreitWigner::sig(mass, m0[90.,50.,130.], w[1.0,0.,5.0])')
bkg = workspace.factory('Exponential::bkg(mass, lambda[-1.,-100.,0.])')
spb = workspace.factory('SUM::spb(nsig[200.,0.,10000.] * sig,nbkg[200.,0.,10000.] * bkg)')

spb.fitTo(dataset)

frame = mass.frame()
dataset.plotOn(frame)
spb.plotOn(frame)

canvas = ROOT.TCanvas()
frame.Draw()
canvas.SaveAs('DiEleMass.png')

sys.stdin.readline() # let the script hang around
