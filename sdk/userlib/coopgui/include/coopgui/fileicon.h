#pragma once
#pragma interface

__COOPGUI_NAMESPACE_BEGIN

struct IFileIcon
{
	virtual void Dispose() = 0;
	virtual const color_t* GetData() = 0;
};

using FileIconPtr = ResPtr<IFileIcon>;

__COOPGUI_NAMESPACE_END
