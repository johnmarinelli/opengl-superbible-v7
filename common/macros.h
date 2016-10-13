#ifndef J_OGL_MACROS
#define J_OGL_MACROS

#define PRN_GLM(glm_obj) \
    std::cout<<#glm_obj<<glm::to_string(glm_obj) << '\n';

#define PRN_GLM_1(glm_obj) \
  static bool _##glm_obj##_PRNTD = false; \
  if (_##glm_obj##_PRNTD) \
    (void)0; \
  else { \
    _##glm_obj##_PRNTD = true; \
    PRN_GLM(glm_obj) \
  };

#endif
