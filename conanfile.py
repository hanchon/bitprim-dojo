#
# Copyright (c) 2017 Bitprim developers (see AUTHORS)
#
# This file is part of Bitprim.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

import os
from conans import ConanFile, CMake
from conans import __version__ as conan_version
from conans.model.version import Version

def get_content(path):
    with open(path, 'r') as f:
        return f.read().replace('\n', '').replace('\r', '')

def option_on_off(option):
    return "ON" if option else "OFF"

class BitprimDojoConan(ConanFile):
    name = "bitprim-dojo"
    version = get_content('conan_version')
    license = "http://www.boost.org/users/license.html"
    url = "https://github.com/hanchon/bitprim-dojo"
    description = "Bitcoin Dojo Example"
    settings = "os", "compiler", "build_type", "arch"

    if conan_version < Version("1.4"):
        raise Exception ("Conan version should be greater or equal than 1.4")

    options = {"with_tests": [True, False],
               "currency": ['BCH', 'BTC', 'LTC'],
               "no_compilation": [True, False]
               }

    default_options = "with_tests=False", \
                      "currency=BCH", \
                      "no_compilation=False"

    generators = "cmake"
    exports = "conan_version", "conan_channel"
    exports_sources = "src/*", "CMakeLists.txt", "include/*", "cli/*", "bitprimbuildinfo.cmake", "dojo/*"
    build_policy = "missing"

    def requirements(self):
        if not self.options.no_compilation and self.settings.get_safe("compiler") is not None:
            self.requires("bitprim-core/0.11.0@bitprim/testing")

    def configure(self):
        if self.options.no_compilation or (self.settings.compiler == None and self.settings.arch == 'x86_64' and self.settings.os in ('Linux', 'Windows', 'Macos')):
            self.settings.remove("compiler")
            self.settings.remove("build_type")

        self.options["*"].currency = self.options.currency
        self.options["bitprim-core"].shared = False
        self.output.info("Compiling for currency: %s" % (self.options.currency,))

    def package_id(self):
        self.info.requires.clear()
        self.info.settings.compiler = "ANY"
        self.info.settings.build_type = "ANY"
        self.info.options.no_compilation = "ANY"
        #For Bitprim Packages libstdc++ and libstdc++11 are the same
        if self.settings.compiler == "gcc" or self.settings.compiler == "clang":
            if str(self.settings.compiler.libcxx) == "libstdc++" or str(self.settings.compiler.libcxx) == "libstdc++11":
                self.info.settings.compiler.libcxx = "ANY"

    def deploy(self):
        self.copy("bitprim-cli.exe", src="bin")
        self.copy("bitprim-cli", src="bin")
        self.copy("bitprim-dojo.exe", src="bin")
        self.copy("bitprim-dojo", src="bin")

    def build(self):
        cmake = CMake(self)
        cmake.verbose = False
        cmake.definitions["CURRENCY"] = self.options.currency

        if self.settings.get_safe("compiler") is not None:
            if self.settings.compiler == "gcc":
                if float(str(self.settings.compiler.version)) >= 5:
                    cmake.definitions["NOT_USE_CPP11_ABI"] = option_on_off(False)
                else:
                    cmake.definitions["NOT_USE_CPP11_ABI"] = option_on_off(True)
            elif self.settings.compiler == "clang":
                if str(self.settings.compiler.libcxx) == "libstdc++" or str(self.settings.compiler.libcxx) == "libstdc++11":
                    cmake.definitions["NOT_USE_CPP11_ABI"] = option_on_off(False)

        cmake.configure(source_dir=self.source_folder)
        cmake.build()

    def package(self):
        self.copy("bitprim-cli.exe", dst="bin", src="bin") # Windows
        self.copy("bitprim-cli", dst="bin", src="bin") # Linux / Macos
        self.copy("bitprim-dojo.exe", dst="bin", src="bin") # Windows
        self.copy("bitprim-dojo", dst="bin", src="bin") # Linux / Macos