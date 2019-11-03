#!/bin/bash

ipcs -m
ps -e | grep "usr"
ps -e | grep "oss"
