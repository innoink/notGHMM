# Author : Weipeng He <heweipeng@gmail.com>
# Copyright (c) 2014, All rights reserved.

add_executable(genseq_test genseq_test.c notghmm.c utils.c)
target_link_libraries(genseq_test ${GSL_LIBRARIES} ${FLANN_LIBRARIES})
if(UNIX)
  target_link_libraries(genseq_test m)
endif(UNIX)

add_executable(wr_test writeread_test.c notghmm.c utils.c)
target_link_libraries(wr_test ${GSL_LIBRARIES} ${FLANN_LIBRARIES})

add_executable(init_test init_test.c notghmm.c utils.c)
target_link_libraries(init_test ${GSL_LIBRARIES} ${FLANN_LIBRARIES})

add_library(notghmm SHARED notghmm.c utils.c)
target_link_libraries(notghmm PRIVATE ${GSL_LIBRARIES} ${FLANN_LIBRARIES})

install(TARGETS notghmm
  LIBRARY DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DESTINATION include FILES_MATCHING PATTERN "*.h")

