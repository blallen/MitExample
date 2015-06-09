import sys
import ROOT

ROOT.gSystem.Load('libRooFit.so')
ROOT.gSystem.Load('libMitExampleDataFormats.so')
ROOT.gSystem.Load('libMitExampleFitting.so')


def fitZpeak( mass, name, dataset ):
    sig = workspace.factory('BreitWigner::sig_'+name+'(mass_'+name+', m0_'+name+'[90.,50.,130.], w_'+name+'[1.0,0.,5.0])')
    bkg = workspace.factory('Exponential::bkg_'+name+'(mass_'+name+', lambda_'+name+'[-1.,-100.,0.])')
    spb = workspace.factory('SUM::spb_'+name+'(nsig_'+name+'[200.,0.,10000.] * sig_'+name+',nbkg_'+name+'[200.,0.,1000.] * bkg_'+name+')')

    spb.fitTo(dataset)

    frame = mass.frame()
    dataset.plotOn(frame)
    spb.plotOn(frame)
    
    canvas = ROOT.TCanvas()
    frame.Draw()
    canvas.SaveAs('DiEleMass_'+name+'.png')

    return spb
    
    # bad integral code
    '''
    mass.setRange('Zpeak', 80., 100.)
    tmp = ROOT.RooArgSet(mass)
    print mass
    nReal = spb.createIntegral( mass, mass, RooFit.Range('Zpeak'))
    
    return nReal
    '''

RooFit = ROOT.RooFit
mithep = ROOT.mithep

source = ROOT.TFile.Open('ntuples_noid_skim.root')
tree = source.Get('events')
        
# Yutaro code
workspace = ROOT.RooWorkspace('workspace')
mass_noID = workspace.factory('mass_noID[0.,200.]')
mass_passID = workspace.factory('mass_passID[0.,200.]')
argset = ROOT.RooArgSet(mass_noID, mass_passID)

dataset = mithep.FittingUtils.createDataSet(tree, argset)

spb_noID = fitZpeak( mass_noID, 'noID', dataset)
spb_passID = fitZpeak( mass_passID, 'passID', dataset)

'''
nNoID = fitZpeak( mass_noID, 'noID', dataset)
nPassID = fitZpeak( mass_passID, 'passID', dataset)

eff = nPassID / nNoID

print 'Electron Efficiency is '+str(eff)
'''

sys.stdin.readline() # let the script hang around


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
