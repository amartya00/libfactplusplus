project(
    'factplusplus', 
    'cpp',
    version : '0.0.1',
    license: 'GPLV2'
)

add_global_arguments('--std=c++17', '-Werror', language : 'cpp')


inc = include_directories('headers')
pkg = import('pkgconfig')

subdir('headers')
subdir('lib')



pkg.generate(
    libraries : libfactplusplus,
    subdirs : 'factplusplus',
    version : '0.0.1',
    name : 'factplusplus',
    filebase : 'factplusplus',
    description : 'An efficient Description Logic reasoner compatible with OWL DL and OWL 2.'
)




