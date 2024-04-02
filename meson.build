project('bmctester', 'cpp',
    default_options: [
            'b_lto_mode=default',
            'b_lto_threads=0',
            'b_lto=true',
            'b_ndebug=if-release',
            'buildtype=debugoptimized',
            'cpp_rtti=false',
            'cpp_std=c++20',
            'warning_level=3',
            'werror=false',
           ],
    version: '1.0.0',
)

cxx = meson.get_compiler('cpp')
build = get_option('buildtype')
optimization = get_option('optimization')
summary('Build Type',build, section : 'Build Info')
summary('Optimization',optimization, section : 'Build Info')

# Validate the c++ Standard

if get_option('cpp_std') != 'c++20'
    error('This project requires c++20 support')
endif

root_inc = include_directories('include')

root_src = files(
    'src/main.cpp',
    'src/boost_asio.cpp',
)

add_project_arguments(
  cxx.get_supported_arguments([
  '-Wcast-align',
  '-Wconversion',
  '-Wformat=2',
  '-Woverloaded-virtual',
  '-Wsign-conversion',
  '-Wunused',
  '-Wno-attributes',
  '-std=gnu++20',
  '-std=c++20',
  ]),
  language: 'cpp'
)

if (cxx.get_id() == 'clang' and cxx.version().version_compare('>9.0'))
add_project_arguments(
  cxx.get_supported_arguments([
    '-Weverything',
    '-Wno-c++98-compat-pedantic',
    '-Wno-c++98-compat',
    '-Wno-documentation-unknown-command',
    '-Wno-documentation',
    '-Wno-exit-time-destructors',
    '-Wno-global-constructors',
    '-Wno-newline-eof',
    '-Wno-padded',
    '-Wno-shadow',
    '-Wno-used-but-marked-unused',
    '-Wno-weak-vtables',
    '-Wno-switch-enum',
    '-Wno-unused-macros',
  ]),
  language:'cpp')
endif

if (cxx.get_id() == 'gcc' and cxx.version().version_compare('>8.0'))
  add_project_arguments(
    cxx.get_supported_arguments([
     '-Wduplicated-cond',
     '-Wduplicated-branches',
     '-Wlogical-op',
     '-Wnull-dereference',
     '-Wunused-parameter',
     '-Wdouble-promotion',
     '-Wshadow',
     '-Wno-psabi',
     ]),
    language:'cpp')
endif

add_project_arguments(
cxx.get_supported_arguments([
  '-DBOOST_ASIO_DISABLE_CONCEPTS',
  '-DBOOST_ALL_NO_LIB',
  '-DBOOST_ALLOW_DEPRECATED_HEADERS',
  '-DBOOST_ASIO_DISABLE_THREADS',
  '-DBOOST_ASIO_NO_DEPRECATED',
  '-DBOOST_ASIO_SEPARATE_COMPILATION',
  '-DBOOST_BEAST_SEPARATE_COMPILATION',
  '-DBOOST_EXCEPTION_DISABLE',
  '-DBOOST_NO_EXCEPTIONS',
  '-DBOOST_URL_NO_SOURCE_LOCATION',
  '-DJSON_NOEXCEPTION',
  '-DOPENSSL_NO_FILENAMES',
  '-DSDBUSPLUS_DISABLE_BOOST_COROUTINES',
]),
language : 'cpp')

bmctester_dependencies = []

libcurldep = dependency('libcurl')
nlohmann_json_dep = dependency('nlohmann_json', version: '>=3.11.2', include_type: 'system')
bmctester_dependencies += [libcurldep, nlohmann_json_dep]

boost = dependency(
  'boost',
  modules: [
    'url',
    ],
  version : '>=1.83.0',
  required : false,
  include_type: 'system'
)
if boost.found()
  bmctester_dependencies += [boost]
else
  cmake = import('cmake')
  opt = cmake.subproject_options()
  opt.add_cmake_defines({
    'BOOST_INCLUDE_LIBRARIES': 'asio;beast;callable_traits;format;headers;process;type_index;url;uuid'
  })
  boost = cmake.subproject('boost', required: true, options: opt)
  boost_asio = boost.dependency('boost_asio').as_system()
  boost_beast = boost.dependency('boost_beast').as_system()
  boost_callable_traits = boost.dependency('boost_callable_traits').as_system()
  boost_format = boost.dependency('boost_format').as_system()
  boost_headers = boost.dependency('boost_headers').as_system()
  boost_process = boost.dependency('boost_process').as_system()
  boost_type_index = boost.dependency('boost_type_index').as_system()
  boost_url = boost.dependency('boost_url').as_system()
  boost_uuid = boost.dependency('boost_uuid').as_system()
  bmctester_dependencies += [
    boost_asio,
    boost_beast,
    boost_callable_traits,
    boost_format,
    boost_headers,
    boost_process,
    boost_type_index,
    boost_url,
    boost_uuid
  ]
endif

executable('bmctester', root_src, 
    include_directories: root_inc, 
    dependencies : bmctester_dependencies,
    link_args: '-Wl,--gc-sections',
)