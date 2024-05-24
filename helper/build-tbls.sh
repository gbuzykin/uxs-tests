#!/bin/bash -e
LEXEGEN_PATH=${LEXEGEN_PATH:-lexegen}
$LEXEGEN_PATH ../uxs/src/db/json.lex --header-file=../uxs/src/db/json_lex_defs.h --outfile=../uxs/src/db/json_lex_analyzer.inl --use-int8-if-possible --compress 1
$LEXEGEN_PATH ../uxs/src/db/xml.lex --header-file=../uxs/src/db/xml_lex_defs.h --outfile=../uxs/src/db/xml_lex_analyzer.inl --use-int8-if-possible --compress 1
