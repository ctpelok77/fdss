Bootstrap: docker
From: ubuntu

%setup
     ## The "%setup"-part of this script is called to bootstrap an empty
     ## container. It copies the source files from the branch of your
     ## repository where this file is located into the container to the
     ## directory "/planner". Do not change this part unless you know
     ## what you are doing and you are certain that you have to do so.

    REPO_ROOT=`dirname $SINGULARITY_BUILDDEF`
    cp -r $REPO_ROOT/ $SINGULARITY_ROOTFS/planner

%post

    ## The "%post"-part of this script is called after the container has
    ## been created with the "%setup"-part above and runs "inside the
    ## container". Most importantly, it is used to install dependencies
    ## and build the planner. Add all commands that have to be executed
    ## once before the planner runs in this part of the script.

    ## Install all necessary dependencies.
    apt-get update
    apt-get -y install cmake g++ make python

    ## Build planner.
    cd /planner
    ./build.py release64

    ## Build h2-mutexes preprocessor.
    mkdir -p builds/h2-mutexes/
    cd builds/h2-mutexes/
    cmake ../../h2-mutexes/
    make -j4

%runscript
    ## The runscript is called whenever the container is used to solve
    ## an instance.

    DOMAINFILE=$1
    PROBLEMFILE=$2
    PLANFILE=$3

    ## Call your planner.
    /planner/fast-downward.py \
        --build=release64 \
        --plan-file $PLANFILE \
        --transform-task /planner/builds/h2-mutexes/bin/preprocess \
        --alias seq-sat-remix \
        $DOMAINFILE \
        $PROBLEMFILE

## Update the following fields with meta data about your submission.
## Please use the same field names and use only one line for each value.
%labels
Name        Remix
Description portfolio of satisficing Fast Downward configurations
Authors     Jendrik Seipp <jendrik.seipp@unibas.ch>
SupportsDerivedPredicates yes
SupportsQuantifiedPreconditions yes
SupportsQuantifiedEffects yes
