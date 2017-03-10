#!/usr/bin/env python
"""Reads .
This is a long, multiline description
"""

#########################
##    IMPORT MODULES   ##
#########################
import sys
import glob
import os #we use os.path
import subprocess

#########################
##      HEADER         ##
#########################
__author__ = "Miguel Riem de Oliveira"
__date__ = "February 2015"
__credits__ = ["Miguel Riem de Oliveira"]
__license__ = "GPL"
__version__ = "1.0"
__maintainer__ = "Miguel Oliveira"
__email__ = "m.riem.oliveira@gmail.com"
__status__ = "Development"


#########################
## FUNCTION DEFINITION ##
#########################

#########################
##  GLOBAL VARIABLES   ##
#########################

#Taken from https://github.com/miguelriemoliveira/rws2016_moliveira/wiki/List-of-players-and-teams
urls = ["https://github.com/miguelriemoliveira/rwsua2017_moliveira","https://github.com/joanagilf/rwsua2017_jferreira","https://github.com/jdiogomc/rwsua2017_dcorreia","https://github.com/VSilvaa/rwsua2017_vsilva","https://github.com/bvieira21/rwsua2017_bvieira","https://github.com/fsilva02/rwsua2017_fsilva","https://github.com/cyberholl/rwsua2017_rmartins","https://github.com/blistering12/rwsua2017_jsousa","https://github.com/BrunoMMRocha/rwsua2017_brocha.git"]

#Can be changed with command line argument clone, i.e., ./pull_all.py clone
clone_mode = False

##########
## MAIN ##
##########

if __name__ == "__main__":

    #--------------------#
    ### Initialization ###
    #--------------------#

    if len(sys.argv)>1:
        if sys.argv[1] == "clone":
            clone_mode = True
        else:
            clone_mode = False

    path = "../"

    #--------------------#
    ###   Clone mode   ###
    #--------------------#
    if clone_mode:
        print "*** Clone mode selected *** " 
        print "Cloning reps to path ../" 

        for url in urls:

            print url
            name = url.split("/")[-1][:-4]

            files = sorted(glob.glob(path + name)) 

            if not len(files) == 0:
                print "Repository " + name + " already exists in " + path + " , will not clone"
            else:
                cmd = "cd " + path + " && git clone " + url
                print "git clone " + url
                p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                for line in p.stdout.readlines():
                    print line,
                    retval = p.wait()

    else:

        print "pulling all reps from path ../" 

        #List all reps, i.e., folders starting by rws2016
        rep_paths = sorted([os.path.join(path,o) for o in os.listdir(path) if os.path.isdir(os.path.join(path,o)) and 'rwsfi2016' in o])
        #print rep_paths

        for rep_path in rep_paths:
            cmd = "cd " + rep_path + " && git pull"
            print "git pull " + rep_path
            p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            for line in p.stdout.readlines():
                print line,
                retval = p.wait()


    #paths_reps = sorted(glob.glob(p + '/views/pan_tilt/*_view*.xml')) 
    #print files_pan_tilt_views



    sys.exit()
    
