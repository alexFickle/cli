import typing

from conans import ConanFile, CMake, tools


class CLIConanFile(ConanFile):
    name = "cli"
    version = "0.0.0"
    license = "MIT"
    description = "command line argument parser library"
    url = "https://github.com/alexFickle/cli"
    settings = "os", "arch", "compiler", "build_type"
    generators = "cmake"
    exports_sources = ("examples/*", "include/*", "test/*", "CMakeLists.txt",
                       "LICENSE")
    # https://github.com/alexFickle/keyword
    requires = "keyword/0.0.0@fickle/testing"
    build_requires = "gtest/1.8.1@bincrafters/stable"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        if not tools.cross_building(self.settings):
            cmake.test(output_on_failure=True)

    def package_id(self):
        self.info.header_only()

    def package(self):
        self.copy("*.hpp", src="include", dst="include")
        self.copy("LICENSE", src=".", dst=".")
