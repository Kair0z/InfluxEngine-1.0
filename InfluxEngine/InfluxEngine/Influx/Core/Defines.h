#pragma once

// Undefines
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

#pragma region DelCpyMove
// Delete Copying class-methods
#define IFX_DelCpy(cls)\
	cls(const cls& other) = delete; \
	cls& operator=(const cls& other) = delete;

// Delete Moving class-methods
#define IFX_DelMove(cls)\
	cls(cls&& other) = delete; \
	cls& operator=(cls&& other) = delete;
	
// Delete Copying & Moving class-methods
#define IFX_DelCpyMove(cls) \
	IFX_DelCpy(cls); \
	IFX_DelMove(cls);
#pragma endregion