import ROOT
from optparse import OptionParser

### Read Rootfile name from command line
parser = OptionParser()
parser.add_option('-f', help='root file to make flat', dest='FileName', action='store', metavar='<name>')

(opts, args) = parser.parse_args()

mandatories = ['FileName']
for m in mandatories:
    if not opts.__dict__[m]:
        print "\nMandatory option is missing\n"
        parser.print_help()
        exit(-1)

FileName = opts.FileName.split('.')

### Make FlatTree

print FileName[0]
source = ROOT.TFile.Open(FileName[0]+'.root')
tree = source.FindObjectAny('events')

output = ROOT.TFile(FileName[0]+'_flat.root', 'recreate')
output.cd()
tree.CloneTree().Write()
output.Write()
output.Close()
source.Close()
