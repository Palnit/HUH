#include "HUH/Types/array.h"
#include "HUH/logging.h"
#include "HUH/string_operations.h"

#include <fstream>
#include <iostream>

#include <args.hxx>
#include <filesystem>

#include <clang-c/Index.h>

HUH::LogCategory CudaBuilder("Cuda Builder");

std::string ToString(CXString str) {
    std::string s = clang_getCString(str);
    clang_disposeString(str);
    return s;
}

struct ClangCTX {
    HUH::Array<std::string> FunctionNames;
    bool InsideExternC = false;
};

CXChildVisitResult Visitor(CXCursor cursor, CXCursor parent, CXClientData clientData) {
    CXCursorKind kind = clang_getCursorKind(cursor);

    if (kind == CXCursor_MacroExpansion) {
        return CXChildVisit_Continue;
    }
    if (clang_isDeclaration(kind)) {
        if (!clang_Location_isFromMainFile(clang_getCursorLocation(cursor)))
            return CXChildVisit_Continue;
    }

    ClangCTX ctx = *static_cast<ClangCTX*>(clientData);

    if (kind == CXCursor_LinkageSpec) {
        std::string spelling = ToString(clang_getCursorSpelling(cursor));
        HUH_TLOG("Spelling {}", spelling)
        ctx.InsideExternC = !spelling.empty() && spelling == "C";
    }

    if (kind == CXCursor_FunctionDecl && ctx.InsideExternC) {
        auto name = ToString(clang_getCursorSpelling(cursor));
        HUH_TLOG("Name {}", name)

        if (!name.empty()) {
            ctx.FunctionNames.Emplace(name);
        }

        clang_visitChildren(cursor, Visitor, &ctx);
    }

    return CXChildVisit_Continue;
}

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
    for (auto& lib : libs) {
        libraries.Emplace(HUH::Split(lib, " "));
    }

    HUH::Array<std::string> includes;
    for (auto& inc : include) {
        includes.Emplace(HUH::Split(inc, " "));
    }
    std::string cudaPathOption = "--cuda-path=" + cudaPath.Get();
    std::string resourceDirOption = "-resource-dir=" + clangResourceDir.Get();

    HUH::Array<const char*> clangOptions;
    clangOptions.Emplace("-x");
    clangOptions.Emplace("cuda");
    clangOptions.Emplace("-std=c++20");
    clangOptions.Emplace("--cuda-gpu-arch=sm_75");
    clangOptions.Emplace(cudaPathOption.c_str());
    clangOptions.Emplace(resourceDirOption.c_str());
    for (auto& inc : includes) {
        inc = "-I" + inc;
        clangOptions.Emplace(inc.c_str());
    }

    HUH_ILOG(CudaBuilder, "Options for clang: ")
    for (auto& option : clangOptions) {
        HUH_ILOG(CudaBuilder, "{}", option)
    }

    CXIndex Index = clang_createIndex(0, 0);// Create index
    HUH::Array<CXTranslationUnit> units;
    HUH_ILOG(CudaBuilder, "Starting File Parse:")
    for (auto& file : files) {
        HUH_ILOG(CudaBuilder, "Building Translation Unit for: {}", file)
        CXTranslationUnit unit = clang_parseTranslationUnit(
            Index, file.c_str(), clangOptions.GetData(), static_cast<int>(clangOptions.Size()), nullptr, 0,
            CXTranslationUnit_IncludeAttributedTypes | CXTranslationUnit_VisitImplicitAttributes
                | CXTranslationUnit_SkipFunctionBodies | CXTranslationUnit_DetailedPreprocessingRecord);

        if (unit == nullptr) {
            HUH_ELOG(CudaBuilder, "Failed to parse translation unit");
            continue;
        }

        for (unsigned i = 0; i < clang_getNumDiagnostics(unit); ++i) {
            CXDiagnostic diag = clang_getDiagnostic(unit, i);

            auto severity = clang_getDiagnosticSeverity(diag);

            auto text =
                ToString(clang_formatDiagnostic(diag,
                                                CXDiagnostic_DisplaySourceLocation | CXDiagnostic_DisplayColumn
                                                    | CXDiagnostic_DisplaySourceRanges | CXDiagnostic_DisplayOption));

            if (severity == CXDiagnostic_Error || severity == CXDiagnostic_Fatal) {
                HUH_ELOG(CudaBuilder, "Error during translation unit creation: {}", text);
                clang_disposeDiagnostic(diag);
                clang_disposeTranslationUnit(unit);
                continue;
            }

            if (severity == CXDiagnostic_Warning || severity == CXDiagnostic_Ignored) {
                HUH_WLOG(CudaBuilder, "Warning during translation unit creation: {}", text);
            }

            if (severity == CXDiagnostic_Note) {
                HUH_ILOG(CudaBuilder, "Note during translation unit creation: {}", text);
            }

            clang_disposeDiagnostic(diag);
        }
        units.Emplace(unit);
    }
    ClangCTX ctx;
    for (auto& unit : units) {
        ctx.InsideExternC = false;
        CXCursor cursor = clang_getTranslationUnitCursor(unit);

        clang_visitChildren(cursor, Visitor, &ctx);
    }

    for (auto& function : ctx.FunctionNames) {
        HUH_ILOG(CudaBuilder, "Function Name: {}", function)
    }

    if (output) {
        std::ofstream test(output.Get() / "include" / "HUH" / "Cuda" / "Gen"
                           / std::filesystem::path(libname.Get() + ".gen.h"));
        test << "Hello World!" << std::endl;
        test.close();

        std::ofstream test2(output.Get() / std::filesystem::path(libname.Get() + ".gen.cpp"));
        test2 << "Hello World!" << std::endl;
        test2.close();
    }

    for (auto& unit : units) {
        clang_disposeTranslationUnit(unit);
    }
    clang_disposeIndex(Index);

    return 0;
}
