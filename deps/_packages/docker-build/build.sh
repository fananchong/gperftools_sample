#!/bin/bash

set -ex

out_path=$PWD/../../

docker build -t plengine/build-centos7 ./centos7.3

docker build -t plengine/build-gperftools ./gperftools
docker run --rm -v $out_path:/tmp/home plengine/build-gperftools /bin/bash "/setup.sh"

