import typing

from conans import ConanFile, CMake, tools

class CLIConanFile(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    generators = "cmake"

    build_requires = "gtest/1.8.1@bincrafters/stable"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        if not tools.cross_building(self.settings):
            cmake.test()
