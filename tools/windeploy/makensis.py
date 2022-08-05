#!/usr/bin/python
import os, sys
from argparse import ArgumentParser
from subprocess import call

ap = ArgumentParser(description="Make the NSIS installer package")
ap.add_argument ("--builddir", nargs=1, metavar="builddir", default=['.'], help="Use custom dist dir")
ap.add_argument ("--certificate", nargs=1, metavar="certificate", default=[''])
ap.add_argument ("--password", nargs=1, metavar="password", default=[''])

config = ap.parse_args()

builddir = config.builddir[0]
tempinstaller = os.path.join (builddir, 'tempinstaller.exe')
script = os.path.join (builddir, 'tools/windeploy/setup.nsi')
certfile = config.certificate[0].strip()
password = config.password[0].strip()
codesign = os.path.exists(certfile) and len(password) > 0

print ("Element Installer Maker")
print ("[info] processing uninstaller")
call (['makensis', '/DINNER', '/V0', script])
input ("Run 'tempinstaller.exe' as admin, then press enter...")
if codesign:
    print("[info] code sign uninstaller")

print ("[info] processing installer")
call (['makensis', '/V0', script])
if codesign:
    print("[info] code sign installer")

sys.exit (0)
