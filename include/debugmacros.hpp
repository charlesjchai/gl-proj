// C++ commitee fail
#define RELASSERT(expr)                                                        \
    do {                                                                       \
        if (!(expr)) {                                                         \
            std::cerr << "Assertion failed: (" << #expr << "), function "      \
                      << __func__ << ", file " << __FILE__ << ", line "        \
                      << __LINE__ << ".\n";                                    \
            std::abort();                                                      \
        }                                                                      \
    } while (0)

// Calls a function and watches it for OpenGL errors
#define RELGL_CALL(x)                                                          \
    while (glGetError() != GL_NO_ERROR)                                        \
        ;                                                                      \
    x;                                                                         \
    do {                                                                       \
        if (GLenum err = glGetError()) {                                       \
            std::cerr << "OpenGL ERROR (" << err << ") at '" << #x             \
                      << "' in file '" << __FILE__ << "' at function "         \
                      << __func__ << " on line " << __LINE__ << '\n';          \
            std::abort();                                                      \
        }                                                                      \
    } while (0)
// Debug-only version
#ifdef NDEBUG
#define GL_CALL(x) x
#else
#define GL_CALL(x) RELGL_CALL(x)
#endif
