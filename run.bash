#!/bin/bash

# default params
driver="game"
build_type="debug"
run_cmake=0

# check for cmdline flags
while getopts ":t:d:c" opt
do
    case $opt in
        t)
            build_type="$OPTARG"
            if [[ $build_type != "opt" && $build_type != "debug" && $build_type != "debug-opt" ]]
            then
                echo "Invalid build type: $OPTARG" >&2
                exit 1
            fi
            ;;
        c)
            run_cmake=1
            ;;
        d)
            driver="$OPTARG"
            ;;
        \?)
            echo "Invalid option: -$OPTARG" >&2
            exit 1
            ;;
        :)
            echo "Option -$OPTARG requires an argument" >&2
            exit 1
            ;;
    esac
done

# shift the args so that $1 is the first regular argument that getopt doesn't parse
shift $(($OPTIND - 1))

# go to the build directory
pushd build-"$build_type"

# run cmake if requested
if ((run_cmake == 1))
then
    cmake ..
fi

# run make, pop back to parent dir and run the driver with the rest of the args
make -j4 $driver
popd
build-"$build_type"/drivers/$driver $@
