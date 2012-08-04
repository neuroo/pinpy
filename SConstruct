import os
import sys

__normalize_path = lambda x: os.path.abspath(x)

def good_ext(fext, l=None):
	return fext.lower() in l if l else False

def scandir(directory, files, l_ext, ignore=None):
	names = os.listdir(directory)
	for name in names:
		if ignore and name in ignore:
			continue
		srcname = __normalize_path(os.path.join(directory, name))
		try:
			if os.path.isdir(srcname):
				scandir(srcname, files, l_ext, ignore)
			elif  os.path.isfile(srcname) and good_ext(srcname[srcname.rfind('.') + 1:], l_ext):
				if srcname not in files:
					files.append(srcname)
		except IOError, error:
			continue

def SelectBuildDir(build_dir, platform=None):
	# if no platform is specified, then default to sys.platform
	if not(platform):
		platform = sys.platform
	print "Looking for build directory for platform '%s'" % platform
	# setup where we start looking at first
	test_dir = os.path.join(build_dir, platform)
	default_dir = os.path.join(build_dir, 'default')

	# we look for a directory named exactly after the
	# platform so that very specific builds can be done
	if os.path.exists(test_dir):
		# make sure it is a directory
		target_dir = test_dir
	print "Found directory %s, will build there" % target_dir
	return target_dir


lib_search_path = ['/lib','/usr/lib','/usr/local/lib']
include_search_path =  ['/usr/include', '/usr/local/include','#src']

## These are our source files
sources = []
ignored_files = []
if sys.platform in ('darwin', 'win32'):
	ignored_files = ["pythonprobe.cpp"]
	
scandir('./src', sources, ["cpp"], ignored_files)

# Build info
source_base_dir = 'src'
build_base_dir = 'build'
target_name = 'pinpy'

# Setup some of our requirements
env = Environment()

# setup the include paths where boost::python and Pin
env.Append(CPPPATH=include_search_path)
env.Append(LIBS=['boost_python', 'python', 'xml2'])

# variables the sub build directories need
Export('env', 'sources', 'target_name')

# start the build
target_dir = '#'  + SelectBuildDir(build_base_dir)
SConscript(os.path.join(target_dir,'SConscript'))

env.VariantDir(target_dir, source_base_dir, duplicate=0)
