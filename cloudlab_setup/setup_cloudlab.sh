#!/bin/bash

user=$1
host=$2

ssh -t -o StrictHostKeyChecking=no $user@$host 'bash -s' < install_deps.sh
