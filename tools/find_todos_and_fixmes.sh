#!/bin/sh
grep -EIHnr --color --exclude-dir=tools 'TODO|FIXME' .
