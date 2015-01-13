//==============================================================================
// LNFXManager 
//------------------------------------------------------------------------------
///**
//  @file       LNFXManager.h
//  @brief      LNFXManager
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../FileIO/Interface.h"
#include "../Interface.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

class IAnnotatedObject
{
public:
	virtual ~IAnnotatedObject() {}

public:
	virtual void addAnnotation( const char* name, const char* value ) = 0;
};

//==============================================================================
// �� IGLShaderBuilder
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class IGLShaderBuilder
{
public:
	virtual IAnnotatedObject*	OnCreateTechnique( const char* name ) = 0;
	virtual IAnnotatedObject*	OnCreatePass( IAnnotatedObject* parentTech, const char* name ) = 0;
	virtual void				OnCreateVertexShader( IAnnotatedObject* parentPass, const char* code, size_t size ) = 0;
	virtual void				OnCreatePixelShader( IAnnotatedObject* parentPass, const char* code, size_t size ) = 0;
	virtual void				OnSetRenderState( IAnnotatedObject* parentPass, const char* name, const char* value ) = 0;
	virtual void				OnEndPass( IAnnotatedObject* pass ) = 0;
	virtual IAnnotatedObject*	OnTrySetVariableParam( const char* targetName, const char* semantic, bool shared, const char* attachedTextureVarName ) = 0;

};

//==============================================================================
// �� LNFXManager
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class LNFXManager
{
public:
	static const lnChar*	Magic;				///< "lnfx"

	static const char*		LNoteHLDLMacro;		///< "LNOTE_HLSL"

	static const lnChar*	BeginHLSLTag;		///< "@BEGINHLSL"
	static const lnChar*	EndHLSLTag;			///< "@ENDHLSL"
	static const lnChar*	BeginGLSLTag;		///< "@BEGINGLSL["	���p�X����[�܂�
	static const lnChar*	EndGLSLTag;			///< "@ENDGLSL"
	static const lnChar*	BeginInfoTag;		///< "@BEGININFO"
	static const lnChar*	EndInfoTag;			///< "@ENDINFO"

	static const char*		VariableElementName;		///< "Variable"
	static const char*		AnnotationElementName;		///< "Annotation"
	static const char*		TechniqueElementName;		///< "Technique"
	static const char*		PassElementName;			///< "Pass"
	static const char*		VertexShaderElementName;	///< "VertexShader"
	static const char*		PixelShaderElementName;		///< "PixelShader"
	static const char*		RenderStateElementName;		///< "RenderState"

	static const char*		NameAttributeName;			///< "Name"
	static const char*		SemanticAttributeName;		///< "Semantic"
	static const char*		SharedAttributeName;		///< "Shared"
	static const char*		TextureAttributeName;		///< "Texture"
	static const char*		ValueAttributeName;			///< "Value"
	static const char*		EntryPointAttributeName;	///< "EntryPoint"

	static const char*		BeginHLSLTag_ReplaceTarget;	///< "#ifdef LNOTE_HLSL"
	static const char*		EndHLSLTag_ReplaceTarget;	///< "#endif"

	static const char*		BeginGLSLTag_ReplaceTarget;	///< "#ifdef LNOTE_GLSL_"	���̌�ɃV�F�[�_��������
	static const char*		EndGLSLTag_ReplaceTarget;	///< "#endif"

	static const char*		BeginInfoTag_ReplaceTarget;	///< "#ifdef LNOTE_INFO"
	static const char*		EndInfoTag_ReplaceTarget;	///< "#endif"

/* ��)
	<Variable Name="_World" Semantic="WORLD" />
	<Variable Name="_CameraPos Semantic="POSITION">
		<Annotation Name="Object" Value="Camera">
	</Variable>
	<Variable Name="_MatSmp" Shared="true" Texture="_MaterialTexture" />	���T���v���^�ϐ��̎Q�Ɛ�e�N�X�`��
	<Technique>
		<Pass>
			<VertexShader Name="vsBasic">
			<PixelShader Name="psBasic">
		</Pass>
	</Technique>
	<Technique>
		<Annotation Name="Script" Value="Clear=Color;">
		<Pass>
			<Annotation Name="Script" Value="Clear=Color;">
			<RenderState Name="ZWRITEENABLE" Value="TRUE">		�����ׂđ啶��
			<VertexShader Name="vsBasic">
			<PixelShader Name="psBasic">
		</Pass>
	</Technique>
*/

public:
	LNFXManager();
	~LNFXManager();
	
public:

	/// �t�@�C�����J��
	///		�E���� HLSL �t�@�C�����J�����Ƃ��� addDepsInclude �� true �̏ꍇ�A
	///		  MMM �w�b�_�������I�� include ����B
	///		�ELNFX ���J�����Ƃ��Aapi �� OpenGL �ł���� glShaderBuilder ���R�[���o�b�N�����B
	void open( FileIO::Stream* stream, LNGraphicsAPI api, bool addDepsInclude, IGLShaderBuilder* glShaderBuilder );

	const lnString& getHLSLCode() const { return mHLSLCode; }
	
	/// Info �^�O����ǂݎ�����������ɍ\�z
	void buildShaderClasses( IGLShaderBuilder* builder );

private:
	struct GLSLCode
	{
		lnRefString	TagName;
		lnRefString	Code;
	};
	typedef std::vector<GLSLCode>	GLSLCodeArray;

private:
	void _analyzeHLSL( const char* code );

	GLSLCode* _findShaderCode( const char* programName );
	void _analyzeXML( const char* xml, size_t size );
	void _analyzeXMLVariableElement( XML::Element* xmlElement );
	void _analyzeXMLAnnotationElement( XML::Element* xmlElement, IAnnotatedObject* obj );
	void _analyzeXMLTechniqueElement( XML::Element* xmlElement );
	void _analyzeXMLPassElement( XML::Element* xmlElement, IAnnotatedObject* parentTech );

private:

	lnString			mHLSLCode;
	GLSLCodeArray		mGLSLCodeArray;
	lnString			mInfoCode;
	IGLShaderBuilder*	mBuilder;


public:
	// �o�͗p

	/// �t�@�C�����J��
	bool openOutFile( const lnChar* filePath );

	/// ����
	void closeOutFile();

	/// XML (�t�@�C���̓��e����荞��)
	//void writeIndexXML( const lnChar* xmlFilePath );

	/// HLSL
	void writeHLSL( const std::string& code );
	
private:
	FILE*		mOutStream;
	lnString	mOutFilePath;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================