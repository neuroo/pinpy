"""
    PinPy -- Shake your hooks with some Python!
    by Romain Gaucher <r@rgaucher.info> - http://rgaucher.info

    Copyright (c) 2011-2012 Romain Gaucher     @rgaucher
                            Haris Andrianakis  @tl0gic

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
"""
import os, sys
import ctypes

def writeInt(address, integer):
	''' Write an "integer" at "address" '''
	p = ctypes.c_void_p(address)
	i = ctypes.cast(p, ctypes.POINTER(ctypes.c_int))
	i[0] = integer

def writeStr(address, string):
	''' Write the contents of "string" to the buffer at "address" '''
	p = ctypes.c_void_p(address)
	void_ptr = ctypes.cast(p, ctypes.POINTER(ctypes.c_void_p))
	ctypes.memmove(void_ptr.contents.value, string, len(string))

def replaceStr(address, string):
	''' Replace the char* pointer at "address" with our string's address '''
	p = ctypes.c_void_p(address)
	char_ptr = ctypes.cast(p, ctypes.POINTER(ctypes.c_char_p))
	char_ptr[0] = string

def readInt(address):
	''' Read an integer from "address" '''
	p = ctypes.c_void_p(address)
	i = ctypes.cast(p, ctypes.POINTER(ctypes.c_int))
	return i.contents.value

def readStr(address):
	''' Read the "string" at "address" '''
	p = ctypes.c_void_p(address)
	char_ptr = ctypes.cast(p, ctypes.POINTER(ctypes.c_char_p))
	return char_ptr.contents.value

def simpleHello(a_dict):
	# Handlers for beforeCall
	if a_dict["@callMode"] == "beforeCall":

		if a_dict["@funcName"] == "purple_ssl_read":
			data = 'unknown'
			gsc  = length = -1

			if a_dict.has_key("gsc"):
				gsc = readInt(a_dict["gsc"])

			if a_dict.has_key("data"):
				data = readStr(a_dict["data"])

			if a_dict.has_key("len"):
				length = readInt(a_dict["len"])

			print "purple_ssl_read(gsc=" + str(hex(gsc)) + ", data=" + data + ", length=" + str(length) + ")"

		# Handler for wpurple_send beforeCall
		if a_dict["@funcName"] == "purple_ssl_write":
			data = 'unknown'
			gsc  = length = -1

			if a_dict.has_key("gsc"):
				gsc = readInt(a_dict["gsc"])

			if a_dict.has_key("data"):
				data = readStr(a_dict["data"])

			if a_dict.has_key("len"):
				length = readInt(a_dict["len"])

			print "purple_ssl_write(gsc=" + str(hex(gsc)) + ", data=" + data + ", length=" + str(length) + ")"

			idx = data.find('<body>')
			if idx > 0:
				part1 = data[:idx+6]
				part2 = data[idx+11:]
				newdata = part1+"pwned"+part2
				replaceStr(a_dict["data"], newdata)
				print "modified purple_ssl_write(gsc=" + str(hex(gsc)) + ", data=" + newdata + ", length=" + str(length) + ")"

		# Handler for sleep beforeCall
		if a_dict["@funcName"] == "sleep":
			if a_dict.has_key("seconds"):
				print "sleep("+str(readInt(a_dict["seconds"]))+")",
			else:
				print 'python script- error called sleep but arg "seconds" is not set in dict'

		# Handler for myfunction2 beforeCall
		# Example usage of replaceStr and writeStr.
		# Make sure there is enough space in the buffer you are overwritting
		# when using writeStr to hold your new string. Also you cannot (obviously) write
		# into constant strings :p
		if a_dict["@funcName"] == "myfunction2":
			if a_dict.has_key("message"):
				print 'beforeCall for myfunction2'
				print "message is", readStr(a_dict["message"])
	
				new_message = "this is a new message"
				replaceStr(a_dict["message"], new_message)
				# writeStr(a_dict["message"], new_message)

	# Handlers for afterCall
	elif a_dict["@callMode"] == "afterCall":

		# Handler for wpurple_send afterCall
		if a_dict["@funcName"] == "purple_ssl_write":
			if a_dict.has_key("ret"):
				print "purple_ssl_write- ret:", readInt(a_dict["ret"])
			else:
				print "- ret: unknown"

		# Handler for sleep afterCall
		if a_dict["@funcName"] == "sleep":
			if a_dict.has_key("ret"):
				print "- retVal:", readInt(a_dict["ret"])
			else:
				print 'python script- error we are in afterCall and dont have access to the retVal'

