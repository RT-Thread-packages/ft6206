from building import *
Import('rtconfig')

src   = []
cwd   = GetCurrentDir()

# add ft6206 src files.
if GetDepend('PKG_USING_FT6206'):
    src += Glob('src/ft6206.c')

if GetDepend('PKG_USING_FT6206_SAMPLE'):
    src += Glob('samples/ft6206_sample.c')

# add ft6206 include path.
path  = [cwd + '/inc']

# add src and include to group.
group = DefineGroup('ft6206', src, depend = ['PKG_USING_FT6206'], CPPPATH = path)

Return('group')
