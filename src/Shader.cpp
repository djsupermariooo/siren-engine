#include <Shader.h>
#include <iostream>
#include <vector>

using namespace Siren;

std::string Shader::LoadShaderFile(std::string strFile)
{
	std::ifstream fin(strFile);

	if (!fin)
		return "";

	std::string strLine = "";
	std::string strText = "";

	while (getline(fin, strLine))
	{
		strText = strText + "\n" + strLine;
	}

	fin.close();

	return strText;
}

void Shader::Initialize(std::string strVertexFile, std::string strFragmentFile)
{
	std::string strVShader, strFShader;

	if (!strVertexFile.length() || !strFragmentFile.length())
		return;

	if (VertexShaderId || FragmentShaderId || ShaderProgramId)
		Destroy();

	GLenum ErrorCheckValue = glGetError();

	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	strVShader = LoadShaderFile(strVertexFile);
	strFShader = LoadShaderFile(strFragmentFile);

	const char *szVShader = strVShader.c_str();
	const char *szFShader = strFShader.c_str();

	glShaderSource(VertexShaderId, 1, &szVShader, NULL);
	glShaderSource(FragmentShaderId, 1, &szFShader, NULL);

	glCompileShader(VertexShaderId);
	char error[1000];
	glGetShaderInfoLog(VertexShaderId, 1000, NULL, error);
	std::cout << "Compile status: \n" << error << std::endl;
	GLint success = 0;
	glGetShaderiv(VertexShaderId, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
		std::cout << "Failed\n" << std::endl;
	else
		std::cout << "Success\n" << std::endl;

	glCompileShader(FragmentShaderId);
	glGetShaderInfoLog(FragmentShaderId, 1000, NULL, error);
	std::cout << "Compile status: \n" << error << std::endl;
	success = 0;
	glGetShaderiv(FragmentShaderId, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
		std::cout << "Failed\n" << std::endl;
	else
		std::cout << "Success\n" << std::endl;

	ShaderProgramId = glCreateProgram();

	glAttachShader(ShaderProgramId, VertexShaderId);
	glAttachShader(ShaderProgramId, FragmentShaderId);

	glLinkProgram(ShaderProgramId);
	glGetProgramInfoLog(ShaderProgramId, 1000, NULL, error);
	std::cout << "Link status: \n" << error << std::endl;
	success = 0;
	glGetProgramiv(ShaderProgramId, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
		std::cout << "Failed\n" << std::endl;
	else
		std::cout << "Success\n" << std::endl;

	ErrorCheckValue = glGetError();

	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(stderr, "ERROR: Could not create the shader program with error Id: %d\n", ErrorCheckValue);
		exit(-1);
	} 
}

GLint Shader::GetVariable(std::string strVariable)
{
	if (!ShaderProgramId)
		return -1;

	return glGetUniformLocation(ShaderProgramId, strVariable.c_str());
}

void Shader::Destroy()
{
	if (VertexShaderId)
	{
		glDetachShader(ShaderProgramId, VertexShaderId);
		glDeleteShader(VertexShaderId);
		VertexShaderId = 0;
	}

	if (FragmentShaderId)
	{
		glDetachShader(ShaderProgramId, FragmentShaderId);
		glDeleteShader(FragmentShaderId);
		FragmentShaderId = 0;
	}

	if (ShaderProgramId)
	{
		glDeleteShader(ShaderProgramId);
		ShaderProgramId = 0;
	}
}
