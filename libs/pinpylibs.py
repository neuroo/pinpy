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

isiterable = lambda obj: isinstance(obj, basestring) or getattr(obj, '__iter__', False)
__normalize_argmt = lambda x: ''.join(x.lower().split())
__normalize_paths = lambda x: [os.path.abspath(of) for of in x]
__normalize_path = lambda x: os.path.abspath(x)

# Define several decorators for fast processing, and arguments checking
class Memoized(object):
    def __init__(self, func):
        self.func = func
        self.memoized = {}
        self.method_cache = {}
    def __call__(self, *args):
        return self.cache_get(self.memoized, args, lambda: self.func(*args))
    def __get__(self, obj, objtype):
        return self.cache_get(self.method_cache, obj, lambda: self.__class__(functools.partial(self.func, obj)))
    def cache_get(self, cache, key, func):
        try:
            return cache[key]
        except KeyError:
            cache[key] = func()
            return cache[key]

class NonNullParameters(object):
    def __init__(self, func):
        self.func = func
    def __call__(self, *args):
        if isiterable(args):
            for e in args:
                if e is None:
                    return None
        else:
            if args is None:
                return None
        return self.func(*args)