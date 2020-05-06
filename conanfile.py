"""Conan package manager config file. https://docs.conan.io/en/latest/"""
import re
from conans import ConanFile, CMake, tools


def get_project_version():
    try:
        content = tools.load("CMakeLists.txt")
        version = re.search(r"project\([^\)]+VERSION (\d+\.\d+\.\d+)[^\)]*\)", content).group(1)
        return version.strip()
    except Exception:
        return None

		
def get_project_name():
    try:
        content = tools.load("CMakeLists.txt")
        name = re.search(r"project\(([^\) ]+)[^\)]*\)", content).group(1)
        return name.strip()
    except Exception:
        return None


class Project(ConanFile):
    """Conan package file."""
    name = get_project_name()
    version = get_project_version()

    settings = "os", "compiler", "build_type", "arch"
    requires = ["doctest/2.3.7"]

    options = {
        "tests": [True, False],
        "examples": [True, False],
    }

    default_options = {
        "tests": False,
        "examples": False,
        "gtest:shared": False
    }

    scm = {
        "type": "git",
        "url": "auto",
        "revision": "auto",
        "submodule": "recursive"
    }
    
    generators = "cmake"

    def imports(self):
        self.copy("*.dll", "", "bin")
        self.copy("*.dylib", "", "lib")

    def build(self):
        cmake = CMake(self, generator="Ninja")
        cmake.definitions["CMAKE_EXPORT_COMPILE_COMMANDS"] = True

        if self.options.tests:
            cmake.definitions[self.name + "_BUILD_TESTS"] = True
        if self.options.examples:
            cmake.definitions[self.name + "_BUILD_EXAMPLES"] = True

        cmake.configure()
        cmake.build()
