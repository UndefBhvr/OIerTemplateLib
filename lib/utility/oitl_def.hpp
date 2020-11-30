#if defined(__GNUC__)
	#define _OITL_LANG_VER __cplusplus
	#if __GNUC__>=10
		#define _OITL_CONCEPT_LIMIT 201907L
	#else
		#define _OITL_CONCEPT_LIMIT 0x7FFFFFFFL
	#endif

#elif defined(_MSC_VER)
	#define _OITL_LANG_VER _MSVC_LANG
	#define _OITL_CONCEPT_LIMIT 201811L

#endif

#if __cpp_concepts>=_OITL_CONCEPT_LIMIT
	#define _OITL_CONCEPT_AVAILABLE
#endif