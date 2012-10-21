

stem_lib_objs = Object('stem_lib.c',CCFLAGS=['-Wall','-ggdb3'])

#flags = ['-Wall','-ggdb3']
flags = ['-Wall','-O2']

Program('runner',['test_count.c',]+stem_lib_objs, LIBS=['cunit',],CCFLAGS=flags,LDFLAGS=flags)

Program('count_terms', ['count_terms.c',]+stem_lib_objs,CCFLAGS=flags,LDFLAGS=flags)

