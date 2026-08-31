#include "HUH/Types/array.h"
#include "HUH/logging.h"
#include "HUH/string_operations.h"

#include <fstream>
#include <iostream>

#include <args.hxx>
#include <filesystem>

int main(int argc, char* argv[]) {
    args::ArgumentParser parser("This is a program to generate cpp bindings for a cuda file");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::ValueFlagList<std::string> files(parser, "files", "The files to parse", {'f', "files"});
    args::ValueFlagList<std::string> libs(parser, "libs", "The libs to bind", {'l', "libs"});
    args::ValueFlagList<std::string> include(parser, "include", "The name of the cuda library", {'i', "include"});
    args::ValueFlag<std::string> libname(parser, "libname", "The name of the cuda library", {'n', "name"});
    args::ValueFlag<std::string> cudaPath(parser, "cudaPath", "The name of the cuda library", {'c', "cuda_path"});
    args::ValueFlag<std::string> clangResourceDir(parser, "Clang Resource Dir", "The name of the cuda library",
                                                  {'r', "resource_dir"});
    args::ValueFlag<std::filesystem::path> output(parser, "output", "The output directory of the bindings",
                                                  {'o', "output"});

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Help&) {
        std::cout << parser;
        return 0;
    } catch (const args::ParseError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    } catch (const args::ValidationError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    HUH::Array<std::string> libraries;
    for (auto& file : libs) {
        libraries.Emplace(HUH::Split(file, " "));
    }

    HUH::Array<std::string> includes;
    for (auto& file : include) {
        includes.Emplace(HUH::Split(file, " "));
    }

    for (auto& file : includes) {
        HUH_TLOG("Inc: {}", file)
    }

    for (auto& file : libraries) {
        HUH_TLOG("Libs: {}", file)
    }

    if (output) {
        std::ofstream test(output.Get() / "test.cpp");
        test << "Hello World!" << std::endl;
        test.close();
    }

    return 0;
}
