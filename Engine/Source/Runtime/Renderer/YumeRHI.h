//----------------------------------------------------------------------------
//Yume Engine
//Copyright (C) 2015  arkenthera
//This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//You should have received a copy of the GNU General Public License along
//with this program; if not, write to the Free Software Foundation, Inc.,
//51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.*/
//----------------------------------------------------------------------------
//
// File : <Filename> YumeGraphics.h
// Date : 2.19.2016
// Comments :
//
//----------------------------------------------------------------------------
#ifndef __YumeGraphicsInterface_h__
#define __YumeGraphicsInterface_h__
//----------------------------------------------------------------------------
#include "YumeRequired.h"
#include "Renderer/YumeRendererDefs.h"

#include "Math/YumeRect.h"
#include "Math/YumeColor.h"
#include "Math/YumeVector4.h"
#include "Core/YumeVariant.h"
#include "Renderer/YumeImage.h"

#include <boost/shared_array.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/thread/mutex.hpp>

#include <SDL.h>
//----------------------------------------------------------------------------
namespace YumeEngine
{
	class YumeGpuResource;
	class YumeRendererImpl;
	class YumeImage;
	class YumeShaderVariation;
	class YumeShader;
	class YumeVertexBuffer;
	class YumeConstantBuffer;
	class YumeRenderable;
	class YumeTexture2D;
	class YumeTexture;

	struct ScratchBuffer
	{
		ScratchBuffer():
			size_(0),
			reserved_(false)
		{
		}

		/// Buffer data.
		boost::shared_array<unsigned char> data_;
		/// Data size.
		unsigned size_;
		/// Reserved flag.
		bool reserved_;
	};

	typedef std::vector<YumeGpuResource*> GpuResourceVector;

	class YumeAPIExport YumeRHI
	{
	public:
		YumeRHI();

		virtual ~YumeRHI();

		virtual bool SetGraphicsMode(int width,int height,bool fullscreen,bool borderless,bool resizable,bool vsync,bool tripleBuffer,
			int multiSample) = 0;

		virtual bool BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void Clear(unsigned flags,const Vector4& color = Vector4(0.0f,0.0f,0.0f,0.0f),float depth = 1.0f,unsigned stencil = 0) = 0;

		virtual Vector2 GetRenderTargetDimensions() const = 0;

		virtual void SetFlushGPU(bool flushGpu) = 0;
		virtual void CreateRendererCapabilities() = 0;
		virtual YumeVector<int>::type GetMultiSampleLevels() const = 0;

		virtual void Close() = 0;
		virtual void AdjustWindow(int& newWidth,int& newHeight,bool& newFullscreen,bool& newBorderless) = 0;
		virtual void Maximize() = 0;
		virtual void SetWindowPos(const Vector2& pos) = 0;
		virtual void SetWindowTitle(const YumeString& string) = 0;

		virtual void AddGpuResource(YumeGpuResource* object) = 0;
		virtual void RemoveGpuResource(YumeGpuResource* object) = 0;


		virtual YumeConstantBuffer* GetOrCreateConstantBuffer(ShaderType type,unsigned index,unsigned size) = 0;

		virtual void ResetCache() = 0;


		//Getters
		YumeShaderVariation* GetVertexShader() const { return vertexShader_; }
		YumeShaderVariation* GetPixelShader() const { return pixelShader_; }

		YumeVertexBuffer* GetVertexBuffer(unsigned index) const;
		virtual YumeShaderVariation* GetShader(ShaderType type,const YumeString& name,const YumeString& defines = "") const = 0;
		virtual YumeShaderVariation* GetShader(ShaderType type,const char* name,const char* defines) const = 0;

		TextureFilterMode GetDefaultTextureFilterMode() const { return defaultTextureFilterMode_; }
		unsigned GetTextureAnisotropy() const { return textureAnisotropy_; }
		virtual unsigned GetFormat(CompressedFormat format) const = 0;

		//Setters
		void SetWindowIcon(YumeImage* image);
		virtual void SetShaders(YumeShaderVariation* vs,YumeShaderVariation* ps) = 0;

		virtual void SetShaderParameter(YumeHash  param,const float* data,unsigned count) = 0;
		/// Set shader float constant.
		virtual void SetShaderParameter(YumeHash  param,float value) = 0;
		/// Set shader boolean constant.
		virtual void SetShaderParameter(YumeHash  param,bool value) = 0;
		/// Set shader color constant.
		virtual void SetShaderParameter(YumeHash  param,const YumeColor& color) = 0;
		/// Set shader 2D vector constant.
		virtual void SetShaderParameter(YumeHash  param,const Vector2& vector) = 0;
		/// Set shader 3x3 matrix constant.
		virtual void SetShaderParameter(YumeHash  param,const Matrix3& matrix) = 0;
		/// Set shader 3D vector constant.
		virtual void SetShaderParameter(YumeHash  param,const Vector3& vector) = 0;
		/// Set shader 4x4 matrix constant.
		virtual void SetShaderParameter(YumeHash  param,const Matrix4& matrix) = 0;
		/// Set shader 4D vector constant.
		virtual void SetShaderParameter(YumeHash param,const Vector4& vector) = 0;

		virtual void SetShaderParameter(YumeHash param,const YumeVariant& value) = 0;

		virtual void SetVertexBuffer(YumeVertexBuffer* buffer) = 0;
		/// Set multiple vertex buffers.
		virtual bool SetVertexBuffers
			(const YumeVector<YumeVertexBuffer*>::type& buffers,const YumeVector<unsigned>::type& elementMasks,unsigned instanceOffset = 0) = 0;
		/// Set multiple vertex buffers.
		virtual bool SetVertexBuffers
			(const YumeVector<SharedPtr<YumeVertexBuffer> >::type& buffers,const YumeVector<unsigned>::type& elementMasks,unsigned instanceOffset = 0) = 0;


		virtual void SetTexture(unsigned index,YumeTexture* texture) = 0;
		void SetTextureAnisotropy(unsigned level);
		void SetTextureParametersDirty();

		void ResetRenderTargets();
		/// Reset specific rendertarget.
		void ResetRenderTarget(unsigned index);
		/// Reset depth-stencil surface.
		void ResetDepthStencil();
		/// Set rendertarget.
		void SetRenderTarget(unsigned index,YumeRenderable* renderTarget);
		/// Set rendertarget.
		void SetRenderTarget(unsigned index,YumeTexture2D* texture);
		/// Set depth-stencil surface.
		void SetDepthStencil(YumeRenderable* depthStencil);
		/// Set depth-stencil surface.
		void SetDepthStencil(YumeTexture2D* texture);
		virtual void SetViewport(const IntRect& rect) = 0;


		void* ReserveScratchBuffer(unsigned size);
		void FreeScratchBuffer(void* buffer);
		void CleanupScratchBuffers();

		bool HasTextureUnit(TextureUnit unit);
		TextureUnit GetTextureUnit(const YumeString& name);
		const YumeString& GetTextureUnitName(TextureUnit unit);
		YumeTexture* GetTexture(unsigned index) const;

		YumeRenderable* GetRenderTarget(unsigned index) const;

		/// Return current depth-stencil surface.
		YumeRenderable* GetDepthStencil() const { return depthStencil_; }

		/// Return the viewport coordinates.
		IntRect GetViewport() const { return viewport_; }

	protected:
		void CreateWindowIcon();
		bool useClipPlane_;
		Vector4 clipPlane_;
	protected:
		SDL_Window* window_;

		YumeImage*	windowIcon_;

		mutable SharedPtr<YumeShader> lastShader_;
		mutable YumeString lastShaderName_;

		YumeString shaderPath_;
		YumeString shaderExtension_;

		YumeVertexBuffer* vertexBuffers_[MAX_VERTEX_STREAMS];
		unsigned elementMasks_[MAX_VERTEX_STREAMS];

	protected:
		YumeString								windowTitle_;
		int										windowWidth_;
		int										windowHeight_;
		Vector2									windowPos_;
		bool									fullscreen_;
		bool									borderless_;
		bool									resizeable_;
		bool									tripleBuffer_;
		int										multiSample_;
		bool									vsync_;
		bool									flushGpu_;

		int										numPrimitives_;
		int										numBatches_;

		IntRect									viewport_;

	protected:
		TextureFilterMode defaultTextureFilterMode_;
		unsigned textureAnisotropy_;

	protected:
		boost::mutex							gpuResourceMutex_;
		GpuResourceVector						gpuResources_;

	protected:
		bool renderTargetsDirty_;
		bool texturesDirty_;
		bool vertexDeclarationDirty_;
		bool blendStateDirty_;
		bool depthStateDirty_;
		bool rasterizerStateDirty_;
		bool scissorRectDirty_;
		bool stencilRefDirty_;

	protected:
		unsigned firstDirtyVB_;
		unsigned lastDirtyVB_;
		unsigned firstDirtyTexture_;
		unsigned lastDirtyTexture_;

		/// Largest scratch buffer request this frame.
		unsigned maxScratchBufferRequest_;
		/// Scratch buffers.
		YumeVector<ScratchBuffer>::type scratchBuffers_;
		YumeMap<unsigned,SharedPtr<YumeConstantBuffer> >::type constantBuffers_;
		YumeVector<YumeConstantBuffer*>::type dirtyConstantBuffers_;
		YumeMap<YumeString,TextureUnit>::type textureUnits_;

		const void* shaderParameterSources_[MAX_SHADER_PARAMETER_GROUPS];

		YumeShaderVariation* vertexShader_;
		YumeShaderVariation* pixelShader_;

		YumeTexture* textures_[MAX_TEXTURE_UNITS];
		YumeRenderable* renderTargets_[MAX_RENDERTARGETS];
		YumeRenderable* depthStencil_;
	};
}


//----------------------------------------------------------------------------
#endif
