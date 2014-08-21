#!/usr/bin/env python

# Code Beispiele für das Python Frontend:
# https://github.com/eliben/llvm-clang-samples/tree/master/src_clang/experimental
# CINDEX Dokumentation:
# http://clang.llvm.org/doxygen/group__CINDEX.html
#

import pprint
import sys
import clang.cindex
from clang.cindex import CursorKind

def handle_function_decl(fdecl_cursor):
    children = list(fdecl_cursor.get_children())
    print fdecl_cursor.displayname

    # Look at param decls
    for c in children:
        if c.kind == CursorKind.PARM_DECL:
            print c.spelling
            

def handle_variable_decl(vdecl_cursor):
    if vdecl_cursor.kind.is_attribute():
        print "Attribute: " + vdecl_cursor.displayname
        # VariableDecl is Attribute -> check for the right Attribute spelling
        # Scheint nicht zu gehen -> findet keine Attribute -> Was ist ein Attribut in c++?
    else :
        print "Variable: " + vdecl_cursor.displayname
        # is global VariableDecl -> check for the right Variable spelling
        
def handle_class_decl(cdecl_cursor):
    print cdecl_cursor.displayname


index = clang.cindex.Index.create()
tu = index.parse(sys.argv[1])

diagnostics = list(tu.diagnostics)
if len(diagnostics) > 0:
    print 'There were parse errors'
    pprint.pprint(diagnostics)
else:
    print dir(CursorKind)
    for c in tu.cursor.walk_preorder():
        if c.kind == CursorKind.FUNCTION_DECL:
            handle_function_decl(c)
        elif c.kind == CursorKind.VAR_DECL:
            handle_variable_decl(c)
        elif c.kind == CursorKind.CLASS_DECL:
            handle_class_decl(c)
        elif c.kind == CursorKind.CXX_METHOD:
            handle_function_decl(c)