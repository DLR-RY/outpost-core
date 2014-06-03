#!/usr/bin/env python

import os
import sys
import re
import fileinput

# -----------------------------------------------------------------------------
class FileList(list):
	"""
	Special list for file object. Checks if a file is already in the list
	when adding it.
	"""
	def append(self, item):
		if not isinstance(item, basestring) and hasattr(item, '__getitem__'):
			self.extend(item)
		else:
			self.__append(item)
	
	def __append(self, item):
		if not self.__contains__(item):
			list.append(self, item)
	
	def extend(self, l):
		for item in l:
			self.append(item)
	
	def __iadd__(self, item):
		self.append(item)
		return self


# -----------------------------------------------------------------------------
def listify(node):
	return [node,] if (not isinstance(node, list)) else node


def exclude_directories(directories):
	# if the this directory should be excluded, remove all the
	# subdirectories from the list to exclude them as well
	temp_directories = directories[:]
	for d in temp_directories:
		directories.remove(d)
	return directories


def is_samefile(src, dst):
	# Macintosh, Unix
	if hasattr(os.path,'samefile'):
		try:
			#print src, dst
			return os.path.samefile(src, dst)
		except OSError:
			return False
	
	# All other platforms: check for same pathname.
	return (os.path.normcase(os.path.abspath(src)) ==
			os.path.normcase(os.path.abspath(dst)))


def contains_directory(directory, directorylist):
	# returns true if file (or directory) is contained in fileList
	for f in directorylist:
		if is_samefile(directory, f):
			return True
	return False


def in_list(filename, filelist):
	for f in filelist:
		if filename == f:
			return True
	return False


def scan(path, ignore=None):
	if ignore is None:
		ignorelist = []
	else:
		ignorelist = listify(ignore)
	pathlist = listify(path)
	
	filelist = FileList()
	
	for basepath in pathlist:
		for path, directories, files in os.walk(basepath):
			# Exclude all SVN directories
			if path.endswith('.svn') or contains_directory(path, ignorelist):
				directories = exclude_directories(directories)
				continue
			
			for filename in files:
				if in_list(filename, ignorelist):
					continue
				
				extension = os.path.splitext(filename)[1]
				if extension in ['.cpp', '.h']:
					filelist.append(os.path.join(path, filename))
	
	return filelist


# -----------------------------------------------------------------------------
def convert_comments(filename):
	linenumber = 0
	whitespace = None
	group = False
	prefix = ""
	
	for line in fileinput.input(filename, inplace=True):
#	for line in fileinput.input(filename):
		linenumber += 1
		
		inline = re.match("^(\s*)/\*(.*)(\s*)\*/(\s*)$", line)
		if inline:
			whitespace = inline.group(1)
			content = inline.group(2).rstrip()
			if content.startswith('*'):
				prefix = "///"
				content = content[1:]
			else:
				prefix = "//"
			sys.stdout.write(''.join([whitespace, prefix, content, '\n']))
			continue
		
		start = re.match("^(\s*)/\*(.*)$", line)
		if start:
			# start of new group
			whitespace = start.group(1)
			content = start.group(2).rstrip()
			
			if content.startswith('*'):
				prefix = "///"
				content = content[1:]
			else:
				prefix = "//"

			group = True
			if content != '' or linenumber == 1:
				sys.stdout.write(''.join([whitespace, prefix, content, '\n']))
			continue
		
		if group:
			if line.startswith(whitespace):
				end = re.match("^(\s*)\*/(.*)$", line)
				if end:
					group = False
					content = (' ' + end.group(2)).rstrip()
				else:
					content = line[len(whitespace):].rstrip()
				
				if content.strip().startswith('*'):
					content = content.strip()[1:]
				
				sys.stdout.write(''.join([whitespace, prefix, content, '\n']))
			else:
				sys.stdout.write(line.rstrip() + '\n')
				group = False
		else:
			sys.stdout.write(line.rstrip() + '\n')


files  = scan('../modules/ccsds')
files += scan('../modules/pus')
files += scan('../modules/rtos', '../modules/rtos/ext')
files += scan('../modules/smpc')
files += scan('../modules/time')
files += scan('../modules/utils', '../modules/utils/ext')

for filename in files:
	print filename
	convert_comments(filename)

