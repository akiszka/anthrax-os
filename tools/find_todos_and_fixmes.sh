#!/bin/sh
grep -EIHnr --color --exclude-dir={tools,.git} 'TODO|FIXME' .
