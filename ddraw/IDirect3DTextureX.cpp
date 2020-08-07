/**
* Copyright (C) 2020 Elisha Riedlinger
*
* This software is  provided 'as-is', without any express  or implied  warranty. In no event will the
* authors be held liable for any damages arising from the use of this software.
* Permission  is granted  to anyone  to use  this software  for  any  purpose,  including  commercial
* applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*   1. The origin of this software must not be misrepresented; you must not claim that you  wrote the
*      original  software. If you use this  software  in a product, an  acknowledgment in the product
*      documentation would be appreciated but is not required.
*   2. Altered source versions must  be plainly  marked as such, and  must not be  misrepresented  as
*      being the original software.
*   3. This notice may not be removed or altered from any source distribution.
*/

#include "ddraw.h"

HRESULT m_IDirect3DTextureX::QueryInterface(REFIID riid, LPVOID FAR * ppvObj, DWORD DirectXVersion)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	if (ppvObj && riid == IID_GetRealInterface)
	{
		*ppvObj = ProxyInterface;
		return DD_OK;
	}
	if (ppvObj && riid == IID_GetInterfaceX)
	{
		*ppvObj = this;
		return DD_OK;
	}

	DWORD DxVersion = (CheckWrapperType(riid) && (Config.Dd7to9 || Config.ConvertToDirect3D7)) ? GetGUIDVersion(riid) : DirectXVersion;

	return ProxyQueryInterface(ProxyInterface, riid, ppvObj, GetWrapperType(DxVersion), GetWrapperInterfaceX(DxVersion));
}

void *m_IDirect3DTextureX::GetWrapperInterfaceX(DWORD DirectXVersion)
{
	switch (DirectXVersion)
	{
	case 1:
		return WrapperInterface;
	case 2:
		return WrapperInterface2;
	default:
		LOG_LIMIT(100, __FUNCTION__ << " Error: wrapper interface version not found: " << DirectXVersion);
		return nullptr;
	}
}

ULONG m_IDirect3DTextureX::AddRef()
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	if (ProxyDirectXVersion > 3)
	{
		return InterlockedIncrement(&RefCount);
	}

	return ProxyInterface->AddRef();
}

ULONG m_IDirect3DTextureX::Release()
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	LONG ref;

	if (ProxyDirectXVersion > 3)
	{
		ref = InterlockedDecrement(&RefCount);
	}
	else
	{
		ref = ProxyInterface->Release();
	}

	if (ref == 0)
	{
		delete this;
	}

	return ref;
}

HRESULT m_IDirect3DTextureX::Initialize(LPDIRECT3DDEVICE lpDirect3DDevice, LPDIRECTDRAWSURFACE lplpDDSurface)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	if (ProxyDirectXVersion != 1)
	{
		return D3D_OK;
	}

	if (lpDirect3DDevice)
	{
		lpDirect3DDevice->QueryInterface(IID_GetRealInterface, (LPVOID*)&lpDirect3DDevice);
	}
	if (lplpDDSurface)
	{
		lplpDDSurface->QueryInterface(IID_GetRealInterface, (LPVOID*)&lplpDDSurface);
	}

	return GetProxyInterfaceV1()->Initialize(lpDirect3DDevice, lplpDDSurface);
}

HRESULT m_IDirect3DTextureX::GetHandle(LPDIRECT3DDEVICE2 lpDirect3DDevice2, LPD3DTEXTUREHANDLE lpHandle)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	if (lpDirect3DDevice2)
	{
		lpDirect3DDevice2->QueryInterface(IID_GetRealInterface, (LPVOID*)&lpDirect3DDevice2);
	}

	switch (ProxyDirectXVersion)
	{
	case 1:
		return GetProxyInterfaceV1()->GetHandle((LPDIRECT3DDEVICE)lpDirect3DDevice2, lpHandle);
	case 2:
		return GetProxyInterfaceV2()->GetHandle(lpDirect3DDevice2, lpHandle);
	case 7:
	case 9:
		LOG_LIMIT(100, __FUNCTION__ << " Not Implemented");
		return DDERR_UNSUPPORTED;
	default:
		return DDERR_GENERIC;
	}
}

HRESULT m_IDirect3DTextureX::PaletteChanged(DWORD dwStart, DWORD dwCount)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	switch (ProxyDirectXVersion)
	{
	case 1:
		return GetProxyInterfaceV1()->PaletteChanged(dwStart, dwCount);
	case 2:
		return GetProxyInterfaceV2()->PaletteChanged(dwStart, dwCount);
	case 7:
	case 9:
		LOG_LIMIT(100, __FUNCTION__ << " Not Implemented");
		return DDERR_UNSUPPORTED;
	default:
		return DDERR_GENERIC;
	}
}

HRESULT m_IDirect3DTextureX::Load(LPDIRECT3DTEXTURE2 lpD3DTexture2)
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	if (lpD3DTexture2)
	{
		lpD3DTexture2->QueryInterface(IID_GetRealInterface, (LPVOID*)&lpD3DTexture2);
	}

	switch (ProxyDirectXVersion)
	{
	case 1:
		return GetProxyInterfaceV1()->Load((LPDIRECT3DTEXTURE)lpD3DTexture2);
	case 2:
		return GetProxyInterfaceV2()->Load(lpD3DTexture2);
	case 7:
	case 9:
		LOG_LIMIT(100, __FUNCTION__ << " Not Implemented");
		return DDERR_UNSUPPORTED;
	default:
		return DDERR_GENERIC;
	}
}

HRESULT m_IDirect3DTextureX::Unload()
{
	Logging::LogDebug() << __FUNCTION__ << " (" << this << ")";

	if (ProxyDirectXVersion != 1)
	{
		LOG_LIMIT(100, __FUNCTION__ << " Not Implemented");
		return DDERR_UNSUPPORTED;
	}

	return GetProxyInterfaceV1()->Unload();
}
