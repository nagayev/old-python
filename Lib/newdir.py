# New dir() function and other attribute-related goodies

# This should become a built-in function
#
def getattr(x, name):
	return eval('x.'+name)

# This should be the new dir(), except that it should still list
# the current local name space by default
#
def listattrs(x):
	try:
		dictkeys = x.__dict__.keys()
	except (NameError, TypeError):
		dictkeys = []
	#
	try:
		methods = x.__methods__
	except (NameError, TypeError):
		methods = []
	#
	try:
		members = x.__members__
	except (NameError, TypeError):
		members = []
	#
	try:
		the_class = x.__class__
	except (NameError, TypeError):
		the_class = None
	#
	try:
		bases = x.__bases__
	except (NameError, TypeError):
		bases = ()
	#
	total = dictkeys + methods + members
	if the_class:
		# It's a class instace; add the class's attributes
		# that are functions (methods)...
		class_attrs = listattrs(the_class)
		class_methods = []
		for name in class_attrs:
			if is_function(getattr(the_class, name)):
				class_methods.append(name)
		total = total + class_methods
	elif bases:
		# It's a derived class; add the base class attributes
		for base in bases:
			base_attrs = listattrs(base)
			total = total + base_attrs
	total.sort()
	return total
	i = 0
	while i+1 < len(total):
		if total[i] = total[i+1]:
			del total[i+1]
		else:
			i = i+1
	return total

# Helper to recognize functions
#
def is_function(x):
	return type(x) = type(is_function)

# Approximation of builtin dir(); this lists the user's
# variables by default, not the current local name space.
# Use a class method to make a function that can be called
# with or without arguments.
#
class _dirclass():
	def dir(args):
		if type(args) = type(()):
			return listattrs(args[1])
		else:
			import __main__
			return listattrs(__main__)
dir = _dirclass().dir