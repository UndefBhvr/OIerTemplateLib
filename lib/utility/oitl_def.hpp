#if defined(__GNUC__)
	#define _OITL_LANG_VER __cplusplus
	#define _OITL_CONCEPT_LIMIT 201907L

#elif defined(_MSC_VER)
	#define _OITL_LANG_VER _MSVC_LANG
	#define _OITL_CONCEPT_LIMIT 201811L

#endif

#if __cpp_concepts>=_OITL_CONCEPT_LIMIT
	#define _OITL_CONCEPT_AVAILABLE
#endif