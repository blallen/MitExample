import sys
import ROOT

ROOT.gSystem.Load('libRooFit.so')
ROOT.gSystem.Load('libMitExampleDataFormats.so')
ROOT.gSystem.Load('libMitExampleFitting.so')

RooFit = ROOT.RooFit
mithep = ROOT.mithep

def fitZpeak( mass, name ):
    '''Fit mass peak with BreitWigner signal plus Exponential exponential background and extract number of electrons within Z window [80,100]'''

    print 'Fitting for '+name
    
    sig = workspace.factory('BreitWigner::sig_'+name+'(mass_'+name+', m0_'+name+'[90.,50.,130.], w_'+name+'[1.0,0.,5.0])')
    bkg = workspace.factory('Exponential::bkg_'+name+'(mass_'+name+', lambda_'+name+'[-1.,-100.,0.])')

    nsig = workspace.factory('nsig_'+name+'[200.,0.,'+str(nEvents)+']')
    nbkg = workspace.factory('nbkg_'+name+'[200.,0.,'+str(nEvents)+']')
    
    spb = workspace.factory('SUM::spb_'+name+'(nsig_'+name+' * sig_'+name+',nbkg_'+name+' * bkg_'+name+')')

    spb.fitTo(dataset)

    frame = mass.frame()
    dataset.plotOn(frame)
    spb.plotOn(frame)
    
    canvas = ROOT.TCanvas()
    frame.Draw()
    canvas.SaveAs('DiEleMass_'+name+'.png')

    mass.setRange('Zpeak_'+name, 80., 100.)
    mass.setRange('all_'+name,0.,maxmass)
    tmp = ROOT.RooArgSet(mass)
    fReal = sig.createIntegral( tmp, 'Zpeak_'+name).getVal() / sig.createIntegral( tmp, 'all_'+name).getVal()
    nReal = fReal * ( nsig.getVal())
    
    print 'Total number of events is '+str(nsig.getVal())
    print 'Fraction on Z peak is '+str(fReal)
    print 'Number of Events on Z peak is '+str(nReal)    

    return nReal

###

source = ROOT.TFile.Open('ntuples_noid_skim.root')
tree = source.Get('events')
nEvents = tree.GetEntries()
print 'Total number of events: '+str(nEvents)
maxmass = 200.

# Yutaro code
workspace = ROOT.RooWorkspace('workspace')
mass_noID = workspace.factory('mass_noID[0.,'+str(maxmass)+']')
mass_passID = workspace.factory('mass_passID[0.,'+str(maxmass)+']')
argset = ROOT.RooArgSet(mass_noID, mass_passID)

dataset = mithep.FittingUtils.createDataSet(tree, argset)

nNoID = fitZpeak( mass_noID, 'noID')
nPassID = fitZpeak( mass_passID, 'passID')

eff = nPassID / nNoID

print 'Electron Efficiency is '+str(eff)

sys.stdin.readline() # let the script hang around
