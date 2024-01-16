#pragma once
class IInputSystem
{
public:
	bool IsRelativeMouseMode( )
	{
		// @ida: 'IInputSystem::SetRelativeMouseMode'.
		return *reinterpret_cast< bool* >( reinterpret_cast< unsigned long long >( this ) + 0x4F );
	}

	void* GetSDLWindow( )
	{
		// @ida: IInputSystem::DebugSpew -> #STR: "Current coordinate bias %s: %g,%g scale %g,%g\n"
		return *reinterpret_cast< void** >( reinterpret_cast< unsigned long long >( this ) + 0x2678 );
	}
};
