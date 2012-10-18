

stem_lib_objs = Object('stem_lib.c',CCFLAGS=['-Wall','-ggdb3'])

Program('runner',['test_count.c',]+stem_lib_objs, LIBS=['cunit',],CCFLAGS=['-Wall','-ggdb3'])

Program('count_terms', ['count_terms.c',]+stem_lib_objs,CCFLAGS=['-Wall','-ggdb3'])

