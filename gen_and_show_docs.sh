#! /bin/bash

ENTRY_POINT_HTML=./doxy_docs/html/index.html

if ! [[ -f "$ENTRY_POINT_HTML" ]]; then
	echo "ne postoji !!!!!"
	doxygen dconfig		
fi



xdg-open $ENTRY_POINT_HTML &
