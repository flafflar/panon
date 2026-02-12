#!/bin/bash
git describe --tags --abbrev=7 --dirty --broken | sed 's/^v//;s/-/.r/;s/-/./;'
