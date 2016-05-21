#!/bin/bash

BASEPATH=$1
shift
$BASEPATH/bin/aniaivega --base_path $BASEPATH $@
