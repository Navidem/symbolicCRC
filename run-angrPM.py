#!/usr/bin/env python

import angr
import sys
import claripy

'''
Usage: python agr-pm.py <crc-check-bin> <symbolic input size>
'''

pm = True
interActive = False

subjectBin = sys.argv[1]
symb_arg_size = int(sys.argv[2])

if len(sys.argv) > 3:
        if "-i" in sys.argv[3:]:
                interActive = True

def basic_symbolic_execution():
        p = angr.Project(subjectBin, auto_load_libs=False)

        spiller = angr.exploration_techniques.Spiller()
        argv = [p.filename]   #argv[0]
        symb_arg = claripy.BVS('sym_arg', 8*symb_arg_size)
        argv.append(symb_arg)
        entryS = p.factory.entry_state(args = argv)
        if pm == False:
                customSim = p.factory.simgr(entryS)
        else:
                customSim = p.factory.simgr(entryS, veritesting=True)

        if interActive:
                import IPython; IPython.embed()

        customSim.use_technique(spiller)
        customSim.explore()
        slv = customSim.deadended[2].solver.eval(argv[1], cast_to=bytes)
        print (slv)


if __name__ == '__main__':
        basic_symbolic_execution()

