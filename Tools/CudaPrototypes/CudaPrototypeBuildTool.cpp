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

struct FunctionData {
    std::string Name;
    HUH::Array<std::string> ParamNames;
};

struct ClangCTX {
    HUH::Array<FunctionData> FunctionNames;
    bool InsideExternC = false;
    bool InsideFunction = false;
};

CXChildVisitResult Visitor(CXCursor cursor, CXCursor parent, const CXClientData clientData) {

    auto kind = clang_getCursorKind(cursor);

    // Only filter declarations by location.
    if (kind == CXCursor_MacroExpansion) {
        return CXChildVisit_Continue;
    }
    if (clang_isDeclaration(kind)) {
        if (!clang_Location_isFromMainFile(clang_getCursorLocation(cursor)))
            return CXChildVisit_Continue;
    }

    const auto ctx = static_cast<ClangCTX*>(clientData);

    if (!ctx->InsideExternC) {
        if (kind == CXCursor_LinkageSpec) {
            ctx->InsideExternC = true;
            clang_visitChildren(cursor, Visitor, ctx);
            ctx->InsideExternC = false;
            return CXChildVisit_Continue;
        }
        return CXChildVisit_Recurse;
    }

    if (!ctx->InsideFunction) {
        if (kind == CXCursor_FunctionDecl) {
            ctx->InsideFunction = true;
            clang_visitChildren(cursor, Visitor, ctx);
            ctx->InsideFunction = false;
            return CXChildVisit_Continue;
        }
        return CXChildVisit_Recurse;
    }

    if (kind == CXCursor_CUDAGlobalAttr) {
        ctx->FunctionNames.Emplace(ToString(clang_getCursorSpelling(parent)));
        return CXChildVisit_Continue;
    }

    if (kind == CXCursor_ParmDecl) {
        ctx->FunctionNames.Back().ParamNames.Emplace(ToString(clang_getCursorSpelling(cursor)));
        return CXChildVisit_Continue;
    }

    return CXChildVisit_Recurse;
}

std::string toUpperUnderscore(std::string str) {
    for (char& c : str) {
        if (c == '-')
            c = '_';
        else
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    return str;
}

int main(int argc, char* argv[]) {
    args::ArgumentParser parser("This is a program to generate cpp bindings for a cuda file");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::ValueFlagList<std::string> files(parser, "files", "The files to parse", {'f', "files"});
    args::ValueFlagList<std::string> ptxFiles(parser, "ptxFiles", "The ptx files", {'p', "ptxFiles"});
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
        HUH_ILOG(CudaBuilder, "\t\"{}\"", option)
    }

    CXIndex Index = clang_createIndex(0, 0);// Create index
    HUH::Array<CXTranslationUnit> units;
    HUH_ILOG(CudaBuilder, "Starting File Parse:")
    for (auto& file : files) {
        HUH_ILOG(CudaBuilder, "\tBuilding Translation Unit for: {}", file)
        CXTranslationUnit unit = clang_parseTranslationUnit(
            Index, file.c_str(), clangOptions.GetData(), static_cast<int>(clangOptions.Size()), nullptr, 0,
            CXTranslationUnit_IncludeAttributedTypes | CXTranslationUnit_VisitImplicitAttributes
                | CXTranslationUnit_SkipFunctionBodies | CXTranslationUnit_DetailedPreprocessingRecord);

        if (unit == nullptr) {
            HUH_ELOG(CudaBuilder, "\tFailed to parse translation unit");
            continue;
        }

        for (unsigned i = 0; i < clang_getNumDiagnostics(unit); ++i) {
            CXDiagnostic diag = clang_getDiagnostic(unit, i);

            auto severity = clang_getDiagnosticSeverity(diag);
            CXSourceLocation location = clang_getDiagnosticLocation(diag);

            CXFile diagFile = nullptr;
            unsigned line = 0;
            unsigned column = 0;
            unsigned offset = 0;

            clang_getFileLocation(location, &diagFile, &line, &column, &offset);

            auto text = ToString(clang_formatDiagnostic(diag, CXDiagnostic_DisplayOption));

            if (severity == CXDiagnostic_Error || severity == CXDiagnostic_Fatal) {
                if (diagFile) {

                    HUH::Logging::AddLog(CudaBuilder, HUH::Logging::Error,
                                         "\tError during translation unit creation: \"{}\"",
                                         ToString(clang_getFileName(diagFile)), std::to_string(line), text);
                    ;
                } else {

                    HUH_ELOG(CudaBuilder, "\tError during translation unit creation: \"{}\"", text);
                }
                clang_disposeDiagnostic(diag);
                clang_disposeTranslationUnit(unit);
                continue;
            }

            if (severity == CXDiagnostic_Warning || severity == CXDiagnostic_Ignored) {
                if (diagFile) {
                    HUH::Logging::AddLog(CudaBuilder, HUH::Logging::Warning,
                                         "\tWarning during translation unit creation: {}",
                                         ToString(clang_getFileName(diagFile)), std::to_string(line), text);
                } else {
                    HUH_WLOG(CudaBuilder, "\tWarning during translation unit creation: {}", text);
                }
            }

            if (severity == CXDiagnostic_Note) {
                if (diagFile) {
                    HUH::Logging::AddLog(CudaBuilder, HUH::Logging::Log, "\tNote during translation unit creation: {}",
                                         ToString(clang_getFileName(diagFile)), std::to_string(line), text);
                    ;
                } else {
                    HUH_ILOG(CudaBuilder, "\tNote during translation unit creation: {}", text);
                }
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
        HUH_ILOG(CudaBuilder, "Found function: {}", function.Name)
    }

    if (output) {
        std::ofstream header(output.Get() / "include" / "HUH" / "Cuda" / "Gen"
                             / std::filesystem::path(libname.Get() + ".gen.h"));
        header << "#pragma once" << std::endl;
        header << "#include <HUH/definitions.h>" << std::endl;
        header << "#include <HUH/Cuda/module.h>" << std::endl;
        header << "namespace HUH::Cuda::Gen::" << libname.Get() << " {" << std::endl;
        auto api = toUpperUnderscore(libname.Get() + "-Gen_API");
        for (auto& function : ctx.FunctionNames) {
            auto staticName = "s_" + function.Name;
            header << "extern " << api << " HUH::Cuda::Function " << staticName << ";" << std::endl << std::endl;
            std::stringstream ss;
            for (auto& param : function.ParamNames) {
                ss << "typename " << param << "Type,";
            }
            auto templates = ss.str();
            if (!templates.empty()) {
                templates.pop_back();
                header << "template<";
                header << templates << ">" << std::endl;
            }

            header << "HUH_FORCE_INLINE bool " << function.Name << "(";
            std::stringstream ss2;
            ss2 << "const HUH::Vector3ui& gridDim,";
            ss2 << "const HUH::Vector3ui& blocDim,";
            for (auto& param : function.ParamNames) {
                ss2 << param << "Type " << param << ", ";
            }
            ss2 << "size_t sharedMemorySize = 0,";
            ss2 << "Stream* stream = nullptr";
            header << ss2.str() << ") {" << std::endl;
            header << std::setw(4) << " " << staticName << ".SetGrid(gridDim);" << std::endl;
            header << std::setw(4) << " " << staticName << ".SetBlock(blocDim);" << std::endl;

            header << std::setw(4) << " " << "if(sharedMemorySize) {" << std::endl;
            header << std::setw(8) << " " << staticName << ".SetSharedMemory(sharedMemorySize);" << std::endl;
            header << std::setw(4) << " " << "}" << std::endl;
            header << std::setw(4) << " " << "if(stream) {" << std::endl;
            header << std::setw(8) << " " << staticName << ".SetStream(stream);" << std::endl;
            header << std::setw(4) << " " << "}" << std::endl;

            header << std::setw(4) << " " << "return " << staticName << "(";

            std::stringstream ss3;
            for (auto& param : function.ParamNames) {
                ss3 << "std::forward<" << param << "Type>(" << param << "), ";
            }
            auto inputParams = ss3.str();
            if (!inputParams.empty()) {

                inputParams.pop_back();
                inputParams.pop_back();
                header << inputParams << ");" << std::endl;
            } else {
                header << ");" << std::endl;
            }
            header << "}" << std::endl << std::endl;
        }

        header << api << " bool Initialize(HUH::Cuda::Device* device);" << std::endl;

        header << "}" << std::endl;
        header.close();

        std::ofstream source(output.Get() / std::filesystem::path(libname.Get() + ".gen.cpp"));
        source << "#include <HUH/Cuda/Gen/" << libname.Get() << ".gen.h>" << std::endl << std::endl;
        source << "namespace HUH::Cuda::Gen::" << libname.Get() << " {" << std::endl;
        source << "inline HUH::LogCategory LogGen(\"" << libname.Get() << "-Gen" << "\");" << std::endl;
        for (auto& function : ctx.FunctionNames) {
            auto staticName = "s_" + function.Name;
            source << api << " HUH::Cuda::Function " << staticName << ";" << std::endl << std::endl;
        }

        source << "HUH::Cuda::Module s_cudaModule;" << std::endl;
        source << "HUH::Cuda::Linker s_linker;" << std::endl;

        source << api << " bool Initialize(HUH::Cuda::Device* device) {" << std::endl;

        source << std::setw(4) << " " << "if(!device) {" << std::endl;
        source << std::setw(8) << " " << "return false;" << std::endl;
        source << std::setw(4) << " " << "}" << std::endl;

        source << std::setw(4) << " " << "s_linker.Init(*device);" << std::endl;
        for (auto& lib : libraries) {
            if (lib.ends_with(".ptx")) {
                source << std::setw(4) << " " << "s_linker.AddPtx(\"" << lib << "\");" << std::endl;
            }
            if (lib.ends_with(".o")) {
                source << std::setw(4) << " " << "s_linker.AddObject(\"" << lib << "\");" << std::endl;
            }
            if (lib.ends_with(".fatbin")) {
                source << std::setw(4) << " " << "s_linker.AddFatbin(\"" << lib << "\");" << std::endl;
            }
            if (lib.ends_with(".dll") || lib.ends_with(".a") || lib.ends_with(".so") || lib.ends_with(".lib")) {
                source << std::setw(4) << " " << "s_linker.AddLib(\"" << lib << "\");" << std::endl;
            }
        }

        source << std::setw(4) << " " << "s_linker.Complete();" << std::endl;
        source << std::setw(4) << " " << "if (!s_cudaModule.Load(s_linker)) {" << std::endl;
        source << std::setw(8) << " " << "return false;" << std::endl;
        source << std::setw(4) << " " << "}" << std::endl;

        for (auto& function : ctx.FunctionNames) {
            auto staticName = "s_" + function.Name;
            source << std::setw(4) << " " << staticName << " = s_cudaModule.GetFunction(\"" << function.Name << "\");"
                   << std::endl;
            source << std::setw(4) << " " << "if (!" << staticName << ") {" << std::endl;
            source << std::setw(8) << " " << "HUH_WLOG(LogGen,\"" << function.Name << " couldn't be loaded\");"
                   << std::endl;
            source << std::setw(4) << " " << "}" << std::endl;
        }
        source << std::setw(4) << " " << "return true;" << std::endl;
        source << "}" << std::endl << std::endl;
        source << "}" << std::endl;
        source.close();
    }

    for (auto& unit : units) {
        clang_disposeTranslationUnit(unit);
    }
    clang_disposeIndex(Index);

    return 0;
}
