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
// File : <Filename>
// Date : <Date>
// Comments :
//
//----------------------------------------------------------------------------
#ifndef __SceneNode_h__
#define __SceneNode_h__
//----------------------------------------------------------------------------
#include "YumeRequired.h"
#include <DirectXMath.h>
//----------------------------------------------------------------------------
namespace YumeEngine
{
	enum GeometryType
	{
		GT_STATIC,
		GT_LIGHT
	};
	class YumeAPIExport SceneNode : public YumeBase
	{
	public:
		SceneNode(GeometryType type);
		virtual ~SceneNode();

		void SetPosition(const DirectX::XMVECTOR&,bool setAsInitial = false);
		void SetRotation(const DirectX::XMVECTOR&);
		void SetDirection(const DirectX::XMVECTOR&);
		void SetScale(float x,float y,float z);


		void Translate(const DirectX::XMVECTOR&);

		void SetName(const YumeString& name) { name_ = name; }

		void SetWorld(const DirectX::XMMATRIX& world);

		const DirectX::XMFLOAT4& GetPosition() const { return pos_; }
		const DirectX::XMFLOAT4& GetInitialPosition() const { return initialPos_; }

		const DirectX::XMFLOAT4& GetRotation() const { return rot_; }
		const DirectX::XMFLOAT4& GetDirection() const { return dir_; }

		DirectX::XMMATRIX GetTransformation();

		GeometryType GetType() const { return type_; }

		void SetBoundingBox(const DirectX::XMFLOAT3& bbMin,const DirectX::XMFLOAT3& bbMax);

		const DirectX::XMFLOAT3& GetBbMin() const { return bbMin; }
		const DirectX::XMFLOAT3& GetBbMax() const { return bbMax; }
	protected:
		GeometryType type_;

		DirectX::XMFLOAT4 pos_;
		DirectX::XMFLOAT4 initialPos_;
		DirectX::XMFLOAT4 rot_;
		DirectX::XMFLOAT4 dir_;

		DirectX::XMFLOAT3 bbMin;
		DirectX::XMFLOAT3 bbMax;

		DirectX::XMFLOAT4X4 Scale;
		DirectX::XMFLOAT4X4 World;

		YumeString name_;
	};
}


//----------------------------------------------------------------------------
#endif
