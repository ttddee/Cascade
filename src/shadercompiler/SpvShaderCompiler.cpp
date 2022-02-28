#include "SpvShaderCompiler.h"

#include <iostream>

#ifdef _WIN32
    #include "glslang/Public/ShaderLang.h"
    #include "glslang/SPIRV/GlslangToSpv.h"
    #include "DirStackFileIncluder.h"
    #include "glslang/Include/ResourceLimits.h"
#elif __linux__
    #include <glslang/Public/ShaderLang.h>
	#include <glslang/SPIRV/GlslangToSpv.h>
    #include "DirStackFileIncluder.h"
	#include <glslang/Include/ResourceLimits.h>
#endif

struct SpvCompiler::Impl
{
	Impl()
	{
		// Initialize GLSL
		if (!glslang::InitializeProcess())
		{
			throw std::runtime_error("Failed to initialize glslang.");
		}
	};

	std::string getFilePath(const std::string& s);
	std::string getFileName(const std::string& s);
	std::string getFileExtension(const std::string& s);

	EShLanguage getShaderStage(const std::string& fileExtension);

	bool compile(const std::string& shaderType, const std::string& shaderCode, std::vector<unsigned int>& spirV);

	std::vector<unsigned int> spirV;

	std::string error;

	const TBuiltInResource defaultTBuiltInResource = {
		/* .MaxLights = */ 32,
		/* .MaxClipPlanes = */ 6,
		/* .MaxTextureUnits = */ 32,
		/* .MaxTextureCoords = */ 32,
		/* .MaxVertexAttribs = */ 64,
		/* .MaxVertexUniformComponents = */ 4096,
		/* .MaxVaryingFloats = */ 64,
		/* .MaxVertexTextureImageUnits = */ 32,
		/* .MaxCombinedTextureImageUnits = */ 80,
		/* .MaxTextureImageUnits = */ 32,
		/* .MaxFragmentUniformComponents = */ 4096,
		/* .MaxDrawBuffers = */ 32,
		/* .MaxVertexUniformVectors = */ 128,
		/* .MaxVaryingVectors = */ 8,
		/* .MaxFragmentUniformVectors = */ 16,
		/* .MaxVertexOutputVectors = */ 16,
		/* .MaxFragmentInputVectors = */ 15,
		/* .MinProgramTexelOffset = */ -8,
		/* .MaxProgramTexelOffset = */ 7,
		/* .MaxClipDistances = */ 8,
		/* .MaxComputeWorkGroupCountX = */ 65535,
		/* .MaxComputeWorkGroupCountY = */ 65535,
		/* .MaxComputeWorkGroupCountZ = */ 65535,
		/* .MaxComputeWorkGroupSizeX = */ 1024,
		/* .MaxComputeWorkGroupSizeY = */ 1024,
		/* .MaxComputeWorkGroupSizeZ = */ 64,
		/* .MaxComputeUniformComponents = */ 1024,
		/* .MaxComputeTextureImageUnits = */ 16,
		/* .MaxComputeImageUniforms = */ 8,
		/* .MaxComputeAtomicCounters = */ 8,
		/* .MaxComputeAtomicCounterBuffers = */ 1,
		/* .MaxVaryingComponents = */ 60,
		/* .MaxVertexOutputComponents = */ 64,
		/* .MaxGeometryInputComponents = */ 64,
		/* .MaxGeometryOutputComponents = */ 128,
		/* .MaxFragmentInputComponents = */ 128,
		/* .MaxImageUnits = */ 8,
		/* .MaxCombinedImageUnitsAndFragmentOutputs = */ 8,
		/* .MaxCombinedShaderOutputResources = */ 8,
		/* .MaxImageSamples = */ 0,
		/* .MaxVertexImageUniforms = */ 0,
		/* .MaxTessControlImageUniforms = */ 0,
		/* .MaxTessEvaluationImageUniforms = */ 0,
		/* .MaxGeometryImageUniforms = */ 0,
		/* .MaxFragmentImageUniforms = */ 8,
		/* .MaxCombinedImageUniforms = */ 8,
		/* .MaxGeometryTextureImageUnits = */ 16,
		/* .MaxGeometryOutputVertices = */ 256,
		/* .MaxGeometryTotalOutputComponents = */ 1024,
		/* .MaxGeometryUniformComponents = */ 1024,
		/* .MaxGeometryVaryingComponents = */ 64,
		/* .MaxTessControlInputComponents = */ 128,
		/* .MaxTessControlOutputComponents = */ 128,
		/* .MaxTessControlTextureImageUnits = */ 16,
		/* .MaxTessControlUniformComponents = */ 1024,
		/* .MaxTessControlTotalOutputComponents = */ 4096,
		/* .MaxTessEvaluationInputComponents = */ 128,
		/* .MaxTessEvaluationOutputComponents = */ 128,
		/* .MaxTessEvaluationTextureImageUnits = */ 16,
		/* .MaxTessEvaluationUniformComponents = */ 1024,
		/* .MaxTessPatchComponents = */ 120,
		/* .MaxPatchVertices = */ 32,
		/* .MaxTessGenLevel = */ 64,
		/* .MaxViewports = */ 16,
		/* .MaxVertexAtomicCounters = */ 0,
		/* .MaxTessControlAtomicCounters = */ 0,
		/* .MaxTessEvaluationAtomicCounters = */ 0,
		/* .MaxGeometryAtomicCounters = */ 0,
		/* .MaxFragmentAtomicCounters = */ 8,
		/* .MaxCombinedAtomicCounters = */ 8,
		/* .MaxAtomicCounterBindings = */ 1,
		/* .MaxVertexAtomicCounterBuffers = */ 0,
		/* .MaxTessControlAtomicCounterBuffers = */ 0,
		/* .MaxTessEvaluationAtomicCounterBuffers = */ 0,
		/* .MaxGeometryAtomicCounterBuffers = */ 0,
		/* .MaxFragmentAtomicCounterBuffers = */ 1,
		/* .MaxCombinedAtomicCounterBuffers = */ 1,
		/* .MaxAtomicCounterBufferSize = */ 16384,
		/* .MaxTransformFeedbackBuffers = */ 4,
		/* .MaxTransformFeedbackInterleavedComponents = */ 64,
		/* .MaxCullDistances = */ 8,
		/* .MaxCombinedClipAndCullDistances = */ 8,
		/* .MaxSamples = */ 4,

		/* .maxMeshOutputVerticesNV = */ 256,
		/* .maxMeshOutputPrimitivesNV = */ 512,
		/* .maxMeshWorkGroupSizeX_NV = */ 32,
		/* .maxMeshWorkGroupSizeY_NV = */ 1,
		/* .maxMeshWorkGroupSizeZ_NV = */ 1,
		/* .maxTaskWorkGroupSizeX_NV = */ 32,
		/* .maxTaskWorkGroupSizeY_NV = */ 1,
		/* .maxTaskWorkGroupSizeZ_NV = */ 1,
		/* .maxMeshViewCountNV = */ 4,

		/* .limits = */
		/* .nonInductiveForLoops = */ 1,
        {
            /* .whileLoops = */ 1,
            /* .doWhileLoops = */ 1,
            /* .generalUniformIndexing = */ 1,
            /* .generalAttributeMatrixVectorIndexing = */ 1,
            /* .generalVaryingIndexing = */ 1,
            /* .generalSamplerIndexing = */ 1,
            /* .generalVariableIndexing = */ 1,
            /* .generalConstantMatrixVectorIndexing = */ 1
        }
	};
};



SpvCompiler::SpvCompiler()
	: impl(new Impl())
{
	
}

SpvCompiler::~SpvCompiler() = default;

std::string SpvCompiler::Impl::getFilePath(const std::string& path)
{
	size_t parts = path.find_last_of("/\\");
	return path.substr(0, parts);
}

std::string SpvCompiler::Impl::getFileName(const std::string& path)
{
	size_t parts = path.find_last_of("/\\");
	return path.substr(parts + 1);
}

std::string SpvCompiler::Impl::getFileExtension(const std::string& path)
{
	const size_t pos = path.rfind('.');
	if (pos == std::string::npos)
	{
		return "";
	}
	else 
	{
		return path.substr(path.rfind('.') + 1);
	}
}

EShLanguage SpvCompiler::Impl::getShaderStage(const std::string& fileExtension)
{
	if (fileExtension == "vert")
	{
		return EShLangVertex;
	}
	else if (fileExtension == "tesc")
	{
		return EShLangTessControl;
	}
	else if (fileExtension == "tese")
	{
		return EShLangTessEvaluation;
	}
	else if (fileExtension == "geom")
	{
		return EShLangGeometry;
	}
	else if (fileExtension == "frag")
	{
		return EShLangFragment;
	}
	else if (fileExtension == "comp")
	{
		return EShLangCompute;
	}
	else
	{
		std::cout << "getShaderStage: " << fileExtension << std::endl;
		throw std::runtime_error("File extension doesn't match any known shader stage.");
	}
}

bool SpvCompiler::Impl::compile(
        const std::string& shaderType,
        const std::string& shaderCode,
        std::vector<unsigned int>& spirV)
{
	auto shaderStage = getShaderStage(shaderType);

	const char* inputCString = shaderCode.c_str();

	error.clear();

	// Create TShader and pass input to it
	glslang::TShader shader(shaderStage);
	shader.setStrings(&inputCString, 1);

	// Set up resources
	int clientInputSemanticsVersion = 100;
	glslang::EShTargetClientVersion vulkanClientVersion = glslang::EShTargetVulkan_1_0;
	glslang::EShTargetLanguageVersion targetVersion = glslang::EShTargetSpv_1_0;

	shader.setEnvInput(glslang::EShSourceGlsl, shaderStage, glslang::EShClientVulkan, clientInputSemanticsVersion);
	shader.setEnvClient(glslang::EShClientVulkan, vulkanClientVersion);
	shader.setEnvTarget(glslang::EShTargetSpv, targetVersion);

	TBuiltInResource resources;
	resources = defaultTBuiltInResource;
	EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

	const int defaultVersion = 100;

	// Preprocessing
    DirStackFileIncluder includer;

	//std::string filepath = getFilePath(path);
	//includer.pushExternalLocalDirectory(filepath);

	std::string preprocessedGLSL;

	if (!shader.preprocess(&resources, defaultVersion, ENoProfile, false, false, messages, &preprocessedGLSL, includer))
	{
		std::cout << "GLSL Preprocessing Failed for:" << std::endl;
		std::cout << shader.getInfoLog() << std::endl;
		std::cout << shader.getInfoDebugLog() << std::endl;
		error.append(shader.getInfoLog());

		return false;
	}

	const char* preprocessedCStr = preprocessedGLSL.c_str();
	shader.setStrings(&preprocessedCStr, 1);

	// Parse shader
	if (!shader.parse(&resources, 100, false, messages))
	{
		std::cout << "GLSL parsing failed." << std::endl;
		std::cout << shader.getInfoLog() << std::endl;
		std::cout << shader.getInfoDebugLog() << std::endl;
		error.append(shader.getInfoLog());

		return false;
	}

	// Pass to program and link
	glslang::TProgram program;
	program.addShader(&shader);

	if (!program.link(messages))
	{
		std::cout << "GLSL Linking Failed." << std::endl;
		std::cout << shader.getInfoLog() << std::endl;
		std::cout << shader.getInfoDebugLog() << std::endl;
		error.append(shader.getInfoLog());

		return false;
	}

	spv::SpvBuildLogger logger;
	glslang::SpvOptions spvOptions;
	glslang::GlslangToSpv(*program.getIntermediate(shaderStage), spirV, &logger, &spvOptions);

	return true;
}

//bool SpvCompiler::compileGLSLFromFile(const std::string& path)
//{
//	// Open file
//	std::ifstream file(path);
//	if (!file.is_open())
//	{
//		throw std::runtime_error("Failed to open file: " + path);
//	}

//	std::string inputGLSL((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

//	auto extension = impl->getFileExtension(path);

//	impl->spirV.clear();

//	if (impl->compile(extension, inputGLSL, impl->spirV))
//	{
//		return true;
//	}

//	return false;
//}

bool SpvCompiler::compileGLSLFromCode(const std::string& code, const std::string& shaderType)
{
	impl->spirV.clear();

	if (impl->compile(shaderType, code, impl->spirV))
	{
		return true;
	}

	return false;
}

std::vector<unsigned int> SpvCompiler::getSpirV()
{
	return impl->spirV;
}

std::string SpvCompiler::getError()
{
	return impl->error;
}


