#include "WXAS_ContextFactory.hpp"
#include "WXAS_wxDrawingContext.hpp"

#define USE_WX_CONTEXT

namespace WXAS
{

void* GetNativeHandle (wxPanel* panel)
{
	return panel;
}

std::unique_ptr<NUIE::NativeDrawingContext> CreateNativeDrawingContext ()
{
	return std::unique_ptr<NUIE::NativeDrawingContext> (new wxDrawingContext ());
}

}
